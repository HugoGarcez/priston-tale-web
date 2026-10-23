@echo off
rem Builds and runs the ptsound self-test (silent: buffers play at minimum volume).
setlocal
set "VS=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
call "%VS%\VC\Auxiliary\Build\vcvars32.bat" >nul
cd /d "%~dp0"
set SDL=..\thirdparty\SDL2
cl /nologo /EHsc /W3 /DPT_GFX_GL /I..\ptsound /I%SDL%\include ptsound_selftest.cpp ..\ptsound\ptsound.cpp /link /LIBPATH:%SDL%\lib\x86 SDL2.lib user32.lib /OUT:ptsound_selftest.exe > ptsound_selftest_build.log 2>&1 || (type ptsound_selftest_build.log & exit /b 1)
copy /y %SDL%\lib\x86\SDL2.dll . >nul
ptsound_selftest.exe
