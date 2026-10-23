@echo off
setlocal
set "VS=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
call "%VS%\VC\Auxiliary\Build\vcvars32.bat" >nul
cd /d "%~dp0"
cl /nologo /EHsc /W3 ptgfx_compiletest.cpp /Fe:ptgfx_compiletest.exe /Fo:ptgfx_compiletest.obj > ptgfx_compiletest.log 2>&1
type ptgfx_compiletest.log
