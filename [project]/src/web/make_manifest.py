"""Lists the game data for the browser build: web\\out\\data_manifest.txt.

One line per file: "<path relative to PristonWeb\\game, forward slashes>\\t<size>\\t<hash>".
The browser loads this list at start-up and fetches a file from /data/<path>
the first time the game opens it (see web\\ptwin\\src\\ptwin_fs.cpp). The hash
(of the file's contents) tells the page's service worker (web\\sw.js) whether its
stored copy is still current; the game itself only reads path and size.

Usage: python web\\make_manifest.py [game folder] [output file]
  e.g. after changing files in the IIS site's data folder:
  python web\\make_manifest.py C:\\inetpub\\site\\data C:\\inetpub\\site\\data_manifest.txt
"""
import hashlib, os, sys

HERE = os.path.dirname(os.path.abspath(__file__))

SKIP_EXT = ('.exe', '.dll', '.pdb', '.dmp', '.log', '.lib', '.ilk')
SKIP_PREFIX = ('ptgfx_frame', 'ptgfx_trace', 'disconnect')
SKIP_NAMES = ('web.config',)            # IIS settings in the site's data folder

def file_hash(path):
    h = hashlib.blake2b(digest_size=8)
    with open(path, 'rb') as fp:
        for chunk in iter(lambda: fp.read(1 << 20), b''):
            h.update(chunk)
    return h.hexdigest()

def main():
    game = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(os.path.dirname(HERE)), 'game')
    out = sys.argv[2] if len(sys.argv) > 2 else os.path.join(HERE, 'out', 'data_manifest.txt')
    lines = []
    total = 0
    for d, dirs, files in os.walk(game):
        dirs.sort()
        for f in sorted(files):
            if f.lower().endswith(SKIP_EXT) or f.lower().startswith(SKIP_PREFIX) or f.lower() in SKIP_NAMES:
                continue
            p = os.path.join(d, f)
            rel = os.path.relpath(p, game).replace(os.sep, '/')
            size = os.path.getsize(p)
            total += size
            lines.append('%s\t%d\t%s' % (rel, size, file_hash(p)))
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, 'w', encoding='utf-8', newline='\n') as fp:
        fp.write('\n'.join(lines) + '\n')
    print('%d files, %.1f GB -> %s' % (len(lines), total / 1e9, out))

if __name__ == '__main__':
    main()
