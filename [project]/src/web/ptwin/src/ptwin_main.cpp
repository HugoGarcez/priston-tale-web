// ptwin_main.cpp - web build entry point. The browser calls main(); the game
// starts in WinMain as on Windows.
//
// The same game.wasm runs twice: as the game on the page, and as the offline
// server in a Web Worker (web\server_worker.js sets Module.ptServer). The server
// works in /game/server with server.ini, like server.exe in game\server.
#include <windows.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <emscripten.h>

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, INT nCmdShow );

EM_JS( int, PtWeb_IsServerJS, (), { return Module['ptServer'] ? 1 : 0; } );
EM_JS( void, PtWeb_StartServerJS, (), { if ( Module['ptStartServer'] ) Module['ptStartServer'](); } );

static int s_IsServer = -1;

int PtWeb_IsServer()
{
	if ( s_IsServer<0 ) s_IsServer = PtWeb_IsServerJS();
	return s_IsServer;
}

void PtWeb_StartServer() { PtWeb_StartServerJS(); }

// Shared server: config.js sets Module.ptServerUrl (the WebSocket bridge to server.exe)
EM_JS( int, PtWeb_RemoteServerJS, (), { return Module['ptServerUrl'] ? 1 : 0; } );
int PtWeb_RemoteServer() { return PtWeb_RemoteServerJS(); }

int main( int argc, char **argv )
{
	static char szCmdLine[] = "";
	const char *dir = PtWeb_IsServer() ? "/game/server" : "/game";
	mkdir( "/game", 0777 );
	mkdir( dir, 0777 );			// folders appear as their files are fetched; make sure ours exists
	if ( chdir( dir ) ) printf( "[ptwin] no %s folder\n", dir );		// each runs from its own folder
	if ( PtWeb_IsServer() ) printf( "[ptwin] starting as the offline server\n" );
	return WinMain( (HINSTANCE)0x400000, NULL, szCmdLine, SW_SHOWNORMAL );
}
