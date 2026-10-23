// OfflineServer.h - solo, offline play.
//
// When server.exe and server.ini sit next to game.exe, the game starts
// server.exe in the background, waits for it to accept connections and
// connects to it on 127.0.0.1. The server closes again when the game exits.
#pragma once

#include <windows.h>

// Result of StartOfflineServer()
#define OFFLINE_SERVER_NONE		0	// no server.exe/server.ini: normal online game
#define OFFLINE_SERVER_READY	1	// server is running and accepting connections
#define OFFLINE_SERVER_FAILED	2	// server.exe did not start or never opened its port

int  StartOfflineServer( WORD wPort );
void StopOfflineServer();
