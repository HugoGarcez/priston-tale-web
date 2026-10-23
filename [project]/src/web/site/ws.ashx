<%@ WebHandler Language="C#" Class="PristonBridge" %>
// ws.ashx - WebSocket bridge between the browser game and server.exe (shared server).
//
// Browsers cannot open TCP connections, so each game connection is a WebSocket to
// this handler; it connects to server.exe on this computer and passes the bytes both
// ways (binary messages carry the raw TCP stream). config.js: serverUrl 'ws.ashx'.
// The port the game asks for is checked against web.config appSettings
// "PristonGamePorts" (default 10007); server.exe is always reached on 127.0.0.1.
//
// Needs the IIS features "WebSocket Protocol" and "ASP.NET 4.8" (see README.txt).
using System;
using System.Configuration;
using System.Net.Sockets;
using System.Net.WebSockets;
using System.Threading;
using System.Threading.Tasks;
using System.Web;
using System.Web.WebSockets;

public class PristonBridge : IHttpHandler
{
	public bool IsReusable { get { return true; } }

	public void ProcessRequest( HttpContext context )
	{
		if ( !context.IsWebSocketRequest ) {
			context.Response.StatusCode = 400;
			context.Response.Write( "Priston Tale game server bridge: WebSocket connections only." );
			return;
		}
		int port;
		if ( !int.TryParse( context.Request.QueryString["port"], out port ) || !PortAllowed( port ) ) {
			context.Response.StatusCode = 403;
			return;
		}
		context.AcceptWebSocketRequest( ws => Bridge( ws.WebSocket, port ) );
	}

	static bool PortAllowed( int port )
	{
		string list = ConfigurationManager.AppSettings["PristonGamePorts"] ?? "10007";
		foreach ( string p in list.Split( ',' ) ) {
			int n;
			if ( int.TryParse( p.Trim(), out n ) && n==port ) return true;
		}
		return false;
	}

	static async Task Bridge( WebSocket ws, int port )
	{
		using ( TcpClient tcp = new TcpClient() ) {
			bool connected = true;
			try {
				tcp.NoDelay = true;
				await tcp.ConnectAsync( "127.0.0.1", port );
			}
			catch ( Exception ) { connected = false; }
			if ( !connected ) {
				// server.exe not running: the game shows that it cannot connect
				await CloseQuietly( ws, WebSocketCloseStatus.EndpointUnavailable, "game server not running" );
				return;
			}
			NetworkStream stream = tcp.GetStream();
			CancellationTokenSource stop = new CancellationTokenSource();

			Task browserToServer = Task.Run( async () => {
				byte[] buf = new byte[65536];
				try {
					while ( ws.State==WebSocketState.Open ) {
						WebSocketReceiveResult r = await ws.ReceiveAsync( new ArraySegment<byte>( buf ), stop.Token );
						if ( r.MessageType==WebSocketMessageType.Close ) break;
						if ( r.Count>0 ) await stream.WriteAsync( buf, 0, r.Count, stop.Token );
					}
				}
				catch ( Exception ) { }
			} );

			Task serverToBrowser = Task.Run( async () => {
				byte[] buf = new byte[65536];
				try {
					while ( true ) {
						int n = await stream.ReadAsync( buf, 0, buf.Length, stop.Token );
						if ( n<=0 ) break;
						await ws.SendAsync( new ArraySegment<byte>( buf, 0, n ), WebSocketMessageType.Binary, true, stop.Token );
					}
				}
				catch ( Exception ) { }
			} );

			await Task.WhenAny( browserToServer, serverToBrowser );
			stop.Cancel();
			try { tcp.Client.Shutdown( SocketShutdown.Both ); } catch ( Exception ) { }
			await CloseQuietly( ws, WebSocketCloseStatus.NormalClosure, "" );
		}
	}

	static async Task CloseQuietly( WebSocket ws, WebSocketCloseStatus status, string reason )
	{
		try {
			if ( ws.State==WebSocketState.Open || ws.State==WebSocketState.CloseReceived )
				await ws.CloseAsync( status, reason, CancellationToken.None );
		}
		catch ( Exception ) { }
	}
}
