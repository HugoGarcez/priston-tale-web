# Builds the Debug server, runs it in play\server and reports whether it starts
# listening; on a crash prints where (via crashinfo.py).
param([int]$Port = 10007, [int]$Wait = 240, [int]$Linger = 60, [switch]$NoBuild)
$d = Split-Path $PSScriptRoot
$s = Join-Path (Split-Path $d -Parent) "game\server"
if (-not $NoBuild) {
    & "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "$d\PristonServer.vcxproj" /p:Configuration=Debug /p:Platform=Win32 /m /nologo /v:m |
        Select-String " error " | ForEach-Object { ($_.Line -replace '\s*\[X:.*$','') -replace '^.*\\','' }
}
Remove-Item "$s\server_debug.exe.dmp", "$s\missing_models.txt" -Force -ErrorAction SilentlyContinue
Copy-Item "$d\bin\Debug\server.exe" "$s\server_debug.exe" -Force
$p = Start-Process "$s\server_debug.exe" -WorkingDirectory $s -WindowStyle Minimized -PassThru
$open = $false
for ($i = 0; $i -lt $Wait; $i++) {
    Start-Sleep 1
    if ($p.HasExited) { break }
    $c = New-Object Net.Sockets.TcpClient
    try { $c.Connect("127.0.0.1", $Port); $open = $true; $c.Close(); break } catch {}
}
"server exited: $($p.HasExited)  port open: $open  after ${i}s"
# keep it running a while: it loads maps after opening the port
if ($open -and $Linger -gt 0) {
    $p.WaitForExit($Linger * 1000) | Out-Null
    "after ${Linger}s more: still running = $(-not $p.HasExited)"
}
if (-not $p.HasExited) { Stop-Process -Id $p.Id -Force }
if (Test-Path "$s\server_debug.exe.dmp") {
    python -I "$PSScriptRoot\crashinfo.py" "$s\server_debug.exe.dmp" "$d\bin\Debug\server.map" "$s\server_debug.exe" | Select-Object -First 14
}
if (Test-Path "$s\missing_models.txt") { "--- missing_models.txt:"; Get-Content "$s\missing_models.txt" | Select-Object -First 20 }
