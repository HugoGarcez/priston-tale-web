Priston Tale in the browser - shared server
===========================================

Everyone who opens the website plays in one world: the game runs in the browser,
the world runs in server.exe on your Windows server. The browser cannot talk to
server.exe directly, so the website includes a small bridge (ws.ashx) that IIS
runs: browser <-WebSocket-> IIS (ws.ashx) <-TCP 127.0.0.1:10007-> server.exe.

  site\     the website: copy everything into your IIS site folder
  server\   the game server: copy anywhere on the same computer, run server.exe


1. IIS features (once)
----------------------
Server Manager > Add Roles and Features > Web Server (IIS) > Web Server >
  - Application Development > WebSocket Protocol
  - Application Development > ASP.NET 4.8 (also adds .NET Extensibility 4.8, ISAPI)
(Windows 10/11: "Turn Windows features on or off" > Internet Information Services >
 World Wide Web Services > Application Development Features: the same two.)

2. The website
--------------
- Create a site (or use an existing one) and copy the CONTENTS of site\ into its
  folder: index.html, game.js, game.wasm, ..., data\, web.config, ws.ashx, config.js.
- Application pool: ".NET CLR Version v4.0", Managed pipeline mode "Integrated"
  (the defaults).
- HTTPS is REQUIRED. Browsers only allow the game's threads on secure pages
  (https://, or http://localhost on the server itself). Add an https binding with
  a certificate - for example a free Let's Encrypt certificate with win-acme
  (https://www.win-acme.com).
- The data\ folder is about 4 GB. Players download what they need while playing;
  the first visit downloads ~95 MB before the login screen.

3. The game server
------------------
- Copy server\ anywhere on the same computer (for example C:\PristonServer) and run
  server.exe. Leave it running; close it to shut the world down (characters are
  saved when players log out and when the server closes).
- Port 10007 is only used inside the computer (IIS -> server.exe). Do not open it
  in the firewall.
- Accounts: there is no account database. The first login to an ID creates it and
  sets its password (server.ini *OFFLINE_PASSWORDS); later logins need the same
  password. To reset a password delete server\DataServer\password\<ID>.pw.
- Back up server\DataServer (accounts and characters) now and then.
- Settings: server\server.ini (experience/drop rates are listed there).

4. Check it works
-----------------
- https://your-site/ws.ashx in a browser should show "Priston Tale game server
  bridge: WebSocket connections only." If you get an error page instead, ASP.NET
  4.8 is missing (step 1) or the application pool is not .NET v4.0.
- https://your-site/ should show the intro and the login screen. "Unable to connect
  to server" after Login means server.exe is not running, or the WebSocket Protocol
  feature is missing.
- Press F12 in the browser: the page's log (under the game) and the console show
  what failed.

Updating
--------
Run python web\make_site.py again on the build computer: copy site\ over the IIS
folder (config.js is kept) and replace server\server.exe (server.ini and
DataServer are kept). Stop server.exe first.

Players' browsers keep the game files (sw.js, the service worker): the next visit
only downloads files that changed. data_startup.pack is compared with the server
by date; every file under data\ by the hash in data_manifest.txt. So if you change
files in the IIS data\ folder by hand, make the list again or players keep their
old copies:
  python web\make_manifest.py <IIS site>\data <IIS site>\data_manifest.txt

Solo instead of shared
----------------------
config.js: serverUrl: '' makes every visitor run their own private world inside
the browser (no server.exe; progress saved in the browser, Export/Import buttons).
That needs data_server.pack and the server data under data\server\ - make_site.py
does not include them; ask for a solo package if you want that.
