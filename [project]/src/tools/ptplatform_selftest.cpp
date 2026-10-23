// ptplatform_selftest.cpp - checks the SDL2 platform layer (ptplatform.cpp) on a
// hidden window: SDL attaches to it, the OpenGL context comes from SDL, and SDL
// events reach a game-style WndProc as the right WM_ messages.
// Build + run: tools\ptplatform_selftest.bat   (no window is shown)

#include <windows.h>
#include <stdio.h>
#include <vector>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "..\ptplatform\ptplatform.h"
#include "..\ptgfx\ptgl.h"

static int g_Fail = 0;
#define CHECK( cond, what ) do { BOOL ok_ = (cond) ? TRUE : FALSE; printf( "  %s %s\n", ok_ ? "ok  " : "FAIL", what ); if ( !ok_ ) g_Fail++; } while ( 0 )

struct Msg { UINT msg; WPARAM w; LPARAM l; };
static std::vector<Msg> g_Msgs;

static LRESULT CALLBACK GameProc( HWND hWnd, UINT msg, WPARAM w, LPARAM l )
{
	switch ( msg ) {
	case WM_KEYDOWN: case WM_KEYUP: case WM_SYSKEYDOWN: case WM_SYSKEYUP: case WM_CHAR:
	case WM_MOUSEMOVE: case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONUP: case WM_MOUSEWHEEL: case WM_TIMER: case WM_USER+77:
		{ Msg m = { msg, w, l }; g_Msgs.push_back( m ); }
		return 0;
	}
	return DefWindowProcA( hWnd, msg, w, l );
}

static const Msg *Find( UINT msg, WPARAM w = (WPARAM)-1 )
{
	for ( size_t i=0; i<g_Msgs.size(); i++ )
		if ( g_Msgs[i].msg==msg && ( w==(WPARAM)-1 || g_Msgs[i].w==w ) ) return &g_Msgs[i];
	return NULL;
}

static void Key( SDL_Scancode sc, BOOL bDown, Uint16 mod = 0 )
{
	SDL_Event e; SDL_zero( e );
	e.type = bDown ? SDL_KEYDOWN : SDL_KEYUP;
	e.key.state = bDown ? SDL_PRESSED : SDL_RELEASED;
	e.key.keysym.scancode = sc;
	e.key.keysym.mod = mod;
	SDL_PushEvent( &e );
}

