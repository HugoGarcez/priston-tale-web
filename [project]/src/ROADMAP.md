# Roadmap: Priston Tale in the browser

Goal: the game client running in a web browser (WebAssembly + WebGL2), talking
to our server, with the Windows build getting faster along the way.

## What the code uses today (measured with `tools\apiusage.py`)

| Area | Use | Notes |
|---|---|---|
| Direct3D 6 device | ~1000 calls, 45 files | Only **14 methods**; mostly SetRenderState / SetTextureStageState, plus DrawPrimitive |
| DirectDraw | ~870 uses, 62 files | ~24 methods; mostly GetDC/ReleaseDC for text, CreateSurface, Blt, Lock |
| GDI text | ~1500 calls, 43 files | TextOut / GetTextExtentPoint32 / SetTextColor / SelectObject on surface DCs |
| 2D image helpers | ~590 calls | DrawTexImage-style wrappers (already go through D3D) |
| Win32 window/input | ~240 calls | WndProc message handling |
| DirectSound | 27 calls, 2 files | small |
| Winsock | ~110 calls, mostly smwsock.cpp | |

Key facts:
- **Vertices are already transformed** (`D3DTLVERTEX`, `XYZRHW`): the engine does
  its own 3D math in software. The GPU side only draws screen-space triangles
  with textures, so emulation is simple.
- Render states used: blend (SRCALPHA/INVSRCALPHA/ONE/ZERO/SRCCOLOR...), depth
  (LESSEQUAL), cull, alpha test, colour key, linear fog, up to 8 texture stages
  with MODULATE / ADD / SELECTARG / ADDSIGNED / SUBTRACT. All of it fits WebGL2.
- Browsers run **32-bit** WebAssembly, so the web build does not need the 64-bit
  conversion (which is hard: model/map files store raw 32-bit pointers).

## Approach: imitate the old APIs instead of rewriting 3,400 call sites

Write our own small versions of the DirectDraw / Direct3D 6 objects the game
calls (`IDirectDraw4`, `IDirectDrawSurface4`, `IDirect3DDevice3`,
`IDirect3DTexture2`), implemented on **OpenGL 3.3 / GLES 3** (the same code
becomes WebGL2 in the browser). Game code keeps calling `lpD3DDevice->...` and
barely changes.

- Surfaces keep a CPU copy of their pixels (Lock, Blt, GetDC work on it);
  textures are uploaded to the GPU when changed.
- `GetDC` on the back buffer returns a transparent text overlay instead of
  locking the GPU, composited at the end of the frame. Text drawing no longer
  stalls the GPU (bigger FPS win than the hold-A fix).
- Texture-stage states become a small generated shader.

## Status

- 2026-10-18: Milestone 1 first version written: `ptgfx\` (OpenGL 3.3 core,
  WebGL2-compatible subset). `build.bat Release GL` builds it;
  `install_build.ps1` copies it into `..\game\` (data from `..\import_v3215_data.ps1`). `tools\ptgfx_selftest.bat`
  draws through it in a hidden window and checks pixels (all 21 checks pass).
  Not yet tested in the real game.
- 2026-10-18: **Milestone 1 done.** The GL build runs the real game: login,
  server select, character select/creation and in-game all render correctly,
  and it loads into the game fast. Self-test: 31 checks pass. Fixes along the way:
  CS_OWNDC window class, viewport Clear2, and DirectX 5 TEXTUREMAPBLEND MODULATE
  alpha = texture alpha x vertex alpha (the login fade quad covered the screen).
  Diagnostics still in: `ptgfx.log`, frame BMP snapshots, one-frame `ptgfx_trace.txt`
  (frame 300) - remove or put behind an ini switch before release.
- 2026-10-18: **Milestone 2 started** (GL build; SDL2 2.30.9 and stb_truetype in `thirdparty\`).
  - Text: `ptgfx\ptgfx_text.cpp` draws TextOut / DrawText / GetTextExtentPoint32 /
    GetTextMetrics with stb_truetype (antialiased, Tahoma from `fonts\` or Windows\Fonts)
    by patching game.exe's imports - no game code changed. Non-DIB DCs fall back to GDI.
  - Sound: `ptsound\ptsound.cpp` emulates DirectSound (buffers, Lock, Play, volume/pan/
    frequency, play cursor for BGM streaming) on one SDL audio mixer. game.exe no longer
    uses dsound.dll. `tools\ptsound_selftest.bat`: 13 checks pass.
  - Window/input: `ptplatform\ptplatform.cpp`, opt-in with `*PLATFORM SDL` in hotuk.ini.
    SDL attaches to the game window, owns the GL context and input; SDL events become
    the WM_ messages WndProc expects (raw Win32 input filtered). Edit controls, timers
    and socket messages stay Win32 for now. `tools\ptplatform_selftest.bat`: 18 checks pass.
  - Play-tested 2026-10-18 with `*PLATFORM SDL`: text, sound, keyboard and mouse all work.
    **Milestone 2 done on Windows.**
  - Left for the browser build (Milestone 3): our own edit-control/IME replacement,
    GetKeyState, timers, MessageBox, WSAAsyncSelect -> WebSocket, mmio/timeSetEvent in Dxwav.
- 2026-10-18: **Milestone 3 started.** Emscripten SDK (latest) in `..\emsdk`.
  `web\build_hello.bat` + `web\serve.py` (local server with the COOP/COEP headers threads
  need): SDL2 + WebGL2 + a blocking game loop (Asyncify) + a worker thread all run in the browser.

## Milestone 3 plan (browser build)

What game.exe needs from Windows (from its import table, `build\game_imports.txt`):
164 KERNEL32 (mostly C runtime internals; files, threads, critical sections, time),
66 USER32, 33 GDI32 (already ours: text), 23 WINMM (mmio wave reading, timers),
22 WS2_32 (sockets), 10 IMM32 (Korean/Japanese input: stubs), registry, wininet,
shell (stubs).

Building blocks, in order:
1. **`ptwin\` - our own small Windows layer** for the web build: headers with the types,
   constants and declarations the game uses, plus implementations. Windows (HWND),
   messages (PostMessage/SendMessage/queue), timers, GetKeyState, the hidden EDIT
   control the game types into, MessageBox (HTML dialog), files (CreateFile/_lopen
   -> fopen), critical sections/threads (pthreads), registry/ini (in memory + local
   storage), DirectDraw/D3D/DirectSound come from ptgfx/ptsound (WebGL2 shader header
   `#version 300 es`).
