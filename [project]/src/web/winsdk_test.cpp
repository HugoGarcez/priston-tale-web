// Can em++ compile against the Windows SDK headers? (web build experiment)
#include <windows.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <d3d.h>
#include <dsound.h>
#include <winsock.h>
#include <imm.h>

static_assert( sizeof(DDSURFACEDESC2)==124, "DDSURFACEDESC2 layout" );
static_assert( sizeof(D3DTLVERTEX)==32, "D3DTLVERTEX layout" );
static_assert( sizeof(MSG)==28, "MSG layout" );

static_assert( sizeof(LONG)==4 && sizeof(void *)==4, "32-bit" );

int main()
{
	RECT r;
	SetRect( &r, 0, 0, 1, 1 );
	return 0;
}
