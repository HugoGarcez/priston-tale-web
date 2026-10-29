// bridge.js - WebSocket bridge between the browser game and server.exe, for Linux.
//
// Same job as ws.ashx on IIS: browsers cannot open TCP connections, so each game
// connection is a WebSocket to /ws.ashx?port=N; this connects to server.exe and
// passes the bytes both ways (binary messages carry the raw TCP stream).
//
// Settings (environment):
//   PT_BRIDGE_PORT   port this listens on for the web server's proxy (default 8081)
//   PT_GAME_HOST     where server.exe runs (default 127.0.0.1)
//   PT_GAME_PORTS    ports of server.exe the game may ask for (default 10007)
'use strict';

const http = require( 'http' );
const net = require( 'net' );
const { WebSocketServer } = require( 'ws' );

const LISTEN_PORT = parseInt( process.env.PT_BRIDGE_PORT || '8081', 10 );
const GAME_HOST = process.env.PT_GAME_HOST || '127.0.0.1';
const GAME_PORTS = new Set( ( process.env.PT_GAME_PORTS || '10007' ).split( ',' ).map( p => parseInt( p.trim(), 10 ) ) );
// stop reading from server.exe while this much is still waiting to reach the browser
const HIGH_WATER = 1 << 20;

const httpServer = http.createServer( ( req, res ) => {
	res.writeHead( 400, { 'Content-Type': 'text/plain' } );
	res.end( 'Priston Tale game server bridge: WebSocket connections only.' );
} );

const wss = new WebSocketServer( { noServer: true, perMessageDeflate: false } );

httpServer.on( 'upgrade', ( req, socket, head ) => {
	const url = new URL( req.url, 'http://bridge' );
	const port = parseInt( url.searchParams.get( 'port' ), 10 );
	if ( !GAME_PORTS.has( port ) ) {
		socket.end( 'HTTP/1.1 403 Forbidden\r\nConnection: close\r\n\r\n' );
		return;
	}
	wss.handleUpgrade( req, socket, head, ws => bridge( ws, port ) );
} );

function bridge( ws, port ) {
	const tcp = net.connect( { host: GAME_HOST, port: port } );
	tcp.setNoDelay( true );
	let connected = false;
	const pending = [];			// browser bytes that came before the TCP connection was up

	tcp.on( 'connect', () => {
		connected = true;
		for ( const b of pending ) tcp.write( b );
		pending.length = 0;
	} );
	tcp.on( 'data', data => {
		if ( ws.readyState!==ws.OPEN ) return;
		ws.send( data, { binary: true } );
		if ( ws.bufferedAmount>HIGH_WATER ) {
			tcp.pause();
			const resume = setInterval( () => {
				if ( ws.readyState!==ws.OPEN || ws.bufferedAmount<=HIGH_WATER/2 ) { clearInterval( resume ); tcp.resume(); }
			}, 20 );
		}
	} );
	tcp.on( 'error', () => {
		// server.exe not running: the game shows that it cannot connect
		if ( !connected ) ws.close( 1001, 'game server not running' );
	} );
	tcp.on( 'close', () => {
		if ( ws.readyState===ws.OPEN ) ws.close( 1000, '' );
	} );

	ws.on( 'message', data => {
		if ( connected ) tcp.write( data );
		else pending.push( data );
	} );
	ws.on( 'close', () => tcp.destroy() );
	ws.on( 'error', () => tcp.destroy() );
}

httpServer.listen( LISTEN_PORT, () => {
	console.log( `[bridge] listening on ${LISTEN_PORT}, server.exe at ${GAME_HOST}:${[...GAME_PORTS].join( ',' )}` );
} );

for ( const sig of [ 'SIGINT', 'SIGTERM' ] ) process.on( sig, () => process.exit( 0 ) );
