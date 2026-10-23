# Priston Tale - VS2022 port

Working copy of the game source from `..\OG Source\J_Server`, set up to build
with Visual Studio 2022. The original folder is untouched.

## Building

- Open `PristonTale.sln` in Visual Studio 2022, pick **Debug** or **Release**
  and press Build. The exe is `bin\Debug\game.exe` / `bin\Release\game.exe`.
- Or double-click `build.bat` (Debug), or run `build.bat Release`.

Still 32-bit (x86). 64-bit comes after the renderer is replaced: the game uses
DirectX 6 (DirectDraw4 / Direct3DDevice3), which Windows does not support in
64-bit programs.

## What changed from the original

**Files**
- Only source code was copied (`.cpp/.h/.c/.rc/.lib`, plus icons and the bitmaps
  compiled into the exe). Game data (maps, textures, sounds) is still in OG Source.
- Backup copies were left out: `사본 - *` (copy), `원본*` (original), `새 폴더`
  (new folder), `임시` (temp), `Old`, `Backup_20071005`, `*.bak`, `__tjclan.cpp`,
  `--isaocheck.cpp`, `o_isaocheck.cpp`, `_cracker.*`, `HoSkyOld.*`, `nprotect\Sample`.
- Korean filenames garbled by the zip extraction were fixed
  (e.g. `ë¡œë”©ì¤‘.bmp` -> `로딩중.bmp`).

**Text encoding**
- Every source file is now UTF-8. Previously each was in its team's Windows code
  page (Korean, Japanese, Chinese, Thai, Vietnamese...), often mixed in one file.
- The compiler is set to `/source-charset:utf-8 /execution-charset:.949`, so
  Korean text in strings compiles to exactly the same bytes as before.
- Strings in other languages (e.g. Japanese menu text) are written as octal
  escapes like `"\203A\203C"` so their bytes are also unchanged; the readable
  text is in a `// comment` at the end of the line.

**Code fixes for the modern compiler** (behaviour unchanged)
- `compat.h` is included into every file automatically. It makes
  `abs(unsigned)` behave like VC7 did.
- Missing `int` added on old "default int" declarations (`static cnt=0;`,
  `extern WinSizeX;`, a few functions in `HoBaram\HoEffect.cpp`).
- `HoEffectModel.h`: the effect-event iterator could be set to `NULL` in VC7;
  a small wrapper keeps that working.
- Removed `std::binary_function` base classes (gone from modern C++),
  `<typeinfo.h>` -> `<typeinfo>`, `_WIN32_WINNT` raised from 0x0400 to 0x0601.
- `nprotect\PT_KeyCrypt.cpp` taken out of the build: it is the nProtect
  anti-cheat keyboard driver and every call to it was already commented out.
- The linker ignores `LIBC.lib`, an old C runtime the prebuilt `.lib` files ask for.

## Fixes

**FPS drop when holding A (item names on the floor).** Each item label locked
the DirectDraw back buffer with `GetDC()` to draw its text, twice in the English
build since it also measured the text that way. Every lock makes the GPU
finish all queued drawing, so 100 items meant 100-200 GPU stalls per frame.
Now `Winmain.cpp` wraps the hold-A loop in `BeginLabelTextBatch()` /
`EndLabelTextBatch()` (in `Park\HoMessageBox.cpp`): label frames draw as
before, the text is queued and drawn with one `GetDC()` per frame, and text is
measured on a memory DC. One visible difference: where labels overlap, all
names now draw on top of all frames, instead of a later frame covering an
earlier name.

## Offline solo play

The solution builds two programs from the same source:

- `game.exe` (`PristonTale.vcxproj`): the game.
- `server.exe` (`PristonServer.vcxproj`): the same code with `_W_SERVER`
  defined, which is how the original `server1024.exe` was made. It reads
  `server.ini` instead of `hotuk.ini`.

**Setup:** put `game.exe`, `server.exe` and both files from `offline\`
(`server.ini`, `hotuk.ini`) in the game folder, next to the game data and the
server data (`gameserver\`). Then just run `game.exe`.

**What happens:**
1. `game.exe` sees `server.exe` + `server.ini`, starts the server hidden
   (`OfflineServer.cpp`) and waits until it accepts connections.
2. The game connects to `127.0.0.1` instead of the old login servers.
3. `*OFFLINE_MODE` in `server.ini` makes the server accept any ID and password
   with no account database, and skips the billing/log DLLs (`SQL.dll`,
   `Clan.dll`, `URSLogin.dll`, `PristonSQLDll.dll`). Characters are saved as
   files under `DataServer\` like the real server did.
4. When the game exits, the server is told to quit (File > Quit). If the game
   crashes, Windows closes the server too.

Without `server.ini`, `game.exe` behaves like the original online client.

`PristonSQLDll.dll` is now delay-loaded, so neither exe needs it to start.

## Playing with the v3215 data

PristonWeb keeps its own copy of everything (nothing links to other folders):

- `..\import_v3215_data.ps1` (run once) copies the v3215 client data into
  `..\game\` and the server data into `..\game\server\`.
- `build.bat Release GL` then `install_build.ps1` copies `game.exe`,
  `server.exe` and the default `hotuk.ini` / `server.ini` into `..\game\`.
- Run `..\game\game.exe`; it starts the server itself (see `..\game\offline.log`).

Fixes needed for the newer v3215 data and the modern compiler:
- **Fixed load address** (`/DYNAMICBASE:NO`, base 0x400000, DEP off): old
  anti-hack code reads its own exe at 0x400000.
- **`.inx` model info**: v3215 stores 104 item codes per motion (this source
  had 52). `MOTION_TOOL_MAX` is now 104 and old-size files are converted on
  load (`fileread.cpp`, `smModelInfoFromOldFile`).
- **Map area index** is 32-bit in v3215 maps (`smAREA_INDEX` in
  `smStage3d.h`), and the reader now refuses data that would overflow.
- **Model loader** seeks to each object's stored file position, rejects
  insane counts, and no longer frees the raw `Physique` pointer from the file.
- NPCs whose model cannot load are skipped and listed in
  `game\server\missing_models.txt`.
- `NetType.hpp` always defines `_W_SERVER` (the original exe was client and
  server in one). `server.exe` is told apart by `PT_SERVER_EXE`.
- `hotuk.ini` `*MODE` / `*SCREENSIZE` now win over the registry settings
  the old launcher left behind.

Debugging without a debugger: `tools\crashinfo.py <dmp> <map> [exe]` reads
the game's own crash dumps; `tools\test_server.ps1` runs the Debug server;
Debug builds check the heap (`heap_report.txt`).

## Language

`language.h` currently selects `_LANGUAGE_JAPANESE` (this is the Japanese
server source). Switching to English means changing that define, and the
game data has to match the language.
