// Runs only the offline-server launcher (OfflineServer.cpp) without the game,
// so it can be tested without a fullscreen window. Copy next to game.exe and run.
#include <windows.h>
#include <stdio.h>
#include "..\\OfflineServer.h"

int main()
{
	char szDir[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, szDir );
	printf( "current dir: %s\n", szDir );
	int r = StartOfflineServer( 10007 );
	printf( "StartOfflineServer -> %d (0 none, 1 ready, 2 failed)\n", r );
	if ( r==OFFLINE_SERVER_READY ) {
		printf( "server is up; stopping it again\n" );
		StopOfflineServer();
	}
	return 0;
}
