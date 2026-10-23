// Compile check: every stub class must be concrete (all interface methods implemented).
#define DIRECT3D_VERSION 0x0600
#define DIRECTDRAW_VERSION 0x0600
#include <windows.h>
#include "../ptgfx/ptgfx_stubs.h"
int main()
{
	IUnknown *objs[] = { new PtStub_IDirectDraw4, new PtStub_IDirectDrawSurface4, new PtStub_IDirectDrawClipper,
		new PtStub_IDirect3D3, new PtStub_IDirect3DDevice3, new PtStub_IDirect3DTexture2, new PtStub_IDirect3DViewport3 };
	for ( IUnknown *p : objs ) p->Release();
	return 0;
}
