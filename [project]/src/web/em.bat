@echo off
rem Runs em++ with the Emscripten SDK environment: web\em.bat <em++ arguments>
call "%~dp0..\..\emsdk\emsdk_env.bat" >nul 2>nul
em++ %*
