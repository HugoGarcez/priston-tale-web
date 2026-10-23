// ptwin_net.cpp - Winsock for the web build: TCP sockets that never leave the browser.
//
// The game (page) and the server (a second instance of the same game.wasm in a
// Web Worker) talk to each other exactly as on Windows - socket/connect/accept/
// send/recv/select/WSAAsyncSelect - but the bytes travel between the two
// instances with postMessage (web\pre.js forwards them). A connection inside one
// instance (the server talking to itself) is wired up directly.
//
// Messages between the instances (kind, link id, port, bytes):
//   1 connect  2 accepted  3 refused  4 data  5 closed
#include "ptwin_internal.h"
#include <winsock.h>
#include <emscripten.h>
#include <emscripten/threading.h>
#include <deque>
#include <algorithm>

enum { NET_CONNECT = 1, NET_ACCEPTED, NET_REFUSED, NET_DATA, NET_CLOSED };
enum { S_NEW, S_LISTENING, S_CONNECTING, S_CONNECTED, S_REFUSED, S_CLOSED };

struct PtSock {
	SOCKET				id;
	int					state;
	bool				nonblock;
	u_short				localPort, remotePort;
	std::deque<char>	rx;
	bool				peerClosed;
	std::deque<PtSock *> pendingAccept;			// listener: connected sockets waiting for accept()
	PtSock				*localPeer;					// connection inside this instance
	int					link;						// connection to the other instance (0 = none)
	HWND				asyncWnd;
	UINT				asyncMsg;
	long				asyncEvents;
};

static pthread_mutex_t				s_Lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t				s_Cond = PTHREAD_COND_INITIALIZER;
static std::map<SOCKET, PtSock *>	s_Socks;
static std::map<u_short, PtSock *>	s_Listeners;
static std::map<int, PtSock *>		s_Links;
static std::vector<std::pair<int, u_short>> s_PendingConnects;		// server: connects that came before listen()
static SOCKET						s_NextSock = 1000;
static int							s_NextLink = 1;
static thread_local int				t_Err = 0;

EM_JS( int, PtNet_IsServerInstance, (), { return Module['ptServer'] ? 1 : 0; } );
EM_JS( int, PtNet_DebugFlag, (), { return ( Module['ptDebugNet'] || ( typeof location!='undefined' && /debugnet/.test( location.search ) ) ) ? 1 : 0; } );
int g_PtNetDebug = 0;
#define s_Debug g_PtNetDebug					// ?debugnet in the page address: log the traffic
static const char *const s_KindName[] = { "", "connect", "accepted", "refused", "data", "closed" };

static PtSock *Get( SOCKET s ) { auto it = s_Socks.find( s ); return it==s_Socks.end() ? NULL : it->second; }

static void Notify( PtSock *p, long ev )
{
	if ( s_Debug ) PtWin_Log( "net: sock %d event %lX%s", (int)p->id, ev, p->asyncWnd && ( p->asyncEvents & ev ) ? " posted" : "" );
	if ( p->asyncWnd && ( p->asyncEvents & ev ) )
		PostMessageA( p->asyncWnd, p->asyncMsg, (WPARAM)p->id, WSAMAKESELECTREPLY( ev, 0 ) );
}

