"""Rewrites non-ASCII characters inside C/C++ string and character literals as CP949
octal escapes, in place.

Why: Visual Studio compiles the game with /execution-charset:.949, so a Korean
literal like "*배경화면" becomes CP949 bytes - the bytes the data files and
network protocol expect. clang (the web build) only writes UTF-8. Escaping the
literals gives both compilers the identical CP949 bytes. Comments, #include and
#pragma lines are left alone (file names stay as they are on disk).

Usage: python tools\\escape_cp949_literals.py [--dry-run]   (from the src folder)
"""
import os, sys

SKIP_DIRS = {'thirdparty', 'web', 'tools', 'bin', 'build', '.claude'}
EXTS = ('.cpp', '.c', '.h', '.hpp', '.inl')

def escape_literal(body, path, lineno, problems):
    out = []
    for ch in body:
        if ord(ch) < 128:
            out.append(ch)
            continue
        try:
            b = ch.encode('cp949')
        except UnicodeEncodeError:
            problems.append('%s:%d: %r not in CP949' % (path, lineno, ch))
            out.append(ch)
            continue
        out.append(''.join('\\%03o' % x for x in b))
    return ''.join(out)

def process(text, path, problems):
    res = []
    i, n, line = 0, len(text), 1
    at_line_start = True
    skip_line = False          # inside #include / #pragma
    changed = 0
    while i < n:
        c = text[i]
        if c == '\n':
            line += 1; at_line_start = True; skip_line = False
            res.append(c); i += 1; continue
        if at_line_start and c in ' \t﻿':
            res.append(c); i += 1; continue
        if at_line_start:
            at_line_start = False
            if c == '#':
                rest = text[i+1:i+40].lstrip()
                if rest.startswith('include') or rest.startswith('pragma') or rest.startswith('import'):
                    skip_line = True
        if c == '/' and text.startswith('//', i):
            j = text.find('\n', i)
            j = n if j < 0 else j
            res.append(text[i:j]); i = j; continue
        if c == '/' and text.startswith('/*', i):
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            seg = text[i:j]
            line += seg.count('\n')
            res.append(seg); i = j; continue
        if c in '"\'':
            # literal: find the closing quote (handle escapes; stop at end of line)
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                j += 2 if text[j] == '\\' else 1
            body = text[i+1:j]
            if not skip_line and any(ord(ch) >= 128 for ch in body):
                body = escape_literal(body, path, line, problems)
                changed += 1
            res.append(c + body)
            i = j
            if i < n and text[i] == c:
                res.append(c); i += 1
            continue
        res.append(c); i += 1
    return ''.join(res), changed

def main():
    dry = '--dry-run' in sys.argv
    root = os.getcwd()
    total_files = total_lits = 0
    problems = []
    for d, dirs, files in os.walk(root):
        rel = os.path.relpath(d, root)
        if rel.split(os.sep)[0] in SKIP_DIRS:
            dirs[:] = []
            continue
        for f in files:
            if not f.lower().endswith(EXTS):
                continue
            p = os.path.join(d, f)
            raw = open(p, 'rb').read()
            try:
                text = raw.decode('utf-8')
            except UnicodeDecodeError:
                problems.append('%s: not UTF-8, skipped' % p)
                continue
            if all(b < 128 for b in raw):
                continue
            new, changed = process(text, os.path.relpath(p, root), problems)
            if changed:
                total_files += 1
                total_lits += changed
                if not dry:
                    open(p, 'wb').write(new.encode('utf-8'))
    print('%s %d literals in %d files' % ('would escape' if dry else 'escaped', total_lits, total_files))
    for pr in problems[:50]:
        print('  ' + pr)
    if len(problems) > 50:
        print('  ... %d more' % (len(problems) - 50))

if __name__ == '__main__':
    main()
