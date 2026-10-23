@echo off
rem Toolchain check: builds web\hello\hello.cpp into web\out\hello.html
setlocal
call "%~dp0..\..\emsdk\emsdk_env.bat" >nul 2>nul
cd /d "%~dp0"
if not exist out mkdir out
em++ hello\hello.cpp -O2 -sUSE_SDL=2 -sMAX_WEBGL_VERSION=2 -sMIN_WEBGL_VERSION=2 -sFULL_ES3 -sASYNCIFY -pthread -sPTHREAD_POOL_SIZE=2 -o out\hello.html