// Wait (with s_Lock held) until pred() or timeout. The instance's main thread
// lets the browser run meanwhile (messages from the other instance arrive then).
template<class F> static bool WaitFor( F pred, int timeoutMs )
{
	double end = emscripten_get_now() + ( timeoutMs<0 ? 1e12 : timeoutMs );
	while ( !pred() ) {
		if ( emscripten_get_now()>=end ) return false;
		if ( emscripten_is_main_runtime_thread() ) {
			pthread_mutex_unlock( &s_Lock );
			PtWin_CheckStall( "a socket call" );
			emscripten_sleep( 2 );
			pthread_mutex_lock( &s_Lock );
		}
		else {
			struct timespec ts;
			clock_gettime( CLOCK_REALTIME, &ts );
			ts.tv_nsec += 5000000;
			if ( ts.tv_nsec>=1000000000 ) { ts.tv_sec++; ts.tv_nsec -= 1000000000; }
			pthread_cond_timedwait( &s_Cond, &s_Lock, &ts );
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////
// To the other instance (runs the JS on this instance's main thread)
//////////////////////////////////////////////////////////////////////////////
static void Post( int kind, int link, int port, const char *data, int len )
{
	if ( s_Debug ) PtWin_Log( "net: -> %s link %X port %d len %d", s_KindName[kind], link, port, len );
	char *copy = (char *)malloc( len>0 ? len : 1 );
	if ( len>0 ) memcpy( copy, data, len );
	MAIN_THREAD_ASYNC_EM_ASM( {
		var bytes = HEAPU8.slice( $3, $3 + $4 );
		_free( $3 );
		if ( Module['ptNetPost'] ) Module['ptNetPost']( $0, $1, $2, bytes );
	}, kind, link, port, copy, len );
}

// From the other instance (called by pre.js on this instance's main thread)
extern "C" EMSCRIPTEN_KEEPALIVE void PtNet_Receive( int kind, int link, int port, const char *data, int len );

static PtSock *NewSock()
{
	PtSock *p = new PtSock;
	p->id = s_NextSock++;
	p->state = S_NEW; p->nonblock = false; p->localPort = p->remotePort = 0;
	p->peerClosed = false; p->localPeer = NULL; p->link = 0;
	p->asyncWnd = NULL; p->asyncMsg = 0; p->asyncEvents = 0;
	s_Socks[p->id] = p;
	return p;
}

// A connection request arrives at listener l (lock held)
static PtSock *AcceptInto( PtSock *l, u_short remotePort )
{
	PtSock *c = NewSock();
	c->state = S_CONNECTED;
	c->localPort = l->localPort;
	c->remotePort = remotePort;
	l->pendingAccept.push_back( c );
	Notify( l, FD_ACCEPT );
	return c;
}

static void HandleRemoteConnect( int link, u_short port )
{
	auto it = s_Listeners.find( port );
	if ( it==s_Listeners.end() ) {
		if ( PtNet_IsServerInstance() ) { s_PendingConnects.push_back( std::make_pair( link, port ) ); return; }		// not listening yet
		Post( NET_REFUSED, link, port, NULL, 0 );
		return;
	}
	PtSock *c = AcceptInto( it->second, 40000 + ( link & 0x3FFF ) );
	c->link = link;
	s_Links[link] = c;
	Post( NET_ACCEPTED, link, port, NULL, 0 );
}

extern "C" EMSCRIPTEN_KEEPALIVE void PtNet_Receive( int kind, int link, int port, const char *data, int len )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *p = s_Links.count( link ) ? s_Links[link] : NULL;
	if ( s_Debug ) PtWin_Log( "net: <- %s link %X port %d len %d%s", s_KindName[kind], link, port, len, p || kind==NET_CONNECT ? "" : " (no socket)" );
	switch ( kind ) {
	case NET_CONNECT:	HandleRemoteConnect( link, (u_short)port ); break;
	case NET_ACCEPTED:	if ( p ) { p->state = S_CONNECTED; Notify( p, FD_CONNECT ); Notify( p, FD_WRITE ); } break;
	case NET_REFUSED:	if ( p ) p->state = S_REFUSED; break;
	case NET_DATA:		if ( p ) { p->rx.insert( p->rx.end(), data, data+len ); Notify( p, FD_READ ); } break;
	case NET_CLOSED:	if ( p ) { p->peerClosed = true; Notify( p, FD_CLOSE ); } break;
	}
	pthread_cond_broadcast( &s_Cond );
	pthread_mutex_unlock( &s_Lock );
}

//////////////////////////////////////////////////////////////////////////////
// Winsock API
//////////////////////////////////////////////////////////////////////////////
int WINAPI WSAStartup( WORD ver, LPWSADATA data )
{
	s_Debug = PtNet_DebugFlag();
	if ( data ) { ZeroMemory( data, sizeof(*data) ); data->wVersion = ver; data->wHighVersion = ver; strcpy( data->szDescription, "ptwin" ); }
	return 0;
}
int WINAPI WSACleanup() { return 0; }
int WINAPI WSAGetLastError() { return t_Err; }
void WINAPI WSASetLastError( int e ) { t_Err = e; }
static int Fail( int e ) { t_Err = e; return SOCKET_ERROR; }

SOCKET WINAPI socket( int af, int type, int proto )
{
	if ( type!=SOCK_STREAM ) { t_Err = WSAESOCKTNOSUPPORT; return INVALID_SOCKET; }
	pthread_mutex_lock( &s_Lock );
	SOCKET s = NewSock()->id;
	pthread_mutex_unlock( &s_Lock );
	return s;
}

int WINAPI bind( SOCKET s, const struct sockaddr *addr, int )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *p = Get( s );
	if ( p ) p->localPort = ntohs( ((const sockaddr_in *)addr)->sin_port );
	pthread_mutex_unlock( &s_Lock );
	return p ? 0 : Fail( WSAENOTSOCK );
}

int WINAPI listen( SOCKET s, int )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *p = Get( s );
	if ( !p ) { pthread_mutex_unlock( &s_Lock ); return Fail( WSAENOTSOCK ); }
	p->state = S_LISTENING;
	s_Listeners[p->localPort] = p;
	PtWin_Log( "net: listening on port %d", p->localPort );
	// connections that arrived before the server was ready
	std::vector<std::pair<int, u_short>> pending;
	pending.swap( s_PendingConnects );
	for ( auto &c : pending ) HandleRemoteConnect( c.first, c.second );
	pthread_mutex_unlock( &s_Lock );
	return 0;
}

