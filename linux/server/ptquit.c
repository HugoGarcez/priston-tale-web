// ptquit.c - asks server.exe to quit the normal way, like its File > Quit menu.
//
// WM_CLOSE only hides server.exe's window (SrcServer\OnSever.cpp), so taskkill
// without /f does nothing. This posts WM_COMMAND ID_QUIT to every top-level window
// of every server.exe, the same as StopOfflineServer() in OfflineServer.cpp: the
// server saves Bless Castle and the characters online, then exits.
// Built with mingw in linux/server/Dockerfile; run as `wine ptquit.exe`.
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#define ID_QUIT		40004		// resource.h

static DWORD s_Pids[64];
static int s_PidCount;
static int s_Posted;

static BOOL CALLBACK QuitWindow( HWND hWnd, LPARAM lParam )
{
	DWORD pid = 0;
	GetWindowThreadProcessId( hWnd, &pid );
	for ( int i = 0; i<s_PidCount; i++ ) {
		if ( s_Pids[i]==pid ) {
			PostMessageA( hWnd, WM_COMMAND, ID_QUIT, 0 );
			s_Posted++;
		}
	}
	return TRUE;
}

int main( void )
{
	HANDLE snap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	if ( snap!=INVALID_HANDLE_VALUE && Process32First( snap, &pe ) ) {
		do {
			if ( !lstrcmpiA( pe.szExeFile, "server.exe" ) && s_PidCount<64 )
				s_Pids[s_PidCount++] = pe.th32ProcessID;
		} while ( Process32Next( snap, &pe ) );
	}
	if ( snap!=INVALID_HANDLE_VALUE ) CloseHandle( snap );

	if ( !s_PidCount ) {
		printf( "ptquit: server.exe is not running\n" );
		return 1;
	}
	EnumWindows( QuitWindow, 0 );
	printf( "ptquit: quit sent to %d window(s) of %d server.exe\n", s_Posted, s_PidCount );
	return s_Posted ? 0 : 2;
}
