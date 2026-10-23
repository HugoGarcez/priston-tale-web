@echo off
rem Builds test_launcher.exe into ..\..\game\ and runs it there (no game window):
rem starts and stops the offline server the way game.exe does.
setlocal
set "VS=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
call "%VS%\VC\Auxiliary\Build\vcvars32.bat" >nul
cd /d "%~dp0"
cl /nologo /EHsc /I.. test_launcher.cpp ..\OfflineServer.cpp ws2_32.lib user32.lib /Fe:..\..\game\test_launcher.exe /Fo:.\ > test_launcher_build.log 2>&1 || (type test_launcher_build.log & exit /b 1)
cd /d "%~dp0..\..\game"
test_launcher.exe
type offline.log