SOCKET WINAPI accept( SOCKET s, struct sockaddr *addr, int *addrLen )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *l = Get( s );
	if ( !l || l->state!=S_LISTENING ) { pthread_mutex_unlock( &s_Lock ); t_Err = WSAEINVAL; return INVALID_SOCKET; }
	if ( l->pendingAccept.empty() && ( l->nonblock || !WaitFor( [&] { return !l->pendingAccept.empty(); }, -1 ) ) ) {
		pthread_mutex_unlock( &s_Lock ); t_Err = WSAEWOULDBLOCK; return INVALID_SOCKET;
	}
	PtSock *c = l->pendingAccept.front();
	l->pendingAccept.pop_front();
	if ( addr && addrLen && *addrLen>=(int)sizeof(sockaddr_in) ) {
		sockaddr_in *a = (sockaddr_in *)addr;
		ZeroMemory( a, sizeof(*a) );
		a->sin_family = AF_INET;
		a->sin_port = htons( c->remotePort );
		a->sin_addr.s_addr = htonl( 0x7F000001 );
		*addrLen = sizeof(sockaddr_in);
	}
	SOCKET id = c->id;
	pthread_mutex_unlock( &s_Lock );
	return id;
}

int WINAPI connect( SOCKET s, const struct sockaddr *addr, int )
{
	u_short port = ntohs( ((const sockaddr_in *)addr)->sin_port );
	pthread_mutex_lock( &s_Lock );
	PtSock *p = Get( s );
	if ( !p ) { pthread_mutex_unlock( &s_Lock ); return Fail( WSAENOTSOCK ); }
	p->remotePort = port;
	auto it = s_Listeners.find( port );
	if ( it!=s_Listeners.end() ) {				// inside this instance
		PtSock *c = AcceptInto( it->second, 50000 + (u_short)( p->id & 0x3FFF ) );
		p->localPeer = c; c->localPeer = p;
		p->state = S_CONNECTED;
		Notify( p, FD_CONNECT ); Notify( p, FD_WRITE );
		pthread_mutex_unlock( &s_Lock );
		return 0;
	}
	// to the other instance (the server starts listening a little after the page loads)
	p->link = ( PtNet_IsServerInstance() ? 0x40000000 : 0 ) | s_NextLink++;
	s_Links[p->link] = p;
	p->state = S_CONNECTING;
	Post( NET_CONNECT, p->link, port, NULL, 0 );
	if ( p->nonblock ) { pthread_mutex_unlock( &s_Lock ); return Fail( WSAEWOULDBLOCK ); }
	bool done = WaitFor( [&] { return p->state!=S_CONNECTING; }, 180000 );
	int state = p->state;
	pthread_mutex_unlock( &s_Lock );
	if ( !done ) return Fail( WSAETIMEDOUT );
	if ( state!=S_CONNECTED ) return Fail( WSAECONNREFUSED );
	return 0;
}

