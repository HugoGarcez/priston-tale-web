// Site settings for the browser game (copied next to game.html).
//
// serverUrl: where the game server is.
//   ''          - solo: every player runs their own offline server inside the browser,
//                 progress is kept in the browser (Export/Import saves buttons).
//   'ws.ashx'   - shared: everyone plays on the server.exe you run; the browser talks to
//                 it through the WebSocket bridge ws.ashx on this site (IIS), or
//                 web\serve.py when testing locally.
window.PT_CONFIG = {
	serverUrl: 'ws.ashx'
};