2. **Main loop**: unchanged game loop; `PtPlat_PumpEvents` yields to the browser
   (Asyncify, proven by the hello test).
3. **Game data**: 6.9 GB cannot be preloaded. Files are fetched from the web server on
   first open and cached in the browser (IndexedDB); start-up set (login screen, UI,
   character select) preloaded as one compressed package.
4. **Server in the browser too** (decided 2026-10-18: the whole solo game runs in the
   browser, no PC or host server). server.exe is the same code built with PT_SERVER_EXE,
   so it becomes a second WebAssembly module running in a Web Worker. The game's and the
   server's Winsock calls go to our own socket layer: connect/send/recv between the two
   modules over a browser MessageChannel (no real network). Server saves (characters,
   DataServer) live in browser storage (IndexedDB/OPFS) with export/import for backup.
5. Leave out Windows-only code paths in the web build: anti-cheat, process checks,
   OfflineServer launching, crash dumps, ODBC.
6. First goal: login screen in the browser. Then character select, then in game.

### Compiling for the web (status 2026-10-18)

- Headers: the game compiles against the **mingw-w64 v12 Windows headers**
  (`thirdparty\mingw`, includes ddraw/d3d/dsound/winsock/imm; changes listed in
  `thirdparty\mingw\PATCHES.md`) on top of Emscripten's C library. 32-bit layouts checked
  (`web\winsdk_test.cpp`: DDSURFACEDESC2 124, D3DTLVERTEX 32, MSG 28, WCHAR 2 bytes).
  Flags in `web\emflags.txt`; `_WIN32`/`WIN32` are not defined (libc++/SDL/GL stay on their
  Emscripten paths; the mingw headers were patched to accept `__wasm__`).
- `python web\check_sources.py` syntax-checks all 162 game sources in ~20 s and writes
  `web\out\check_report.txt`. **45 of 162 compile already.** Remaining blockers:
  - `atltime.h` (ATL `CTime`, via smPacket.h - 74 files), `atlbase.h`, `tchar.h`, `io.h`,
    `process.h`, `conio.h`: small replacements in `web\ptwin\include`.
  - Old Visual C++ `for`-loop scope (`for(int i..)` then `i` used after): CAutoCamera.cpp,
    HoEffectMain.cpp - fix in the source (works for both builds).
  - `gethostname` declared by both winsock.h and unistd.h.
  - Our own ptgfx/ptgl: WebGL2 path (GLES 3 headers, `#version 300 es`, context from SDL).
- **Update 2026-10-18 (later): all 162 files compile, the whole game links** (`python web\build_web.py`
  -> `web\out\game.html`), with our Windows layer in `web\ptwin\src` (kernel, files, windows/messages/
  EDIT control, GDI, mmio/timers, stubs, Winsock placeholder), Lua 5.0.2 from source, CP949 literals
  escaped (`tools\escape_cp949_literals.py`, Windows exe byte-identical), old for-scope fixed
  (`tools\fix_for_scope.py`). In the browser (`python web\serve.py`, open /game.html) it starts:
  WebGL2 context, data manifest (`web\make_manifest.py`), files fetched on demand. Start-up is slow
  (~1700 sounds fetched one by one): next step is `web\make_pack.py` (start-up pack, loaded by
  `web\pre.js`) from the list of files fetched, then check the login screen renders, then the
  in-browser server link (ptwin_net.cpp placeholder today), saves in browser storage, fonts.