int WINAPI send( SOCKET s, const char *buf, int len, int )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *p = Get( s );
	if ( !p || p->state!=S_CONNECTED ) { pthread_mutex_unlock( &s_Lock ); return Fail( p ? WSAENOTCONN : WSAENOTSOCK ); }
	if ( p->peerClosed ) { pthread_mutex_unlock( &s_Lock ); return Fail( WSAECONNRESET ); }
	if ( p->localPeer ) {
		p->localPeer->rx.insert( p->localPeer->rx.end(), buf, buf+len );
		Notify( p->localPeer, FD_READ );
		pthread_cond_broadcast( &s_Cond );
	}
	else if ( p->link ) Post( NET_DATA, p->link, 0, buf, len );
	pthread_mutex_unlock( &s_Lock );
	return len;
}

int WINAPI recv( SOCKET s, char *buf, int len, int flags )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *p = Get( s );
	if ( !p ) { pthread_mutex_unlock( &s_Lock ); return Fail( WSAENOTSOCK ); }
	if ( p->rx.empty() && !p->peerClosed ) {
		if ( p->nonblock ) { pthread_mutex_unlock( &s_Lock ); return Fail( WSAEWOULDBLOCK ); }
		WaitFor( [&] { return !p->rx.empty() || p->peerClosed || p->state==S_CLOSED; }, -1 );
	}
	int n = std::min( len, (int)p->rx.size() );
	std::copy( p->rx.begin(), p->rx.begin()+n, buf );
	if ( !( flags & MSG_PEEK ) ) p->rx.erase( p->rx.begin(), p->rx.begin()+n );
	if ( s_Debug ) PtWin_Log( "net: sock %d recv %d (%d left)", (int)s, n, (int)p->rx.size() );
	pthread_mutex_unlock( &s_Lock );
	return n;			// 0 = connection closed
}

static void ClosePeerSide( PtSock *p )
{
	if ( p->localPeer ) { p->localPeer->peerClosed = true; Notify( p->localPeer, FD_CLOSE ); p->localPeer->localPeer = NULL; }
	else if ( p->link && p->state==S_CONNECTED ) {
		// the other instance loses its connection: say who closed it (rare, and the reason is never obvious)
		static char stack[4096];
		emscripten_get_callstack( EM_LOG_C_STACK, stack, sizeof(stack) );
		PtWin_Log( "net: closing connection %X (sock %d)\n%s", p->link, (int)p->id, stack );
		Post( NET_CLOSED, p->link, 0, NULL, 0 );
	}
}

int WINAPI shutdown( SOCKET s, int )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *p = Get( s );
	if ( p ) { ClosePeerSide( p ); p->localPeer = NULL; }
	pthread_cond_broadcast( &s_Cond );
	pthread_mutex_unlock( &s_Lock );
	return 0;
}

int WINAPI closesocket( SOCKET s )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *p = Get( s );
	if ( !p ) { pthread_mutex_unlock( &s_Lock ); return Fail( WSAENOTSOCK ); }
	ClosePeerSide( p );
	if ( p->state==S_LISTENING ) s_Listeners.erase( p->localPort );
	if ( p->link ) s_Links.erase( p->link );
	for ( auto c : p->pendingAccept ) { s_Socks.erase( c->id ); delete c; }
	s_Socks.erase( s );
	p->state = S_CLOSED;
	pthread_cond_broadcast( &s_Cond );
	delete p;
	pthread_mutex_unlock( &s_Lock );
	return 0;
}

int WINAPI ioctlsocket( SOCKET s, LONG cmd, u_long *arg )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *p = Get( s );
	int r = 0;
	if ( !p ) r = Fail( WSAENOTSOCK );
	else if ( cmd==(LONG)FIONBIO ) p->nonblock = *arg!=0;
	else if ( cmd==(LONG)FIONREAD ) { *arg = (u_long)p->rx.size(); if ( s_Debug ) PtWin_Log( "net: sock %d FIONREAD %d", (int)s, (int)*arg ); }
	pthread_mutex_unlock( &s_Lock );
	return r;
}

