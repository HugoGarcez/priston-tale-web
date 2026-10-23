// compat.h - force-included into every source file (see /FI in PristonTale.vcxproj).
// Shims that let 2002-era Visual C++ 7 code build with Visual Studio 2022
// without changing how it behaves.
#pragma once

#include <stdlib.h>

#ifdef __cplusplus
// VC7 picked abs(int) for unsigned arguments like abs(dwTimeA - dwTimeB).
// Modern compilers call that ambiguous; keep the old behaviour.
inline int  abs(unsigned int x)  { return ::abs((int)x); }
inline long abs(unsigned long x) { return ::labs((long)x); }
#endif

// Font creation goes through PT_CreateFont. The OpenGL renderer (PT_GFX_GL)
// draws GDI text through a transparent overlay and needs non-antialiased fonts
// (antialiased edges would blend with the overlay's key colour).
struct HFONT__;
struct HFONT__ * __stdcall PtGfx_CreateFontA( int, int, int, int, int, unsigned long, unsigned long, unsigned long,
	unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, const char * );
#ifdef PT_GFX_GL
#define PT_CreateFont	PtGfx_CreateFontA
#else
#define PT_CreateFont	CreateFontA
#endif

// Sound: the GL build plays DirectSound buffers through SDL2 (ptsound\ptsound.cpp).
// Defined before <dsound.h>, so its declaration is renamed too.
#ifdef PT_GFX_GL
#define DirectSoundCreate	PtSound_DirectSoundCreate
#endif
