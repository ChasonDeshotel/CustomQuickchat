function Get-UnicodeChar($codepoint) {
    return [char]::ConvertFromUtf32($codepoint)
}

function Get-CMakeVersion {
    $version = cmake --version 2>$null | Select-String -Pattern "cmake version (\d+\.\d+\.\d+)" | ForEach-Object { $_.Matches.Groups[1].Value }
    if (-not [string]::IsNullOrEmpty($version)) {
        Write-Host "$(Get-UnicodeChar 0x1F50D) CMake is installed: Version $version"
        return $version
    }
    Write-Host "$(Get-UnicodeChar 0x26A0) CMake is not installed or not found in PATH." -ForegroundColor Yellow
    return "0.0.0"
}

function Get-LatestCMakeVersion {
    $CMakeBaseUrl = "https://cmake.org/files/LatestRelease/"
    $CMakeLatestVersion = Invoke-WebRequest -Uri $CMakeBaseUrl -UseBasicParsing |
        Select-String -Pattern "cmake-(\d+\.\d+\.\d+)-windows-x86_64.msi" |
        ForEach-Object { $_.Matches.Groups[1].Value } |
        Select-Object -First 1
}

function Get-NinjaVersion {
    $version = ninja --version 2>$null
    if (-not [string]::IsNullOrEmpty($version)) {
        Write-Host "$(Get-UnicodeChar 0x1F50D) Ninja is installed: Version $version"
        return $version
    }
    Write-Host "$(Get-UnicodeChar 0x26A0) Ninja is not installed or not found in PATH." -ForegroundColor Yellow
    return "0.0.0"
}

function Get-LatestNinjaVersion {
    try {
        $response = Invoke-RestMethod -Uri "https://api.github.com/repos/ninja-build/ninja/releases/latest" -UseBasicParsing
        return $response.tag_name -replace "^v", ""  # Remove 'v' prefix if present
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) Failed to fetch the latest Ninja version from GitHub." -ForegroundColor Yellow
        return "0.0.0"
    }
}

function Test-IsAdmin {
    $identity = [System.Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object System.Security.Principal.WindowsPrincipal($identity)
    return $principal.IsInRole([System.Security.Principal.WindowsBuiltInRole]::Administrator)
}

function Install-CMake {
    Write-Host "$(Get-UnicodeChar 0x1F680) Installing latest CMake..."
    $CMakeBaseUrl = "https://cmake.org/files/LatestRelease/"
    $CMakeLatestVersion = Invoke-WebRequest -Uri $CMakeBaseUrl -UseBasicParsing |
        Select-String -Pattern "cmake-(\d+\.\d+\.\d+)-windows-x86_64.msi" |
        ForEach-Object { $_.Matches.Groups[1].Value } |
        Select-Object -First 1

    Write-Host "$(Get-UnicodeChar 0x1F50D) Latest CMake version found: $CMakeLatestVersion"
    $CMakeUrl = "https://cmake.org/files/LatestRelease/cmake-$CMakeLatestVersion-windows-x86_64.msi"
    Write-Host "$(Get-UnicodeChar 0x1F4E5) Downloading from: $CMakeUrl"
    
    Invoke-WebRequest -Uri $CMakeUrl -OutFile "$env:TEMP\cmake-installer.msi"
    Start-Process -FilePath "msiexec.exe" -ArgumentList "/i `"$env:TEMP\cmake-installer.msi`" /quiet /norestart" -Wait
}

function Install-Ninja {
    Write-Host "$(Get-UnicodeChar 0x1F680) Installing latest Ninja..."
    $NinjaUrl = "https://github.com/ninja-build/ninja/releases/latest/download/ninja-win.zip"
    Invoke-WebRequest -Uri $NinjaUrl -OutFile "$env:TEMP\ninja.zip"
    Expand-Archive -Path "$env:TEMP\ninja.zip" -DestinationPath "$env:LOCALAPPDATA\ninja-build" -Force
}

$CMakeInstalledVersion = Get-CMakeVersion
$CMakeLatestVersion = Get-LatestCMakeVersion
$needs_cmake_install = ([version]$CMakeInstalledVersion -lt [version]$CMakeLatestVersion)

$NinjaInstalledVersion = Get-NinjaVersion
$NinjaLatestVersion = Get-LatestNinjaVersion
$needs_ninja_install = ([version]$NinjaInstalledVersion -lt [version]$NinjaLatestVersion)

if (-not $needs_cmake_install -and -not $needs_ninja_install) {
    Write-Host "$(Get-UnicodeChar 0x2705) CMake and Ninja are already up to date!"
    Write-Host ""
    Write-Host "$(Get-UnicodeChar 0x1F91D) Press any key to exit..." -ForegroundColor Cyan
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit
}

if (-not (Test-IsAdmin)) {
    Write-Host "$(Get-UnicodeChar 0x26A0) WARNING: Admin rights required to install CMake/Ninja." -ForegroundColor Yellow
    Write-Host "🚀 Restarting as Administrator..."
    
    Start-Process powershell -ArgumentList "-File `"$PSCommandPath`"" -Verb RunAs
    exit
}

if ($needs_cmake_install) {
    Install-CMake
}
if ($needs_ninja_install) {
    Install-Ninja
}

Write-Host "$(Get-UnicodeChar 0x2705) Installation completed successfully!"
Write-Host ""
Write-Host "$(Get-UnicodeChar 0x1F91D) Press any key to exit..." -ForegroundColor Cyan
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
exit
