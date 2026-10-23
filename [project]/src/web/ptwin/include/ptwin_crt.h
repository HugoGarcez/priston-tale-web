// ptwin_crt.h - web build: the Microsoft C runtime extras the game uses, on top of
// Emscripten's C library. Included from ptwin_prelude.h; functions in ptwin_crt.cpp.
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <alloca.h>
#include <wchar.h>
#include <assert.h>

#define _MAX_PATH	260
#define _MAX_DRIVE	3
#define _MAX_DIR	256
#define _MAX_FNAME	256
#define _MAX_EXT	256

#define _strdup		strdup
#define _snprintf	snprintf
#define _vsnprintf	vsnprintf
#define _vsnwprintf	vswprintf
#define _snwprintf	swprintf
#define _alloca		alloca
#define _fileno		fileno

#ifdef __cplusplus
extern "C" {
#endif
// own functions rather than macros onto strcasecmp: <strings.h> would also bring in
// index(), which collides with the game's many variables called "index"
int stricmp( const char *a, const char *b );
int strnicmp( const char *a, const char *b, size_t n );
#define _stricmp	stricmp
#define strcmpi		stricmp
#define _strcmpi	stricmp
#define _strnicmp	strnicmp
int _wcsicmp( const wchar_t *a, const wchar_t *b );
char *strupr( char *s );
char *strlwr( char *s );
#define _strupr strupr
#define _strlwr strlwr
char *itoa( int value, char *buf, int radix );
char *ltoa( long value, char *buf, int radix );
char *ultoa( unsigned long value, char *buf, int radix );
#define _itoa itoa
#define _ltoa ltoa
#define _ultoa ultoa
void _splitpath( const char *path, char *drive, char *dir, char *fname, char *ext );
char *_ui64toa( unsigned long long value, char *buf, int radix );
char *_i64toa( long long value, char *buf, int radix );
#ifdef __cplusplus
}
#endif

// debug CRT (crtdbg.h): no-ops
#define _ASSERT(expr)		((void)0)
#define _ASSERTE(expr)		((void)0)
#define _CrtSetDbgFlag(f)	0
#define _CRTDBG_ALLOC_MEM_DF		0x01
#define _CRTDBG_LEAK_CHECK_DF		0x20
#define _CRTDBG_CHECK_ALWAYS_DF		0x04
#define _CRTDBG_REPORT_FLAG			(-1)
#define _CrtCheckMemory()	1
#define _CrtDumpMemoryLeaks()	0

#ifdef __cplusplus
#include <type_traits>
// Visual C++'s windows.h has min/max macros the game relies on; functions here so
// std::min/std::max in the C++ library keep working.
template<class A, class B> inline auto max( A a, B b ) -> decltype( a>b ? a : b ) { return a>b ? a : b; }
template<class A, class B> inline auto min( A a, B b ) -> decltype( a<b ? a : b ) { return a<b ? a : b; }

// The game keeps times in 32-bit longs (built with _USE_32BIT_TIME_T); time_t is 64-bit here.
// (template so that time(NULL) still picks the normal function)
template<class T, class = typename std::enable_if<std::is_same<T, long>::value>::type>
inline long time( T *t ) { long v = (long)::time( (time_t *)0 ); if ( t ) *t = v; return v; }
#endif
