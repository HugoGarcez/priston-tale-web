@echo off
rem Builds and runs the SDL platform-layer self-test (hidden window, nothing is shown).
setlocal
set "VS=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
call "%VS%\VC\Auxiliary\Build\vcvars32.bat" >nul
cd /d "%~dp0"
if not exist platformtest mkdir platformtest
set SDL=..\thirdparty\SDL2
cl /nologo /EHsc /W3 /DPT_GFX_GL /DPT_USE_SDL /I..\ptgfx /I%SDL%\include ptplatform_selftest.cpp ..\ptplatform\ptplatform.cpp ..\ptgfx\ptgl.cpp /Fo:platformtest\ /link /LIBPATH:%SDL%\lib\x86 SDL2.lib user32.lib gdi32.lib /OUT:platformtest\ptplatform_selftest.exe > ptplatform_selftest_build.log 2>&1 || (type ptplatform_selftest_build.log & exit /b 1)
copy /y %SDL%\lib\x86\SDL2.dll platformtest >nul
cd platformtest
ptplatform_selftest.exe
