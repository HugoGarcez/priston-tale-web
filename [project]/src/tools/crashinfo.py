"""Print where a crash happened, from a minidump (.dmp) and the linker map file.

Usage: python crashinfo.py game.exe.dmp bin\\Release\\game.map
No debugger needed: reads the dump's exception record, finds which module the
crash address is in, and for game.exe/server.exe names the function from the
map file. Also lists the return addresses found on the crashing thread's stack.
"""
import struct, sys, bisect

def read_dump(path):
    d = open(path, 'rb').read()
    sig, ver, nstreams, dir_rva = struct.unpack_from('<4sIII', d, 0)
    assert sig == b'MDMP', 'not a minidump'
    streams = {}
    for i in range(nstreams):
        stype, size, rva = struct.unpack_from('<III', d, dir_rva + i * 12)
        streams[stype] = (size, rva)
    return d, streams

def memory_reader(d, streams):
    """Return read(addr, size) over the dump's captured memory (or None)."""
    ranges = []
    if 5 in streams:                      # MemoryListStream
        size, rva = streams[5]
        (n,) = struct.unpack_from('<I', d, rva)
        for i in range(n):
            start, dsize, drva = struct.unpack_from('<QII', d, rva + 4 + i * 16)
            ranges.append((start, dsize, drva))
    if 9 in streams:                      # Memory64ListStream
        size, rva = streams[9]
        n, base_rva = struct.unpack_from('<QQ', d, rva)
        off = base_rva
        for i in range(n):
            start, dsize = struct.unpack_from('<QQ', d, rva + 16 + i * 16)
            ranges.append((start, dsize, off))
            off += dsize
    def read(addr, size):
        for start, dsize, drva in ranges:
            if start <= addr and addr + size <= start + dsize:
                return d[drva + addr - start: drva + addr - start + size]
        return None
    return read

def modules(d, streams):
    out = []
    if 4 not in streams: return out
    size, rva = streams[4]
    (n,) = struct.unpack_from('<I', d, rva)
    for i in range(n):
        base = rva + 4 + i * 108
        img_base, img_size = struct.unpack_from('<QI', d, base)
        name_rva = struct.unpack_from('<I', d, base + 20)[0]
        (nlen,) = struct.unpack_from('<I', d, name_rva)
        name = d[name_rva + 4:name_rva + 4 + nlen].decode('utf-16le')
        out.append((img_base, img_size, name))
    return out

def load_map(path):
    syms = []
    preferred = 0x400000
    for line in open(path, encoding='latin-1'):
        if 'Preferred load address is' in line:
            preferred = int(line.split()[-1], 16)
        parts = line.split()
        # function symbols only (flagged "f" in the map)
        if len(parts) >= 5 and ':' in parts[0] and len(parts[2]) == 8 and 'f' in parts[3:-1]:
            try:
                addr = int(parts[2], 16)
            except ValueError:
                continue
            if addr:
                syms.append((addr - preferred, parts[1], parts[-1]))
    syms.sort()
    return syms

def lookup(syms, rva):
    keys = [s[0] for s in syms]
    i = bisect.bisect_right(keys, rva) - 1
    if i < 0: return '?'
    a, name, obj = syms[i]
    return '%s+0x%x (%s)' % (name, rva - a, obj)

def show_material(d, streams, frames, stack_start, stack_size, stack_rva):
    """For a crash inside SetD3DRendState: print the material and its textures."""
    read = memory_reader(d, streams)
    for ebp, prev, w in frames:
        if 'SetD3DRendState' not in w:
            continue
        # 'w' is a return address inside SetD3DRendState, so 'prev' is its frame
        mat, texclip = struct.unpack_from('<II', read(prev + 8, 8), 0)
        print('material 0x%08X  TexClip %d' % (mat, texclip))
        m = read(mat, 8 + 4 * 8) if read else None
        if not m:
            print('  (material memory not in dump)'); return
        inuse, texcnt = struct.unpack_from('<II', m, 0)
        handles = struct.unpack_from('<8I', m, 8)
        print('  InUse %d TextureCounter %d' % (inuse, texcnt))
        for i, h in enumerate(handles[:max(texcnt, 1)]):
            hb = read(h, 168)
            if not hb:
                print('  tex[%d] handle 0x%08X (not in dump)' % (i, h)); continue
            name = hb[0:64].split(b'\0')[0].decode('latin-1')
            namea = hb[64:128].split(b'\0')[0].decode('latin-1')
            d3dtex, dds, large, sysmem, wdt, hgt, used, usecnt = struct.unpack_from('<8I', hb, 128)
            print('  tex[%d] handle 0x%08X %r %r' % (i, h, name, namea))
            print('         lpD3DTexture 0x%08X lpDDSurface 0x%08X Large 0x%08X size %dx%d UseCounter %d' %
                  (d3dtex, dds, large, wdt, hgt, usecnt))
        return