int main()
{
	// hotuk.ini with *PLATFORM SDL next to the test
	FILE *fp = fopen( "hotuk.ini", "wt" );
	fprintf( fp, "*MODE WINDOW\n*PLATFORM SDL\n" );
	fclose( fp );
	CHECK( PtPlat_WantSDL(), "hotuk.ini *PLATFORM SDL is read" );

	WNDCLASSA wc; ZeroMemory( &wc, sizeof(wc) );
	wc.style = CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = GameProc;
	wc.hInstance = GetModuleHandleA( NULL );
	wc.lpszClassName = "PtPlatTest";
	RegisterClassA( &wc );
	HWND hWnd = CreateWindowA( "PtPlatTest", "test", WS_POPUP, 0, 0, 320, 240, NULL, NULL, wc.hInstance, NULL );	// never shown

	CHECK( PtPlat_Attach( hWnd, GameProc ) && PtPlat_IsSDL(), "SDL attaches to the game window" );
	CHECK( PtGL_CreateContext( hWnd ), "OpenGL 3.3 context through SDL" );
	glClearColor( 0, 1, 0, 1 );
	glClear( GL_COLOR_BUFFER_BIT );
	BYTE px[4] = { 0 };
	glReadPixels( 10, 10, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px );
	CHECK( px[1]==255 && px[0]==0, "GL draws through the SDL context" );
	PtGL_SwapBuffers();

	// keyboard
	Key( SDL_SCANCODE_A, TRUE ); Key( SDL_SCANCODE_A, FALSE );
	Key( SDL_SCANCODE_RETURN, TRUE );
	Key( SDL_SCANCODE_LEFT, TRUE );
	Key( SDL_SCANCODE_F10, TRUE );
	Key( SDL_SCANCODE_X, TRUE, KMOD_LALT );			// Alt+X: dropped like the original message loop
	SDL_Event t; SDL_zero( t ); t.type = SDL_TEXTINPUT; strcpy( t.text.text, "h\xc3\xa9" );		// "hé"
	SDL_PushEvent( &t );
	// mouse
	SDL_Event m; SDL_zero( m );
	m.type = SDL_MOUSEMOTION; m.motion.x = 100; m.motion.y = 50; SDL_PushEvent( &m );
	SDL_zero( m ); m.type = SDL_MOUSEBUTTONDOWN; m.button.button = SDL_BUTTON_LEFT; m.button.clicks = 1; m.button.x = 100; m.button.y = 50; SDL_PushEvent( &m );
	m.type = SDL_MOUSEBUTTONUP; SDL_PushEvent( &m );
	m.type = SDL_MOUSEBUTTONDOWN; m.button.clicks = 2; SDL_PushEvent( &m );
	SDL_zero( m ); m.type = SDL_MOUSEBUTTONDOWN; m.button.button = SDL_BUTTON_RIGHT; m.button.clicks = 1; m.button.x = 7; m.button.y = 8; SDL_PushEvent( &m );
	SDL_zero( m ); m.type = SDL_MOUSEWHEEL; m.wheel.y = -1; SDL_PushEvent( &m );
	// non-input Win32 messages still reach the game; raw Win32 input does not
	PostMessageA( hWnd, WM_USER+77, 5, 6 );
	PostMessageA( hWnd, WM_KEYDOWN, 'Q', 0 );
	SetTimer( hWnd, 1, 1, NULL );
	Sleep( 30 );

	PtPlat_PumpEvents();
	PtPlat_PumpEvents();

	const Msg *k = Find( WM_KEYDOWN, 'A' );
	CHECK( k && ( k->l & 0xFFFF )==1 && !( k->l & (1<<30) ), "key A -> WM_KEYDOWN 'A'" );
	k = Find( WM_KEYUP, 'A' );
	CHECK( k && ( k->l & (1u<<31) ), "key A up -> WM_KEYUP with transition bit" );
	CHECK( Find( WM_KEYDOWN, VK_RETURN ) && Find( WM_CHAR, 13 ), "Enter -> WM_KEYDOWN VK_RETURN + WM_CHAR 13" );
	k = Find( WM_KEYDOWN, VK_LEFT );
	CHECK( k && ( k->l & (1<<24) ), "arrow key -> VK_LEFT, extended" );
	CHECK( Find( WM_SYSKEYDOWN, VK_F10 ) != NULL, "F10 -> WM_SYSKEYDOWN" );
	CHECK( !Find( WM_SYSKEYDOWN, 'X' ) && !Find( WM_KEYDOWN, 'X' ), "Alt+X dropped (as before)" );
	CHECK( Find( WM_CHAR, 'h' ) && Find( WM_CHAR, 0xE9 ), "text input -> WM_CHAR per character (code page)" );
	const Msg *mm = Find( WM_MOUSEMOVE );
	CHECK( mm && LOWORD( mm->l )==100 && HIWORD( mm->l )==50, "mouse motion -> WM_MOUSEMOVE x,y" );
	CHECK( Find( WM_LBUTTONDOWN ) && Find( WM_LBUTTONUP ) && Find( WM_LBUTTONDBLCLK ), "left click + double click" );
	mm = Find( WM_RBUTTONDOWN );
	CHECK( mm && LOWORD( mm->l )==7 && HIWORD( mm->l )==8, "right button at its position" );
	mm = Find( WM_MOUSEWHEEL );
	CHECK( mm && (short)HIWORD( mm->w )==-WHEEL_DELTA, "wheel down -> WM_MOUSEWHEEL -120" );
	mm = Find( WM_USER+77 );
	CHECK( mm && mm->w==5 && mm->l==6, "posted game message still arrives" );
	CHECK( Find( WM_TIMER ) != NULL, "WM_TIMER still arrives" );
	CHECK( !Find( WM_KEYDOWN, 'Q' ), "raw Win32 key message is filtered out" );

	KillTimer( hWnd, 1 );
	PtGL_DestroyContext();
	printf( g_Fail ? "\n%d check(s) FAILED\n" : "\nall checks passed\n", g_Fail );
	return g_Fail ? 1 : 0;
}
