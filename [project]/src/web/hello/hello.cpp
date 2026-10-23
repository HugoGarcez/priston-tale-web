// hello.cpp - toolchain check for the browser build, built the way the game will be:
//   SDL2 window + WebGL2 (GLES 3) context, a blocking while(1) loop like
//   Winmain.cpp that yields to the browser every frame (Asyncify), and a worker
//   thread like the game's network threads (pthreads).
// Build: web\build_hello.bat  ->  web\out\hello.html

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <SDL.h>
#include <GLES3/gl3.h>
#include <emscripten.h>

static std::atomic<int> g_Ticks( 0 );

static void *Worker( void * )
{
	for ( ;; ) { g_Ticks++; usleep( 100000 ); }		// 10 per second
	return NULL;
}

int main()
{
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
	SDL_Window *win = SDL_CreateWindow( "hello", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL );
	SDL_GLContext ctx = SDL_GL_CreateContext( win );
	printf( "GL: %s / %s\n", glGetString( GL_VERSION ), glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	pthread_t th;
	pthread_create( &th, NULL, Worker, NULL );

	int frame = 0, clicks = 0;
	while ( 1 ) {											// blocking loop, like the game
		SDL_Event e;
		while ( SDL_PollEvent( &e ) ) {
			if ( e.type==SDL_MOUSEBUTTONDOWN ) clicks++;
			if ( e.type==SDL_KEYDOWN ) printf( "key %s\n", SDL_GetKeyName( e.key.keysym.sym ) );
		}
		float t = ( frame % 120 ) / 120.0f;
		glClearColor( t, 0.3f, clicks%2 ? 0.8f : 0.2f, 1 );
		glClear( GL_COLOR_BUFFER_BIT );
		SDL_GL_SwapWindow( win );
		if ( ++frame % 60==0 ) {
			printf( "frame %d, worker ticks %d, clicks %d\n", frame, g_Ticks.load(), clicks );
			EM_ASM( { document.title = "hello ok " + $0; }, frame );
		}
		emscripten_sleep( 16 );								// give the browser its turn
	}
	(void)ctx;
	return 0;
}
