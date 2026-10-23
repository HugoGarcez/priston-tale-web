"""Check .smd map (stage) files against the smSTAGE3D size our loader expects.

Usage: python checkstages.py <Field folder> [<expected sizeof smSTAGE3D>]
A stage file is: header (556 bytes), a raw smSTAGE3D, the material group
(88 bytes), materials (320 bytes each, plus texture names for used ones)...
We find the first texture name and work back to the smSTAGE3D size.
"""
import glob, re, struct, sys, collections

root = sys.argv[1]
expect = int(sys.argv[2]) if len(sys.argv) > 2 else 262260
name_re = re.compile(rb'[A-Za-z0-9_\\\-\.]+\.(?:bmp|tga|jpg|dds)\x00', re.I)

sizes = collections.Counter()
examples = {}
for f in glob.glob(root + '/**/*.smd', recursive=True):
    d = open(f, 'rb').read()
    if not d.startswith(b'SMD Stage data'):
        continue
    m = name_re.search(d, 556)
    if not m:
        continue
    strlen_ofs = m.start() - 4
    strlen = struct.unpack_from('<i', d, strlen_ofs)[0]
    # The material before the name is in use; count unused ones before it.
    best = None
    for k in range(0, 8):
        s = strlen_ofs - 556 - 88 - 320 * (k + 1)
        if s == expect:
            best = s
            break
    if best is None:
        best = strlen_ofs - 556 - 88 - 320   # assume first material in use
    sizes[best] += 1
    examples.setdefault(best, (f, strlen))

print('expected sizeof(smSTAGE3D):', expect)
for s, n in sizes.most_common(10):
    print('  %8d bytes  x%d   e.g. %s (StrLen %d)' % (s, n, examples[s][0], examples[s][1]))
