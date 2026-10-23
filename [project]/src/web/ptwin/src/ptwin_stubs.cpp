// ptwin_stubs.cpp - web build: Windows features the browser game does not use.
// Each answers the way Windows does when the feature is unavailable, so the
// game's existing error paths run (no registry, no ODBC database, no DirectShow
// video, no wininet downloads, no input method editor, no tray icon).
#include "ptwin_internal.h"
#include <objbase.h>
#include <tlhelp32.h>
#include <wininet.h>
#include <imm.h>
#include <shellapi.h>
#include <sql.h>
#include <sqlext.h>
#include <mmsystem.h>

// COM (DirectShow video player)
HRESULT WINAPI CoInitialize( LPVOID ) { return S_OK; }
void WINAPI CoUninitialize() {}
HRESULT WINAPI CoCreateInstance( REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID *pp ) { if ( pp ) *pp = NULL; return REGDB_E_CLASSNOTREG; }

// Process list (anti-cheat checks)
HANDLE WINAPI CreateToolhelp32Snapshot( DWORD, DWORD ) { return INVALID_HANDLE_VALUE; }
BOOL WINAPI Module32First( HANDLE, LPMODULEENTRY32 ) { return FALSE; }
BOOL WINAPI Module32Next( HANDLE, LPMODULEENTRY32 ) { return FALSE; }

// wininet (patch/notice downloads)
HINTERNET WINAPI InternetOpenA( LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD ) { return NULL; }
HINTERNET WINAPI InternetOpenUrlA( HINTERNET, LPCSTR, LPCSTR, DWORD, DWORD, DWORD_PTR ) { return NULL; }
BOOL WINAPI InternetReadFile( HINTERNET, LPVOID, DWORD, LPDWORD n ) { if ( n ) *n = 0; return FALSE; }
BOOL WINAPI InternetQueryDataAvailable( HINTERNET, LPDWORD n, DWORD, DWORD_PTR ) { if ( n ) *n = 0; return FALSE; }
BOOL WINAPI InternetCloseHandle( HINTERNET ) { return TRUE; }
BOOL WINAPI HttpQueryInfoA( HINTERNET, DWORD, LPVOID, LPDWORD, LPDWORD ) { return FALSE; }

// Input method editor (Korean/Japanese/Chinese input)
HIMC WINAPI ImmGetContext( HWND ) { return NULL; }
BOOL WINAPI ImmReleaseContext( HWND, HIMC ) { return TRUE; }
BOOL WINAPI ImmSetConversionStatus( HIMC, DWORD, DWORD ) { return FALSE; }
BOOL WINAPI ImmGetConversionStatus( HIMC, LPDWORD a, LPDWORD b ) { if ( a ) *a = 0; if ( b ) *b = 0; return FALSE; }
BOOL WINAPI ImmSetOpenStatus( HIMC, BOOL ) { return FALSE; }
BOOL WINAPI ImmGetOpenStatus( HIMC ) { return FALSE; }

// Registry: always empty (settings come from hotuk.ini)
LONG WINAPI RegOpenKeyA( HKEY, LPCSTR, PHKEY p ) { if ( p ) *p = NULL; return ERROR_FILE_NOT_FOUND; }
LONG WINAPI RegOpenKeyExA( HKEY, LPCSTR, DWORD, REGSAM, PHKEY p ) { if ( p ) *p = NULL; return ERROR_FILE_NOT_FOUND; }
LONG WINAPI RegCreateKeyExA( HKEY, LPCSTR, DWORD, LPSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY p, LPDWORD ) { if ( p ) *p = NULL; return ERROR_ACCESS_DENIED; }
LONG WINAPI RegQueryValueExA( HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD ) { return ERROR_FILE_NOT_FOUND; }
LONG WINAPI RegSetValueExA( HKEY, LPCSTR, DWORD, DWORD, const BYTE *, DWORD ) { return ERROR_ACCESS_DENIED; }
LONG WINAPI RegDeleteValueA( HKEY, LPCSTR ) { return ERROR_FILE_NOT_FOUND; }
LONG WINAPI RegCloseKey( HKEY ) { return ERROR_SUCCESS; }

