// ptgfx.h - DirectDraw / Direct3D 6 emulation on OpenGL (and later WebGL2).
//
// The game keeps calling lpDD / lpD3D / lpD3DDevice / surfaces exactly as it
// did with DirectX 6. In the GL build (build.bat Release GL, PT_GFX_GL)
// CreateD3D() gets its first object from PtGfx_DirectDrawCreate() instead of
// DirectDrawCreate(), and every other object comes from that one.
//
// Design notes:
// - The game sends pre-transformed vertices (D3DTLVERTEX, XYZRHW): the vertex
//   shader only maps screen pixels to clip space and keeps 1/rhw as w.
// - The back buffer is an OpenGL framebuffer at the game's resolution; Present
//   (primary Blt / Flip) scales it to the window.
// - Every surface keeps a CPU copy of its pixels in its own DirectDraw pixel
//   format (Lock/Blt/GetDC work on it). Textures upload on first use after a
//   change, converted to RGBA.
// - GetDC on the back buffer returns a transparent overlay bitmap. GDI's own
//   bounds tracking says which rectangle changed; only that is uploaded and
//   drawn, in order, without stalling the GPU.
#pragma once

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0600
#endif
#ifndef DIRECTDRAW_VERSION
#define DIRECTDRAW_VERSION 0x0600
#endif
#include <windows.h>
#include <ddraw.h>
#include <d3d.h>

// Replacement for DirectDrawCreate(): returns the IDirectDraw4 object directly.
HRESULT PtGfx_DirectDrawCreate( LPDIRECTDRAW4 *lplpDD );

// Fonts are forced to non-antialiased so GDI text has no colour fringes when
// drawn into the transparent text overlay.
HFONT WINAPI PtGfx_CreateFontA( int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight,
	DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision,
	DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName );
