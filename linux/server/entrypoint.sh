#!/bin/sh
# Starts server.exe under Wine and shuts it down the normal way on `docker stop`:
# taskkill without /f closes its window (WM_CLOSE), like closing it on Windows,
# so characters of players still online are saved.
set -e

cd /srv/server
if [ ! -f server.exe ]; then
	echo "server.exe not found in /srv/server (mount [project]/dist/server there)" >&2
	exit 1
fi

rm -f /tmp/.X99-lock
Xvfb :99 -screen 0 1024x768x16 -nolisten tcp &
XVFB=$!

wine server.exe &
SERVER=$!

stop() {
	echo "[entrypoint] closing server.exe (saving characters)..."
	wine taskkill /im server.exe >/dev/null 2>&1 || true
	wait $SERVER 2>/dev/null || true
	wineserver -w
	kill $XVFB 2>/dev/null || true
	exit 0
}
trap stop TERM INT

echo "[entrypoint] server.exe started, give it 1-2 minutes to load the world"
wait $SERVER
STATUS=$?
kill $XVFB 2>/dev/null || true
exit $STATUS
