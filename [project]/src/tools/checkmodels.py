"""Check .smd/.smb model files against the smOBJ3D layout our loader expects.

Usage: python checkmodels.py <folder> [more folders...]
Each object in a model file is a raw smOBJ3D (2236 bytes, see structinfo.exe)
followed by its arrays. For every object we compare the size the file says the
object takes (smDFILE_OBJINFO.Length) with the size our layout implies.
"""
import os, struct, sys
from collections import Counter

OBJ = 2236
OFF_NV, OFF_NF, OFF_NT = 84, 88, 92
OFF_PHYS = 16
OFF_ROT, OFF_POS, OFF_SCL = 684, 688, 692
HDR = 556

def expected(nv, nf, nt, nr, np_, ns, phys):
    return OBJ + nv * 24 + nf * 36 + nt * 32 + nr * 20 + np_ * 16 + ns * 16 + nr * 64 + (nv * 32 if phys else 0)

def check(path):
    d = open(path, 'rb').read()
    if not d.startswith(b'SMD Model data'):
        return None
    objcnt, matcnt, matpt, firstinfo = struct.unpack_from('<iiii', d, 24)
    if not (0 < objcnt < 1000):
        return ('bad header', objcnt)
    results = []
    for i in range(objcnt):
        info = HDR + i * 40
        length, fpos = struct.unpack_from('<ii', d, info + 32)
        nv, nf, nt = struct.unpack_from('<iii', d, fpos + OFF_NV)
        phys = struct.unpack_from('<I', d, fpos + OFF_PHYS)[0]
        nr, np_, ns = struct.unpack_from('<iii', d, fpos + OFF_ROT)
        results.append(length - expected(nv, nf, nt, nr, np_, ns, phys))
    return results

def main():
    diffs = Counter()
    bad = []
    total = 0
    for root in sys.argv[1:]:
        for dp, dn, fn in os.walk(root):
            for f in fn:
                if not f.lower().endswith(('.smd', '.smb')):
                    continue
                p = os.path.join(dp, f)
                try:
                    r = check(p)
                except Exception as e:
                    r = ('error', str(e))
                if r is None:
                    continue
                total += 1
                if isinstance(r, tuple):
                    bad.append((p, r)); continue
                for x in r:
                    diffs[x] += 1
                if any(x != 0 for x in r):
                    bad.append((p, sorted(set(r))))
    print('model files checked:', total)
    print('object size difference (file - expected) -> count:', diffs.most_common(10))
    print('files that do not match:', len(bad))
    for p, r in bad[:15]:
        print('  ', p, r)

main()
