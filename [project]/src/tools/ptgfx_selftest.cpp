// ptgfx_selftest.cpp - checks the OpenGL DirectX-6 emulation without showing
// anything: uses a hidden window, draws like the game does, reads pixels back.
// Build + run: tools\ptgfx_selftest.bat
#include <stdio.h>
#include "../ptgfx/ptgfx.h"

static int g_Fail = 0;
#define CHECK(cond, msg) do { if (cond) printf("  ok   %s\n", msg); else { printf("  FAIL %s\n", msg); g_Fail++; } } while(0)

static LPDIRECTDRAWSURFACE4 g_Back;
static const int W = 256, H = 256;

// Reads back-buffer pixel (x,y) as 0xRRGGBB (back buffer is 16-bit 565)
static DWORD BackPixel( int x, int y )
{
	DDSURFACEDESC2 d; ZeroMemory( &d, sizeof(d) ); d.dwSize = sizeof(d);
	if ( g_Back->Lock( NULL, &d, DDLOCK_WAIT | DDLOCK_READONLY, NULL )!=DD_OK ) return 0xDEAD;
	WORD p = *(WORD *)((BYTE *)d.lpSurface + y*d.lPitch + x*2);
	g_Back->Unlock( NULL );
	DWORD r = (p>>11)&31, g = (p>>5)&63, b = p&31;
	return ((r*255/31)<<16) | ((g*255/63)<<8) | (b*255/31);
}

static BOOL Near( DWORD c, DWORD want )
{
	for ( int s=0; s<24; s+=8 ) { int a = (c>>s)&0xFF, b = (want>>s)&0xFF; if ( abs(a-b)>12 ) return FALSE; }
	return TRUE;
}

