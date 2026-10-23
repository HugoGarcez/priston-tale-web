"""Check a .smd map: face vertex/material indices vs counts, and materials.
Usage: python stagefaces.py <file.smd>"""
import struct, sys
HDR, STAGE = 556, 262260
OFS = dict(nVertex=262196, nFace=262200, nTexLink=262204, nLight=262208)
d = open(sys.argv[1], 'rb').read()
g = lambda k: struct.unpack_from('<i', d, HDR + OFS[k])[0]
nv, nf, nt, nl = g('nVertex'), g('nFace'), g('nTexLink'), g('nLight')
pos = HDR + STAGE
matcnt = struct.unpack_from('<I', d, pos + 8)[0]
pos += 88
mats = []
for i in range(matcnt):
    inuse, texcnt = struct.unpack_from('<II', d, pos)
    mats.append((inuse, texcnt))
    pos += 320
    if inuse:
        slen = struct.unpack_from('<i', d, pos)[0]
        pos += 4 + slen
print('verts %d faces %d texlinks %d lights %d materials %d' % (nv, nf, nt, nl, matcnt))
print('materials (InUse, TextureCounter) first 20:', mats[:20])
pos += nv * 28
bad_v = bad_m = 0
used = set()
for i in range(nf):
    a, b, c, m = struct.unpack_from('<4H', d, pos + i * 28 + 8)
    if max(a, b, c) >= nv: bad_v += 1
    if m >= matcnt: bad_m += 1
    used.add(m)
print('faces with vertex index >= nVertex:', bad_v, ' material index >= count:', bad_m)
unused_but_drawn = [m for m in sorted(used) if m < matcnt and not mats[m][0]]
print('materials used by faces:', len(used), ' of which InUse=0:', unused_but_drawn[:20])
print('used materials with TextureCounter>8:', [m for m in used if m < matcnt and mats[m][1] > 8][:10])
