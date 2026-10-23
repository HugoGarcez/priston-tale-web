// ptgl.cpp - OpenGL 3.3 core context on a Win32 window + function loading.
// (The browser build will get its WebGL2 context from Emscripten/SDL instead.)

#include <stdio.h>
#include <stdarg.h>
#include "ptgl.h"

#ifdef PT_WEB
//////////////////////////////////////////////////////////////////////////////
// Browser: WebGL2 context on the SDL window (canvas) - see ptplatform.
//////////////////////////////////////////////////////////////////////////////
#include <SDL.h>
#include "../ptplatform/ptplatform.h"

static SDL_GLContext s_WebContext = NULL;

void PtGL_Log( const char *szFormat, ... )
{
	va_list args;
	va_start( args, szFormat );
	printf( "[ptgfx] " );
	vprintf( szFormat, args );			// browser console
	printf( "\n" );
	va_end( args );
}

BOOL PtGL_CreateContext( HWND hWnd )
{
	SDL_Window *pWin = PtPlat_GetSDLWindow();
	if ( !pWin ) { PtGL_Log( "no SDL window" ); return FALSE; }
	s_WebContext = SDL_GL_CreateContext( pWin );
	if ( !s_WebContext ) { PtGL_Log( "WebGL2 context failed: %s", SDL_GetError() ); return FALSE; }
	PtGL_Log( "WebGL: %s / %s", glGetString( GL_VERSION ), glGetString( GL_RENDERER ) );
	return TRUE;
}

void PtGL_DestroyContext()
{
	if ( s_WebContext ) { SDL_GL_DeleteContext( s_WebContext ); s_WebContext = NULL; }
}

void PtGL_SwapBuffers()
{
	SDL_GL_SwapWindow( PtPlat_GetSDLWindow() );
}

#else // Windows

#pragma comment( lib, "opengl32.lib" )

#ifdef PT_USE_SDL
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "..\ptplatform\ptplatform.h"
static SDL_GLContext s_SDLContext = NULL;
#endif

#define PTGL_DEFINE(ret, name, args) PFN_##name pt_##name = 0;
PTGL_FUNCTIONS(PTGL_DEFINE)
#undef PTGL_DEFINE

// WGL_ARB_create_context
#define WGL_CONTEXT_MAJOR_VERSION_ARB		0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB		0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB		0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB	0x00000001
typedef HGLRC (WINAPI *PFN_wglCreateContextAttribsARB)( HDC hDC, HGLRC hShareContext, const int *attribList );
typedef BOOL (WINAPI *PFN_wglSwapIntervalEXT)( int interval );

static HWND		hGLWnd = NULL;
static HDC		hGLDC = NULL;
static HGLRC	hGLRC = NULL;

void PtGL_Log( const char *szFormat, ... )
{
	FILE *fp = fopen( "ptgfx.log", "at" );
	if ( !fp ) return;
	va_list args;
	va_start( args, szFormat );
	vfprintf( fp, szFormat, args );
	va_end( args );
	fputc( '\n', fp );
	fclose( fp );
}

static void *GetGLProc( const char *szName )
{
#ifdef PT_USE_SDL
	if ( s_SDLContext ) return SDL_GL_GetProcAddress( szName );
#endif
	void *p = (void *)wglGetProcAddress( szName );
	if ( p==0 || p==(void *)1 || p==(void *)2 || p==(void *)3 || p==(void *)-1 ) {
		static HMODULE hGL = LoadLibraryA( "opengl32.dll" );
		p = (void *)GetProcAddress( hGL, szName );
	}
	return p;
}

