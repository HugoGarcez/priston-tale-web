// ptwin_gdi.cpp - gdi32 for the web build: device contexts, 32-bit DIB bitmaps,
// fonts/brushes/pens as objects, BitBlt, text colours/modes and bounds.
// Text itself is drawn by ptgfx\ptgfx_text.cpp (stb_truetype) on top of these.
#include "ptwin_internal.h"
#include <algorithm>

#define PTGDI_MAGIC 0x70474449		// 'pGDI'

enum { PTGDI_BITMAP = 1, PTGDI_FONT, PTGDI_BRUSH, PTGDI_PEN, PTGDI_DC };

struct PtGdi { unsigned magic; int type; bool stock; };

struct PtBitmap : PtGdi {
	int w, h;
	DWORD *bits;			// 32-bit BGRA, top-down
	bool owned;
};

struct PtFontObj : PtGdi { LOGFONTA lf; };
struct PtBrush : PtGdi { COLORREF color; bool null; };
struct PtPen : PtGdi { COLORREF color; int width; };

struct PtDC : PtGdi {
	PtBitmap	*bmp;
	PtFontObj	*font;
	PtBrush		*brush;
	PtPen		*pen;
	COLORREF	textColor, bkColor;
	int			bkMode;
	UINT		textAlign;
	bool		boundsOn;
	RECT		bounds;
	HWND		hwnd;		// window DC (GetDC)
};

template<class T> static T *G( HGDIOBJ h, int type )
{
	PtGdi *g = (PtGdi *)h;
	if ( !g || g->magic!=PTGDI_MAGIC || ( type && g->type!=type ) ) return NULL;
	return (T *)g;
}

static PtBitmap *NewBitmap( int w, int h )
{
	PtBitmap *b = new PtBitmap;
	b->magic = PTGDI_MAGIC; b->type = PTGDI_BITMAP; b->stock = false;
	b->w = std::max( 1, w ); b->h = std::max( 1, h );
	b->bits = (DWORD *)calloc( b->w * b->h, 4 );
	b->owned = true;
	return b;
}

static PtFontObj *DefaultFont()
{
	static PtFontObj *f = NULL;
	if ( !f ) {
		f = new PtFontObj;
		f->magic = PTGDI_MAGIC; f->type = PTGDI_FONT; f->stock = true;
		ZeroMemory( &f->lf, sizeof(f->lf) );
		f->lf.lfHeight = 12; f->lf.lfWeight = FW_NORMAL;
		strcpy( f->lf.lfFaceName, "Tahoma" );
	}
	return f;
}
static PtBitmap *StockBitmap() { static PtBitmap *b = NULL; if ( !b ) { b = NewBitmap( 1, 1 ); b->stock = true; } return b; }
static PtBrush *StockBrush( COLORREF c, bool null )
{
	PtBrush *b = new PtBrush;
	b->magic = PTGDI_MAGIC; b->type = PTGDI_BRUSH; b->stock = true; b->color = c; b->null = null;
	return b;
}

//////////////////////////////////////////////////////////////////////////////
// DCs
//////////////////////////////////////////////////////////////////////////////
static PtDC *NewDC( HWND hwnd )
{
	PtDC *d = new PtDC;
	d->magic = PTGDI_MAGIC; d->type = PTGDI_DC; d->stock = false;
	d->bmp = StockBitmap();
	d->font = DefaultFont();
	d->brush = NULL; d->pen = NULL;
	d->textColor = RGB( 0, 0, 0 ); d->bkColor = RGB( 255, 255, 255 );
	d->bkMode = OPAQUE; d->textAlign = TA_LEFT | TA_TOP;
	d->boundsOn = false; SetRectEmpty( &d->bounds );
	d->hwnd = hwnd;
	return d;
}

HDC WINAPI CreateCompatibleDC( HDC ) { return (HDC)NewDC( NULL ); }
HDC WINAPI CreateDCA( LPCSTR, LPCSTR, LPCSTR, const DEVMODEA * ) { return (HDC)NewDC( NULL ); }
BOOL WINAPI DeleteDC( HDC h ) { PtDC *d = G<PtDC>( h, PTGDI_DC ); if ( !d ) return FALSE; d->magic = 0; delete d; return TRUE; }
// A window DC: the canvas is drawn by WebGL, so this only serves measuring text.
HDC WINAPI GetDC( HWND h ) { return (HDC)NewDC( h ); }
int WINAPI ReleaseDC( HWND, HDC h ) { return DeleteDC( h ); }
HDC WINAPI GetWindowDC( HWND h ) { return GetDC( h ); }

