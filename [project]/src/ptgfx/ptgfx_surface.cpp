// ptgfx_surface.cpp - DirectDraw emulation: pixel formats, surfaces, blits,
// GetDC/text overlay, presenting, and the IDirectDraw4 object.
// See ptgfx.h for the overall design.

#include <stdio.h>
#include "ptgfx_internal.h"

PtGLState	g_PtGL;
PtFormat	g_PtDisplayFormat;
static DWORD g_PtDisplayW = 800, g_PtDisplayH = 600;


//////////////////////////////////////////////////////////////////////////////
// PtFormat
//////////////////////////////////////////////////////////////////////////////
static int MaskShift( DWORD m ) { int s = 0; if ( !m ) return 0; while ( !(m&1) ) { m >>= 1; s++; } return s; }
static int MaskBits( DWORD m )  { int b = 0; m >>= MaskShift( m ); while ( m&1 ) { m >>= 1; b++; } return b; }

void PtFormat::SetRGB( DWORD nBpp, DWORD r, DWORD g, DWORD b, DWORD a )
{
	bpp = nBpp;
	mask[0] = r; mask[1] = g; mask[2] = b; mask[3] = a;
	for ( int i=0; i<4; i++ ) { shift[i] = MaskShift( mask[i] ); bits[i] = MaskBits( mask[i] ); }
}

void PtFormat::Set( const DDPIXELFORMAT &pf )
{
	if ( pf.dwFlags & DDPF_ZBUFFER ) { SetRGB( pf.dwZBufferBitDepth ? pf.dwZBufferBitDepth : 16, 0, 0, 0, 0 ); return; }
	SetRGB( pf.dwRGBBitCount ? pf.dwRGBBitCount : 16, pf.dwRBitMask, pf.dwGBitMask, pf.dwBBitMask,
		(pf.dwFlags & DDPF_ALPHAPIXELS) ? pf.dwRGBAlphaBitMask : 0 );
}

void PtFormat::ToDDPF( DDPIXELFORMAT *pf ) const
{
	ZeroMemory( pf, sizeof(DDPIXELFORMAT) );
	pf->dwSize = sizeof(DDPIXELFORMAT);
	pf->dwFlags = DDPF_RGB | ( mask[3] ? DDPF_ALPHAPIXELS : 0 );
	pf->dwRGBBitCount = bpp;
	pf->dwRBitMask = mask[0]; pf->dwGBitMask = mask[1]; pf->dwBBitMask = mask[2];
	pf->dwRGBAlphaBitMask = mask[3];
}

DWORD PtFormat::Read( const BYTE *p ) const
{
	switch ( bpp ) {
	case 16: return *(const WORD *)p;
	case 24: return p[0] | (p[1]<<8) | (p[2]<<16);
	case 32: return *(const DWORD *)p;
	}
	return *p;
}

void PtFormat::Write( BYTE *p, DWORD raw ) const
{
	switch ( bpp ) {
	case 16: *(WORD *)p = (WORD)raw; break;
	case 24: p[0] = (BYTE)raw; p[1] = (BYTE)(raw>>8); p[2] = (BYTE)(raw>>16); break;
	case 32: *(DWORD *)p = raw; break;
	default: *p = (BYTE)raw;
	}
}

static inline DWORD Expand( DWORD raw, DWORD mask, int shift, int bits, DWORD def )
{
	if ( !mask ) return def;
	DWORD c = (raw & mask) >> shift;
	DWORD maxv = (1u<<bits) - 1;
	return ( c*255 + maxv/2 ) / maxv;
}

DWORD PtFormat::ToRGBA( DWORD raw ) const
{
	DWORD r = Expand( raw, mask[0], shift[0], bits[0], 0 );
	DWORD g = Expand( raw, mask[1], shift[1], bits[1], 0 );
	DWORD b = Expand( raw, mask[2], shift[2], bits[2], 0 );
	DWORD a = Expand( raw, mask[3], shift[3], bits[3], 255 );
	return r | (g<<8) | (b<<16) | (a<<24);
}

static inline DWORD Pack( DWORD c8, DWORD mask, int shift, int bits )
{
	if ( !mask ) return 0;
	DWORD maxv = (1u<<bits) - 1;
	return ( ((c8*maxv + 127)/255) << shift ) & mask;
}

DWORD PtFormat::FromRGBA( DWORD rgba ) const
{
	return Pack( rgba&0xFF, mask[0], shift[0], bits[0] ) | Pack( (rgba>>8)&0xFF, mask[1], shift[1], bits[1] ) |
		   Pack( (rgba>>16)&0xFF, mask[2], shift[2], bits[2] ) | Pack( rgba>>24, mask[3], shift[3], bits[3] );
}

//////////////////////////////////////////////////////////////////////////////
// PtTexture: IDirect3DTexture2 view of a surface
//////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(HRESULT) PtTexture::QueryInterface( REFIID riid, LPVOID FAR * ppvObj ) { return m_pSurface->QueryInterface( riid, ppvObj ); }
STDMETHODIMP_(ULONG) PtTexture::AddRef()  { return m_pSurface->AddRef(); }
STDMETHODIMP_(ULONG) PtTexture::Release() { return m_pSurface->Release(); }

//////////////////////////////////////////////////////////////////////////////
// PtSurface
//////////////////////////////////////////////////////////////////////////////
PtSurface::PtSurface()
{
	ZeroMemory( &m_Desc, sizeof(m_Desc) );
	ZeroMemory( &m_Format, sizeof(m_Format) );
	m_Kind = PTSURF_PLAIN;
	m_pPixels = 0; m_Pitch = 0;
	m_pAttached = 0;
	m_Texture.m_pSurface = this;
	m_dwLockFlags = 0;
	m_bHasSrcKey = FALSE;
	ZeroMemory( &m_SrcKey, sizeof(m_SrcKey) );
	m_GLTex = 0; m_bDirty = TRUE; m_bGPUNewer = FALSE; m_FBO = 0; m_DepthRB = 0;
	m_hDC = 0; m_hDIB = 0; m_hOldBmp = 0; m_pDIBBits = 0;
}

PtSurface::~PtSurface()
{
	if ( g_PtGL.pBack==this ) g_PtGL.pBack = 0;
	if ( g_PtGL.pPrimary==this ) g_PtGL.pPrimary = 0;
	if ( g_PtGL.pRenderTarget==this ) g_PtGL.pRenderTarget = g_PtGL.pBack;
	if ( m_pAttached ) m_pAttached->Release();
	if ( m_hDC ) { SelectObject( m_hDC, m_hOldBmp ); DeleteDC( m_hDC ); }
	if ( m_hDIB ) DeleteObject( m_hDIB );
	if ( g_PtGL.bContext ) {
		if ( m_GLTex ) glDeleteTextures( 1, &m_GLTex );
		if ( m_FBO ) glDeleteFramebuffers( 1, &m_FBO );
		if ( m_DepthRB ) glDeleteRenderbuffers( 1, &m_DepthRB );
	}
	free( m_pPixels );
}

