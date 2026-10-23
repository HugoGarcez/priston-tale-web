@echo off
rem Builds and runs the hidden-window ptgfx self-test (no window is shown).
setlocal
set "VS=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
call "%VS%\VC\Auxiliary\Build\vcvars32.bat" >nul
cd /d "%~dp0"
cl /nologo /EHsc /W3 /DPT_GFX_GL /I..\ptgfx ptgfx_selftest.cpp ..\ptgfx\ptgl.cpp ..\ptgfx\ptgfx_surface.cpp ..\ptgfx\ptgfx_device.cpp ..\ptgfx\ptgfx_text.cpp user32.lib gdi32.lib dxguid.lib /Fe:ptgfx_selftest.exe /Fo:.\ > ptgfx_selftest_build.log 2>&1 || (type ptgfx_selftest_build.log & exit /b 1)
