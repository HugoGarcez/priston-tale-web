# Copies a fresh build into the playable game folder:
#
#   ..\game\game.exe + hotuk.ini              (game data: run ..\import_v3215_data.ps1 once)
#   ..\game\server\server.exe + server.ini
#
# Usage:  .\install_build.ps1 [-Config Release] [-Gfx GL|DX6]
# Saves (game\server\DataServer) and settings already in the game folder are kept.

param(
    [string]$Config = "Release",
    [string]$Gfx = "GL"           # GL = OpenGL renderer, DX6 = original DirectX 6
)

$ErrorActionPreference = "Stop"
$Src = $PSScriptRoot
$Game = Join-Path (Split-Path $Src -Parent) "game"
$GameServer = Join-Path $Game "server"
if (-not (Test-Path (Join-Path $Game "char"))) {
    throw "No game data in $Game - run ..\import_v3215_data.ps1 first"
}
New-Item -ItemType Directory -Force $GameServer | Out-Null

Copy-Item (Join-Path $Src "bin\$Config$Gfx\game.exe") $Game -Force
Copy-Item (Join-Path $Src "bin\$Config$Gfx\server.exe") $GameServer -Force
if ($Gfx -eq "GL") {
    # GL build: sound (and soon window/input) through SDL2
    Copy-Item (Join-Path $Src "thirdparty\SDL2\lib\x86\SDL2.dll") $Game -Force
}
if (-not (Test-Path (Join-Path $Game "hotuk.ini"))) {
    Copy-Item (Join-Path $Src "offline\hotuk.ini") $Game
}
if (-not (Test-Path (Join-Path $GameServer "server.ini"))) {
    Copy-Item (Join-Path $Src "offline\server.ini") $GameServer
}

Write-Host "Installed $Config $Gfx build into $Game"
Write-Host "Run: $Game\game.exe"