int WINAPI select( int, fd_set *rd, fd_set *wr, fd_set *ex, const struct timeval *tv )
{
	int timeoutMs = tv ? (int)( tv->tv_sec*1000 + tv->tv_usec/1000 ) : -1;
	fd_set inR, inW;
	if ( rd ) inR = *rd; else inR.fd_count = 0;
	if ( wr ) inW = *wr; else inW.fd_count = 0;
	pthread_mutex_lock( &s_Lock );
	int n = 0;
	auto check = [&]() {
		n = 0;
		if ( rd ) { rd->fd_count = 0; }
		if ( wr ) { wr->fd_count = 0; }
		for ( u_int i=0; i<inR.fd_count; i++ ) {
			PtSock *p = Get( inR.fd_array[i] );
			if ( p && ( !p->rx.empty() || p->peerClosed || !p->pendingAccept.empty() || p->state==S_REFUSED ) ) { rd->fd_array[rd->fd_count++] = p->id; n++; }
		}
		for ( u_int i=0; i<inW.fd_count; i++ ) {
			PtSock *p = Get( inW.fd_array[i] );
			if ( p && p->state==S_CONNECTED ) { wr->fd_array[wr->fd_count++] = p->id; n++; }
		}
		return n>0;
	};
	if ( !check() && timeoutMs!=0 ) WaitFor( check, timeoutMs );
	if ( ex ) ex->fd_count = 0;
	pthread_mutex_unlock( &s_Lock );
	return n;
}

int WINAPI WSAAsyncSelect( SOCKET s, HWND hWnd, u_int msg, LONG events )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *p = Get( s );
	if ( !p ) { pthread_mutex_unlock( &s_Lock ); return Fail( WSAENOTSOCK ); }
	p->asyncWnd = hWnd; p->asyncMsg = msg; p->asyncEvents = events;
	p->nonblock = true;					// as on Windows
	// report what is already pending
	if ( !p->pendingAccept.empty() ) Notify( p, FD_ACCEPT );
	if ( !p->rx.empty() ) Notify( p, FD_READ );
	if ( p->state==S_CONNECTED ) Notify( p, FD_WRITE );
	pthread_mutex_unlock( &s_Lock );
	return 0;
}

int WINAPI setsockopt( SOCKET, int, int, const char *, int ) { return 0; }
int WINAPI getsockopt( SOCKET, int, int opt, char *val, int *len ) { if ( val && len && *len>=4 ) { *(int *)val = 65536; *len = 4; } return 0; }

int WINAPI getpeername( SOCKET s, struct sockaddr *addr, int *len )
{
	pthread_mutex_lock( &s_Lock );
	PtSock *p = Get( s );
	u_short port = p ? p->remotePort : 0;
	pthread_mutex_unlock( &s_Lock );
	if ( !p ) return Fail( WSAENOTSOCK );
	sockaddr_in *a = (sockaddr_in *)addr;
	ZeroMemory( a, sizeof(*a) );
	a->sin_family = AF_INET; a->sin_port = htons( port ); a->sin_addr.s_addr = htonl( 0x7F000001 );
	if ( len ) *len = sizeof(sockaddr_in);
	return 0;
}
int WINAPI getsockname( SOCKET s, struct sockaddr *addr, int *len ) { return getpeername( s, addr, len ); }

// Name lookups: everything is this computer.
struct hostent *WINAPI gethostbyname( const char *name )
{
	static thread_local struct hostent h;
	static thread_local u_long addr;
	static thread_local char *list[2];
	addr = htonl( 0x7F000001 );
	list[0] = (char *)&addr; list[1] = NULL;
	h.h_name = (char *)"localhost";
	h.h_aliases = NULL;
	h.h_addrtype = AF_INET;
	h.h_length = 4;
	h.h_addr_list = list;
	return &h;
}

unsigned long WINAPI inet_addr( const char *cp )
{
	unsigned a, b, c, d;
	if ( !cp || sscanf( cp, "%u.%u.%u.%u", &a, &b, &c, &d )!=4 ) return INADDR_NONE;
	return htonl( ( a<<24 ) | ( b<<16 ) | ( c<<8 ) | d );
}

char *WINAPI inet_ntoa( struct in_addr in )
{
	static thread_local char buf[16];
	unsigned v = ntohl( in.s_addr );
	sprintf( buf, "%u.%u.%u.%u", v>>24, ( v>>16 ) & 255, ( v>>8 ) & 255, v & 255 );
	return buf;
}

u_short WINAPI htons( u_short v ) { return (u_short)( ( v>>8 ) | ( v<<8 ) ); }
u_short WINAPI ntohs( u_short v ) { return htons( v ); }
u_long WINAPI htonl( u_long v ) { return ( v>>24 ) | ( ( v>>8 ) & 0xFF00 ) | ( ( v<<8 ) & 0xFF0000 ) | ( v<<24 ); }
u_long WINAPI ntohl( u_long v ) { return htonl( v ); }
