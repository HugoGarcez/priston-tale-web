// process.h - web build: thread start functions of the MS C runtime (ptwin, on pthreads).
#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
uintptr_t _beginthread( void (*start)( void * ), unsigned stack, void *arg );
void _endthread( void );
uintptr_t _beginthreadex( void *security, unsigned stack, unsigned (__stdcall *start)( void * ), void *arg, unsigned flags, unsigned *thrdaddr );
void _endthreadex( unsigned retval );
int _getpid( void );
#define P_WAIT		0
#define P_NOWAIT	1
#define _P_WAIT		P_WAIT
#define _P_NOWAIT	P_NOWAIT
intptr_t spawnl( int mode, const char *path, const char *arg0, ... );	// web: fails (no processes)
#define _spawnl spawnl
#ifdef __cplusplus
}
#endif
