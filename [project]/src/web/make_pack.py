"""Bundles the game's start-up data files into one download: web\\out\\data_startup.pack.

The browser build fetches each data file the first time the game opens it; at
start-up that is ~2000 small files (sounds, interface images), far too many single
requests. The page loads this pack first (web\\pre.js) and unpacks it into the
browser file system, so the game finds them already there.

Input: the order the game asked for files, web\\out\\data_access.log (written by
web\\serve.py while the game runs), or a file given on the command line.

Pack format (little endian): "PTPK", u32 count, then per file:
u16 path length, path (UTF-8, relative to the game folder), u32 size, data.

Usage: python web\\make_pack.py [list file] [--limit-mb N]
"""
import os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
GAME = os.path.join(os.path.dirname(os.path.dirname(HERE)), 'game')

def main():
    args = [a for i, a in enumerate(sys.argv[1:], 1) if not a.startswith('--') and sys.argv[i - 1] not in ('--out', '--limit-mb')]
    limit = 200
    if '--limit-mb' in sys.argv:
        limit = int(sys.argv[sys.argv.index('--limit-mb') + 1])
    src = args[0] if args else os.path.join(HERE, 'out', 'data_access.log')
    names = []
    seen = set()
    for line in open(src, encoding='utf-8'):
        rel = line.strip().replace('\\', '/')
        if rel and rel not in seen:
            seen.add(rel)
            names.append(rel)
    name = sys.argv[sys.argv.index('--out') + 1] if '--out' in sys.argv else 'data_startup.pack'
    out = os.path.join(HERE, 'out', name)
    total = 0
    count = 0
    with open(out, 'wb') as fp:
        fp.write(b'PTPK' + struct.pack('<I', 0))
        for rel in names:
            p = os.path.join(GAME, rel)
            if not os.path.isfile(p):
                continue
            data = open(p, 'rb').read()
            if total + len(data) > limit * 1024 * 1024:
                print('size limit reached at %s' % rel)
                break
            b = rel.encode('utf-8')
            fp.write(struct.pack('<H', len(b)) + b + struct.pack('<I', len(data)) + data)
            total += len(data)
            count += 1
        fp.seek(4)
        fp.write(struct.pack('<I', count))
    print('%d files, %.1f MB -> %s' % (count, total / 1e6, out))

if __name__ == '__main__':
    main()
