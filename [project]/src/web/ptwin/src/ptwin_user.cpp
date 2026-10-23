// ptwin_user.cpp - user32 for the web build: windows, window procedures, the
// message queue, timers, focus and keyboard state, the EDIT control the game
// types into, message boxes and the rectangle helpers.
//
// There is one real surface (the canvas, owned by ptplatform/SDL); "windows" here
// are objects with a window procedure, so the game's WndProc/subclassing works
// as on Windows. Input from SDL arrives through ptplatform as WM_ messages.
#include "ptwin_internal.h"
#include <emscripten.h>
#include <emscripten/threading.h>
#include <deque>
#include <algorithm>
#include <strings.h>
#include <SDL.h>

#define PTWND_MAGIC 0x7057444E		// 'pWND'

struct PtWnd {
	unsigned	magic;
	std::string	className;
	WNDPROC		proc;
	HWND		parent;
	DWORD		style, exStyle;
	LONG_PTR	id, userData;
	RECT		rect;				// in parent client coordinates (top-level: screen)
	std::string	text;
	BOOL		visible, enabled;
	HINSTANCE	hInst;
	// EDIT control state
	int			selStart, selEnd;
	int			limit;
};

static std::map<std::string, WNDCLASSA>	s_Classes;		// lower-case name -> class
static std::vector<PtWnd *>				s_Windows;
static HWND								s_hFocus = NULL;
static HWND								s_hMain = NULL;
static pthread_mutex_t					s_WndLock = PTHREAD_MUTEX_INITIALIZER;

static std::string Lower( const char *s ) { std::string r = s ? s : ""; for ( auto &c : r ) c = (char)tolower( (unsigned char)c ); return r; }

static PtWnd *W( HWND h )
{
	PtWnd *w = (PtWnd *)h;
	if ( !w ) return NULL;
	pthread_mutex_lock( &s_WndLock );
	bool ok = std::find( s_Windows.begin(), s_Windows.end(), w )!=s_Windows.end();
	pthread_mutex_unlock( &s_WndLock );
	return ok ? w : NULL;
}

HWND PtWin_MainWindow() { return s_hMain; }

