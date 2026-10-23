// OfflineServer.cpp - starts and stops the local server.exe for solo play.
// See OfflineServer.h.

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#include "OfflineServer.h"
#include "resource.h"		// ID_QUIT

#define OFFLINE_SERVER_EXE			"server.exe"
#define OFFLINE_SERVER_INI			"server.ini"
#define OFFLINE_SERVER_WAIT_MS		(3*60*1000)		// server loads every map and monster first
#define OFFLINE_SERVER_CLOSE_MS		(15*1000)		// time to save characters before it is killed

static PROCESS_INFORMATION	OfflineServerProcess;
static HANDLE				hOfflineServerJob = NULL;

// Writes a line to offline.log next to game.exe (helps when the server will not start).
static void OfflineLog( const char *szFormat, ... )
{
	FILE *fp = fopen( "offline.log", "at" );
	if ( !fp ) return;
	va_list args;
	va_start( args, szFormat );
	vfprintf( fp, szFormat, args );
	va_end( args );
	fputc( '\n', fp );
	fclose( fp );
}

static BOOL FileExists( const char *szPath )
{
	return GetFileAttributes( szPath )!=INVALID_FILE_ATTRIBUTES;
}

// Is something accepting connections on 127.0.0.1:wPort?
static BOOL IsPortOpen( WORD wPort )
{
	SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( sock==INVALID_SOCKET ) return FALSE;

	sockaddr_in addr;
	ZeroMemory( &addr, sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( wPort );
	addr.sin_addr.s_addr = htonl( INADDR_LOOPBACK );

	BOOL bOpen = connect( sock, (sockaddr *)&addr, sizeof(addr) )==0;
	closesocket( sock );
	return bOpen;
}

#ifdef PT_WEB
void PtWeb_StartServer();		// web\ptwin\src\ptwin_main.cpp: the server runs in a Web Worker
int PtWeb_RemoteServer();		// the site plays on a shared server.exe instead (config.js)
#endif

int StartOfflineServer( WORD wPort )
{
#ifdef PT_WEB
	// Browser: the same game.wasm runs the server in a Web Worker; connections to it
	// wait until it listens (web\ptwin\src\ptwin_net.cpp).
	if ( PtWeb_RemoteServer() ) return OFFLINE_SERVER_NONE;
	if ( GetFileAttributes( "server\\server.ini" )==INVALID_FILE_ATTRIBUTES ) return OFFLINE_SERVER_NONE;
	PtWeb_StartServer();
	return OFFLINE_SERVER_READY;
#endif
	char szDir[MAX_PATH];
	char szExe[MAX_PATH];
	char szIni[MAX_PATH];

	GetModuleFileName( NULL, szDir, MAX_PATH );
	char *p = strrchr( szDir, '\\' );
	if ( p ) *p = 0;

	// The server has its own data (GameServer\, char\, Field\), so it can live
	// in a "server" subfolder; otherwise look next to game.exe.
	char szServerDir[MAX_PATH];
	wsprintf( szServerDir, "%s\\server", szDir );
	wsprintf( szExe, "%s\\%s", szServerDir, OFFLINE_SERVER_EXE );
	wsprintf( szIni, "%s\\%s", szServerDir, OFFLINE_SERVER_INI );

	if ( !FileExists( szExe ) || !FileExists( szIni ) ) {
		lstrcpy( szServerDir, szDir );
		wsprintf( szExe, "%s\\%s", szServerDir, OFFLINE_SERVER_EXE );
		wsprintf( szIni, "%s\\%s", szServerDir, OFFLINE_SERVER_INI );
		if ( !FileExists( szExe ) || !FileExists( szIni ) ) {
			OfflineLog( "no server.exe + server.ini in %s\\server or %s: online mode", szDir, szDir );
			return OFFLINE_SERVER_NONE;
		}
	}

	WSADATA wsaData;
	WSAStartup( MAKEWORD(2,2), &wsaData );

	// Already running (e.g. left open by an earlier session): just use it.
	OfflineLog( "server: %s (port %d)", szExe, wPort );
	if ( IsPortOpen( wPort ) ) {
		OfflineLog( "port %d already open: using the server that is already running", wPort );
		return OFFLINE_SERVER_READY;
	}

	// Kill the server automatically if the game crashes.
	hOfflineServerJob = CreateJobObject( NULL, NULL );
	if ( hOfflineServerJob ) {
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobInfo;
		ZeroMemory( &jobInfo, sizeof(jobInfo) );
		jobInfo.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
		SetInformationJobObject( hOfflineServerJob, JobObjectExtendedLimitInformation, &jobInfo, sizeof(jobInfo) );
	}

	STARTUPINFO si;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	ZeroMemory( &OfflineServerProcess, sizeof(OfflineServerProcess) );
	if ( !CreateProcess( szExe, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, szServerDir, &si, &OfflineServerProcess ) ) {
		OfflineLog( "CreateProcess failed, error %d", GetLastError() );
		return OFFLINE_SERVER_FAILED;
	}

	if ( hOfflineServerJob )
		AssignProcessToJobObject( hOfflineServerJob, OfflineServerProcess.hProcess );
	ResumeThread( OfflineServerProcess.hThread );

	DWORD dwStart = GetTickCount();
	while ( GetTickCount()-dwStart < OFFLINE_SERVER_WAIT_MS ) {
		if ( IsPortOpen( wPort ) ) {
			OfflineLog( "server ready after %d ms", GetTickCount()-dwStart );
			return OFFLINE_SERVER_READY;
		}
		if ( WaitForSingleObject( OfflineServerProcess.hProcess, 250 )==WAIT_OBJECT_0 ) {
			DWORD dwCode = 0;
			GetExitCodeProcess( OfflineServerProcess.hProcess, &dwCode );
			OfflineLog( "server.exe quit by itself, exit code 0x%08X", dwCode );
			break;
		}
	}

	StopOfflineServer();
	return OFFLINE_SERVER_FAILED;
}

// The server's own File > Quit menu (WM_CLOSE only hides its window).
static BOOL CALLBACK QuitServerWindow( HWND hWnd, LPARAM lParam )
{
	PostMessage( hWnd, WM_COMMAND, ID_QUIT, 0 );
	return TRUE;
}

void StopOfflineServer()
{
	if ( !OfflineServerProcess.hProcess ) return;

	// Give the server a moment to save the character after the game
	// disconnects, ask it to quit, and only force it if it hangs.
	Sleep( 2000 );
	EnumThreadWindows( OfflineServerProcess.dwThreadId, QuitServerWindow, 0 );
	if ( WaitForSingleObject( OfflineServerProcess.hProcess, OFFLINE_SERVER_CLOSE_MS )!=WAIT_OBJECT_0 )
		TerminateProcess( OfflineServerProcess.hProcess, 0 );

	CloseHandle( OfflineServerProcess.hThread );
	CloseHandle( OfflineServerProcess.hProcess );
	ZeroMemory( &OfflineServerProcess, sizeof(OfflineServerProcess) );

	if ( hOfflineServerJob ) {
		CloseHandle( hOfflineServerJob );
		hOfflineServerJob = NULL;
	}
}
