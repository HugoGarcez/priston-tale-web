@echo off
rem Builds PristonTale with Visual Studio 2022.
rem Usage: build.bat [Debug|Release] [DX6|GL]   (defaults: Debug DX6)
rem   DX6 = original DirectDraw/Direct3D 6 renderer
rem   GL  = ptgfx OpenGL renderer (work in progress, see ROADMAP.md)
rem Output: bin\<Config><Renderer>\game.exe and server.exe
setlocal
set CONFIG=%1
if "%CONFIG%"=="" set CONFIG=Debug
set GFX=%2
if "%GFX%"=="" set GFX=DX6
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -find MSBuild\**\Bin\MSBuild.exe`) do set MSBUILD=%%i
"%MSBUILD%" "%~dp0PristonTale.sln" /p:Configuration=%CONFIG% /p:Platform=x86 /p:PtGfx=%GFX% /m /nologo /v:m "/flp:logfile=%~dp0build.log;verbosity=minimal"
