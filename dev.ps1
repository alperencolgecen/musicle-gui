$cmakePath = "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
$exePath = "build\Debug\MusicLe.exe"
$srcPath = "src"

function BuildAndRun {
    Write-Output "--- Change detected. Rebuilding... ---"
    & $cmakePath --build build --config Debug
    if ($LASTEXITCODE -eq 0) {
        Write-Output "Build success. Restarting app..."
        # Wait a bit to ensure the file is not locked or previous process is gone
        Start-Sleep -Milliseconds 500
        Get-Process "MusicLe" -ErrorAction SilentlyContinue | Stop-Process -Force
        Start-Process $exePath
    } else {
        Write-Output "Build failed. Fix errors to restart."
    }
}

# Initial run
BuildAndRun

$watcher = New-Object System.IO.FileSystemWatcher
$watcher.Path = (Get-Item $srcPath).FullName
$watcher.IncludeSubdirectories = $true
$watcher.EnableRaisingEvents = $true

Write-Output "Watching $srcPath for changes. Press Ctrl+C to stop."

while ($true) {
    $result = $watcher.WaitForChanged([System.IO.WatcherChangeTypes]::All, 500)
    if ($result.TimedOut -eq $false) {
        # Simple debounce
        Start-Sleep -Milliseconds 200
        BuildAndRun
    }
}
