# One-time import of the game data PristonWeb needs, copied (not linked) from
# the v3215 English client and server. The originals are only read.
#
#   PristonWeb\game\          client data (char, Field, image, wav, ...)
#   PristonWeb\game\server\   server data (char, Field, GameServer, ...)
#
# Safe to re-run: robocopy only copies what is missing or changed.

param(
    [string]$Client = "X:\Projects\Pristontale Source\3215\PristonTale_English_Client-v3215",
    [string]$Server = "X:\Projects\Pristontale Source\3215\PristonTale_English_Server-v3215"
)

$Root = $PSScriptRoot
$Game = Join-Path $Root "game"
$GameServer = Join-Path $Game "server"
New-Item -ItemType Directory -Force $Game, $GameServer | Out-Null

function Copy-Tree($From, $To) {
    # /E all subfolders, /XJ never follow links, /R:1 /W:1 short retries
    robocopy $From $To /E /XJ /R:1 /W:1 /NFL /NDL /NJH /NP /XF Thumbs.db | Select-Object -Last 4
    if ($LASTEXITCODE -ge 8) { throw "copy failed: $From" }
}

foreach ($d in "char", "cSelect", "Effect", "Field", "image", "rain", "sky", "StartImage", "wav", "weapons") {
    Write-Host "client $d"
    Copy-Tree (Join-Path $Client $d) (Join-Path $Game $d)
}
foreach ($f in "shortcut.ini", "setfile.lst", "AssaLoadingFile.txt") {
    $src = Join-Path $Client $f
    if (Test-Path $src) { Copy-Item $src $Game -Force }
}

foreach ($d in "char", "Field", "GameServer", "Bellatra", "FuncBox", "PostBox") {
    Write-Host "server $d"
    Copy-Tree (Join-Path $Server $d) (Join-Path $GameServer $d)
}
foreach ($f in "BlessCastle.dat", "Top10.ini") {
    $src = Join-Path $Server $f
    if (Test-Path $src) { Copy-Item $src $GameServer -Force }
}

Write-Host "Game data ready in $Game"
