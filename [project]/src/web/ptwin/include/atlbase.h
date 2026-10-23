// atlbase.h - web build: strclass.h includes ATL but only needs the Windows types.
#pragma once
#include <windows.h>
#ifndef ATLASSERT
#define ATLASSERT(expr) ((void)0)
#endif
