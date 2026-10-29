# server.exe on Linux: 32-bit Wine with a virtual display (Xvfb), since server.exe
# is a Windows program with a window. The server folder ([project]/dist/server) is
# mounted at /srv/server, so accounts (DataServer/) stay on the host.

# ptquit.exe: closes server.exe the way its File > Quit menu does (see ptquit.c)
FROM debian:bookworm-slim AS ptquit
RUN apt-get update \
	&& apt-get install -y --no-install-recommends gcc-mingw-w64-i686 \
	&& rm -rf /var/lib/apt/lists/*
COPY ptquit.c /src/ptquit.c
RUN i686-w64-mingw32-gcc -O2 -s -o /src/ptquit.exe /src/ptquit.c

FROM debian:bookworm-slim

RUN dpkg --add-architecture i386 \
	&& apt-get update \
	&& apt-get install -y --no-install-recommends wine wine32 xvfb xauth procps ca-certificates \
	&& rm -rf /var/lib/apt/lists/*

ENV WINEPREFIX=/opt/wine \
	WINEARCH=win32 \
	WINEDEBUG=-all \
	DISPLAY=:99

# create the Wine prefix now, so the container does not spend a minute on it at every start
RUN Xvfb :99 -screen 0 1024x768x16 -nolisten tcp & \
	wineboot --init && wineserver -w; \
	kill %1 2>/dev/null; true

COPY --from=ptquit /src/ptquit.exe /opt/ptquit.exe
COPY entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh

WORKDIR /srv/server
EXPOSE 10007
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