def main():
    d, streams = read_dump(sys.argv[1])
    syms = load_map(sys.argv[2]) if len(sys.argv) > 2 else []
    mods = modules(d, streams)

    def where(addr):
        for b, s, n in mods:
            if b <= addr < b + s:
                short = n.split('\\')[-1]
                if short.lower().endswith('.exe') and syms:
                    return '%s!%s' % (short, lookup(syms, addr - b))
                return '%s+0x%x' % (short, addr - b)
        return None

    size, rva = streams[6]
    thread_id = struct.unpack_from('<I', d, rva)[0]
    code, flags = struct.unpack_from('<II', d, rva + 8)
    addr = struct.unpack_from('<Q', d, rva + 8 + 16)[0]
    nparams = struct.unpack_from('<I', d, rva + 8 + 24)[0]
    params = struct.unpack_from('<%dQ' % nparams, d, rva + 8 + 32) if nparams else ()
    print('exception 0x%08X at 0x%08X  %s' % (code, addr, where(addr)))
    if code == 0xE06D7363 and len(params) >= 3 and len(sys.argv) > 3:
        # C++ throw: params[2] is the ThrowInfo in the exe; walk it to the type name.
        exe = open(sys.argv[3], 'rb').read()
        pe = struct.unpack_from('<I', exe, 0x3C)[0]
        nsec = struct.unpack_from('<H', exe, pe + 6)[0]
        opt = struct.unpack_from('<H', exe, pe + 20)[0]
        base = struct.unpack_from('<I', exe, pe + 24 + 28)[0]
        secs = []
        for i in range(nsec):
            s = pe + 24 + opt + i * 40
            vsize, vaddr, rsize, rptr = struct.unpack_from('<IIII', exe, s + 8)
            secs.append((vaddr, max(vsize, rsize), rptr))
        def rd(va):
            rva = va - base
            for vaddr, sz, rptr in secs:
                if vaddr <= rva < vaddr + sz:
                    return rptr + rva - vaddr
            raise ValueError('va not in file')
        try:
            ti = rd(params[2])
            cta = struct.unpack_from('<I', exe, ti + 12)[0]
            n = struct.unpack_from('<I', exe, rd(cta))[0]
            for i in range(n):
                ct = struct.unpack_from('<I', exe, rd(cta) + 4 + 4 * i)[0]
                td = struct.unpack_from('<I', exe, rd(ct) + 4)[0]
                name = exe[rd(td) + 8: rd(td) + 200].split(b'\0')[0].decode('latin-1')
                print('  C++ exception type:', name)
        except Exception as e:
            print('  (could not decode C++ exception type: %s)' % e)
    if code == 0xC0000005 and len(params) >= 2:
        print('  access violation %s address 0x%08X' % (('reading', 'writing', '?', '?', '?', '?', '?', '?', 'executing')[params[0]] if params[0] < 9 else params[0], params[1]))

    # Walk the EBP frame chain from the exception context (accurate in Debug
    # builds, which keep frame pointers).
    ctx_size, ctx_rva = struct.unpack_from('<II', d, rva + 8 + 152)
    ebp, eip = struct.unpack_from('<II', d, ctx_rva + 0xB4)
    if 3 in streams:
        tsize, trva = streams[3]
        (n,) = struct.unpack_from('<I', d, trva)
        for i in range(n):
            t = trva + 4 + i * 48
            if struct.unpack_from('<I', d, t)[0] != thread_id: continue
            stack_start, stack_size, stack_rva = struct.unpack_from('<QII', d, t + 24)
            print('call chain (EBP walk):')
            frames = []
            read = memory_reader(d, streams)
            for depth in range(40):
                b = read(ebp, 8)
                if not b: break
                prev, ret = struct.unpack_from('<II', b, 0)
                w = where(ret) or '0x%08X' % ret
                print('  ', w)
                frames.append((ebp, prev, w))
                if prev <= ebp: break
                ebp = prev
            if '--material' in sys.argv:
                show_material(d, streams, frames, stack_start, stack_size, stack_rva)

    # crashing thread's stack: print anything that looks like a code address
    if 3 in streams and '--scan' in sys.argv:
        size, rva = streams[3]
        (n,) = struct.unpack_from('<I', d, rva)
        for i in range(n):
            t = rva + 4 + i * 48
            tid = struct.unpack_from('<I', d, t)[0]
            if tid != thread_id: continue
            stack_start, stack_size, stack_rva = struct.unpack_from('<QII', d, t + 24)
            stack = d[stack_rva:stack_rva + stack_size]
            print('stack (possible callers, innermost first):')
            shown = 0
            for off in range(0, len(stack) - 3, 4):
                v = struct.unpack_from('<I', stack, off)[0]
                w = where(v)
                if w and ('game.exe' in w or 'server.exe' in w):
                    print('  ', w)
                    shown += 1
                    if shown >= 25: break

main()
