// ptwin_internal.h - shared pieces of ptwin, our small Windows layer for the web build.
#pragma once

#include <windows.h>
#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <map>

void PtWin_Log( const char *szFormat, ... );		// browser console: "[ptwin] ..."

// Kernel object handles (files, threads, events, mutexes, find handles, mappings).
// Every HANDLE the layer hands out points at one of these.
enum PtObjType { PTOBJ_FILE = 1, PTOBJ_THREAD, PTOBJ_EVENT, PTOBJ_MUTEX, PTOBJ_FIND, PTOBJ_MAPPING, PTOBJ_DUMMY };

struct PtObject {
	unsigned	magic;					// PTOBJ_MAGIC while alive
	PtObjType	type;
	virtual ~PtObject() {}
};
#define PTOBJ_MAGIC 0x7057494E		// 'pWIN'

template<class T> T *PtObj( HANDLE h, PtObjType type )
{
	PtObject *p = (PtObject *)h;
	if ( !p || h==INVALID_HANDLE_VALUE || p->magic!=PTOBJ_MAGIC || p->type!=type ) return NULL;
	return (T *)p;
}

// Called from WaitForSingleObject for threads/events (ptwin_kernel.cpp).
DWORD PtWin_WaitObject( PtObject *p, DWORD dwMs );

// Message queue (ptwin_user.cpp) - wakes up a thread blocked in GetMessage/WaitMessage.
void PtWin_PostQuit();

// Called by the main loop when the browser should get its turn (Asyncify).
void PtWin_Yield();
void PtWin_FrameDone();					// the game finished a frame (for the stall check)
void PtWin_CheckStall( const char *szWhere );	// main thread waiting: report if no frame for a while

// Current keyboard state for GetKeyState (set by ptplatform's event translation).
void PtWin_SetKeyDown( int vk, BOOL bDown );
