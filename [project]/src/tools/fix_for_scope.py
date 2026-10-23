"""Fixes old Visual C++ for-loop scoping, guided by compiler errors.

Visual C++ 6/7 (and /Zc:forScope-) kept a variable declared in a for-statement
alive after the loop:      for( int i=0; ... ) {...}   x = i;
Standard C++ (and clang) end it at the loop. For each "use of undeclared
identifier 'i'" (or a related error) in web\\out\\check_errors.txt, this finds the
nearest earlier `for( TYPE i =` in the same file and moves the declaration in
front of the loop:         int i; for( i=0; ... )
which means the same thing in both compilers.

Usage: python tools\\fix_for_scope.py   (run web\\check_sources.py first; repeat both
until no more fixes)
"""
import os, re, sys, collections

SRC = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ERRORS = os.path.join(SRC, 'web', 'out', 'check_errors.txt')

err_re = re.compile(r"^(?P<file>.+?):(?P<line>\d+):(?P<col>\d+): error: (?P<msg>.*)$")
ident_msgs = [
    re.compile(r"use of undeclared identifier '(\w+)'"),
    re.compile(r"invalid use of non-static data member '(\w+)'"),
]
TYPE = r"(?:unsigned\s+|signed\s+|const\s+)?(?:int|long|short|char|DWORD|WORD|BYTE|UINT|BOOL|float|double|size_t|[A-Za-z_]\w*)(?:\s*\*)?"

def main():
    wanted = collections.defaultdict(set)      # file -> {(line, ident)}
    for l in open(ERRORS, encoding='utf-8', errors='replace'):
        m = err_re.match(l.strip())
        if not m:
            continue
        for r in ident_msgs:
            mm = r.search(m.group('msg'))
            if mm:
                wanted[os.path.normpath(m.group('file'))].add((int(m.group('line')), mm.group(1)))
    fixes = 0
    for path, uses in wanted.items():
        if 'thirdparty' in path or 'emsdk' in path or not os.path.exists(path):
            continue
        raw = open(path, 'rb').read()
        bom = raw.startswith(b'\xef\xbb\xbf')
        text = raw.decode('utf-8-sig')
        lines = text.split('\n')
        done = set()
        for line, ident in sorted(uses):
            pat = re.compile(r"\bfor\s*\(\s*(" + TYPE + r")\s+(" + re.escape(ident) + r")\s*=")
            for k in range(min(line, len(lines)) - 1, max(-1, line - 600), -1):
                m = pat.search(lines[k])
                if m:
                    if (k, ident) in done:
                        break
                    typ = m.group(1)
                    s = lines[k]
                    # The loop may be the body of an unbraced for/while/if/else: then the
                    # declaration must go above that statement, not become its body.
                    top = k
                    while True:
                        j = top - 1
                        while j >= 0 and (lines[j].strip() == '' or lines[j].strip().startswith('//')):
                            j -= 1
                        prev = re.sub(r'//.*', '', lines[j]).strip() if j >= 0 else ''
                        if prev and not prev.endswith((';', '{', '}', ':', '*/')) and not prev.startswith('#'):
                            top = j
                        else:
                            break
                    if top == k:
                        lines[k] = s[:m.start()] + '%s %s; for( %s =' % (typ, ident, ident) + s[m.end():]
                    else:
                        lines[k] = s[:m.start()] + 'for( %s =' % ident + s[m.end():]
                        indent = re.match(r'\s*', lines[top]).group(0)
                        lines[top] = indent + '%s %s;\n' % (typ, ident) + lines[top]
                    done.add((k, ident))
                    fixes += 1
                    break
        if done:
            out = '\n'.join(lines).encode('utf-8')
            open(path, 'wb').write((b'\xef\xbb\xbf' if bom else b'') + out)
            print('%3d  %s' % (len(done), os.path.relpath(path, SRC)))
    print('%d for-loop declarations moved' % fixes)

if __name__ == '__main__':
    main()
