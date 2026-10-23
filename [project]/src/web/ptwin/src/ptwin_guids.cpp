// ptwin_guids.cpp - web build: the COM interface IDs the game refers to
// (DirectDraw/Direct3D/DirectShow). On Windows they come from dxguid.lib/strmiids.lib.
#define INITGUID
#include <windows.h>
#include <initguid.h>
#include <ddraw.h>
#include <d3d.h>
#include <dsound.h>
#include <dshow.h>
#include <unknwn.h>

// declared but not defined by the mingw headers (uuid.lib on Windows)
extern "C" const IID IID_IUnknown = { 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
