"""Syntax-checks the game's sources with Emscripten (web build) and groups the errors.

Usage: python web\\check_sources.py [-j N] [--limit N] [file filter]
Writes web\\out\\check_report.txt (errors grouped by message, files by error count).
"""
import os, re, subprocess, sys, collections
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.dirname(HERE)
EMSDK = os.path.join(os.path.dirname(SRC), 'emsdk')
MINGW = os.path.join(SRC, 'thirdparty', 'mingw', 'include')
PTWIN = os.path.join(HERE, 'ptwin', 'include')

def emsdk_env():
    out = subprocess.run('call "%s" >nul 2>nul && set' % os.path.join(EMSDK, 'emsdk_env.bat'),
                         shell=True, capture_output=True, text=True).stdout
    env = dict(os.environ)
    for line in out.splitlines():
        if '=' in line:
            k, v = line.split('=', 1)
            env[k] = v
    return env

def sources():
    proj = open(os.path.join(SRC, 'PristonTale.vcxproj'), encoding='utf-8').read()
    return re.findall(r'<ClCompile Include="([^"]+)"', proj)

def main():
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    args = sys.argv[1:]
    jobs, limit, filt = os.cpu_count() or 8, 30, None
    while args:
        a = args.pop(0)
        if a == '-j': jobs = int(args.pop(0))
        elif a == '--limit': limit = int(args.pop(0))
        else: filt = a.lower()
    files = [f for f in sources() if not filt or filt in f.lower()]
    env = emsdk_env()
    emxx = os.path.join(EMSDK, 'upstream', 'emscripten', 'em++.exe')
    flags = open(os.path.join(HERE, 'emflags.txt')).read().split()
    incs = ['-I' + PTWIN, '-I' + SRC] + ['-I' + os.path.join(SRC, d) for d in
            ['smLib3d', 'sinbaram', 'HoBaram', r'HoBaram\NewEffect\HoEffect\Script']] + ['-idirafter', MINGW]

    def check(f):
        cmd = [emxx, '-fsyntax-only', '-ferror-limit=%d' % limit, '-Wno-everything'] + flags + incs + \
              ['-include', os.path.join(SRC, 'compat.h'), os.path.join(SRC, f)]
        r = subprocess.run(cmd, capture_output=True, text=True, errors='replace', env=env, cwd=SRC)
        errs = [l for l in r.stderr.splitlines() if ': error:' in l or 'fatal error:' in l]
        return f, errs

    by_msg = collections.Counter()
    example = {}
    per_file = []
    all_errors = []
    with ThreadPoolExecutor(jobs) as ex:
        for i, (f, errs) in enumerate(ex.map(check, files)):
            per_file.append((len(errs), f))
            all_errors.extend(errs)
            for e in errs:
                msg = e.split('error:', 1)[1].strip()
                key = msg if 'file not found' in msg else re.sub(r"'[^']*'", "'X'", msg)
                by_msg[key] += 1
                example.setdefault(key, e)
            print('%3d/%d %4d %s' % (i + 1, len(files), len(errs), f), flush=True)

    os.makedirs(os.path.join(HERE, 'out'), exist_ok=True)
    with open(os.path.join(HERE, 'out', 'check_report.txt'), 'w', encoding='utf-8') as fp:
        clean = sum(1 for n, _ in per_file if n == 0)
        fp.write('%d files, %d without errors, %d errors (first %d per file)\n\n' % (len(per_file), clean, sum(n for n, _ in per_file), limit))
        fp.write('== errors by kind ==\n')
        for key, n in by_msg.most_common():
            fp.write('%5d  %s\n       e.g. %s\n' % (n, key, example[key]))
        fp.write('\n== files ==\n')
        for n, f in sorted(per_file, reverse=True):
            fp.write('%5d  %s\n' % (n, f))
    # every error with its location (input for tools\fix_for_scope.py)
    with open(os.path.join(HERE, 'out', 'check_errors.txt'), 'w', encoding='utf-8') as fp:
        fp.write('\n'.join(sorted(set(all_errors))) + '\n')
    print('report: web\\out\\check_report.txt  (%d of %d files clean)' % (clean, len(per_file)))

if __name__ == '__main__':
    main()