BOOL PtSurface::Create( DDSURFACEDESC2 *lpDesc, PtSurfaceKind kind )
{
	m_Desc = *lpDesc;
	m_Desc.dwSize = sizeof(DDSURFACEDESC2);
	m_Kind = kind;

	if ( kind==PTSURF_PRIMARY || !(m_Desc.dwFlags & DDSD_WIDTH) ) {
		if ( kind==PTSURF_PRIMARY || kind==PTSURF_BACK ) {
			m_Desc.dwWidth = g_PtDisplayW;
			m_Desc.dwHeight = g_PtDisplayH;
		}
	}
	if ( !m_Desc.dwWidth ) m_Desc.dwWidth = 1;
	if ( !m_Desc.dwHeight ) m_Desc.dwHeight = 1;

	if ( kind==PTSURF_ZBUFFER )
		m_Format.Set( m_Desc.ddpfPixelFormat );
	else if ( (m_Desc.dwFlags & DDSD_PIXELFORMAT) && m_Desc.ddpfPixelFormat.dwRGBBitCount )
		m_Format.Set( m_Desc.ddpfPixelFormat );
	else
		m_Format = g_PtDisplayFormat;

	m_Desc.dwFlags |= DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_CAPS | DDSD_PITCH;
	if ( kind!=PTSURF_ZBUFFER ) m_Format.ToDDPF( &m_Desc.ddpfPixelFormat );

	m_Pitch = ( Width()*m_Format.BytesPerPixel() + 3 ) & ~3;
	m_Desc.lPitch = m_Pitch;
	m_pPixels = (BYTE *)calloc( m_Pitch, Height() );
	if ( !m_pPixels ) return FALSE;

	if ( kind==PTSURF_BACK ) {
		if ( !MakeRenderTarget() ) return FALSE;
		if ( !g_PtGL.pBack ) g_PtGL.pBack = this;
		if ( !g_PtGL.pRenderTarget ) g_PtGL.pRenderTarget = this;
	}
	return TRUE;
}

DWORD PtSurface::PixelRGBA( int x, int y ) const
{
	return m_Format.ToRGBA( m_Format.Read( m_pPixels + y*m_Pitch + x*m_Format.BytesPerPixel() ) );
}

// Converts the CPU copy (and mip chain) to RGBA and uploads it.
GLuint PtSurface::GetGLTexture()
{
	if ( m_GLTex && (!m_bDirty || m_bGPUNewer) ) return m_GLTex;
	if ( !m_GLTex ) glGenTextures( 1, &m_GLTex );
	glBindTexture( GL_TEXTURE_2D, m_GLTex );
	g_PtStats.Uploads++;

	int level = 0;
	for ( PtSurface *s = this; s; s = s->m_pAttached, level++ ) {
		int w = s->Width(), h = s->Height();
		DWORD *rgba = (DWORD *)malloc( w*h*4 );
		if ( !rgba ) break;
		DWORD bpp = s->m_Format.BytesPerPixel();
		for ( int y=0; y<h; y++ ) {
			const BYTE *src = s->m_pPixels + y*s->m_Pitch;
			DWORD *dst = rgba + y*w;
			for ( int x=0; x<w; x++, src+=bpp ) {
				DWORD raw = s->m_Format.Read( src );
				dst[x] = s->IsKeyed( raw ) ? 0 : s->m_Format.ToRGBA( raw ) | ( s->m_bHasSrcKey && !s->m_Format.mask[3] ? 0xFF000000 : 0 );
			}
		}
		glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
		glTexImage2D( GL_TEXTURE_2D, level, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba );
		free( rgba );
		s->m_bDirty = FALSE;
		if ( !(s->m_pAttached && (s->m_pAttached->m_Desc.ddsCaps.dwCaps & DDSCAPS_MIPMAP)) ) break;
	}
	glTexParameteri( GL_TEXTURE_2D, 0x813D /*GL_TEXTURE_MAX_LEVEL*/, level>0 ? level : 0 );
	m_bDirty = FALSE;
	return m_GLTex;
}

BOOL PtSurface::MakeRenderTarget()
{
	if ( m_FBO ) return TRUE;
	if ( !g_PtGL.bContext ) return FALSE;
	if ( !m_GLTex ) {
		glGenTextures( 1, &m_GLTex );
		glBindTexture( GL_TEXTURE_2D, m_GLTex );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, Width(), Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, 0x813D, 0 );
	}
	glGenRenderbuffers( 1, &m_DepthRB );
	glBindRenderbuffer( GL_RENDERBUFFER, m_DepthRB );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width(), Height() );
	glGenFramebuffers( 1, &m_FBO );
	glBindFramebuffer( GL_FRAMEBUFFER, m_FBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GLTex, 0 );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRB );
	if ( glCheckFramebufferStatus( GL_FRAMEBUFFER )!=GL_FRAMEBUFFER_COMPLETE ) {
		PtGL_Log( "render target %dx%d incomplete", Width(), Height() );
		return FALSE;
	}
	glClearColor( 0, 0, 0, 1 );
	glClearDepth( 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	m_bDirty = FALSE;
	PtGfx_BindRenderTarget( g_PtGL.pRenderTarget ? g_PtGL.pRenderTarget : this );
	return TRUE;
}

// Render targets are kept top-down (row 0 = top of the screen), like uploaded textures.
void PtSurface::ReadBackFromGPU()
{
	if ( !m_FBO || !m_bGPUNewer ) return;
	int w = Width(), h = Height();
	DWORD *rgba = (DWORD *)malloc( w*h*4 );
	if ( !rgba ) return;
	glBindFramebuffer( GL_READ_FRAMEBUFFER, m_FBO );
	glPixelStorei( GL_PACK_ALIGNMENT, 4 );
	glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, rgba );
	DWORD bpp = m_Format.BytesPerPixel();
	for ( int y=0; y<h; y++ ) {
		BYTE *dst = m_pPixels + y*m_Pitch;
		for ( int x=0; x<w; x++, dst+=bpp ) m_Format.Write( dst, m_Format.FromRGBA( rgba[y*w+x] ) );
	}
	free( rgba );
	m_bGPUNewer = FALSE;
	PtGfx_BindRenderTarget( g_PtGL.pRenderTarget );
}

