"""Compares struct layouts between the Windows build (MSVC, x86) and the web build
(clang/Emscripten, wasm32), for the structs that travel between the game and
server.exe. A web client talking to a Windows server.exe needs them identical.

python tools\\compare_layouts.py [file.cpp ...]   (default: the networking sources)
Writes build\\layouts_msvc.txt, build\\layouts_wasm.txt and prints differences of
structs whose names look like packets (TRANS / smTRANS / sTRANS / PACKET / rsPLAY...).
"""
import os, re, shutil, subprocess, sys

SRC = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(SRC, 'web'))
import build_web

DEFAULT_TUS = ['netplay.cpp', 'record.cpp', 'Damage.cpp', 'playmain.cpp', 'Winmain.cpp', 'character.cpp',
               'smwsock.cpp', r'sinbaram\sinInvenTory.cpp', r'sinbaram\sinTrade.cpp', r'SrcServer\OnSever.cpp',
               'playsub.cpp', 'field.cpp', 'AreaServer.cpp']

def vcvars():
    vswhere = os.path.expandvars(r'%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe')
    inst = subprocess.run([vswhere, '-latest', '-property', 'installationPath'], capture_output=True, text=True).stdout.strip()
    bat = os.path.join(inst, r'VC\Auxiliary\Build\vcvars32.bat')
    out = subprocess.run('call "%s" >nul && set' % bat, shell=True, capture_output=True, text=True).stdout
    env = dict(os.environ)
    for l in out.splitlines():
        if '=' in l:
            k, v = l.split('=', 1); env[k] = v
    return env

def msvc_layouts(tu, env):
    defs = ['PT_GFX_GL', 'WIN32', '_WINDOWS', '_CRT_SECURE_NO_WARNINGS', '_CRT_NON_CONFORMING_SWPRINTFS',
            '_WINSOCK_DEPRECATED_NO_WARNINGS', '_USE_32BIT_TIME_T', 'DIRECTINPUT_VERSION=0x0700',
            'DIRECT3D_VERSION=0x0600', 'DIRECTDRAW_VERSION=0x0600', 'NDEBUG', 'PT_USE_SDL']
    incs = [SRC] + [os.path.join(SRC, d) for d in ['smLib3d', 'sinbaram', 'HoBaram', r'HoBaram\NewEffect\HoEffect\Script',
                                                   r'thirdparty\SDL2\include']]
    cl = shutil.which('cl', path=env.get('PATH') or env.get('Path'))
    cmd = [cl, '/nologo', '/c', '/Zs', '/d1reportAllClassLayout', '/source-charset:utf-8', '/execution-charset:.949',
           '/Zc:strictStrings-', '/EHsc'] + ['/D' + d for d in defs] + ['/I' + i for i in incs] + [os.path.join(SRC, tu)]
    r = subprocess.run(cmd, capture_output=True, text=True, errors='replace', env=env, cwd=SRC)
    res = {}
    cur = None
    for line in r.stdout.splitlines():
        m = re.match(r'^(?:class|struct|union)\s+(\S+)\s+size\((\d+)\):', line)
        if m:
            cur = m.group(1); res[cur] = [int(m.group(2)), []]; depth = 0; continue
        if cur is None:
            continue
        if line.strip().startswith('+---'):
            depth += 1
            if depth >= 2: cur = None
            continue
        m = re.match(r'^\s*(\d+)\s*\|\s(\S.*)$', line)		# top level members only: " 4	| code"
        if m and not m.group(2).startswith('|') and not m.group(2).startswith(' '):
            res[cur][1].append((int(m.group(1)), m.group(2).split()[-1].rstrip(';')))
    return res, r.returncode

