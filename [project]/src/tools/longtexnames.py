"""List texture names of 64+ characters in .smd/.smb files (they overflow
smTEXTUREHANDLE.Name/NameA, which are char[64]).
Usage: python longtexnames.py <folder> [more folders]"""
import os, re, sys, collections
name_re = re.compile(rb'[\x21-\x7e]{40,}\.(?:bmp|tga|jpg|dds)', re.I)
lengths = collections.Counter()
longest = []
for root in sys.argv[1:]:
    for dp, dn, fn in os.walk(root):
        for f in fn:
            if not f.lower().endswith(('.smd', '.smb')):
                continue
            d = open(os.path.join(dp, f), 'rb').read()
            for m in name_re.finditer(d):
                n = m.group()
                if len(n) >= 64:
                    longest.append((len(n), os.path.join(dp, f), n.decode('latin-1')))
longest.sort(reverse=True)
print('texture names >= 64 chars:', len(longest))
for l, f, n in longest[:15]:
    print(' ', l, f, n)
