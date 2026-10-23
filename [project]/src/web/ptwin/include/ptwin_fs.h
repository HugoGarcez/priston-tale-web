// ptwin_fs.h - web build: the game's C-runtime file opens go through ptwin's path
// resolver (Windows paths are case-insensitive with backslashes; see ptwin_fs.cpp).
#pragma once
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
FILE *PtWin_fopen( const char *szPath, const char *szMode );
int PtWin_fclose( FILE *fp );
int PtWin_access( const char *szPath, int mode );
int PtWin_mkdir( const char *szPath );
#ifdef __cplusplus
}
#endif
#ifndef PTWIN_FS_IMPL
#define fopen	PtWin_fopen
#define fclose	PtWin_fclose
#endif