//////////////////////////////////////////////////////////////////////////////
// Rectangles
//////////////////////////////////////////////////////////////////////////////
BOOL WINAPI SetRect( LPRECT r, int l, int t, int ri, int b ) { if ( !r ) return FALSE; r->left = l; r->top = t; r->right = ri; r->bottom = b; return TRUE; }
BOOL WINAPI SetRectEmpty( LPRECT r ) { return SetRect( r, 0, 0, 0, 0 ); }
BOOL WINAPI CopyRect( LPRECT d, const RECT *s ) { if ( !d || !s ) return FALSE; *d = *s; return TRUE; }
BOOL WINAPI OffsetRect( LPRECT r, int dx, int dy ) { if ( !r ) return FALSE; r->left += dx; r->right += dx; r->top += dy; r->bottom += dy; return TRUE; }
BOOL WINAPI InflateRect( LPRECT r, int dx, int dy ) { if ( !r ) return FALSE; r->left -= dx; r->right += dx; r->top -= dy; r->bottom += dy; return TRUE; }
BOOL WINAPI IsRectEmpty( const RECT *r ) { return !r || r->right<=r->left || r->bottom<=r->top; }
BOOL WINAPI EqualRect( const RECT *a, const RECT *b ) { return a && b && !memcmp( a, b, sizeof(RECT) ); }
BOOL WINAPI PtInRect( const RECT *r, POINT p ) { return r && p.x>=r->left && p.x<r->right && p.y>=r->top && p.y<r->bottom; }
BOOL WINAPI IntersectRect( LPRECT d, const RECT *a, const RECT *b )
{
	RECT r = { std::max( a->left, b->left ), std::max( a->top, b->top ), std::min( a->right, b->right ), std::min( a->bottom, b->bottom ) };
	if ( IsRectEmpty( &r ) ) { SetRectEmpty( d ); return FALSE; }
	*d = r;
	return TRUE;
}
BOOL WINAPI UnionRect( LPRECT d, const RECT *a, const RECT *b )
{
	if ( IsRectEmpty( a ) ) { *d = *b; return !IsRectEmpty( b ); }
	if ( IsRectEmpty( b ) ) { *d = *a; return TRUE; }
	SetRect( d, std::min( a->left, b->left ), std::min( a->top, b->top ), std::max( a->right, b->right ), std::max( a->bottom, b->bottom ) );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// Message queue and timers
//////////////////////////////////////////////////////////////////////////////
struct PtTimer { HWND hwnd; UINT_PTR id; UINT interval; double next; TIMERPROC proc; };

static std::deque<MSG>		s_Queue;
static std::vector<PtTimer>	s_Timers;
static pthread_mutex_t		s_QueueLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t		s_QueueCond = PTHREAD_COND_INITIALIZER;
static double				s_LastYield = 0;

BOOL WINAPI PostMessageA( HWND h, UINT msg, WPARAM w, LPARAM l )
{
	MSG m = { h, msg, w, l, GetTickCount(), { 0, 0 } };
	pthread_mutex_lock( &s_QueueLock );
	s_Queue.push_back( m );
	pthread_cond_broadcast( &s_QueueCond );
	pthread_mutex_unlock( &s_QueueLock );
	return TRUE;
}
BOOL WINAPI PostThreadMessageA( DWORD, UINT msg, WPARAM w, LPARAM l ) { return PostMessageA( NULL, msg, w, l ); }
void WINAPI PostQuitMessage( int code ) { PostMessageA( NULL, WM_QUIT, (WPARAM)code, 0 ); }

UINT_PTR WINAPI SetTimer( HWND h, UINT_PTR id, UINT ms, TIMERPROC proc )
{
	pthread_mutex_lock( &s_QueueLock );
	for ( auto &t : s_Timers ) if ( t.hwnd==h && t.id==id ) { t.interval = ms; t.next = emscripten_get_now()+ms; t.proc = proc; pthread_mutex_unlock( &s_QueueLock ); return id; }
	if ( !h && !id ) id = 0x1000 + s_Timers.size();
	PtTimer t = { h, id, ms ? ms : 1, emscripten_get_now()+ms, proc };
	s_Timers.push_back( t );
	pthread_mutex_unlock( &s_QueueLock );
	return id ? id : 1;
}

BOOL WINAPI KillTimer( HWND h, UINT_PTR id )
{
	pthread_mutex_lock( &s_QueueLock );
	size_t n = s_Timers.size();
	s_Timers.erase( std::remove_if( s_Timers.begin(), s_Timers.end(), [&]( const PtTimer &t ) { return t.hwnd==h && t.id==id; } ), s_Timers.end() );
	pthread_mutex_unlock( &s_QueueLock );
	return n!=s_Timers.size();
}

static bool Matches( const MSG &m, HWND h, UINT lo, UINT hi )
{
	if ( h && m.hwnd!=h ) return false;
	if ( lo || hi ) return m.message>=lo && m.message<=hi;
	return true;
}

void PtPlat_PumpInput();
struct SDL_Window *PtPlat_GetSDLWindow();

// SendMessage from another thread: as on Windows, the window procedure runs on the
// window's thread (the instance's main thread) the next time it looks at its
// messages, and the sender waits for the result. The socket threads hand every
// received packet to the game this way - running the handler on the socket
// thread itself would change game state in the middle of a frame.
struct PtSentMsg { HWND h; UINT msg; WPARAM wp; LPARAM lp; LRESULT result; bool done; };
static std::deque<PtSentMsg *>	s_Sent;

static void ProcessSentMessages()
{
	pthread_mutex_lock( &s_QueueLock );
	while ( !s_Sent.empty() ) {
		PtSentMsg *m = s_Sent.front();
		s_Sent.pop_front();
		pthread_mutex_unlock( &s_QueueLock );
		PtWnd *w = W( m->h );
		LRESULT r = ( w && w->proc ) ? w->proc( m->h, m->msg, m->wp, m->lp ) : 0;
		pthread_mutex_lock( &s_QueueLock );
		m->result = r;
		m->done = true;
		pthread_cond_broadcast( &s_QueueCond );
	}
	pthread_mutex_unlock( &s_QueueLock );
}

BOOL WINAPI PeekMessageA( LPMSG pMsg, HWND h, UINT lo, UINT hi, UINT remove )
{
	if ( emscripten_is_main_runtime_thread() ) {
		ProcessSentMessages();
		PtPlat_PumpInput();							// SDL input -> window procedures
		double now = emscripten_get_now();
		if ( now - s_LastYield > 50 ) { s_LastYield = now; PtWin_Yield(); }		// loops that only peek
	}
	pthread_mutex_lock( &s_QueueLock );
	for ( auto it = s_Queue.begin(); it!=s_Queue.end(); ++it ) {
		if ( !Matches( *it, h, lo, hi ) ) continue;
		*pMsg = *it;
		if ( remove & PM_REMOVE ) s_Queue.erase( it );
		pthread_mutex_unlock( &s_QueueLock );
		return TRUE;
	}
	// timers fire when nothing else is waiting (as on Windows)
	double now = emscripten_get_now();
	for ( auto &t : s_Timers ) {
		if ( now<t.next ) continue;
		MSG m = { t.hwnd, WM_TIMER, t.id, (LPARAM)t.proc, GetTickCount(), { 0, 0 } };
		if ( !Matches( m, h, lo, hi ) ) continue;
		if ( remove & PM_REMOVE ) t.next = now + t.interval;
		*pMsg = m;
		pthread_mutex_unlock( &s_QueueLock );
		return TRUE;
	}
	pthread_mutex_unlock( &s_QueueLock );
	return FALSE;
}

BOOL WINAPI WaitMessage()
{
	for ( ;; ) {
		MSG m;
		if ( PeekMessageA( &m, NULL, 0, 0, PM_NOREMOVE ) ) return TRUE;
		if ( emscripten_is_main_runtime_thread() ) { PtWin_CheckStall( "WaitMessage" ); s_LastYield = emscripten_get_now(); emscripten_sleep( 5 ); }
		else {
			pthread_mutex_lock( &s_QueueLock );
			struct timespec ts;
			clock_gettime( CLOCK_REALTIME, &ts );
			ts.tv_nsec += 5000000;
			if ( ts.tv_nsec>=1000000000 ) { ts.tv_sec++; ts.tv_nsec -= 1000000000; }
			pthread_cond_timedwait( &s_QueueCond, &s_QueueLock, &ts );
			pthread_mutex_unlock( &s_QueueLock );
		}
	}
}

BOOL WINAPI GetMessageA( LPMSG pMsg, HWND h, UINT lo, UINT hi )
{
	while ( !PeekMessageA( pMsg, h, lo, hi, PM_REMOVE ) ) WaitMessage();
	return pMsg->message!=WM_QUIT;
}

BOOL WINAPI TranslateMessage( const MSG * ) { return FALSE; }		// WM_CHAR comes from SDL text input

LRESULT WINAPI DispatchMessageA( const MSG *m )
{
	if ( m->message==WM_TIMER && m->lParam ) { ((TIMERPROC)m->lParam)( m->hwnd, WM_TIMER, (UINT_PTR)m->wParam, GetTickCount() ); return 0; }
	PtWnd *w = W( m->hwnd );
	if ( !w || !w->proc ) return 0;
	return w->proc( m->hwnd, m->message, m->wParam, m->lParam );
}

LRESULT WINAPI SendMessageA( HWND h, UINT msg, WPARAM wp, LPARAM lp )
{
	PtWnd *w = W( h );
	if ( !w || !w->proc ) return 0;
	if ( emscripten_is_main_runtime_thread() ) return w->proc( h, msg, wp, lp );
	PtSentMsg m = { h, msg, wp, lp, 0, false };		// another thread: the main thread runs it
	pthread_mutex_lock( &s_QueueLock );
	s_Sent.push_back( &m );
	pthread_cond_broadcast( &s_QueueCond );
	while ( !m.done ) pthread_cond_wait( &s_QueueCond, &s_QueueLock );
	pthread_mutex_unlock( &s_QueueLock );
	return m.result;
}
LRESULT WINAPI CallWindowProcA( WNDPROC proc, HWND h, UINT msg, WPARAM wp, LPARAM lp ) { return proc ? proc( h, msg, wp, lp ) : 0; }

//////////////////////////////////////////////////////////////////////////////
// Default window procedure
//////////////////////////////////////////////////////////////////////////////
LRESULT WINAPI DefWindowProcA( HWND h, UINT msg, WPARAM wp, LPARAM lp )
{
	PtWnd *w = W( h );
	switch ( msg ) {
	case WM_CLOSE:			DestroyWindow( h ); return 0;
	case WM_SETTEXT:		if ( w ) w->text = lp ? (const char *)lp : ""; return TRUE;
	case WM_GETTEXT:		if ( !w || !wp ) return 0; lstrcpynA( (LPSTR)lp, w->text.c_str(), (int)wp ); return lstrlenA( (LPSTR)lp );
	case WM_GETTEXTLENGTH:	return w ? (LRESULT)w->text.size() : 0;
	case WM_SYSKEYDOWN:		if ( wp==VK_F4 && ( lp & ( 1<<29 ) ) ) PostMessageA( h, WM_CLOSE, 0, 0 ); return 0;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// EDIT control (the game's hidden text-entry boxes)
//////////////////////////////////////////////////////////////////////////////
static void EditReplaceSel( PtWnd *w, const std::string &s )
{
	int a = std::min( w->selStart, w->selEnd ), b = std::max( w->selStart, w->selEnd );
	a = std::max( 0, std::min( a, (int)w->text.size() ) );
	b = std::max( a, std::min( b, (int)w->text.size() ) );
	std::string ins = s;
	if ( w->limit>0 && (int)( w->text.size() - ( b-a ) + ins.size() ) > w->limit )
		ins = ins.substr( 0, std::max( 0, w->limit - (int)( w->text.size() - ( b-a ) ) ) );
	w->text = w->text.substr( 0, a ) + ins + w->text.substr( b );
	w->selStart = w->selEnd = a + (int)ins.size();
}

static LRESULT CALLBACK EditProc( HWND h, UINT msg, WPARAM wp, LPARAM lp )
{
	PtWnd *w = W( h );
	if ( !w ) return 0;
	int len = (int)w->text.size();
	switch ( msg ) {
	case WM_CHAR:
		if ( wp==8 ) {					// backspace
			if ( w->selStart==w->selEnd && w->selStart>0 ) w->selStart--;
			EditReplaceSel( w, "" );
		}
		else if ( wp==13 ) { if ( ( w->style & ES_MULTILINE ) && ( w->style & ES_WANTRETURN ) ) EditReplaceSel( w, "\r\n" ); }
		else if ( wp>=32 || wp==9 ) {
			if ( ( w->style & ES_NUMBER ) && ( wp<'0' || wp>'9' ) ) return 0;
			EditReplaceSel( w, std::string( 1, (char)wp ) );
		}
		return 0;
	case WM_KEYDOWN:
		switch ( wp ) {
		case VK_LEFT:	w->selStart = w->selEnd = std::max( 0, std::min( w->selStart, w->selEnd ) - ( w->selStart==w->selEnd ? 1 : 0 ) ); break;
		case VK_RIGHT:	w->selStart = w->selEnd = std::min( len, std::max( w->selStart, w->selEnd ) + ( w->selStart==w->selEnd ? 1 : 0 ) ); break;
		case VK_HOME:	w->selStart = w->selEnd = 0; break;
		case VK_END:	w->selStart = w->selEnd = len; break;
		case VK_DELETE:	if ( w->selStart==w->selEnd && w->selEnd<len ) w->selEnd++; EditReplaceSel( w, "" ); break;
		}
		return 0;
	case EM_SETSEL:
		w->selStart = (int)(INT_PTR)wp; w->selEnd = (int)(INT_PTR)lp;
		if ( w->selStart<0 ) { w->selStart = w->selEnd = len; }
		if ( w->selEnd<0 || w->selEnd>len ) w->selEnd = len;
		if ( w->selStart>len ) w->selStart = len;
		return 0;
	case EM_GETSEL:
		if ( wp ) *(DWORD *)wp = (DWORD)w->selStart;
		if ( lp ) *(DWORD *)lp = (DWORD)w->selEnd;
		return MAKELRESULT( w->selStart, w->selEnd );
	case EM_SETLIMITTEXT:	w->limit = (int)wp; return 0;
	case EM_GETLIMITTEXT:	return w->limit>0 ? w->limit : 30000;
	case EM_REPLACESEL:		EditReplaceSel( w, lp ? (const char *)lp : "" ); return 0;
	case WM_SETTEXT:		w->text = lp ? (const char *)lp : ""; w->selStart = w->selEnd = (int)w->text.size(); return TRUE;
	case WM_SETFONT:		return 0;
	}
	return DefWindowProcA( h, msg, wp, lp );
}

static void RegisterBuiltinClasses()
{
	if ( s_Classes.count( "edit" ) ) return;
	WNDCLASSA wc;
	ZeroMemory( &wc, sizeof(wc) );
	wc.lpfnWndProc = EditProc;
	wc.lpszClassName = "EDIT";
	s_Classes["edit"] = wc;
	wc.lpfnWndProc = DefWindowProcA;
	s_Classes["listbox"] = wc;
	s_Classes["button"] = wc;
	s_Classes["static"] = wc;
}

//////////////////////////////////////////////////////////////////////////////
// Window classes and windows
//////////////////////////////////////////////////////////////////////////////
ATOM WINAPI RegisterClassA( const WNDCLASSA *wc )
{
	RegisterBuiltinClasses();
	s_Classes[Lower( wc->lpszClassName )] = *wc;
	return (ATOM)s_Classes.size();
}
BOOL WINAPI UnregisterClassA( LPCSTR name, HINSTANCE ) { return s_Classes.erase( Lower( name ) )>0; }


HWND WINAPI CreateWindowExA( DWORD exStyle, LPCSTR cls, LPCSTR title, DWORD style, int x, int y, int cx, int cy,
	HWND parent, HMENU menu, HINSTANCE hInst, LPVOID param )
{
	RegisterBuiltinClasses();
	auto it = s_Classes.find( Lower( cls ) );
	if ( it==s_Classes.end() ) { PtWin_Log( "CreateWindow: unknown class %s", cls ); return NULL; }
	PtWnd *w = new PtWnd;
	w->magic = PTWND_MAGIC;
	w->className = cls;
	w->proc = it->second.lpfnWndProc;
	w->parent = parent;
	w->style = style; w->exStyle = exStyle;
	w->id = (LONG_PTR)menu; w->userData = 0;
	if ( x==CW_USEDEFAULT ) x = 0;
	if ( y==CW_USEDEFAULT ) y = 0;
	if ( cx==CW_USEDEFAULT ) cx = 800;
	if ( cy==CW_USEDEFAULT ) cy = 600;
	SetRect( &w->rect, x, y, x+cx, y+cy );
	w->text = title ? title : "";
	w->visible = ( style & WS_VISIBLE )!=0;
	w->enabled = TRUE;
	w->hInst = hInst;
	w->selStart = w->selEnd = 0;
	w->limit = 0;
	pthread_mutex_lock( &s_WndLock );
	s_Windows.push_back( w );
	pthread_mutex_unlock( &s_WndLock );
	HWND h = (HWND)w;
	if ( !parent && !s_hMain && Lower( cls )!="edit" ) s_hMain = h;

	CREATESTRUCTA cs = { param, hInst, menu, parent, cy, cx, y, x, (LONG)style, title, cls, exStyle };
	SendMessageA( h, WM_NCCREATE, 0, (LPARAM)&cs );
	if ( SendMessageA( h, WM_CREATE, 0, (LPARAM)&cs )==-1 ) { DestroyWindow( h ); return NULL; }
	SendMessageA( h, WM_SIZE, SIZE_RESTORED, MAKELPARAM( cx, cy ) );
	return h;
}

// CreateWindowA is a macro onto CreateWindowExA in the Windows headers.

HWND WINAPI CreateDialogParamA( HINSTANCE, LPCSTR, HWND, DLGPROC, LPARAM ) { return NULL; }

BOOL WINAPI DestroyWindow( HWND h )
{
	PtWnd *w = W( h );
	if ( !w ) return FALSE;
	SendMessageA( h, WM_DESTROY, 0, 0 );
	SendMessageA( h, WM_NCDESTROY, 0, 0 );
	pthread_mutex_lock( &s_WndLock );
	s_Windows.erase( std::remove( s_Windows.begin(), s_Windows.end(), w ), s_Windows.end() );
	pthread_mutex_unlock( &s_WndLock );
	for ( auto it = s_Timers.begin(); it!=s_Timers.end(); ) it = it->hwnd==h ? s_Timers.erase( it ) : it+1;
	if ( s_hFocus==h ) s_hFocus = NULL;
	if ( s_hMain==h ) s_hMain = NULL;
	w->magic = 0;
	delete w;
	return TRUE;
}

BOOL WINAPI IsWindow( HWND h ) { return W( h )!=NULL; }
BOOL WINAPI IsWindowVisible( HWND h ) { PtWnd *w = W( h ); return w && w->visible; }
BOOL WINAPI ShowWindow( HWND h, int cmd ) { PtWnd *w = W( h ); if ( !w ) return FALSE; BOOL was = w->visible; w->visible = cmd!=SW_HIDE; return was; }
BOOL WINAPI UpdateWindow( HWND h ) { return W( h )!=NULL; }
BOOL WINAPI InvalidateRect( HWND, const RECT *, BOOL ) { return TRUE; }
BOOL WINAPI EnableWindow( HWND h, BOOL b ) { PtWnd *w = W( h ); if ( !w ) return FALSE; BOOL was = !w->enabled; w->enabled = b; return was; }

BOOL WINAPI MoveWindow( HWND h, int x, int y, int cx, int cy, BOOL )
{
	PtWnd *w = W( h );
	if ( !w ) return FALSE;
	SetRect( &w->rect, x, y, x+cx, y+cy );
	SendMessageA( h, WM_SIZE, SIZE_RESTORED, MAKELPARAM( cx, cy ) );
	return TRUE;
}

BOOL WINAPI SetWindowPos( HWND h, HWND, int x, int y, int cx, int cy, UINT flags )
{
	PtWnd *w = W( h );
	if ( !w ) return FALSE;
	if ( flags & SWP_NOMOVE ) { x = w->rect.left; y = w->rect.top; }
	if ( flags & SWP_NOSIZE ) { cx = w->rect.right-w->rect.left; cy = w->rect.bottom-w->rect.top; }
	if ( flags & SWP_SHOWWINDOW ) w->visible = TRUE;
	if ( flags & SWP_HIDEWINDOW ) w->visible = FALSE;
	SetRect( &w->rect, x, y, x+cx, y+cy );
	return TRUE;
}

BOOL WINAPI GetClientRect( HWND h, LPRECT r )
{
	PtWnd *w = W( h );
	if ( !w ) return FALSE;
	SetRect( r, 0, 0, w->rect.right-w->rect.left, w->rect.bottom-w->rect.top );
	return TRUE;
}

static POINT Origin( PtWnd *w )
{
	POINT p = { 0, 0 };
	while ( w ) { p.x += w->rect.left; p.y += w->rect.top; w = W( w->parent ); }
	return p;
}

BOOL WINAPI GetWindowRect( HWND h, LPRECT r )
{
	if ( h==GetDesktopWindow() ) { SetRect( r, 0, 0, 1920, 1080 ); return TRUE; }		// a typical screen (the game picks 800x600 on small ones)
	PtWnd *w = W( h );
	if ( !w ) { SetRect( r, 0, 0, 0, 0 ); return FALSE; }
	POINT o = Origin( W( w->parent ) );
	*r = w->rect;
	OffsetRect( r, o.x, o.y );
	return TRUE;
}

BOOL WINAPI ClientToScreen( HWND h, LPPOINT p ) { POINT o = Origin( W( h ) ); p->x += o.x; p->y += o.y; return TRUE; }
BOOL WINAPI ScreenToClient( HWND h, LPPOINT p ) { POINT o = Origin( W( h ) ); p->x -= o.x; p->y -= o.y; return TRUE; }
BOOL WINAPI AdjustWindowRect( LPRECT, DWORD, BOOL ) { return TRUE; }			// the canvas has no frame
BOOL WINAPI AdjustWindowRectEx( LPRECT, DWORD, BOOL, DWORD ) { return TRUE; }

LONG WINAPI GetWindowLongA( HWND h, int idx )
{
	PtWnd *w = W( h );
	if ( !w ) return 0;
	switch ( idx ) {
	case GWL_WNDPROC:	return (LONG)(LONG_PTR)w->proc;
	case GWL_STYLE:		return (LONG)w->style;
	case GWL_EXSTYLE:	return (LONG)w->exStyle;
	case GWL_ID:		return (LONG)w->id;
	case GWL_USERDATA:	return (LONG)w->userData;
	case GWL_HINSTANCE:	return (LONG)(LONG_PTR)w->hInst;
	}
	return 0;
}

LONG WINAPI SetWindowLongA( HWND h, int idx, LONG v )
{
	PtWnd *w = W( h );
	if ( !w ) return 0;
	LONG old = GetWindowLongA( h, idx );
	switch ( idx ) {
	case GWL_WNDPROC:	w->proc = (WNDPROC)(LONG_PTR)v; break;
	case GWL_STYLE:		w->style = (DWORD)v; break;
	case GWL_EXSTYLE:	w->exStyle = (DWORD)v; break;
	case GWL_ID:		w->id = v; break;
	case GWL_USERDATA:	w->userData = v; break;
	}
	return old;
}

BOOL WINAPI SetWindowTextA( HWND h, LPCSTR s ) { return W( h ) ? (BOOL)SendMessageA( h, WM_SETTEXT, 0, (LPARAM)s ) : FALSE; }
int WINAPI GetWindowTextA( HWND h, LPSTR buf, int n )
{
	if ( !buf || n<=0 ) return 0;
	buf[0] = 0;
	PtWnd *w = W( h );
	if ( !w ) return 0;
	lstrcpynA( buf, w->text.c_str(), n );
	return lstrlenA( buf );
}
int WINAPI GetWindowTextLengthA( HWND h ) { PtWnd *w = W( h ); return w ? (int)w->text.size() : 0; }
int WINAPI GetClassNameA( HWND h, LPSTR buf, int n ) { PtWnd *w = W( h ); if ( !w ) return 0; lstrcpynA( buf, w->className.c_str(), n ); return lstrlenA( buf ); }
HWND WINAPI GetParent( HWND h ) { PtWnd *w = W( h ); return w ? w->parent : NULL; }
HWND WINAPI GetDlgItem( HWND parent, int id )
{
	for ( auto w : s_Windows ) if ( w->parent==parent && w->id==id ) return (HWND)w;
	return NULL;
}

HWND WINAPI FindWindowA( LPCSTR cls, LPCSTR title )
{
	for ( auto w : s_Windows ) {
		if ( w->parent ) continue;
		if ( cls && strcasecmp( w->className.c_str(), cls ) ) continue;
		if ( title && strcmp( w->text.c_str(), title ) ) continue;
		return (HWND)w;
	}
	return NULL;
}
BOOL WINAPI EnumWindows( WNDENUMPROC proc, LPARAM lp )
{
	std::vector<PtWnd *> list = s_Windows;
	for ( auto w : list ) if ( !w->parent && !proc( (HWND)w, lp ) ) break;
	return TRUE;
}
BOOL WINAPI EnumThreadWindows( DWORD, WNDENUMPROC proc, LPARAM lp ) { return EnumWindows( proc, lp ); }

HWND WINAPI GetDesktopWindow() { return (HWND)(intptr_t)0x10010; }
HWND WINAPI GetForegroundWindow() { return s_hMain; }
BOOL WINAPI SetForegroundWindow( HWND ) { return TRUE; }
HWND WINAPI GetActiveWindow() { return s_hMain; }
HWND WINAPI SetActiveWindow( HWND h ) { return s_hMain; }

HWND WINAPI SetFocus( HWND h )
{
	HWND old = s_hFocus;
	if ( old==h ) return old;
	s_hFocus = h;
	if ( old ) SendMessageA( old, WM_KILLFOCUS, (WPARAM)h, 0 );
	if ( h ) SendMessageA( h, WM_SETFOCUS, (WPARAM)old, 0 );
	return old;
}
HWND WINAPI GetFocus() { return s_hFocus; }
HWND WINAPI SetCapture( HWND h ) { return NULL; }
BOOL WINAPI ReleaseCapture() { return TRUE; }

//////////////////////////////////////////////////////////////////////////////
// Keyboard state
//////////////////////////////////////////////////////////////////////////////
static BYTE s_Keys[256];		// 0x80 = down, 0x01 = toggled

void PtWin_SetKeyDown( int vk, BOOL bDown )
{
	if ( vk<0 || vk>255 ) return;
	if ( bDown && !( s_Keys[vk] & 0x80 ) ) s_Keys[vk] ^= 0x01;
	s_Keys[vk] = (BYTE)( ( s_Keys[vk] & 0x01 ) | ( bDown ? 0x80 : 0 ) );
	// generic shift/control/alt follow their left/right versions
	if ( vk==VK_LSHIFT || vk==VK_RSHIFT ) PtWin_SetKeyDown( VK_SHIFT, bDown );
	if ( vk==VK_LCONTROL || vk==VK_RCONTROL ) PtWin_SetKeyDown( VK_CONTROL, bDown );
	if ( vk==VK_LMENU || vk==VK_RMENU ) PtWin_SetKeyDown( VK_MENU, bDown );
}
SHORT WINAPI GetKeyState( int vk ) { return (SHORT)( ( s_Keys[vk & 0xFF] & 0x80 ? 0x8000 : 0 ) | ( s_Keys[vk & 0xFF] & 0x01 ) ); }
SHORT WINAPI GetAsyncKeyState( int vk ) { return (SHORT)( s_Keys[vk & 0xFF] & 0x80 ? 0x8000 : 0 ); }
BOOL WINAPI GetKeyboardState( PBYTE p ) { memcpy( p, s_Keys, 256 ); return TRUE; }

UINT WINAPI MapVirtualKeyA( UINT code, UINT type )
{
	// scan codes of the PC/AT keyboard (set 1) for the common keys
	static const struct { BYTE vk, sc; } map[] = {
		{VK_ESCAPE,1},{'1',2},{'2',3},{'3',4},{'4',5},{'5',6},{'6',7},{'7',8},{'8',9},{'9',10},{'0',11},{VK_OEM_MINUS,12},{VK_OEM_PLUS,13},
		{VK_BACK,14},{VK_TAB,15},{'Q',16},{'W',17},{'E',18},{'R',19},{'T',20},{'Y',21},{'U',22},{'I',23},{'O',24},{'P',25},{VK_OEM_4,26},
		{VK_OEM_6,27},{VK_RETURN,28},{VK_CONTROL,29},{'A',30},{'S',31},{'D',32},{'F',33},{'G',34},{'H',35},{'J',36},{'K',37},{'L',38},
		{VK_OEM_1,39},{VK_OEM_7,40},{VK_OEM_3,41},{VK_SHIFT,42},{VK_OEM_5,43},{'Z',44},{'X',45},{'C',46},{'V',47},{'B',48},{'N',49},
		{'M',50},{VK_OEM_COMMA,51},{VK_OEM_PERIOD,52},{VK_OEM_2,53},{VK_MULTIPLY,55},{VK_MENU,56},{VK_SPACE,57},{VK_CAPITAL,58},
		{VK_F1,59},{VK_F2,60},{VK_F3,61},{VK_F4,62},{VK_F5,63},{VK_F6,64},{VK_F7,65},{VK_F8,66},{VK_F9,67},{VK_F10,68},
		{VK_NUMLOCK,69},{VK_SCROLL,70},{VK_HOME,71},{VK_UP,72},{VK_PRIOR,73},{VK_SUBTRACT,74},{VK_LEFT,75},{VK_NUMPAD5,76},
		{VK_RIGHT,77},{VK_ADD,78},{VK_END,79},{VK_DOWN,80},{VK_NEXT,81},{VK_INSERT,82},{VK_DELETE,83},{VK_F11,87},{VK_F12,88} };
	for ( auto &m : map ) {
		if ( type==MAPVK_VK_TO_VSC && m.vk==code ) return m.sc;
		if ( type==MAPVK_VSC_TO_VK && m.sc==code ) return m.vk;
	}
	if ( type==MAPVK_VK_TO_CHAR ) return ( code>='0' && code<='9' ) || ( code>='A' && code<='Z' ) ? code : 0;
	return 0;
}

HKL WINAPI GetKeyboardLayout( DWORD ) { return (HKL)(intptr_t)0x04090409; }
BOOL WINAPI GetKeyboardLayoutNameA( LPSTR name ) { strcpy( name, "00000409" ); return TRUE; }
HKL WINAPI LoadKeyboardLayoutA( LPCSTR, UINT ) { return (HKL)(intptr_t)0x04090409; }

//////////////////////////////////////////////////////////////////////////////
// Cursor, icons, menus, metrics
//////////////////////////////////////////////////////////////////////////////
static int s_CursorCount = 0;
int WINAPI ShowCursor( BOOL bShow )
{
	s_CursorCount += bShow ? 1 : -1;
	if ( PtPlat_GetSDLWindow() ) SDL_ShowCursor( s_CursorCount>=0 ? SDL_ENABLE : SDL_DISABLE );		// the game draws its own cursor
	return s_CursorCount;
}
HCURSOR WINAPI SetCursor( HCURSOR c ) { return c; }
BOOL WINAPI GetCursorPos( LPPOINT p ) { int x = 0, y = 0; if ( PtPlat_GetSDLWindow() ) SDL_GetMouseState( &x, &y ); p->x = x; p->y = y; return TRUE; }
BOOL WINAPI SetCursorPos( int, int ) { return TRUE; }
HCURSOR WINAPI LoadCursorA( HINSTANCE, LPCSTR ) { return (HCURSOR)(intptr_t)0x20001; }
HICON WINAPI LoadIconA( HINSTANCE, LPCSTR ) { return (HICON)(intptr_t)0x20002; }
BOOL WINAPI DestroyIcon( HICON ) { return TRUE; }
HMENU WINAPI LoadMenuA( HINSTANCE, LPCSTR ) { return (HMENU)(intptr_t)0x20003; }
HMENU WINAPI CreateMenu() { return (HMENU)(intptr_t)0x20003; }
HMENU WINAPI GetMenu( HWND ) { return NULL; }
BOOL WINAPI SetMenu( HWND, HMENU ) { return TRUE; }
DWORD WINAPI CheckMenuItem( HMENU, UINT, UINT ) { return 0; }
BOOL WINAPI EnableMenuItem( HMENU, UINT, UINT ) { return TRUE; }

int WINAPI GetSystemMetrics( int idx )
{
	RECT r = { 0, 0, 1024, 768 };
	if ( s_hMain ) GetClientRect( s_hMain, &r );
	switch ( idx ) {
	case SM_CXSCREEN: case SM_CXFULLSCREEN: case SM_CXVIRTUALSCREEN: return r.right;
	case SM_CYSCREEN: case SM_CYFULLSCREEN: case SM_CYVIRTUALSCREEN: return r.bottom;
	case SM_CYCAPTION:	return 0;
	case SM_CYMENUSIZE: case SM_CYMENU: return 0;
	case SM_CXBORDER: case SM_CYBORDER: case SM_CXFRAME: case SM_CYFRAME: return 0;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Message boxes
//////////////////////////////////////////////////////////////////////////////
EM_JS( void, PtWin_Alert, ( const char *s ), { if ( typeof alert === "function" ) alert( UTF8ToString( s ) ); } );

int WINAPI MessageBoxA( HWND, LPCSTR text, LPCSTR caption, UINT )
{
	PtWin_Log( "MessageBox [%s] %s", caption ? caption : "", text ? text : "" );
	if ( emscripten_is_main_runtime_thread() ) {
		std::string s = std::string( caption ? caption : "" ) + "\n\n" + ( text ? text : "" );
		WCHAR w[1024];
		int n = MultiByteToWideChar( CP_ACP, 0, s.c_str(), -1, w, 1024 );
		char u[3072];
		WideCharToMultiByte( CP_UTF8, 0, w, n, u, sizeof(u), NULL, NULL );
		PtWin_Alert( u );
	}
	return IDOK;
}