HGDIOBJ WINAPI SelectObject( HDC h, HGDIOBJ obj )
{
	PtDC *d = G<PtDC>( h, PTGDI_DC );
	PtGdi *g = G<PtGdi>( obj, 0 );
	if ( !d || !g ) return NULL;
	HGDIOBJ old = NULL;
	switch ( g->type ) {
	case PTGDI_BITMAP:	old = (HGDIOBJ)d->bmp; d->bmp = (PtBitmap *)g; break;
	case PTGDI_FONT:	old = (HGDIOBJ)d->font; d->font = (PtFontObj *)g; break;
	case PTGDI_BRUSH:	old = (HGDIOBJ)d->brush; d->brush = (PtBrush *)g; break;
	case PTGDI_PEN:		old = (HGDIOBJ)d->pen; d->pen = (PtPen *)g; break;
	}
	return old ? old : obj;
}

HGDIOBJ WINAPI GetCurrentObject( HDC h, UINT type )
{
	PtDC *d = G<PtDC>( h, PTGDI_DC );
	if ( !d ) return NULL;
	switch ( type ) {
	case OBJ_BITMAP:	return (HGDIOBJ)d->bmp;
	case OBJ_FONT:		return (HGDIOBJ)d->font;
	case OBJ_BRUSH:		return (HGDIOBJ)d->brush;
	case OBJ_PEN:		return (HGDIOBJ)d->pen;
	}
	return NULL;
}

BOOL WINAPI DeleteObject( HGDIOBJ obj )
{
	PtGdi *g = G<PtGdi>( obj, 0 );
	if ( !g || g->stock || g->type==PTGDI_DC ) return FALSE;
	if ( g->type==PTGDI_BITMAP && ((PtBitmap *)g)->owned ) free( ((PtBitmap *)g)->bits );
	g->magic = 0;
	switch ( g->type ) {
	case PTGDI_BITMAP:	delete (PtBitmap *)g; break;
	case PTGDI_FONT:	delete (PtFontObj *)g; break;
	case PTGDI_BRUSH:	delete (PtBrush *)g; break;
	case PTGDI_PEN:		delete (PtPen *)g; break;
	}
	return TRUE;
}

HGDIOBJ WINAPI GetStockObject( int idx )
{
	static PtBrush *white = StockBrush( RGB( 255, 255, 255 ), false ), *black = StockBrush( 0, false ), *null = StockBrush( 0, true );
	switch ( idx ) {
	case WHITE_BRUSH:	return (HGDIOBJ)white;
	case BLACK_BRUSH:	return (HGDIOBJ)black;
	case NULL_BRUSH:	return (HGDIOBJ)null;
	case SYSTEM_FONT: case DEFAULT_GUI_FONT: case ANSI_VAR_FONT: case ANSI_FIXED_FONT: case OEM_FIXED_FONT: case DEVICE_DEFAULT_FONT:
		return (HGDIOBJ)DefaultFont();
	}
	return (HGDIOBJ)null;
}

