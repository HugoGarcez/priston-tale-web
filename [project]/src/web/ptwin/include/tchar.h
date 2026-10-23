// tchar.h - web build: the game is an ANSI/MBCS build, so the _T/_tcs names map to char.
#pragma once
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ptwin_crt.h"

#ifndef _T
#define _T(x)		x
#define _TEXT(x)	x
#endif
#ifndef _TCHAR_DEFINED
#define _TCHAR_DEFINED
typedef char TCHAR, *PTCHAR;
typedef unsigned char TBYTE, *PTBYTE;
#endif
typedef char _TCHAR;
typedef char _TSCHAR;
typedef unsigned char _TUCHAR;

#define _tcslen		strlen
#define _tcscpy		strcpy
#define _tcsncpy	strncpy
#define _tcscat		strcat
#define _tcsncat	strncat
#define _tcscmp		strcmp
#define _tcsncmp	strncmp
#define _tcsicmp	stricmp
#define _tcsnicmp	strnicmp
#define _tcscoll	strcoll
#define _tcschr		strchr
#define _tcsrchr	strrchr
#define _tcsstr		strstr
#define _tcsspn		strspn
#define _tcscspn	strcspn
#define _tcspbrk	strpbrk
#define _tcstok		strtok
#define _ttoi		atoi
#define _ttol		atol
#define _tstoi		atoi
#define _stprintf	sprintf
#define _sntprintf	snprintf
#define _tprintf	printf
#define _ftprintf	fprintf
#define _tfopen		fopen
#define _tcsdup		strdup
#define _istdigit	isdigit
#define _istalpha	isalpha
#define _istalnum	isalnum
#define _istspace	isspace
#define _istupper	isupper
#define _istlower	islower
#define _totupper	toupper
#define _totlower	tolower
#define _tcstol		strtol
#define _tcstoul	strtoul
#define _tcstod		strtod

// MBCS character steps (single-byte data in the English client)
static inline char *_tcsinc( const char *p ) { return (char *)p + 1; }
static inline char *_tcsdec( const char *start, const char *p ) { return p>start ? (char *)p - 1 : NULL; }
static inline size_t _tclen( const char * ) { return 1; }
static inline char *_tcsrev( char *s ) {
	size_t n = strlen( s );
	for ( size_t i=0; i<n/2; i++ ) { char c = s[i]; s[i] = s[n-1-i]; s[n-1-i] = c; }
	return s;
}
