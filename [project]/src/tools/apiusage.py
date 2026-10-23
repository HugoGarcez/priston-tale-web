"""Count platform-specific API use in the files the game project compiles.
Usage: python apiusage.py   (run from the Claude folder)"""
import re, collections
proj = open('PristonTale.vcxproj', encoding='utf-8').read()
files = re.findall(r'<Cl(?:Compile|Include) Include="([^"]+)"', proj)
cats = {
    'Direct3D 6 device':      r'lpD3DDevice->|IDirect3DDevice3|LPDIRECT3DTEXTURE2|IDirect3D3',
    'DirectDraw':             r'lpDD\w*->|IDirectDraw|LPDIRECTDRAWSURFACE4|DDSURFACEDESC2',
    'GDI text/drawing':       r'->GetDC\(|\bTextOut\(|dsTextLineOut\(|SetTextColor\(|SelectObject\(|GetTextExtentPoint32\(',
    '2D image draw helpers':  r'\b(?:Ho|ds|sin)?DrawTexImage\w*\(|\bdsDrawImage\w*\(',
    'DirectSound':            r'DirectSound|IDirectSoundBuffer|LPDIRECTSOUND',
    'Win32 window/messages':  r'\b(?:CreateWindow|SendMessage|PostMessage|PeekMessage|GetMessage|SetWindowText|GetWindowText|DefWindowProc)\w*\(',
    'Sockets (Winsock)':      r'\b(?:send|recv|closesocket|WSA\w+|select|accept|bind|listen)\s*\(',
    'Threads/locks':          r'\b(?:CreateThread|EnterCriticalSection|SuspendThread|ResumeThread)\s*\(',
    'Win32 file API':         r'\b(?:CreateFile|ReadFile|WriteFile|FindFirstFile)\w*\s*\(',
    'Registry':               r'\bReg(?:Open|Query|Set|Create)\w*\(',
}
tot = {k: 0 for k in cats}
perfile = {k: collections.Counter() for k in cats}
for f in files:
    try:
        t = open(f.replace('\\', '/'), encoding='utf-8-sig', errors='replace').read()
    except OSError:
        continue
    t = re.sub(r'//[^\n]*', '', t)                      # ignore line comments
    t = re.sub(r'/\*.*?\*/', '', t, flags=re.S)         # and block comments
    for k, p in cats.items():
        n = len(re.findall(p, t))
        if n:
            tot[k] += n
            perfile[k][f] += n
print('%-24s %6s %6s   top files' % ('category', 'uses', 'files'))
for k in cats:
    top = ', '.join('%s(%d)' % (f.split('\\')[-1], n) for f, n in perfile[k].most_common(4))
    print('%-24s %6d %6d   %s' % (k, tot[k], len(perfile[k]), top))