def wasm_layouts(tu, env):
    flags = open(os.path.join(SRC, 'web', 'emflags.txt')).read().split()
    incs = ['-I' + os.path.join(SRC, 'web', 'ptwin', 'include'), '-I' + SRC] + ['-I' + os.path.join(SRC, d) for d in
            ['smLib3d', 'sinbaram', 'HoBaram', r'HoBaram\NewEffect\HoEffect\Script']] + ['-idirafter', build_web.MINGW]
    emxx = os.path.join(build_web.EMSDK, 'upstream', 'emscripten', 'em++.exe')
    cmd = [emxx, '-fsyntax-only', '-Wno-everything', '-Xclang', '-fdump-record-layouts-complete'] + flags + incs + \
          ['-include', os.path.join(SRC, 'compat.h'), os.path.join(SRC, tu)]
    r = subprocess.run(cmd, capture_output=True, text=True, errors='replace', env=env, cwd=SRC)
    res = {}
    lines = r.stdout.splitlines()
    i = 0
    while i < len(lines):
        if lines[i].startswith('*** Dumping AST Record Layout'):
            m = re.match(r'^\s*0 \| (?:class|struct|union) (\S+)', lines[i + 1])
            name = m.group(1) if m else None
            members = []
            j = i + 2
            while j < len(lines) and '[sizeof=' not in lines[j]:
                m2 = re.match(r'^\s*(\d+)(?::\S+)? \|   (\S.*)$', lines[j])		# exactly one level of indent
                if m2 and not m2.group(2).startswith(' '):
                    members.append((int(m2.group(1)), m2.group(2).split()[-1]))
                j += 1
            sz = re.search(r'sizeof=(\d+)', lines[j]) if j < len(lines) else None
            if name and sz:
                res[name] = [int(sz.group(1)), members]
            i = j
        i += 1
    return res, r.returncode

def game_struct_names():
    # struct/class/union names declared in the game's own sources (not SDK or third-party)
    names = set()
    rx = re.compile(r'\b(?:struct|class|union)\s+([A-Za-z_]\w*)\s*(?::[^{;]*)?\{|\}\s*([A-Za-z_]\w*)\s*[,;]')
    for d, ds, fs in os.walk(SRC):
        rel = os.path.relpath(d, SRC).lower()
        if rel.startswith(('thirdparty', 'web', 'build', 'bin', 'tools')):
            ds[:] = []; continue
        for f in fs:
            if f.lower().endswith(('.h', '.hpp', '.cpp')):
                t = open(os.path.join(d, f), encoding='utf-8', errors='replace').read()
                for m in rx.finditer(t):
                    names.add(m.group(1) or m.group(2))
    return names

def main():
    tus = sys.argv[1:] or DEFAULT_TUS
    venv, eenv = vcvars(), build_web.emsdk_env()
    msvc, wasm = {}, {}
    for tu in tus:
        a, ra = msvc_layouts(tu, venv); b, rb = wasm_layouts(tu, eenv)
        print('%-28s msvc %4d structs (rc %d)   wasm %4d structs (rc %d)' % (tu, len(a), ra, len(b), rb))
        msvc.update(a); wasm.update(b)
    os.makedirs(os.path.join(SRC, 'build'), exist_ok=True)
    for name, d in (('msvc', msvc), ('wasm', wasm)):
        with open(os.path.join(SRC, 'build', 'layouts_%s.txt' % name), 'w') as f:
            for k in sorted(d):
                f.write('%s %d %s\n' % (k, d[k][0], ' '.join('%d:%s' % m for m in d[k][1])))
    pat = re.compile(r'TRANS|PACKET|PLAYINFO|smCHAR_INFO|sITEM|sITEMINFO|RECORD|GAMESAVE|smCHAR_|_INFO$', re.I)
    common = sorted(set(msvc) & set(wasm) & game_struct_names())
    diffs = []
    for k in common:
        if msvc[k][0] != wasm[k][0] or [o for o, _ in msvc[k][1]] != [o for o, _ in wasm[k][1]][:len(msvc[k][1])]:
            diffs.append(k)
    print('%d structs in both, %d differ' % (len(common), len(diffs)))
    for k in diffs:
        tag = 'PACKET-LIKE ' if pat.search(k) else ''
        print('  %s%s: msvc %d, wasm %d' % (tag, k, msvc[k][0], wasm[k][0]))
        ma, wa = dict((n, o) for o, n in msvc[k][1]), dict((n, o) for o, n in wasm[k][1])
        for n in list(ma)[:400]:
            if n in wa and ma[n] != wa[n]:
                print('      first moved member: %s msvc +%d wasm +%d' % (n, ma[n], wa[n])); break

if __name__ == '__main__':
    main()