int WINAPI GetObjectA( HGDIOBJ obj, int size, LPVOID out )
{
	PtGdi *g = G<PtGdi>( obj, 0 );
	if ( !g ) return 0;
	if ( g->type==PTGDI_FONT ) {
		if ( !out ) return sizeof(LOGFONTA);
		int n = std::min( size, (int)sizeof(LOGFONTA) );
		memcpy( out, &((PtFontObj *)g)->lf, n );
		return n;
	}
	if ( g->type==PTGDI_BITMAP ) {
		PtBitmap *b = (PtBitmap *)g;
		DIBSECTION ds;
		ZeroMemory( &ds, sizeof(ds) );
		ds.dsBm.bmWidth = b->w; ds.dsBm.bmHeight = b->h;
		ds.dsBm.bmWidthBytes = b->w*4; ds.dsBm.bmPlanes = 1; ds.dsBm.bmBitsPixel = 32;
		ds.dsBm.bmBits = b->bits;
		ds.dsBmih.biSize = sizeof(BITMAPINFOHEADER);
		ds.dsBmih.biWidth = b->w; ds.dsBmih.biHeight = b->h;		// reported positive, as Windows does
		ds.dsBmih.biPlanes = 1; ds.dsBmih.biBitCount = 32; ds.dsBmih.biCompression = BI_RGB;
		if ( !out ) return sizeof(DIBSECTION);
		int n = size>=(int)sizeof(DIBSECTION) ? (int)sizeof(DIBSECTION) : std::min( size, (int)sizeof(BITMAP) );
		memcpy( out, &ds, n );
		return n;
	}
	if ( g->type==PTGDI_BRUSH ) {
		LOGBRUSH lb = { BS_SOLID, ((PtBrush *)g)->color, 0 };
		if ( out ) memcpy( out, &lb, std::min( size, (int)sizeof(lb) ) );
		return sizeof(lb);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Bitmaps
//////////////////////////////////////////////////////////////////////////////
HBITMAP WINAPI CreateDIBSection( HDC, const BITMAPINFO *bmi, UINT, VOID **ppBits, HANDLE, DWORD )
{
	const BITMAPINFOHEADER &h = bmi->bmiHeader;
	if ( h.biBitCount!=32 ) PtWin_Log( "CreateDIBSection: %d bpp asked, giving 32", h.biBitCount );
	PtBitmap *b = NewBitmap( h.biWidth, h.biHeight<0 ? -h.biHeight : h.biHeight );
	if ( ppBits ) *ppBits = b->bits;
	return (HBITMAP)b;
}
HBITMAP WINAPI CreateCompatibleBitmap( HDC, int w, int h ) { return (HBITMAP)NewBitmap( w, h ); }
HBITMAP WINAPI CreateBitmap( int w, int h, UINT, UINT, const void *bits ) { PtBitmap *b = NewBitmap( w, h ); if ( bits ) memcpy( b->bits, bits, b->w*b->h*4 ); return (HBITMAP)b; }
HBITMAP WINAPI LoadBitmapA( HINSTANCE, LPCSTR ) { return (HBITMAP)NewBitmap( 1, 1 ); }

int WINAPI GetDIBits( HDC, HBITMAP hbm, UINT start, UINT lines, LPVOID out, LPBITMAPINFO bmi, UINT )
{
	PtBitmap *b = G<PtBitmap>( hbm, PTGDI_BITMAP );
	if ( !b ) return 0;
	BITMAPINFOHEADER &h = bmi->bmiHeader;
	if ( !out ) {			// query the format
		h.biWidth = b->w; h.biHeight = b->h; h.biPlanes = 1; h.biBitCount = 32; h.biCompression = BI_RGB;
		h.biSizeImage = b->w*b->h*4;
		return b->h;
	}
	bool topDown = h.biHeight<0;
	int n = 0;
	for ( UINT i=0; i<lines && (int)( start+i )<b->h; i++, n++ ) {
		int row = topDown ? start+i : b->h-1-( start+i );			// bottom-up by default
		memcpy( (BYTE *)out + i*b->w*4, b->bits + row*b->w, b->w*4 );
	}
	return n;
}

int WINAPI SetDIBitsToDevice( HDC h, int x, int y, DWORD cx, DWORD cy, int sx, int sy, UINT, UINT, const VOID *bits, const BITMAPINFO *bmi, UINT )
{
	PtDC *d = G<PtDC>( h, PTGDI_DC );
	if ( !d || bmi->bmiHeader.biBitCount!=32 ) return 0;
	int sw = bmi->bmiHeader.biWidth, shgt = abs( bmi->bmiHeader.biHeight );
	bool topDown = bmi->bmiHeader.biHeight<0;
	for ( DWORD j=0; j<cy; j++ ) {
		int dy = y+j, srow = sy+j;
		if ( dy<0 || dy>=d->bmp->h || srow>=shgt ) continue;
		const DWORD *src = (const DWORD *)bits + ( topDown ? srow : shgt-1-srow )*sw;
		for ( DWORD i=0; i<cx; i++ ) {
			int dx = x+i;
			if ( dx>=0 && dx<d->bmp->w && sx+(int)i<sw ) d->bmp->bits[dy*d->bmp->w+dx] = src[sx+i];
		}
	}
	return (int)cy;
}

static void AddBounds( PtDC *d, int l, int t, int r, int b )
{
	if ( !d->boundsOn ) return;
	RECT rc = { l, t, r, b };
	UnionRect( &d->bounds, &d->bounds, &rc );
}

BOOL WINAPI BitBlt( HDC hDst, int x, int y, int cx, int cy, HDC hSrc, int sx, int sy, DWORD rop )
{
	PtDC *d = G<PtDC>( hDst, PTGDI_DC ), *s = G<PtDC>( hSrc, PTGDI_DC );
	if ( !d ) return FALSE;
	PtBitmap *db = d->bmp;
	for ( int j=0; j<cy; j++ ) {
		int dy = y+j;
		if ( dy<0 || dy>=db->h ) continue;
		for ( int i=0; i<cx; i++ ) {
			int dx = x+i;
			if ( dx<0 || dx>=db->w ) continue;
			DWORD c;
			if ( rop==BLACKNESS ) c = 0;
			else if ( rop==WHITENESS ) c = 0xFFFFFF;
			else {
				if ( !s ) continue;
				int ssx = sx+i, ssy = sy+j;
				if ( ssx<0 || ssy<0 || ssx>=s->bmp->w || ssy>=s->bmp->h ) continue;
				c = s->bmp->bits[ssy*s->bmp->w+ssx];
				if ( rop==SRCAND ) c &= db->bits[dy*db->w+dx];
				else if ( rop==SRCPAINT ) c |= db->bits[dy*db->w+dx];
				else if ( rop==SRCINVERT ) c ^= db->bits[dy*db->w+dx];
			}
			db->bits[dy*db->w+dx] = c;
		}
	}
	AddBounds( d, x, y, x+cx, y+cy );
	return TRUE;
}

BOOL WINAPI StretchBlt( HDC hDst, int x, int y, int cx, int cy, HDC hSrc, int sx, int sy, int scx, int scy, DWORD rop )
{
	if ( cx==scx && cy==scy ) return BitBlt( hDst, x, y, cx, cy, hSrc, sx, sy, rop );
	PtDC *d = G<PtDC>( hDst, PTGDI_DC ), *s = G<PtDC>( hSrc, PTGDI_DC );
	if ( !d || !s || !cx || !cy ) return FALSE;
	for ( int j=0; j<cy; j++ ) for ( int i=0; i<cx; i++ ) {
		int dx = x+i, dy = y+j, ssx = sx + i*scx/cx, ssy = sy + j*scy/cy;
		if ( dx<0 || dy<0 || dx>=d->bmp->w || dy>=d->bmp->h || ssx<0 || ssy<0 || ssx>=s->bmp->w || ssy>=s->bmp->h ) continue;
		d->bmp->bits[dy*d->bmp->w+dx] = s->bmp->bits[ssy*s->bmp->w+ssx];
	}
	AddBounds( d, x, y, x+cx, y+cy );
	return TRUE;
}

int WINAPI FillRect( HDC h, const RECT *r, HBRUSH hbr )
{
	PtDC *d = G<PtDC>( h, PTGDI_DC );
	PtBrush *b = G<PtBrush>( hbr, PTGDI_BRUSH );
	if ( !d || !b || b->null ) return 0;
	DWORD c = ( GetRValue( b->color )<<16 ) | ( GetGValue( b->color )<<8 ) | GetBValue( b->color );
	for ( int y = std::max( 0, (int)r->top ); y<std::min( d->bmp->h, (int)r->bottom ); y++ )
		for ( int x = std::max( 0, (int)r->left ); x<std::min( d->bmp->w, (int)r->right ); x++ ) d->bmp->bits[y*d->bmp->w+x] = c;
	AddBounds( d, r->left, r->top, r->right, r->bottom );
	return 1;
}

COLORREF WINAPI SetPixel( HDC h, int x, int y, COLORREF c )
{
	PtDC *d = G<PtDC>( h, PTGDI_DC );
	if ( !d || x<0 || y<0 || x>=d->bmp->w || y>=d->bmp->h ) return (COLORREF)-1;
	d->bmp->bits[y*d->bmp->w+x] = ( GetRValue( c )<<16 ) | ( GetGValue( c )<<8 ) | GetBValue( c );
	AddBounds( d, x, y, x+1, y+1 );
	return c;
}

//////////////////////////////////////////////////////////////////////////////
// Fonts, brushes, pens
//////////////////////////////////////////////////////////////////////////////
HFONT WINAPI CreateFontIndirectA( const LOGFONTA *lf )
{
	PtFontObj *f = new PtFontObj;
	f->magic = PTGDI_MAGIC; f->type = PTGDI_FONT; f->stock = false;
	f->lf = *lf;
	return (HFONT)f;
}
HFONT WINAPI CreateFontA( int h, int w, int esc, int orient, int weight, DWORD italic, DWORD underline, DWORD strike,
	DWORD charset, DWORD outPrec, DWORD clipPrec, DWORD quality, DWORD pitch, LPCSTR face )
{
	LOGFONTA lf;
	ZeroMemory( &lf, sizeof(lf) );
	lf.lfHeight = h; lf.lfWidth = w; lf.lfEscapement = esc; lf.lfOrientation = orient; lf.lfWeight = weight;
	lf.lfItalic = (BYTE)italic; lf.lfUnderline = (BYTE)underline; lf.lfStrikeOut = (BYTE)strike;
	lf.lfCharSet = (BYTE)charset; lf.lfOutPrecision = (BYTE)outPrec; lf.lfClipPrecision = (BYTE)clipPrec;
	lf.lfQuality = (BYTE)quality; lf.lfPitchAndFamily = (BYTE)pitch;
	lstrcpynA( lf.lfFaceName, face ? face : "", LF_FACESIZE );
	return CreateFontIndirectA( &lf );
}
int WINAPI AddFontResourceA( LPCSTR ) { return 1; }
BOOL WINAPI RemoveFontResourceA( LPCSTR ) { return TRUE; }

HBRUSH WINAPI CreateSolidBrush( COLORREF c ) { PtBrush *b = StockBrush( c, false ); b->stock = false; return (HBRUSH)b; }
HPEN WINAPI CreatePen( int, int width, COLORREF c )
{
	PtPen *p = new PtPen;
	p->magic = PTGDI_MAGIC; p->type = PTGDI_PEN; p->stock = false; p->color = c; p->width = width;
	return (HPEN)p;
}

//////////////////////////////////////////////////////////////////////////////
// DC state
//////////////////////////////////////////////////////////////////////////////
COLORREF WINAPI SetTextColor( HDC h, COLORREF c ) { PtDC *d = G<PtDC>( h, PTGDI_DC ); if ( !d ) return CLR_INVALID; COLORREF o = d->textColor; d->textColor = c; return o; }
COLORREF WINAPI GetTextColor( HDC h ) { PtDC *d = G<PtDC>( h, PTGDI_DC ); return d ? d->textColor : CLR_INVALID; }
COLORREF WINAPI SetBkColor( HDC h, COLORREF c ) { PtDC *d = G<PtDC>( h, PTGDI_DC ); if ( !d ) return CLR_INVALID; COLORREF o = d->bkColor; d->bkColor = c; return o; }
COLORREF WINAPI GetBkColor( HDC h ) { PtDC *d = G<PtDC>( h, PTGDI_DC ); return d ? d->bkColor : CLR_INVALID; }
int WINAPI SetBkMode( HDC h, int m ) { PtDC *d = G<PtDC>( h, PTGDI_DC ); if ( !d ) return 0; int o = d->bkMode; d->bkMode = m; return o; }
int WINAPI GetBkMode( HDC h ) { PtDC *d = G<PtDC>( h, PTGDI_DC ); return d ? d->bkMode : 0; }
UINT WINAPI SetTextAlign( HDC h, UINT a ) { PtDC *d = G<PtDC>( h, PTGDI_DC ); if ( !d ) return GDI_ERROR; UINT o = d->textAlign; d->textAlign = a; return o; }
UINT WINAPI GetTextAlign( HDC h ) { PtDC *d = G<PtDC>( h, PTGDI_DC ); return d ? d->textAlign : GDI_ERROR; }

int WINAPI GetClipBox( HDC h, LPRECT r )
{
	PtDC *d = G<PtDC>( h, PTGDI_DC );
	if ( !d ) return ERROR;
	SetRect( r, 0, 0, d->bmp->w, d->bmp->h );
	return SIMPLEREGION;
}

UINT WINAPI SetBoundsRect( HDC h, const RECT *r, UINT flags )
{
	PtDC *d = G<PtDC>( h, PTGDI_DC );
	if ( !d ) return 0;
	UINT old = ( d->boundsOn ? DCB_ENABLE : DCB_DISABLE ) | ( IsRectEmpty( &d->bounds ) ? DCB_RESET : DCB_SET );
	if ( flags & DCB_RESET ) SetRectEmpty( &d->bounds );
	if ( ( flags & DCB_ACCUMULATE ) && r ) UnionRect( &d->bounds, &d->bounds, r );
	if ( flags & DCB_ENABLE ) d->boundsOn = true;
	if ( flags & DCB_DISABLE ) d->boundsOn = false;
	return old;
}

UINT WINAPI GetBoundsRect( HDC h, LPRECT r, UINT flags )
{
	PtDC *d = G<PtDC>( h, PTGDI_DC );
	if ( !d ) return 0;
	*r = d->bounds;
	UINT res = IsRectEmpty( &d->bounds ) ? DCB_RESET : DCB_SET;
	if ( flags & DCB_RESET ) SetRectEmpty( &d->bounds );
	return res;
}

BOOL WINAPI GdiFlush() { return TRUE; }
int WINAPI GetDeviceCaps( HDC, int idx ) { return idx==BITSPIXEL ? 32 : idx==LOGPIXELSX || idx==LOGPIXELSY ? 96 : idx==PLANES ? 1 : 0; }
