// server_worker.js - runs the offline server in a Web Worker: the same game.js /
// game.wasm as the page, started with Module.ptServer so it behaves like
// server.exe (web\ptwin\src\ptwin_main.cpp). Network bytes to and from the page
// travel as postMessage (web\pre.js, web\ptwin\src\ptwin_net.cpp).
self.Module = {
	ptServer: true,
	print: function (t) { postMessage({ log: '[server] ' + t }); },
	printErr: function (t) { postMessage({ log: '[server] ' + t }); }
};
importScripts('game.js');
