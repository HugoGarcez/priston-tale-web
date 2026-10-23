# install_autostart.ps1 - keep server.exe running: starts it when Windows starts
# (no sign-in needed) and starts it again within 5 minutes if it ever stops.
#
# Run once in an ADMIN PowerShell:
#   powershell -ExecutionPolicy Bypass -File install_autostart.ps1
#
# It runs as SYSTEM in the background, so there is no server window any more.
#   Stop the world:     Disable-ScheduledTask PristonServer; Stop-ScheduledTask PristonServer
#   Start it again:     Enable-ScheduledTask PristonServer; Start-ScheduledTask PristonServer
#   Remove autostart:   Unregister-ScheduledTask PristonServer -Confirm:$false
# (Stop-ScheduledTask ends server.exe without its own shutdown, like a power cut:
#  characters of players still online lose what they did since their last save.)

$ErrorActionPreference = 'Stop'
$TaskName = 'PristonServer'
$dir = Split-Path -Parent $MyInvocation.MyCommand.Path
$exe = Join-Path $dir 'server.exe'

if ( -not ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator) ) {
	throw 'Run this from an admin PowerShell (right-click > Run as administrator).'
}
if ( -not [IO.File]::Exists($exe) ) { throw "server.exe not found next to this script ($exe)" }

# a server.exe started by hand would hold port 10007: close it the normal way
# (like closing its window, so characters are saved) and wait for it to finish
foreach ( $p in Get-Process server -ErrorAction SilentlyContinue | Where-Object { $_.Path -eq $exe } ) {
	Write-Host "Closing the running server.exe (pid $($p.Id)) so the task can take over..."
	if ( $p.CloseMainWindow() ) { $p.WaitForExit(120000) | Out-Null }
	if ( -not $p.HasExited ) { throw "server.exe (pid $($p.Id)) did not close - close its window, then run this again." }
}

$action = New-ScheduledTaskAction -Execute $exe -WorkingDirectory $dir
# at start-up, and every 5 minutes: while server.exe is running the new start is
# skipped (IgnoreNew), so this only restarts it after it stopped
$atBoot = New-ScheduledTaskTrigger -AtStartup
$every5 = New-ScheduledTaskTrigger -Once -At (Get-Date).Date -RepetitionInterval (New-TimeSpan -Minutes 5) -RepetitionDuration (New-TimeSpan -Days 3650)
$settings = New-ScheduledTaskSettingsSet -MultipleInstances IgnoreNew -ExecutionTimeLimit ([TimeSpan]::Zero) `
	-AllowStartIfOnBatteries -DontStopIfGoingOnBatteries -StartWhenAvailable -Priority 4
$principal = New-ScheduledTaskPrincipal -UserId 'SYSTEM' -LogonType ServiceAccount -RunLevel Highest

Register-ScheduledTask -TaskName $TaskName -Action $action -Trigger $atBoot, $every5 -Settings $settings `
	-Principal $principal -Description 'Priston Tale game server for online.priston.world (server.exe on port 10007)' -Force | Out-Null
Start-ScheduledTask -TaskName $TaskName

Write-Host "Task '$TaskName' registered and started. The world takes about a minute to load."
for ( $i = 0; $i -lt 24; $i++ ) {
	Start-Sleep 5
	if ( Get-NetTCPConnection -LocalPort 10007 -State Listen -ErrorAction SilentlyContinue ) { Write-Host 'server.exe is listening on port 10007.'; return }
}
Write-Warning 'server.exe is not listening on port 10007 yet - check LogFile\ and Task Scheduler > PristonServer > History.'