int main()
{
	DeleteFileA( "ptgfx.log" );
	WNDCLASSA wc; ZeroMemory( &wc, sizeof(wc) );
	wc.lpfnWndProc = DefWindowProcA; wc.hInstance = GetModuleHandle( NULL ); wc.lpszClassName = "ptgfxtest";
	RegisterClassA( &wc );
	HWND hWnd = CreateWindowA( "ptgfxtest", "ptgfx", WS_OVERLAPPEDWINDOW, 0, 0, W, H, NULL, NULL, wc.hInstance, NULL );	// never shown

	LPDIRECTDRAW4 dd = 0;
	CHECK( PtGfx_DirectDrawCreate( &dd )==DD_OK, "PtGfx_DirectDrawCreate" );
	CHECK( dd->SetCooperativeLevel( hWnd, DDSCL_NORMAL )==DD_OK, "SetCooperativeLevel (OpenGL 3.3 context)" );
	if ( g_Fail ) { printf( "see ptgfx.log\n" ); return 1; }

	DDSURFACEDESC2 sd; ZeroMemory( &sd, sizeof(sd) ); sd.dwSize = sizeof(sd);
	sd.dwFlags = DDSD_CAPS; sd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE;
	LPDIRECTDRAWSURFACE4 primary = 0;
	CHECK( dd->CreateSurface( &sd, &primary, NULL )==DD_OK, "primary surface" );

	ZeroMemory( &sd, sizeof(sd) ); sd.dwSize = sizeof(sd);
	sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT; sd.dwWidth = W; sd.dwHeight = H;
	sd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE | DDSCAPS_VIDEOMEMORY;
	CHECK( dd->CreateSurface( &sd, &g_Back, NULL )==DD_OK, "back buffer (render target)" );

	LPDIRECT3D3 d3d = 0;
	CHECK( dd->QueryInterface( IID_IDirect3D3, (void **)&d3d )==DD_OK, "IDirect3D3" );
	LPDIRECT3DDEVICE3 dev = 0;
	CHECK( d3d->CreateDevice( IID_IDirect3DHALDevice, g_Back, &dev, NULL )==D3D_OK, "CreateDevice (shaders compile)" );
	if ( g_Fail ) { printf( "see ptgfx.log\n" ); return 1; }

	// 1. colour fill
	DDBLTFX fx; ZeroMemory( &fx, sizeof(fx) ); fx.dwSize = sizeof(fx); fx.dwFillColor = 0x001F;		// blue in 565
	g_Back->Blt( NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx );
	CHECK( Near( BackPixel( 5, 5 ), 0x0000FF ), "colour fill -> blue" );

	// 2. untextured triangle, red, covering the centre
	D3DTLVERTEX v[4];
	ZeroMemory( v, sizeof(v) );
	float tri[3][2] = { {128,20}, {240,230}, {16,230} };
	for ( int i=0; i<3; i++ ) { v[i].sx = tri[i][0]; v[i].sy = tri[i][1]; v[i].sz = 0.5f; v[i].rhw = 1.0f; v[i].color = 0xFFFF0000; }
	dev->SetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRENDERSTATE_ZENABLE, FALSE );
	dev->SetTexture( 0, NULL );
	dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	dev->BeginScene();
	dev->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, v, 3, 0 );
	dev->EndScene();
	CHECK( Near( BackPixel( 128, 150 ), 0xFF0000 ), "triangle centre -> red" );
	CHECK( Near( BackPixel( 5, 5 ), 0x0000FF ), "triangle outside -> still blue" );
	CHECK( Near( BackPixel( 128, 25 ), 0xFF0000 ) || Near( BackPixel( 128, 30 ), 0xFF0000 ), "top of screen is top (orientation)" );

	// 3. textured quad (565 green texture, modulated with white)
	ZeroMemory( &sd, sizeof(sd) ); sd.dwSize = sizeof(sd);
	sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT; sd.dwWidth = 4; sd.dwHeight = 4;
	sd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	sd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT); sd.ddpfPixelFormat.dwFlags = DDPF_RGB; sd.ddpfPixelFormat.dwRGBBitCount = 16;
	sd.ddpfPixelFormat.dwRBitMask = 0xF800; sd.ddpfPixelFormat.dwGBitMask = 0x07E0; sd.ddpfPixelFormat.dwBBitMask = 0x001F;
	LPDIRECTDRAWSURFACE4 texSurf = 0;
	CHECK( dd->CreateSurface( &sd, &texSurf, NULL )==DD_OK, "texture surface" );
	DDSURFACEDESC2 ld; ZeroMemory( &ld, sizeof(ld) ); ld.dwSize = sizeof(ld);
	texSurf->Lock( NULL, &ld, DDLOCK_WAIT, NULL );
	for ( int y=0; y<4; y++ ) for ( int x=0; x<4; x++ ) *(WORD *)((BYTE *)ld.lpSurface + y*ld.lPitch + x*2) = 0x07E0;
	texSurf->Unlock( NULL );
	LPDIRECT3DTEXTURE2 tex = 0;
	CHECK( texSurf->QueryInterface( IID_IDirect3DTexture2, (void **)&tex )==DD_OK, "IDirect3DTexture2" );
	D3DTLVERTEX q[4]; ZeroMemory( q, sizeof(q) );
	float qp[4][4] = { {10,10,0,0}, {60,10,1,0}, {10,60,0,1}, {60,60,1,1} };
	for ( int i=0; i<4; i++ ) { q[i].sx = qp[i][0]; q[i].sy = qp[i][1]; q[i].tu = qp[i][2]; q[i].tv = qp[i][3]; q[i].sz = 0.1f; q[i].rhw = 1; q[i].color = 0xFFFFFFFF; }
	dev->SetTexture( 0, tex );
	dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, q, 4, 0 );
	CHECK( Near( BackPixel( 35, 35 ), 0x00FF00 ), "textured quad -> green" );

	// 4. alpha blending: 50% white over the green quad
	for ( int i=0; i<4; i++ ) q[i].color = 0x80FFFFFF;
	dev->SetTexture( 0, NULL );
	dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	dev->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	dev->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	dev->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, q, 4, 0 );
	dev->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
	CHECK( Near( BackPixel( 35, 35 ), 0x80FF80 ), "alpha blend -> light green" );

	// 5. GDI text through the overlay
	HDC hdc = 0;
	CHECK( g_Back->GetDC( &hdc )==DD_OK, "GetDC on back buffer" );
	HFONT hFont = PtGfx_CreateFontA( 40, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Arial" );
	HGDIOBJ old = SelectObject( hdc, hFont );
	SetBkMode( hdc, TRANSPARENT );
	SetTextColor( hdc, RGB(255,255,0) );
	TextOutA( hdc, 150, 20, "WW", 2 );
	SelectObject( hdc, old );
	DeleteObject( hFont );
	g_Back->ReleaseDC( hdc );
	int yellow = 0, blueAround = 0;
	for ( int y=20; y<60; y++ ) for ( int x=150; x<220; x++ ) { DWORD c = BackPixel( x, y ); if ( Near( c, 0xFFFF00 ) ) yellow++; }
	blueAround = Near( BackPixel( 250, 5 ), 0x0000FF );
	CHECK( yellow>50, "GDI text drawn (yellow pixels)" );
	CHECK( blueAround, "overlay leaves the rest untouched" );

	// 6. colour-keyed BltFast from a CPU surface (black = transparent)
	ZeroMemory( &sd, sizeof(sd) ); sd.dwSize = sizeof(sd);
	sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT; sd.dwWidth = 8; sd.dwHeight = 8;
	sd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	LPDIRECTDRAWSURFACE4 img = 0;
	CHECK( dd->CreateSurface( &sd, &img, NULL )==DD_OK, "offscreen image surface" );
	img->Lock( NULL, &ld, DDLOCK_WAIT, NULL );
	for ( int y=0; y<8; y++ ) for ( int x=0; x<8; x++ ) *(WORD *)((BYTE *)ld.lpSurface + y*ld.lPitch + x*2) = ( x<4 ) ? 0xFFFF : 0x0000;
	img->Unlock( NULL );
	DDCOLORKEY key = { 0, 0 };
	img->SetColorKey( DDCKEY_SRCBLT, &key );
	g_Back->BltFast( 200, 200, img, NULL, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
	CHECK( Near( BackPixel( 201, 202 ), 0xFFFFFF ), "colour-keyed blit: opaque part drawn" );
	CHECK( !Near( BackPixel( 206, 202 ), 0x000000 ), "colour-keyed blit: key colour skipped" );

	// 7. viewport Clear2: colour and depth (the game clears this way every frame)
	LPDIRECT3DVIEWPORT3 vp = 0;
	CHECK( d3d->CreateViewport( &vp, NULL )==D3D_OK, "viewport" );
	D3DVIEWPORT2 vd; ZeroMemory( &vd, sizeof(vd) ); vd.dwSize = sizeof(vd); vd.dwWidth = W; vd.dwHeight = H;
	vp->SetViewport2( &vd );
	dev->AddViewport( vp );
	dev->SetCurrentViewport( vp );
	D3DRECT all = { 0, 0, W, H };
	CHECK( vp->Clear2( 1, &all, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF00FF00, 1.0f, 0 )==D3D_OK, "Clear2 returns OK" );
	CHECK( Near( BackPixel( 100, 100 ), 0x00FF00 ), "Clear2 colour -> green" );
	// depth: near red quad (z 0.2), then far blue quad (z 0.8) must stay hidden
	dev->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
	dev->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	dev->SetRenderState( D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
	for ( int i=0; i<4; i++ ) { q[i].sx = qp[i][0]*2; q[i].sy = qp[i][1]*2; q[i].sz = 0.2f; q[i].color = 0xFFFF0000; }
	dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, q, 4, 0 );
	for ( int i=0; i<4; i++ ) { q[i].sz = 0.8f; q[i].color = 0xFF0000FF; }
	dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, q, 4, 0 );
	CHECK( Near( BackPixel( 70, 70 ), 0xFF0000 ), "depth test keeps the nearer quad" );
	vp->Clear2( 1, &all, D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
	dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, q, 4, 0 );
	CHECK( Near( BackPixel( 70, 70 ), 0x0000FF ), "Clear2 depth lets the far quad draw" );

	// 8. login-screen fade: DirectX 5 TEXTUREMAPBLEND MODULATE with a texture that has
	//    no alpha takes alpha from the vertex, so a black box at vertex alpha 0 is invisible
	vp->Clear2( 1, &all, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF00FF00, 1.0f, 0 );
	texSurf->Lock( NULL, &ld, DDLOCK_WAIT, NULL );
	for ( int y=0; y<4; y++ ) for ( int x=0; x<4; x++ ) *(WORD *)((BYTE *)ld.lpSurface + y*ld.lPitch + x*2) = 0;		// black
	texSurf->Unlock( NULL );
	dev->SetTexture( 0, tex );
	dev->SetRenderState( D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE );
	dev->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	dev->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	dev->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	for ( int i=0; i<4; i++ ) { q[i].sz = 0.0f; q[i].color = 0x00FFFFFF; }			// vertex alpha 0
	dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, q, 4, 0 );
	CHECK( Near( BackPixel( 70, 70 ), 0x00FF00 ), "MODULATE fade: alpha-less texture uses vertex alpha (0 = invisible)" );
	for ( int i=0; i<4; i++ ) q[i].color = 0xFFFFFFFF;								// vertex alpha 255
	dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, q, 4, 0 );
	CHECK( Near( BackPixel( 70, 70 ), 0x000000 ), "MODULATE fade: vertex alpha 255 = opaque black" );

	//    the real login fade (WhiteBox.tga) is an opaque A4444 texture: vertex alpha 0 must still hide it
	vp->Clear2( 1, &all, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF00FF00, 1.0f, 0 );
	ZeroMemory( &sd, sizeof(sd) ); sd.dwSize = sizeof(sd);
	sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT; sd.dwWidth = 4; sd.dwHeight = 4;
	sd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	sd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT); sd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS; sd.ddpfPixelFormat.dwRGBBitCount = 16;
	sd.ddpfPixelFormat.dwRBitMask = 0x0F00; sd.ddpfPixelFormat.dwGBitMask = 0x00F0; sd.ddpfPixelFormat.dwBBitMask = 0x000F;
	sd.ddpfPixelFormat.dwRGBAlphaBitMask = 0xF000;
	LPDIRECTDRAWSURFACE4 texA = 0;
	CHECK( dd->CreateSurface( &sd, &texA, NULL )==DD_OK, "A4444 texture surface" );
	texA->Lock( NULL, &ld, DDLOCK_WAIT, NULL );
	for ( int y=0; y<4; y++ ) for ( int x=0; x<4; x++ ) *(WORD *)((BYTE *)ld.lpSurface + y*ld.lPitch + x*2) = 0xF000;	// opaque black
	texA->Unlock( NULL );
	LPDIRECT3DTEXTURE2 texA2 = 0;
	texA->QueryInterface( IID_IDirect3DTexture2, (void **)&texA2 );
	dev->SetTexture( 0, texA2 );
	for ( int i=0; i<4; i++ ) q[i].color = 0x00FFFFFF;								// vertex alpha 0
	dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, q, 4, 0 );
	CHECK( Near( BackPixel( 70, 70 ), 0x00FF00 ), "MODULATE fade: opaque alpha texture at vertex alpha 0 = invisible" );
	for ( int i=0; i<4; i++ ) q[i].color = 0xFFFFFFFF;
	dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, q, 4, 0 );
	CHECK( Near( BackPixel( 70, 70 ), 0x000000 ), "MODULATE fade: opaque alpha texture at vertex alpha 255 = black" );
	dev->SetTexture( 0, NULL );
	if ( texA2 ) texA2->Release();
	texA->Release();
	dev->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );

	// 9. text: our rasterizer (ptgfx_text.cpp) draws through the back-buffer overlay
	vp->Clear2( 1, &all, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0 );
	{
		HDC hdc = 0;
		CHECK( g_Back->GetDC( &hdc )==DD_OK && hdc, "back buffer GetDC" );
		HFONT hFont = CreateFontA( 16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Tahoma" );
		HGDIOBJ hOld = SelectObject( hdc, hFont );
		SetTextColor( hdc, RGB( 255, 255, 255 ) );
		SetBkMode( hdc, TRANSPARENT );
		SIZE sz = { 0, 0 };
		GetTextExtentPoint32A( hdc, "Hello", 5, &sz );
		CHECK( sz.cx>=25 && sz.cx<=60 && sz.cy==16, "text extent of \"Hello\" at height 16" );
		TextOutA( hdc, 20, 20, "Hello", 5 );
		RECT rc = { 100, 100, 220, 140 };
		DrawTextA( hdc, "Right", -1, &rc, DT_RIGHT );
		SelectObject( hdc, hOld );
		DeleteObject( hFont );
		g_Back->ReleaseDC( hdc );

		int nFull = 0, nPartial = 0;
		for ( int y=20; y<36; y++ ) for ( int x=20; x<20+sz.cx; x++ ) {
			DWORD c = BackPixel( x, y );
			int g = (c>>8)&0xFF;
			if ( g>230 ) nFull++; else if ( g>40 ) nPartial++;
		}
		CHECK( nFull>20, "TextOut draws white glyph pixels" );
		CHECK( nPartial>5, "TextOut glyph edges are antialiased (own rasterizer)" );
		int nRight = 0, nLeft = 0;
		for ( int y=100; y<116; y++ ) for ( int x=100; x<220; x++ )
			if ( ((BackPixel( x, y )>>8)&0xFF)>100 ) { if ( x>=160 ) nRight++; else nLeft++; }
		CHECK( nRight>10 && nLeft==0, "DrawText DT_RIGHT puts text at the right edge" );
		CHECK( Near( BackPixel( 240, 240 ), 0x000000 ), "text leaves the rest of the screen alone" );
	}

	// 10. present (to the hidden window) must not fail
	RECT r = { 0, 0, W, H };
	CHECK( primary->Blt( &r, g_Back, &r, DDBLT_WAIT, NULL )==DD_OK, "present" );

	printf( g_Fail ? "\n%d check(s) FAILED\n" : "\nall checks passed\n", g_Fail );
	return g_Fail ? 1 : 0;
}
