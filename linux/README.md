# Running on a Linux server

The same world as the Windows/IIS setup, with Docker on Linux in its place:

```
Player's browser  <== https / wss ==>  Caddy (web)  ==>  bridge (/ws.ashx)  <== 127.0.0.1:10007 ==>  server.exe under Wine
```

| Windows | Linux (this folder) |
|---|---|
| IIS, `web.config`, `data/web.config` | Caddy, `caddy/Caddyfile` |
| `ws.ashx` (ASP.NET) | `bridge/bridge.js` (Node.js) |
| `server.exe` | `server.exe` under 32-bit Wine with a virtual display (`server/`) |
| win-acme certificates | Caddy gets and renews HTTPS certificates by itself |
| `install_autostart.ps1` | `restart: unless-stopped` |

`config.js` stays as it is (`serverUrl: 'ws.ashx'`): the bridge answers at the same address.

## What you need

- A Linux server on **x86-64** (Ubuntu 22.04/24.04, Debian 12, ...). ARM servers can't run the 32-bit `server.exe`.
- **Docker** with the compose plugin: `curl -fsSL https://get.docker.com | sh`
- **About 12 GB of disk**: the repo (~9 GB) plus the Wine image.
- **A domain name** pointed at the server, so friends can play over HTTPS.

## Setup

```
git clone https://github.com/HugoGarcez/priston-tale-web.git
cd priston-tale-web/linux
cp .env.example .env
nano .env                      # PT_SITE=play.example.com
docker compose up -d --build
```

Give `server.exe` **1–2 minutes** to load the world, then open `https://play.example.com/`.
Type any ID and password: the first login creates the account.

Open ports **80** and **443** in the firewall (`ufw allow 80,443/tcp`). Port 10007 stays inside Docker: don't open it.

## Running your world

- **Logs:** `docker compose logs -f server` (or `bridge`, `web`)
- **Stop / start:** `docker compose stop` closes `server.exe` the normal way, so online characters are saved (it waits up to 2 minutes). `docker compose start` brings it back.
- **Back up** `[project]/dist/server/DataServer`. It holds every account and character.
- **Forgotten password:** delete `DataServer/password/<ID>.pw`.
- **Rates and settings:** edit `[project]/dist/server/server.ini`, then `docker compose restart server`.
- **Update:** `git pull && docker compose up -d --build`

## If something goes wrong

Add `?debugnet` to the address to log every connection attempt under the game.

| What you see | What to do |
|---|---|
| Login can't connect to the server | Is `server` up and has it had 2 minutes? `docker compose logs server`. |
| Black page or "SharedArrayBuffer" error | You're on `http://` from another PC: set `PT_SITE` to your domain to get HTTPS. |
| `/ws.ashx` shows 502 instead of "WebSocket connections only" | The bridge isn't running: `docker compose logs bridge`. |
| HTTPS certificate fails | The domain must point at this server and ports 80/443 must be open. `docker compose logs web`. |

Testing without Docker: `node bridge/bridge.js` (after `npm ci` in `bridge/`) plus any web server that sends the headers in `caddy/Caddyfile` and proxies `/ws.ashx` to port 8081.