STDMETHODIMP_(HRESULT) PtSurface::QueryInterface( REFIID riid, LPVOID FAR * ppvObj )
{
	if ( !ppvObj ) return E_POINTER;
	if ( riid==IID_IDirect3DTexture2 ) { *ppvObj = &m_Texture; AddRef(); return DD_OK; }
	if ( riid==IID_IDirectDrawSurface4 || riid==IID_IUnknown ) { *ppvObj = this; AddRef(); return DD_OK; }
	*ppvObj = 0;
	return E_NOINTERFACE;
}

STDMETHODIMP_(HRESULT) PtSurface::AddAttachedSurface( LPDIRECTDRAWSURFACE4 ) { return DD_OK; }			// z-buffer: render targets have their own depth
STDMETHODIMP_(HRESULT) PtSurface::DeleteAttachedSurface( DWORD, LPDIRECTDRAWSURFACE4 ) { return DD_OK; }

STDMETHODIMP_(HRESULT) PtSurface::GetAttachedSurface( LPDDSCAPS2 lpCaps, LPDIRECTDRAWSURFACE4 FAR *lplpSurface )
{
	if ( !lplpSurface || !m_pAttached ) return DDERR_NOTFOUND;
	m_pAttached->AddRef();
	*lplpSurface = m_pAttached;
	return DD_OK;
}

STDMETHODIMP_(HRESULT) PtSurface::GetCaps( LPDDSCAPS2 lpCaps )
{
	if ( lpCaps ) *lpCaps = m_Desc.ddsCaps;
	return DD_OK;
}

STDMETHODIMP_(HRESULT) PtSurface::GetColorKey( DWORD dwFlags, LPDDCOLORKEY lpKey )
{
	if ( !m_bHasSrcKey ) return DDERR_NOCOLORKEY;
	if ( lpKey ) *lpKey = m_SrcKey;
	return DD_OK;
}

STDMETHODIMP_(HRESULT) PtSurface::SetColorKey( DWORD dwFlags, LPDDCOLORKEY lpKey )
{
	if ( dwFlags & DDCKEY_SRCBLT ) {
		m_bHasSrcKey = lpKey!=NULL;
		if ( lpKey ) {
			m_SrcKey = *lpKey;
			if ( !(dwFlags & DDCKEY_COLORSPACE) ) m_SrcKey.dwColorSpaceHighValue = m_SrcKey.dwColorSpaceLowValue;
		}
		m_bDirty = TRUE;
	}
	return DD_OK;
}

STDMETHODIMP_(HRESULT) PtSurface::GetPixelFormat( LPDDPIXELFORMAT lpPF )
{
	if ( !lpPF ) return DDERR_INVALIDPARAMS;
	*lpPF = m_Desc.ddpfPixelFormat;
	return DD_OK;
}

STDMETHODIMP_(HRESULT) PtSurface::GetSurfaceDesc( LPDDSURFACEDESC2 lpDesc )
{
	if ( !lpDesc ) return DDERR_INVALIDPARAMS;
	*lpDesc = m_Desc;
	lpDesc->lpSurface = 0;
	return DD_OK;
}

STDMETHODIMP_(HRESULT) PtSurface::Lock( LPRECT lpRect, LPDDSURFACEDESC2 lpDesc, DWORD dwFlags, HANDLE )
{
	if ( !lpDesc ) return DDERR_INVALIDPARAMS;
	if ( m_bGPUNewer ) ReadBackFromGPU();
	m_dwLockFlags = dwFlags;
	*lpDesc = m_Desc;
	lpDesc->lPitch = m_Pitch;
	lpDesc->lpSurface = m_pPixels;
	if ( lpRect ) lpDesc->lpSurface = m_pPixels + lpRect->top*m_Pitch + lpRect->left*m_Format.BytesPerPixel();
	return DD_OK;
}

