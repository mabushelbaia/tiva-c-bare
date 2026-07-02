# Minimal serial monitor for the TM4C123 LaunchPad's ICDI virtual COM port.
# Usage: pwsh scripts/monitor.ps1 [-Port COM12] [-Baud 115200]
# If -Port is omitted, auto-detects the LaunchPad's ICDI virtual COM port.
param(
    [string]$Port,
    [int]$Baud = 115200
)

if (-not $Port) {
    $candidates = @(Get-CimInstance Win32_PnPEntity |
        Where-Object { $_.Name -match 'Stellaris.*\(COM(\d+)\)|ICDI.*\(COM(\d+)\)' })

    if ($candidates.Count -eq 0) {
        Write-Error "No Stellaris/ICDI virtual COM port found. Is the LaunchPad plugged in? Pass -Port explicitly to override."
        exit 1
    }
    if ($candidates.Count -gt 1) {
        Write-Warning "Multiple matching devices found - using the first one:"
        $candidates | ForEach-Object { Write-Warning "  $($_.Name)" }
    }

    $null = $candidates[0].Name -match '\((COM\d+)\)'
    $Port = $Matches[1]
}

$serial = New-Object System.IO.Ports.SerialPort $Port, $Baud, ([System.IO.Ports.Parity]::None), 8, ([System.IO.Ports.StopBits]::One)
$serial.ReadTimeout = 200
$serial.Open()

Write-Host "Listening on $Port @ $Baud baud. Ctrl+C to exit." -ForegroundColor Cyan

try {
    while ($true) {
        try {
            Write-Host -NoNewline $serial.ReadExisting()
        } catch [System.TimeoutException] {
            # no data within the timeout - just loop and check again
        }
        Start-Sleep -Milliseconds 20
    }
} finally {
    $serial.Close()
}
