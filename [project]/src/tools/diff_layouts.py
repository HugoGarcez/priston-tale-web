"""Differences between build\\layouts_msvc.txt and build\\layouts_wasm.txt (written by
tools\\compare_layouts.py) for structs declared in the game's own sources."""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import compare_layouts as c

def load(name):
    d = {}
    for line in open(os.path.join(c.SRC, 'build', 'layouts_%s.txt' % name)):
        parts = line.split()
        if len(parts) < 2: continue
        d[parts[0]] = (int(parts[1]), [(int(x.split(':', 1)[0]), x.split(':', 1)[1]) for x in parts[2:]])
    return d

msvc, wasm = load('msvc'), load('wasm')
names = c.game_struct_names()
common = sorted(set(msvc) & set(wasm) & names)
diff = [k for k in common if msvc[k][0] != wasm[k][0] or msvc[k][1] != wasm[k][1]]
print('%d game structs in both builds, %d differ' % (len(common), len(diff)))
for k in diff:
    print('  %s: msvc %d bytes, wasm %d bytes' % (k, msvc[k][0], wasm[k][0]))
    mo, wo = dict((n, o) for o, n in msvc[k][1]), dict((n, o) for o, n in wasm[k][1])
    for n, o in msvc[k][1]:
        if n in wo and wo[n] != o:
            print('      first moved member: %s (msvc +%d, wasm +%d)' % (n, o, wo[n])); break
    else:
        if set(mo) != set(wo): print('      members differ: %s' % sorted(set(mo) ^ set(wo))[:8])