STDMETHODIMP_(HRESULT) PtSurface::Unlock( LPRECT )
{
	if ( m_dwLockFlags & DDLOCK_READONLY ) { m_dwLockFlags = 0; return DD_OK; }
	m_bDirty = TRUE;
	if ( m_FBO ) {
		// CPU writes into a render target: push them to the GPU copy.
		BOOL bWasNewer = m_bGPUNewer;
		m_bGPUNewer = FALSE;
		GLuint tex = m_GLTex; m_GLTex = 0;				// upload into a fresh texture...
		GetGLTexture();
		RECT r = { 0, 0, Width(), Height() };
		PtSurface *pOld = g_PtGL.pRenderTarget;
		PtGfx_BindRenderTarget( this );
		GLuint uploaded = m_GLTex; m_GLTex = tex;		// ...then draw it into the render target
		PtGfx_Draw2DQuad( uploaded, Width(), Height(), r, r, FALSE, TRUE );
		glDeleteTextures( 1, &uploaded );
		PtGfx_BindRenderTarget( pOld );
		m_bGPUNewer = bWasNewer;
		m_bDirty = FALSE;
	}
	return DD_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Blits
//////////////////////////////////////////////////////////////////////////////
static void ClampRect( RECT &r, int w, int h )
{
	if ( r.left<0 ) r.left = 0;
	if ( r.top<0 ) r.top = 0;
	if ( r.right>w ) r.right = w;
	if ( r.bottom>h ) r.bottom = h;
}

// CPU copy with scaling, format conversion and optional source colour key.
static void CPUBlt( PtSurface *pDst, RECT dst, PtSurface *pSrc, RECT src, BOOL bKey )
{
	int dw = dst.right-dst.left, dh = dst.bottom-dst.top;
	int sw = src.right-src.left, sh = src.bottom-src.top;
	if ( dw<=0 || dh<=0 || sw<=0 || sh<=0 ) return;
	DWORD dbpp = pDst->m_Format.BytesPerPixel(), sbpp = pSrc->m_Format.BytesPerPixel();
	BOOL bSameFormat = pDst->m_Format.bpp==pSrc->m_Format.bpp && !memcmp( pDst->m_Format.mask, pSrc->m_Format.mask, sizeof(pDst->m_Format.mask) );
	for ( int y=0; y<dh; y++ ) {
		int dy = dst.top+y;
		if ( dy<0 || dy>=pDst->Height() ) continue;
		int sy = src.top + y*sh/dh;
		for ( int x=0; x<dw; x++ ) {
			int dx = dst.left+x;
			if ( dx<0 || dx>=pDst->Width() ) continue;
			int sx = src.left + x*sw/dw;
			DWORD raw = pSrc->m_Format.Read( pSrc->m_pPixels + sy*pSrc->m_Pitch + sx*sbpp );
			if ( bKey && pSrc->IsKeyed( raw ) ) continue;
			if ( !bSameFormat ) raw = pDst->m_Format.FromRGBA( pSrc->m_Format.ToRGBA( raw ) );
			pDst->m_Format.Write( pDst->m_pPixels + dy*pDst->m_Pitch + dx*dbpp, raw );
		}
	}
	pDst->m_bDirty = TRUE;
}

STDMETHODIMP_(HRESULT) PtSurface::Blt( LPRECT lpDst, LPDIRECTDRAWSURFACE4 lpSrcSurf, LPRECT lpSrc, DWORD dwFlags, LPDDBLTFX lpFX )
{
	PtSurface *pSrc = (PtSurface *)lpSrcSurf;
	RECT dst = { 0, 0, Width(), Height() };
	if ( lpDst ) dst = *lpDst;

	// Presenting: primary <- back buffer
	if ( m_Kind==PTSURF_PRIMARY ) {
		if ( pSrc ) {
			RECT src = { 0, 0, pSrc->Width(), pSrc->Height() };
			if ( lpSrc ) src = *lpSrc;
			PtGfx_Present( &src );
		}
		return DD_OK;
	}

	if ( dwFlags & DDBLT_COLORFILL ) {
		DWORD fill = lpFX ? lpFX->dwFillColor : 0;
		if ( m_FBO ) {
			PtSurface *pOld = g_PtGL.pRenderTarget;
			PtGfx_BindRenderTarget( this );
			DWORD rgba = m_Format.ToRGBA( fill );
			ClampRect( dst, Width(), Height() );
			glEnable( GL_SCISSOR_TEST );
			glScissor( dst.left, dst.top, dst.right-dst.left, dst.bottom-dst.top );		// top-down render target
			glColorMask( 1, 1, 1, 1 );
			glClearColor( (rgba&0xFF)/255.0f, ((rgba>>8)&0xFF)/255.0f, ((rgba>>16)&0xFF)/255.0f, 1.0f );
			glClear( GL_COLOR_BUFFER_BIT );
			glDisable( GL_SCISSOR_TEST );
			g_PtStats.Fills++;
			if ( g_PtTrace ) { fprintf( g_PtTrace, "COLORFILL %08X rect %d,%d-%d,%d\n", fill, dst.left, dst.top, dst.right, dst.bottom ); PtGfx_TracePixels( "fill" ); }
			PtGfx_InvalidateDeviceState();
			PtGfx_BindRenderTarget( pOld );
			m_bGPUNewer = TRUE;
		}
		else {
			ClampRect( dst, Width(), Height() );
			DWORD bpp = m_Format.BytesPerPixel();
			for ( int y=dst.top; y<dst.bottom; y++ )
				for ( int x=dst.left; x<dst.right; x++ ) m_Format.Write( m_pPixels + y*m_Pitch + x*bpp, fill );
			m_bDirty = TRUE;
		}
		return DD_OK;
	}

	if ( !pSrc ) return DDERR_INVALIDPARAMS;
	RECT src = { 0, 0, pSrc->Width(), pSrc->Height() };
	if ( lpSrc ) src = *lpSrc;
	BOOL bKey = (dwFlags & (DDBLT_KEYSRC|DDBLT_KEYSRCOVERRIDE)) && pSrc->m_bHasSrcKey;

	if ( m_FBO ) {
		// onto the GPU (back buffer / render target): draw the source as a textured quad
		GLuint tex = pSrc->GetGLTexture();
		PtSurface *pOld = g_PtGL.pRenderTarget;
		PtGfx_BindRenderTarget( this );
		PtGfx_Draw2DQuad( tex, pSrc->Width(), pSrc->Height(), src, dst, bKey, !bKey );
		g_PtStats.Blits++;
		if ( g_PtTrace ) { fprintf( g_PtTrace, "BLT src %dx%d key %d -> %d,%d-%d,%d\n", pSrc->Width(), pSrc->Height(), bKey, dst.left, dst.top, dst.right, dst.bottom ); PtGfx_TracePixels( "blt" ); }
		PtGfx_BindRenderTarget( pOld );
		m_bGPUNewer = TRUE;
		return DD_OK;
	}

	if ( pSrc->m_bGPUNewer ) pSrc->ReadBackFromGPU();
	CPUBlt( this, dst, pSrc, src, bKey );
	return DD_OK;
}

STDMETHODIMP_(HRESULT) PtSurface::BltFast( DWORD x, DWORD y, LPDIRECTDRAWSURFACE4 lpSrcSurf, LPRECT lpSrc, DWORD dwTrans )
{
	PtSurface *pSrc = (PtSurface *)lpSrcSurf;
	if ( !pSrc ) return DDERR_INVALIDPARAMS;
	RECT src = { 0, 0, pSrc->Width(), pSrc->Height() };
	if ( lpSrc ) src = *lpSrc;
	RECT dst = { (LONG)x, (LONG)y, (LONG)x + src.right-src.left, (LONG)y + src.bottom-src.top };
	return Blt( &dst, lpSrcSurf, &src, (dwTrans & DDBLTFAST_SRCCOLORKEY) ? DDBLT_KEYSRC : 0, NULL );
}

STDMETHODIMP_(HRESULT) PtSurface::Flip( LPDIRECTDRAWSURFACE4, DWORD )
{
	PtGfx_Present( NULL );
	return DD_OK;
}

//////////////////////////////////////////////////////////////////////////////
// GetDC / ReleaseDC
//////////////////////////////////////////////////////////////////////////////
static HBITMAP CreateDIB32( HDC *phDC, DWORD **ppBits, int w, int h, HGDIOBJ *phOld )
{
	BITMAPINFO bmi;
	ZeroMemory( &bmi, sizeof(bmi) );
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h;			// top-down
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	HBITMAP hBmp = CreateDIBSection( NULL, &bmi, DIB_RGB_COLORS, (void **)ppBits, NULL, 0 );
	if ( !hBmp ) return NULL;
	*phDC = CreateCompatibleDC( NULL );
	*phOld = SelectObject( *phDC, hBmp );
	return hBmp;
}

static inline DWORD BGRAtoRGBA( DWORD c ) { return (c & 0xFF00FF00) | ((c>>16)&0xFF) | ((c&0xFF)<<16); }

STDMETHODIMP_(HRESULT) PtSurface::GetDC( HDC FAR *lphDC )
{
	if ( !lphDC ) return DDERR_INVALIDPARAMS;

	if ( this==g_PtGL.pBack ) {
		// Text overlay: GDI draws onto a key-coloured bitmap; ReleaseDC draws what changed.
		if ( !g_PtGL.hOverlayDC || g_PtGL.OverlayW!=Width() || g_PtGL.OverlayH!=Height() ) {
			if ( g_PtGL.hOverlayDC ) { SelectObject( g_PtGL.hOverlayDC, g_PtGL.hOverlayOld ); DeleteDC( g_PtGL.hOverlayDC ); DeleteObject( g_PtGL.hOverlayDIB ); }
			g_PtGL.hOverlayDIB = CreateDIB32( &g_PtGL.hOverlayDC, &g_PtGL.pOverlayBits, Width(), Height(), &g_PtGL.hOverlayOld );
			if ( !g_PtGL.hOverlayDIB ) return DDERR_OUTOFMEMORY;
			g_PtGL.OverlayW = Width(); g_PtGL.OverlayH = Height();
			for ( int i=0; i<Width()*Height(); i++ ) g_PtGL.pOverlayBits[i] = PT_OVERLAY_KEY;
			if ( !g_PtGL.OverlayTex ) glGenTextures( 1, &g_PtGL.OverlayTex );
			glBindTexture( GL_TEXTURE_2D, g_PtGL.OverlayTex );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, Width(), Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
		}
		SetBoundsRect( g_PtGL.hOverlayDC, NULL, DCB_ENABLE | DCB_RESET );
		*lphDC = g_PtGL.hOverlayDC;
		return DD_OK;
	}

	if ( !m_hDC && !CreateDIB32( &m_hDC, &m_pDIBBits, Width(), Height(), &m_hOldBmp ) ) return DDERR_OUTOFMEMORY;
	if ( !m_hDIB ) m_hDIB = (HBITMAP)GetCurrentObject( m_hDC, OBJ_BITMAP );
	if ( m_bGPUNewer ) ReadBackFromGPU();
	for ( int y=0; y<Height(); y++ )
		for ( int x=0; x<Width(); x++ ) m_pDIBBits[y*Width()+x] = BGRAtoRGBA( PixelRGBA( x, y ) );		// same swap both ways
	*lphDC = m_hDC;
	return DD_OK;
}

STDMETHODIMP_(HRESULT) PtSurface::ReleaseDC( HDC hDC )
{
	GdiFlush();

	if ( this==g_PtGL.pBack ) {
		RECT r;
		UINT res = GetBoundsRect( g_PtGL.hOverlayDC, &r, DCB_RESET );
		SetBoundsRect( g_PtGL.hOverlayDC, NULL, DCB_DISABLE );
		if ( !(res & DCB_SET) ) return DD_OK;
		ClampRect( r, g_PtGL.OverlayW, g_PtGL.OverlayH );
		int w = r.right-r.left, h = r.bottom-r.top;
		if ( w<=0 || h<=0 ) return DD_OK;

		DWORD *rgba = (DWORD *)malloc( w*h*4 );
		if ( !rgba ) return DD_OK;
		for ( int y=0; y<h; y++ ) {
			DWORD *row = g_PtGL.pOverlayBits + (r.top+y)*g_PtGL.OverlayW + r.left;
			for ( int x=0; x<w; x++ ) {
				DWORD p = row[x], c = p & 0x00FFFFFF;
				if ( p>>24 ) rgba[y*w+x] = BGRAtoRGBA( p );		// our antialiased text
				else rgba[y*w+x] = ( c==PT_OVERLAY_KEY ) ? 0 : ( BGRAtoRGBA( c ) | 0xFF000000 );
				row[x] = PT_OVERLAY_KEY;				// clear for the next GetDC
			}
		}
		glBindTexture( GL_TEXTURE_2D, g_PtGL.OverlayTex );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
		glTexSubImage2D( GL_TEXTURE_2D, 0, r.left, r.top, w, h, GL_RGBA, GL_UNSIGNED_BYTE, rgba );
		free( rgba );

		PtSurface *pOld = g_PtGL.pRenderTarget;
		PtGfx_BindRenderTarget( this );
		PtGfx_Draw2DQuad( g_PtGL.OverlayTex, g_PtGL.OverlayW, g_PtGL.OverlayH, r, r, FALSE, FALSE, TRUE );
		g_PtStats.Overlays++;
		if ( g_PtTrace ) { fprintf( g_PtTrace, "TEXT overlay %d,%d-%d,%d\n", r.left, r.top, r.right, r.bottom ); PtGfx_TracePixels( "text" ); }
		PtGfx_BindRenderTarget( pOld );
		m_bGPUNewer = TRUE;
		return DD_OK;
	}

	if ( hDC==m_hDC && m_pDIBBits ) {
		DWORD bpp = m_Format.BytesPerPixel();
		for ( int y=0; y<Height(); y++ ) {
			BYTE *dst = m_pPixels + y*m_Pitch;
			for ( int x=0; x<Width(); x++, dst+=bpp )
				m_Format.Write( dst, m_Format.FromRGBA( BGRAtoRGBA( m_pDIBBits[y*Width()+x] ) | 0xFF000000 ) );
		}
		m_bDirty = TRUE;
		if ( m_FBO ) Unlock( NULL );
	}
	return DD_OK;
}

//////////////////////////////////////////////////////////////////////////////
// OpenGL setup, 2D quads and presenting
//////////////////////////////////////////////////////////////////////////////
GLuint PtGfx_CompileProgram( const char *szVS, const char *szFS, const char **attribs, int nAttribs )
{
	const char *src[2];
	GLuint sh[2];
	GLenum types[2] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	const char *body[2] = { szVS, szFS };
	for ( int i=0; i<2; i++ ) {
		src[0] = PTGL_GLSL_HEADER;
		src[1] = body[i];
		sh[i] = glCreateShader( types[i] );
		glShaderSource( sh[i], 2, src, NULL );
		glCompileShader( sh[i] );
		GLint ok = 0;
		glGetShaderiv( sh[i], GL_COMPILE_STATUS, &ok );
		if ( !ok ) {
			char log[4096] = "";
			glGetShaderInfoLog( sh[i], sizeof(log), NULL, log );
			PtGL_Log( "%s shader error:\n%s", i ? "fragment" : "vertex", log );
			return 0;
		}
	}
	GLuint prog = glCreateProgram();
	glAttachShader( prog, sh[0] );
	glAttachShader( prog, sh[1] );
	for ( int i=0; i<nAttribs; i++ ) glBindAttribLocation( prog, i, attribs[i] );
	glLinkProgram( prog );
	glDeleteShader( sh[0] );
	glDeleteShader( sh[1] );
	GLint ok = 0;
	glGetProgramiv( prog, GL_LINK_STATUS, &ok );
	if ( !ok ) {
		char log[4096] = "";
		glGetProgramInfoLog( prog, sizeof(log), NULL, log );
		PtGL_Log( "shader link error:\n%s", log );
		return 0;
	}
	return prog;
}

static const char *s_VS2D =
	"in vec2 aPos;\n"
	"in vec2 aTex;\n"
	"uniform vec2 uScreen;\n"
	"out vec2 vTex;\n"
	"void main() {\n"
	"	gl_Position = vec4( aPos.x/uScreen.x*2.0-1.0, aPos.y/uScreen.y*2.0-1.0, 0.0, 1.0 );\n"
	"	vTex = aTex;\n"
	"}\n";

static const char *s_FS2D =
	"in vec2 vTex;\n"
	"uniform sampler2D uTex;\n"
	"uniform int uKeyed;\n"
	"uniform int uOpaque;\n"
	"out vec4 oColor;\n"
	"void main() {\n"
	"	vec4 c = texture( uTex, vTex );\n"
	"	if ( uKeyed!=0 && c.a<0.5 ) discard;\n"
	"	if ( uOpaque!=0 ) c.a = 1.0;\n"
	"	oColor = c;\n"
	"}\n";

BOOL PtGfx_InitGL( HWND hWnd )
{
	if ( g_PtGL.bContext ) return TRUE;
	g_PtGL.hWnd = hWnd;
	if ( !PtGL_CreateContext( hWnd ) ) return FALSE;
	g_PtGL.bContext = TRUE;

	const char *attribs[] = { "aPos", "aTex" };
	g_PtGL.Prog2D = PtGfx_CompileProgram( s_VS2D, s_FS2D, attribs, 2 );
	if ( !g_PtGL.Prog2D ) return FALSE;
	g_PtGL.u2D_Screen = glGetUniformLocation( g_PtGL.Prog2D, "uScreen" );
	g_PtGL.u2D_Tex    = glGetUniformLocation( g_PtGL.Prog2D, "uTex" );
	g_PtGL.u2D_Keyed  = glGetUniformLocation( g_PtGL.Prog2D, "uKeyed" );
	g_PtGL.u2D_Opaque = glGetUniformLocation( g_PtGL.Prog2D, "uOpaque" );

	glGenVertexArrays( 1, &g_PtGL.VAO );
	glBindVertexArray( g_PtGL.VAO );
	glGenBuffers( 1, &g_PtGL.VBO );
	glBindBuffer( GL_ARRAY_BUFFER, g_PtGL.VBO );
	glBufferData( GL_ARRAY_BUFFER, 1<<20, NULL, GL_STREAM_DRAW );
	return TRUE;
}

void PtGfx_BindRenderTarget( PtSurface *pSurface )
{
	if ( !pSurface || !pSurface->m_FBO ) return;
	glBindFramebuffer( GL_FRAMEBUFFER, pSurface->m_FBO );
	glViewport( 0, 0, pSurface->Width(), pSurface->Height() );
}

// Draws src (in texture pixels, top-down) to dst (render target pixels, top-down).
void PtGfx_Draw2DQuad( GLuint tex, int texW, int texH, const RECT &src, const RECT &dst, BOOL bKeyed, BOOL bOpaque, BOOL bBlend )
{
	PtSurface *rt = g_PtGL.pRenderTarget;
	GLint vp[4];
	glGetIntegerv( GL_VIEWPORT, vp );

	float u0 = (float)src.left/texW, u1 = (float)src.right/texW;
	float v0 = (float)src.top/texH,  v1 = (float)src.bottom/texH;
	float x0 = (float)dst.left, x1 = (float)dst.right, y0 = (float)dst.top, y1 = (float)dst.bottom;
	float verts[] = { x0,y0,u0,v0,  x1,y0,u1,v0,  x0,y1,u0,v1,  x1,y1,u1,v1 };

	glUseProgram( g_PtGL.Prog2D );
	glUniform2f( g_PtGL.u2D_Screen, (float)vp[2], (float)vp[3] );
	glUniform1i( g_PtGL.u2D_Tex, 0 );
	glUniform1i( g_PtGL.u2D_Keyed, bKeyed );
	glUniform1i( g_PtGL.u2D_Opaque, bOpaque );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, tex );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	if ( bBlend ) {
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}
	else glDisable( GL_BLEND );
	glDisable( GL_SCISSOR_TEST );
	glColorMask( 1, 1, 1, 0 );

	glBindVertexArray( g_PtGL.VAO );
	glBindBuffer( GL_ARRAY_BUFFER, g_PtGL.VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW );
	for ( int i=0; i<16; i++ ) glDisableVertexAttribArray( i );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 16, (void *)0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 16, (void *)8 );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	PtGfx_InvalidateDeviceState();
	(void)rt;
}

