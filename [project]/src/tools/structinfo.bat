@echo off
rem Builds and runs structinfo.exe (prints model struct layouts).
setlocal
set "VS=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
call "%VS%\VC\Auxiliary\Build\vcvars32.bat" >nul
cd /d "%~dp0"
cl /nologo /EHsc /I.. /I..\smLib3d /FI..\compat.h /source-charset:utf-8 /execution-charset:.949 /D_CRT_SECURE_NO_WARNINGS /DDIRECT3D_VERSION=0x0600 /DDIRECTDRAW_VERSION=0x0600 structinfo.cpp /Fe:structinfo.exe /Fo:structinfo.obj > structinfo_build.log 2>&1 || (type structinfo_build.log & exit /b 1)
structinfo.exe