// Shell
HINSTANCE WINAPI ShellExecuteA( HWND, LPCSTR, LPCSTR file, LPCSTR, LPCSTR, INT ) { PtWin_Log( "ShellExecute(%s) ignored", file ? file : "" ); return (HINSTANCE)(intptr_t)2; }
BOOL WINAPI Shell_NotifyIconA( DWORD, PNOTIFYICONDATAA ) { return TRUE; }

// ODBC (the server's account database; offline mode uses files instead)
SQLRETURN SQL_API SQLAllocHandle( SQLSMALLINT, SQLHANDLE, SQLHANDLE *p ) { if ( p ) *p = NULL; return SQL_ERROR; }
SQLRETURN SQL_API SQLFreeHandle( SQLSMALLINT, SQLHANDLE ) { return SQL_SUCCESS; }
SQLRETURN SQL_API SQLSetEnvAttr( SQLHENV, SQLINTEGER, SQLPOINTER, SQLINTEGER ) { return SQL_ERROR; }
SQLRETURN SQL_API SQLConnect( SQLHDBC, SQLCHAR *, SQLSMALLINT, SQLCHAR *, SQLSMALLINT, SQLCHAR *, SQLSMALLINT ) { return SQL_ERROR; }
SQLRETURN SQL_API SQLDisconnect( SQLHDBC ) { return SQL_SUCCESS; }
SQLRETURN SQL_API SQLExecDirect( SQLHSTMT, SQLCHAR *, SQLINTEGER ) { return SQL_ERROR; }
SQLRETURN SQL_API SQLFetch( SQLHSTMT ) { return SQL_NO_DATA; }
SQLRETURN SQL_API SQLBindCol( SQLHSTMT, SQLUSMALLINT, SQLSMALLINT, SQLPOINTER, SQLLEN, SQLLEN * ) { return SQL_ERROR; }
SQLRETURN SQL_API SQLFreeStmt( SQLHSTMT, SQLUSMALLINT ) { return SQL_SUCCESS; }

// PristonSQLDll.dll (account database DLL, delay-loaded on Windows)
int SQLLoginProcess( char * ) { return 0; }
int SQLAddCharProcess( char *, char * ) { return 0; }
int SQLDelCharProcess( char *, char * ) { return 0; }
int SQLLogoutProcess( char *, char *, char *, char *, unsigned long, int ) { return 0; }

// ZipLib.dll (patch download unpacking)
extern "C" int UnZipDataToDirectory( char *, BYTE * ) { return 0; }

// Volume mixer (microphone volume)
MMRESULT WINAPI mixerOpen( LPHMIXER p, UINT, DWORD_PTR, DWORD_PTR, DWORD ) { if ( p ) *p = NULL; return MMSYSERR_NODRIVER; }
MMRESULT WINAPI mixerClose( HMIXER ) { return MMSYSERR_NOERROR; }
MMRESULT WINAPI mixerGetDevCapsA( UINT_PTR, LPMIXERCAPSA, UINT ) { return MMSYSERR_NODRIVER; }
MMRESULT WINAPI mixerGetLineInfoA( HMIXEROBJ, LPMIXERLINEA, DWORD ) { return MMSYSERR_NODRIVER; }
MMRESULT WINAPI mixerGetLineControlsA( HMIXEROBJ, LPMIXERLINECONTROLSA, DWORD ) { return MMSYSERR_NODRIVER; }
MMRESULT WINAPI mixerGetControlDetailsA( HMIXEROBJ, LPMIXERCONTROLDETAILS, DWORD ) { return MMSYSERR_NODRIVER; }
MMRESULT WINAPI mixerSetControlDetails( HMIXEROBJ, LPMIXERCONTROLDETAILS, DWORD ) { return MMSYSERR_NODRIVER; }