//////////////////////////////////////////////////////////////////////////////
// Diagnostics: per-frame counters and occasional back-buffer snapshots
// (ptgfx_frame<N>.bmp next to game.exe) - shows what was really rendered.
//////////////////////////////////////////////////////////////////////////////
PtStats g_PtStats;
FILE *g_PtTrace = 0;

// Logs the render target colour at a few points after a draw (trace only).
void PtGfx_TracePixels( const char *szWhat )
{
	PtSurface *rt = g_PtGL.pRenderTarget;
	if ( !g_PtTrace || !rt ) return;
	int pts[4][2] = { { rt->Width()/2, rt->Height()/2 }, { 100, 100 }, { rt->Width()/2, rt->Height()*3/4 }, { rt->Width()-100, 100 } };
	fprintf( g_PtTrace, "   %s -> pixels", szWhat );
	for ( int i=0; i<4; i++ ) {
		BYTE p[4] = { 0 };
		glReadPixels( pts[i][0], pts[i][1], 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, p );
		fprintf( g_PtTrace, " (%d,%d)=%02X%02X%02X", pts[i][0], pts[i][1], p[0], p[1], p[2] );
	}
	fprintf( g_PtTrace, "\n" );
}

static void SaveBackBufferBMP( PtSurface *pBack, int frame )
{
	int w = pBack->Width(), h = pBack->Height();
	BYTE *rgba = (BYTE *)malloc( w*h*4 );
	if ( !rgba ) return;
	glBindFramebuffer( GL_READ_FRAMEBUFFER, pBack->m_FBO );
	glPixelStorei( GL_PACK_ALIGNMENT, 4 );
	glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, rgba );
	char name[64];
	sprintf( name, "ptgfx_frame%05d.bmp", frame );
	FILE *fp = fopen( name, "wb" );
	if ( fp ) {
		int rowSize = (w*3+3)&~3;
		BITMAPFILEHEADER bf = { 0x4D42, (DWORD)(54 + rowSize*h), 0, 0, 54 };
		BITMAPINFOHEADER bi; ZeroMemory( &bi, sizeof(bi) );
		bi.biSize = 40; bi.biWidth = w; bi.biHeight = -h; bi.biPlanes = 1; bi.biBitCount = 24;
		fwrite( &bf, sizeof(bf), 1, fp ); fwrite( &bi, sizeof(bi), 1, fp );
		BYTE *row = (BYTE *)calloc( rowSize, 1 );
		for ( int y=0; y<h; y++ ) {			// render targets are top-down
			for ( int x=0; x<w; x++ ) { BYTE *p = rgba + (y*w+x)*4; row[x*3] = p[2]; row[x*3+1] = p[1]; row[x*3+2] = p[0]; }
			fwrite( row, rowSize, 1, fp );
		}
		free( row );
		fclose( fp );
	}
	free( rgba );
}

