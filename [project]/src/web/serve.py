"""Local web server for the browser build.

Serves web\\out (the page, game.js, game.wasm, data_manifest.txt) and the game
data folder PristonWeb\\game under /data/. Sends the two headers browsers require
before they allow threads (SharedArrayBuffer): Cross-Origin-Opener-Policy and
Cross-Origin-Embedder-Policy.

Usage: python web\\serve.py [port] [folder]     (defaults: 8080, web\\out)
Then open http://localhost:8080/
A folder made by web\\make_site.py (with its own data\\ folder) can be served too.

/ws.ashx is the same WebSocket bridge as the IIS one (web\\site\\ws.ashx): with
config.js serverUrl 'ws.ashx' the game connects through it to server.exe on this
computer (ports in PT_GAME_PORTS, default 10007).
"""
import base64, hashlib, http.server, os, socket, struct, sys, threading, time, urllib.parse

HERE = os.path.dirname(os.path.abspath(__file__))
port = int(sys.argv[1]) if len(sys.argv) > 1 else 8080
root = sys.argv[2] if len(sys.argv) > 2 else os.path.join(HERE, 'out')
data = os.path.join(root, 'data') if os.path.isdir(os.path.join(root, 'data')) else \
       os.path.join(os.path.dirname(os.path.dirname(HERE)), 'game')
GAME_PORTS = set(int(p) for p in os.environ.get('PT_GAME_PORTS', '10007').split(','))

# Order in which the game asks for data files (input for web\make_pack.py, which
# bundles the start-up files into one download).
ACCESS_LOG = os.path.join(root, 'data_access.log')
_seen = set()
_lock = threading.Lock()

REQUEST_LOG = os.path.join(root, 'data_requests.log')     # every request, with time (progress/debugging)

def record_access(rel):
    rel = rel.replace('\\', '/')
    with _lock:
        with open(REQUEST_LOG, 'a', encoding='utf-8') as fp:
            fp.write('%s %s\n' % (time.strftime('%H:%M:%S'), rel))
        if rel in _seen:
            return
        _seen.add(rel)
        with open(ACCESS_LOG, 'a', encoding='utf-8') as fp:
            fp.write(rel + '\n')

class Handler(http.server.SimpleHTTPRequestHandler):
    protocol_version = 'HTTP/1.1'          # keep connections open: the game fetches thousands of files
    extensions_map = {**http.server.SimpleHTTPRequestHandler.extensions_map, '.wasm': 'application/wasm', '.js': 'text/javascript'}

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=root, **kwargs)

    def translate_path(self, path):
        p = urllib.parse.unquote(urllib.parse.urlsplit(path).path)
        if p.startswith('/data/'):
            rel = os.path.normpath(p[len('/data/'):]).lstrip('\\/')
            if rel.startswith('..'):
                return os.path.join(data, '__invalid__')
            record_access(rel)
            return os.path.join(data, rel)
        return super().translate_path(path)

    def do_GET(self):
        if urllib.parse.urlsplit(self.path).path.lower() == '/ws.ashx':
            return self.bridge()
        return super().do_GET()

    # WebSocket <-> TCP to server.exe (binary messages carry the raw stream)
    def bridge(self):
        if os.environ.get('PT_BRIDGE_DEBUG'):
            sys.stderr.write('bridge request:\n%s\n' % self.headers)
        q = urllib.parse.parse_qs(urllib.parse.urlsplit(self.path).query)
        key = self.headers.get('Sec-WebSocket-Key')
        try:
            game_port = int(q.get('port', ['0'])[0])
        except ValueError:
            game_port = 0
        if not key or self.headers.get('Upgrade', '').lower() != 'websocket':
            self.send_error(400, 'WebSocket only'); return
        if game_port not in GAME_PORTS:
            self.send_error(403, 'port not allowed'); return
        try:
            tcp = socket.create_connection(('127.0.0.1', game_port), timeout=5)
            tcp.settimeout(None)
        except OSError:
            self.send_error(502, 'game server not running'); return
        accept = base64.b64encode(hashlib.sha1((key + '258EAFA5-E914-47DA-95CA-C5AB0DC11B65').encode()).digest()).decode()
        self.send_response(101, 'Switching Protocols')
        self.send_header('Upgrade', 'websocket')
        self.send_header('Connection', 'Upgrade')
        self.send_header('Sec-WebSocket-Accept', accept)
        super().end_headers()
        self.close_connection = True
        ws = self.connection
        ws.settimeout(None)
        lock = threading.Lock()

        def send_frame(op, payload):
            n = len(payload)
            hdr = bytes([0x80 | op]) + (bytes([n]) if n < 126 else bytes([126]) + struct.pack('>H', n) if n < 65536
                                         else bytes([127]) + struct.pack('>Q', n))
            with lock:
                ws.sendall(hdr + payload)

        def tcp_to_ws():
            try:
                while True:
                    d = tcp.recv(65536)
                    if not d: break
                    send_frame(2, d)
            except OSError:
                pass
            try: send_frame(8, b'')
            except OSError: pass
            try: ws.shutdown(socket.SHUT_RDWR)
            except OSError: pass

        t = threading.Thread(target=tcp_to_ws, daemon=True)
        t.start()
        rf = self.rfile
        try:
            while True:
                h = rf.read(2)
                if len(h) < 2: break
                op, n = h[0] & 0x0F, h[1] & 0x7F
                if n == 126: n = struct.unpack('>H', rf.read(2))[0]
                elif n == 127: n = struct.unpack('>Q', rf.read(8))[0]
                mask = rf.read(4) if h[1] & 0x80 else b'\0\0\0\0'
                p = bytearray(rf.read(n))
                for i in range(len(p)): p[i] ^= mask[i & 3]
                if op == 8: break
                if op == 9: send_frame(10, bytes(p)); continue
                if op in (0, 1, 2) and p: tcp.sendall(p)
        except OSError:
            pass
        try: tcp.shutdown(socket.SHUT_RDWR)
        except OSError: pass
        tcp.close()
        t.join(5)

    def end_headers(self):
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        self.send_header('Cross-Origin-Resource-Policy', 'same-origin')
        self.send_header('Cache-Control', 'no-cache')
        super().end_headers()

    def log_message(self, fmt, *args):
        if '/data/' not in (str(args[0]) if args else ''):
            super().log_message(fmt, *args)

class LoopbackServer(http.server.ThreadingHTTPServer):
    daemon_threads = True

class LoopbackServer6(LoopbackServer):
    address_family = socket.AF_INET6

# PT_TLS_CERT / PT_TLS_KEY (PEM files): serve https:// and wss:// - for testing the
# shared server like the real site (some security software breaks plain ws://)
tls = None
if os.environ.get('PT_TLS_CERT'):
    import ssl
    tls = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    tls.load_cert_chain(os.environ['PT_TLS_CERT'], os.environ.get('PT_TLS_KEY'))

def start(srv):
    if tls:
        srv.socket = tls.wrap_socket(srv.socket, server_side=True)
    return srv

# "localhost" may mean ::1 or 127.0.0.1: answer on both (this computer only)
print('serving %s (+ game data %s at /data/) on %s://localhost:%d/' % (root, data, 'https' if tls else 'http', port))
try:
    s6 = start(LoopbackServer6(('::1', port), Handler))
    threading.Thread(target=s6.serve_forever, daemon=True).start()
except OSError:
    pass
start(LoopbackServer(('127.0.0.1', port), Handler)).serve_forever()