- **Update 2026-10-18 (evening): the whole solo game runs in the browser** - login, server
  select, character select, in game in Ricarten (walking, NPCs), ~75 fps, release build
  (`python web\build_web.py --release`, 5 MB wasm). How it fits together:
  - One `game.wasm`, two instances: the page is the client; `web\server_worker.js` runs the
    same module as the offline server in a Web Worker (`Module.ptServer`, cwd /game/server,
    server.ini). `ptwin_net.cpp` is Winsock between the two over postMessage (pre.js relays).
  - Start-up data in two packs loaded by pre.js: `data_startup.pack` (client) and
    `data_server.pack` (server); everything else fetched on first open (`data_manifest.txt`).
    Server needs ~2 GB: `MAXIMUM_MEMORY=4GB`. Large files leave MEMFS when closed.
  - Every thread is a Web Worker: the server's socket thread pool is 4+4 in the web build
    (600 on Windows: `_W_SERVER` is set for all builds in nettype.hpp).
  - Debug aids: `game.html?debugnet` logs socket traffic and threads (both instances);
    `Module.ptDebugInput=1` logs input messages; a main-thread wait of 5 s without a frame
    prints its call stack; DisconnectFlag and the server's hack log print to the page log.
  - Fixed on the way (all web-only): posted messages (socket events, timers) were never
    dispatched (SDL_PollEvent does it on Windows; `PtPlat_PumpEvents` drains the queue now);
    SendMessage from a socket thread ran the packet handler on that thread mid-frame (it now
    waits for the main thread, as on Windows - the race tripped the 8540 skill-checksum kick);
    the server's x86 cipher modules (damage encode/decode, packet cipher) are C in Damage.cpp
    with the keys read from the module; its memory-check modules are skipped; the 10 s
    character-load limit is 180 s (field data downloads meanwhile).
  - **Saves (2026-10-18):** every file the game writes in the game folder (characters in
    server/DataServer, mail, castle, the client's ptReg.rgx and savedata/) is copied to
    IndexedDB ('pristontale-saves') a second after it is closed - only if it really changed,
    logs and diagnostics excluded (`ptwin_fs.cpp` NoteSaved -> `pre.js` PtSaves). Deleted
    files are kept as "deleted" and left out of the manifest. Before each start pre.js puts
    them back over the start-up pack (server worker: server/..., page: the rest).
    Export saves / Import saves buttons under the game: a .ptsave file (format in pre.js);
    import stops the server, replaces the store and reloads. Web-only changes so progress
    survives closing the tab: auto-save every 60 s (5 min on Windows) and the server writes
    every save to its file (`*SERVER_RECORD_MEM` otherwise waits 30 min or for logout).
  - Lost-character fix: a new character's file is a 24 KB zero placeholder until its first
    save; the original server deletes any character whose file does not read back at
    character select. Web build: first save 5 s after a new character enters the world;
    at character select a bad file is replaced by its backup, a played character whose
    file and backup are both bad is kept (only never-saved placeholders are dropped);
    an unreadable account file is kept as .bad. Every deletion shows "[saves] deleted ..."
    in the page log.
  - Next: a free font instead of the copied tahoma.ttf before hosting; first-visit
    download time.
- Watch out: MSVC built with `/execution-charset:.949` (Korean string literals as CP949
  bytes); clang only does UTF-8. Literals compared against data files must be converted
  to CP949 escapes for the web build (tool to write).
- Watch out: `time_t` is 64-bit in Emscripten; the Windows build uses 32-bit
  (`_USE_32BIT_TIME_T`). Check structs saved to disk / sent between client and server.

## Milestones

1. **OpenGL renderer on Windows** (the big one)
   - `ptgfx\` folder: our own DX6-compatible headers + OpenGL backend.
   - Game renders through OpenGL, looks the same as before, faster.
   - DirectX 6 no longer needed.
2. **Portable platform layer**: SDL2 for window, input and sound (replaces
   Win32 window/WndProc plumbing and DirectSound); our own text rasterizer
   (stb_truetype) replaces GDI.
3. **Browser build** (Emscripten)
   - Networking over WebSockets; a small bridge in front of server.exe.
   - Game data streamed/cached in the browser (3.8 GB: load per map, compress).
   - Remove leftover Win32-only code paths (registry, process checks, anti-cheat).
4. **Hosting** (private: password-protected, friends only - the game and its
   assets belong to the publisher).

Optional, separate: 64-bit Windows build (needs converters for the 32-bit
pointer layouts in .smd/.smb/.inx files).