static void LogFrameStats( int frame )
{
	GLenum err = glGetError();
	PtGL_Log( "frame %d: draws %d (verts %d), blits %d, colorfills %d, text overlays %d, uploads %d, rt switches %d, GL error 0x%X, back %dx%d, rt %s",
		frame, g_PtStats.Draws, g_PtStats.Verts, g_PtStats.Blits, g_PtStats.Fills, g_PtStats.Overlays,
		g_PtStats.Uploads, g_PtStats.RTSwitches, err,
		g_PtGL.pBack ? g_PtGL.pBack->Width() : 0, g_PtGL.pBack ? g_PtGL.pBack->Height() : 0,
		g_PtGL.pRenderTarget==g_PtGL.pBack ? "back" : "other" );
}

void PtGfx_Present( const RECT *lpSrc )
{
	PtSurface *pBack = g_PtGL.pBack;
	if ( !pBack || !g_PtGL.bContext ) return;

	static int frame = 0;
	frame++;
	if ( g_PtTrace ) { fclose( g_PtTrace ); g_PtTrace = 0; }
#ifndef PT_WEB			// browser: no snapshots (each one is a GPU read-back and a file in memory)
	if ( frame==299 ) g_PtTrace = fopen( "ptgfx_trace.txt", "wt" );		// traces the next frame (300)
#endif
	if ( frame==1 || frame==60 || frame==300 || frame%1800==0 ) {
		LogFrameStats( frame );
#ifndef PT_WEB
		if ( frame<=18000 ) SaveBackBufferBMP( pBack, frame );
#endif
	}
	ZeroMemory( &g_PtStats, sizeof(g_PtStats) );
	RECT client;
	GetClientRect( g_PtGL.hWnd, &client );
	RECT src = { 0, 0, pBack->Width(), pBack->Height() };
	if ( lpSrc ) src = *lpSrc;

	glBindFramebuffer( GL_READ_FRAMEBUFFER, pBack->m_FBO );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	glViewport( 0, 0, client.right, client.bottom );
	glDisable( GL_SCISSOR_TEST );
	glColorMask( 1, 1, 1, 1 );
	glClearColor( 0, 0, 0, 1 );
	glClear( GL_COLOR_BUFFER_BIT );
	// render targets are top-down; the window is bottom-up: flip while scaling
	glBlitFramebuffer( src.left, src.top, src.right, src.bottom,
		0, client.bottom, client.right, 0, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	PtGL_SwapBuffers();
	PtGfx_BindRenderTarget( g_PtGL.pRenderTarget );
	PtGfx_InvalidateDeviceState();
}

//////////////////////////////////////////////////////////////////////////////
// Clipper (window clipping is implicit with OpenGL)
//////////////////////////////////////////////////////////////////////////////
class PtClipper : public PtStub_IDirectDrawClipper {
public:
	STDMETHODIMP_(HRESULT) SetHWnd( DWORD, HWND ) override { return DD_OK; }
};

//////////////////////////////////////////////////////////////////////////////
// IDirectDraw4
//////////////////////////////////////////////////////////////////////////////
IDirect3D3 *PtGfx_CreateD3D();			// ptgfx_device.cpp

class PtDirectDraw : public PtStub_IDirectDraw4 {
public:
	STDMETHODIMP_(HRESULT) QueryInterface( REFIID riid, LPVOID FAR * ppvObj ) override
	{
		if ( !ppvObj ) return E_POINTER;
		if ( riid==IID_IDirectDraw4 || riid==IID_IUnknown ) { *ppvObj = this; AddRef(); return DD_OK; }
		if ( riid==IID_IDirect3D3 ) { *ppvObj = PtGfx_CreateD3D(); return DD_OK; }
		*ppvObj = 0;
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(HRESULT) SetCooperativeLevel( HWND hWnd, DWORD ) override
	{
		if ( hWnd && !PtGfx_InitGL( hWnd ) ) return DDERR_GENERIC;
		return DD_OK;
	}

	STDMETHODIMP_(HRESULT) SetDisplayMode( DWORD w, DWORD h, DWORD bpp, DWORD, DWORD ) override
	{
		g_PtDisplayW = w; g_PtDisplayH = h;
		if ( bpp==32 ) g_PtDisplayFormat.SetRGB( 32, 0xFF0000, 0xFF00, 0xFF, 0 );
		else g_PtDisplayFormat.SetRGB( 16, 0xF800, 0x07E0, 0x001F, 0 );
		return DD_OK;
	}
	STDMETHODIMP_(HRESULT) RestoreDisplayMode() override { return DD_OK; }
	STDMETHODIMP_(HRESULT) FlipToGDISurface() override { return DD_OK; }
	STDMETHODIMP_(HRESULT) WaitForVerticalBlank( DWORD, HANDLE ) override { return DD_OK; }

	STDMETHODIMP_(HRESULT) GetDisplayMode( LPDDSURFACEDESC2 lpDesc ) override
	{
		if ( !lpDesc ) return DDERR_INVALIDPARAMS;
		ZeroMemory( lpDesc, sizeof(DDSURFACEDESC2) );
		lpDesc->dwSize = sizeof(DDSURFACEDESC2);
		lpDesc->dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
		lpDesc->dwWidth = g_PtDisplayW;
		lpDesc->dwHeight = g_PtDisplayH;
		g_PtDisplayFormat.ToDDPF( &lpDesc->ddpfPixelFormat );
		return DD_OK;
	}

	STDMETHODIMP_(HRESULT) GetCaps( LPDDCAPS lpHW, LPDDCAPS lpHEL ) override
	{
		LPDDCAPS caps[2] = { lpHW, lpHEL };
		for ( int i=0; i<2; i++ ) {
			if ( !caps[i] ) continue;
			DWORD size = caps[i]->dwSize ? caps[i]->dwSize : sizeof(DDCAPS);
			ZeroMemory( caps[i], size );
			caps[i]->dwSize = size;
			caps[i]->dwCaps = DDCAPS_3D | DDCAPS_BLT | DDCAPS_BLTSTRETCH | DDCAPS_COLORKEY | DDCAPS_BLTCOLORFILL;
			caps[i]->dwCKeyCaps = DDCKEYCAPS_SRCBLT;
			caps[i]->dwVidMemTotal = caps[i]->dwVidMemFree = 512*1024*1024;
			caps[i]->ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_TEXTURE | DDSCAPS_ZBUFFER | DDSCAPS_MIPMAP | DDSCAPS_VIDEOMEMORY;
		}
		return DD_OK;
	}

	STDMETHODIMP_(HRESULT) GetAvailableVidMem( LPDDSCAPS2, LPDWORD lpTotal, LPDWORD lpFree ) override
	{
		if ( lpTotal ) *lpTotal = 512*1024*1024;
		if ( lpFree ) *lpFree = 512*1024*1024;
		return DD_OK;
	}

	STDMETHODIMP_(HRESULT) EnumDisplayModes( DWORD, LPDDSURFACEDESC2, LPVOID lpContext, LPDDENUMMODESCALLBACK2 lpCallback ) override
	{
		static const DWORD modes[][2] = { {800,600}, {1024,768}, {1280,1024}, {1600,1200} };
		for ( int bpp=16; bpp<=32; bpp+=16 )
			for ( int i=0; i<4; i++ ) {
				DDSURFACEDESC2 d;
				ZeroMemory( &d, sizeof(d) );
				d.dwSize = sizeof(d);
				d.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
				d.dwWidth = modes[i][0]; d.dwHeight = modes[i][1];
				PtFormat f;
				if ( bpp==32 ) f.SetRGB( 32, 0xFF0000, 0xFF00, 0xFF, 0 ); else f.SetRGB( 16, 0xF800, 0x07E0, 0x001F, 0 );
				f.ToDDPF( &d.ddpfPixelFormat );
				if ( lpCallback( &d, lpContext )==DDENUMRET_CANCEL ) return DD_OK;
			}
		return DD_OK;
	}

	STDMETHODIMP_(HRESULT) CreateClipper( DWORD, LPDIRECTDRAWCLIPPER FAR *lplpClipper, IUnknown FAR * ) override
	{
		if ( !lplpClipper ) return DDERR_INVALIDPARAMS;
		*lplpClipper = new PtClipper;
		return DD_OK;
	}

	STDMETHODIMP_(HRESULT) CreateSurface( LPDDSURFACEDESC2 lpDesc, LPDIRECTDRAWSURFACE4 FAR *lplpSurface, IUnknown FAR * ) override
	{
		if ( !lpDesc || !lplpSurface ) return DDERR_INVALIDPARAMS;
		*lplpSurface = 0;
		DWORD caps = lpDesc->ddsCaps.dwCaps;

		PtSurfaceKind kind = PTSURF_PLAIN;
		if ( caps & DDSCAPS_PRIMARYSURFACE ) kind = PTSURF_PRIMARY;
		else if ( caps & DDSCAPS_ZBUFFER ) kind = PTSURF_ZBUFFER;
		else if ( (caps & DDSCAPS_3DDEVICE) && !(caps & DDSCAPS_TEXTURE) ) kind = PTSURF_BACK;

		PtSurface *pSurf = new PtSurface;
		if ( !pSurf->Create( lpDesc, kind ) ) { pSurf->Release(); return DDERR_OUTOFMEMORY; }

		// full-screen flip chain: primary + back buffer
		if ( kind==PTSURF_PRIMARY && (caps & DDSCAPS_FLIP) && (lpDesc->dwFlags & DDSD_BACKBUFFERCOUNT) && lpDesc->dwBackBufferCount ) {
			DDSURFACEDESC2 bd = *lpDesc;
			bd.dwFlags &= ~DDSD_BACKBUFFERCOUNT;
			bd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER | DDSCAPS_3DDEVICE;
			PtSurface *pBack = new PtSurface;
			if ( !pBack->Create( &bd, PTSURF_BACK ) ) { pBack->Release(); pSurf->Release(); return DDERR_OUTOFMEMORY; }
			pSurf->m_pAttached = pBack;
		}

		// mip-map chain
		if ( (caps & DDSCAPS_MIPMAP) && (lpDesc->dwFlags & DDSD_MIPMAPCOUNT) && lpDesc->dwMipMapCount>1 ) {
			PtSurface *pPrev = pSurf;
			for ( DWORD level=1; level<lpDesc->dwMipMapCount; level++ ) {
				DDSURFACEDESC2 md = pSurf->m_Desc;
				md.dwWidth = max( 1u, pSurf->m_Desc.dwWidth>>level );
				md.dwHeight = max( 1u, pSurf->m_Desc.dwHeight>>level );
				md.dwMipMapCount = lpDesc->dwMipMapCount - level;
				PtSurface *pLevel = new PtSurface;
				if ( !pLevel->Create( &md, PTSURF_PLAIN ) ) { pLevel->Release(); break; }
				pPrev->m_pAttached = pLevel;
				pPrev = pLevel;
			}
		}

		if ( kind==PTSURF_PRIMARY ) g_PtGL.pPrimary = pSurf;
		*lplpSurface = pSurf;
		return DD_OK;
	}
};

HRESULT PtGfx_DirectDrawCreate( LPDIRECTDRAW4 *lplpDD )
{
	if ( !lplpDD ) return DDERR_INVALIDPARAMS;
	if ( !g_PtDisplayFormat.bpp ) g_PtDisplayFormat.SetRGB( 16, 0xF800, 0x07E0, 0x001F, 0 );
	*lplpDD = new PtDirectDraw;
	PtText_Install();
	return DD_OK;
}

HFONT WINAPI PtGfx_CreateFontA( int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight,
	DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision,
	DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName )
{
	return CreateFontA( cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut,
		iCharSet, iOutPrecision, iClipPrecision, NONANTIALIASED_QUALITY, iPitchAndFamily, pszFaceName );
}
