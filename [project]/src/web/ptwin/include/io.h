// io.h - web build: the MS C runtime's low-level file header maps onto POSIX.
#pragma once
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ptwin_fs.h"
#define _access		PtWin_access
#define _mkdir		PtWin_mkdir
#define _open		open
#define _close		close
#define _read		read
#define _write		write
#define _lseek		lseek
#define _unlink		unlink
#ifndef _O_BINARY
#define _O_BINARY	0
#define O_BINARY	0
#define _O_RDONLY	O_RDONLY
#define _O_WRONLY	O_WRONLY
#define _O_RDWR		O_RDWR
#define _O_CREAT	O_CREAT
#define _O_TRUNC	O_TRUNC
#endif
