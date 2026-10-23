// ptwin_prelude.h - force-included first in the web build (see web\emflags.txt).
// The game is compiled against the mingw-w64 Windows headers on top of
// Emscripten's C library; the C library owns the basic types, so mark them as
// defined before any Windows header tries to typedef them again.
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

#define _SIZE_T_DEFINED
#define _SSIZE_T_DEFINED
#define _PTRDIFF_T_DEFINED
#define _WCHAR_T_DEFINED
#define _WCTYPE_T_DEFINED
#define _WINT_T
#define _INTPTR_T_DEFINED
#define __intptr_t_defined
#define _UINTPTR_T_DEFINED
#define __uintptr_t_defined

#include "ptwin_crt.h"
#include "tchar.h"
#include "ptwin_fs.h"
