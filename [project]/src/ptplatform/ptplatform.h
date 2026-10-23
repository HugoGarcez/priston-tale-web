// ptplatform.h - SDL2 platform layer (GL build, opt-in with "*PLATFORM SDL" in hotuk.ini).
//
// SDL attaches to the game's window and owns the OpenGL context and input.
// Raw Win32 keyboard/mouse messages are filtered out; SDL events are turned
// back into the WM_ messages the game's WndProc expects. Everything else
// (edit controls, timers, socket messages) still arrives through Win32.
// In the browser build the same translation feeds WndProc from Emscripten's SDL.
#pragma once

#include <windows.h>

BOOL	PtPlat_WantSDL();							// hotuk.ini says *PLATFORM SDL
BOOL	PtPlat_Attach( HWND hWnd, WNDPROC pfnGameProc );	// call once, after CreateWindow
BOOL	PtPlat_IsSDL();								// attached and running on SDL
void	PtPlat_PumpEvents();						// once per frame: (web: yield to the browser,) SDL events -> game WndProc
void	PtPlat_PumpInput();							// SDL events -> game WndProc only
struct SDL_Window *PtPlat_GetSDLWindow();			// NULL when not on SDL