static BOOL LoadFunctions()
{
	BOOL bOK = TRUE;
#define PTGL_LOAD(ret, name, args) \
	pt_##name = (PFN_##name)GetGLProc( #name ); \
	if ( !pt_##name ) { PtGL_Log( "missing OpenGL function %s", #name ); bOK = FALSE; }
	PTGL_FUNCTIONS(PTGL_LOAD)
#undef PTGL_LOAD
	return bOK;
}

#ifdef PT_USE_SDL
// SDL platform layer (*PLATFORM SDL): SDL owns the context on the game window.
static BOOL CreateSDLContext( HWND hWnd )
{
	SDL_Window *pWin = PtPlat_GetSDLWindow();
	s_SDLContext = SDL_GL_CreateContext( pWin );
	if ( !s_SDLContext ) {
		PtGL_Log( "SDL_GL_CreateContext failed: %s", SDL_GetError() );
		return FALSE;
	}
	hGLWnd = hWnd;
	SDL_GL_SetSwapInterval( 1 );		// vsync
	BOOL bOK = LoadFunctions();
	PtGL_Log( "OpenGL (SDL): %s / %s / %s", glGetString( GL_VENDOR ), glGetString( GL_RENDERER ), glGetString( GL_VERSION ) );
	return bOK;
}
#endif

BOOL PtGL_CreateContext( HWND hWnd )
{
#ifdef PT_USE_SDL
	if ( PtPlat_GetSDLWindow() ) return CreateSDLContext( hWnd );
#endif
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory( &pfd, sizeof(pfd) );
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	hGLWnd = hWnd;
	hGLDC = GetDC( hWnd );
	int format = ChoosePixelFormat( hGLDC, &pfd );
	if ( !format || !SetPixelFormat( hGLDC, format, &pfd ) ) {
		PtGL_Log( "SetPixelFormat failed (%d)", GetLastError() );
		return FALSE;
	}

	// A legacy context is needed first to get wglCreateContextAttribsARB.
	HGLRC hTemp = wglCreateContext( hGLDC );
	if ( !hTemp || !wglMakeCurrent( hGLDC, hTemp ) ) {
		PtGL_Log( "wglCreateContext failed (%d)", GetLastError() );
		return FALSE;
	}

	PFN_wglCreateContextAttribsARB wglCreateContextAttribsARB =
		(PFN_wglCreateContextAttribsARB)GetGLProc( "wglCreateContextAttribsARB" );
	if ( wglCreateContextAttribsARB ) {
		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0 };
		hGLRC = wglCreateContextAttribsARB( hGLDC, 0, attribs );
	}
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hTemp );

	if ( !hGLRC || !wglMakeCurrent( hGLDC, hGLRC ) ) {
		PtGL_Log( "OpenGL 3.3 core context not available - update the graphics driver" );
		return FALSE;
	}

	BOOL bOK = LoadFunctions();

	PFN_wglSwapIntervalEXT wglSwapIntervalEXT = (PFN_wglSwapIntervalEXT)GetGLProc( "wglSwapIntervalEXT" );
	if ( wglSwapIntervalEXT ) wglSwapIntervalEXT( 1 );		// vsync

	PtGL_Log( "OpenGL: %s / %s / %s", glGetString( GL_VENDOR ), glGetString( GL_RENDERER ), glGetString( GL_VERSION ) );
	return bOK;
}

void PtGL_DestroyContext()
{
#ifdef PT_USE_SDL
	if ( s_SDLContext ) { SDL_GL_DeleteContext( s_SDLContext ); s_SDLContext = NULL; return; }
#endif
	if ( hGLRC ) {
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( hGLRC );
		hGLRC = NULL;
	}
	if ( hGLDC ) {
		ReleaseDC( hGLWnd, hGLDC );
		hGLDC = NULL;
	}
}

void PtGL_SwapBuffers()
{
#ifdef PT_USE_SDL
	if ( s_SDLContext ) { SDL_GL_SwapWindow( PtPlat_GetSDLWindow() ); return; }
#endif
	// Present through the DC the context is current on (the window must have
	// CS_OWNDC so this DC stays valid while the game uses GetDC on the window).
	HDC hDC = wglGetCurrentDC();
	if ( !hDC ) hDC = hGLDC;
	if ( hDC && !SwapBuffers( hDC ) ) {
		static int nLogged = 0;
		if ( nLogged++<5 ) PtGL_Log( "SwapBuffers failed (%d)", GetLastError() );
	}
}

#endif // PT_WEB
