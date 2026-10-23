"""Walk a .smd map file and test whether its area index uses WORD or DWORD entries.

Usage: python stagelayout.py <file.smd> [more files...]
Layout (offsets from structinfo.exe): header 556 | smSTAGE3D 262260 |
material group 88 + materials (320 each, + int StrLen + names when InUse) |
vertices 28*n | faces 28*n | texlinks 32*n | lights 28*n | area chunks
(for every non-null StageArea[x][z]: int count + count entries).
"""
import struct, sys

HDR, STAGE = 556, 262260
OFS = dict(nVertex=262196, nFace=262200, nTexLink=262204, nLight=262208, wAreaSize=262240, StageArea=4)
MAP = 256

def walk(path):
    d = open(path, 'rb').read()
    st = HDR
    g = lambda name: struct.unpack_from('<i', d, st + OFS[name])[0]
    nv, nf, nt, nl, area = g('nVertex'), g('nFace'), g('nTexLink'), g('nLight'), g('wAreaSize')
    ptrs = struct.unpack_from('<%dI' % (MAP * MAP), d, st + OFS['StageArea'])
    pos = HDR + STAGE
    matcnt = struct.unpack_from('<I', d, pos + 8)[0]
    pos += 88
    for i in range(matcnt):
        inuse = struct.unpack_from('<I', d, pos)[0]
        pos += 320
        if inuse:
            slen = struct.unpack_from('<i', d, pos)[0]
            pos += 4 + slen
    pos += nv * 28 + nf * 28 + nt * 32 + max(nl, 0) * 28
    area_start = pos
    print('   area data starts at file pos %d (texlinks %d lights %d)' % (area_start, nt, nl))
    p = area_start
    for k, i in enumerate(i for i in range(MAP * MAP) if ptrs[(i % MAP) * MAP + i // MAP]):
        if k >= 3: break
        n = struct.unpack_from('<i', d, p)[0]
        print('     cell %d,%d count %d at pos %d' % (i % MAP, i // MAP, n, p))
        p += 4 + n * 4
    results = {}
    for width in (2, 4):
        p = area_start
        total = 0
        ok = True
        for cnt2 in range(MAP):
            for cnt in range(MAP):
                if ptrs[cnt * MAP + cnt2]:
                    if p + 4 > len(d):
                        ok = False; break
                    n = struct.unpack_from('<i', d, p)[0]
                    if n < 0 or n > nf:
                        ok = False; break
                    p += 4 + n * width
                    total += n
            if not ok: break
        results[width] = (ok, p == len(d), total, len(d) - p)
    print('%s: verts %d faces %d mats %d wAreaSize %d' % (path, nv, nf, matcnt, area))
    for w, (ok, exact, total, left) in results.items():
        print('   %s entries: walk ok=%s, ends exactly at EOF=%s, entries=%d, bytes left=%d' %
              ('WORD ' if w == 2 else 'DWORD', ok, exact, total, left))

for f in sys.argv[1:]:
    walk(f)
