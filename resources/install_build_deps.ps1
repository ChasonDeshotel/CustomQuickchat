function Get-UnicodeChar($codepoint) {
    return [char]::ConvertFromUtf32($codepoint)
}

function Get-CMakeVersion {
    try {
        $version = cmake --version 2>$null | Select-String -Pattern "cmake version (\d+\.\d+\.\d+)" | ForEach-Object { $_.Matches.Groups[1].Value }
        if (-not [string]::IsNullOrEmpty($version)) {
            Write-Host "$(Get-UnicodeChar 0x1F50D) CMake is installed: Version $version"
            return $version
        }
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) CMake is not installed or not found in PATH." -ForegroundColor Yellow
    }
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
    try {
        $version = ninja --version 2>$null
        if (-not [string]::IsNullOrEmpty($version)) {
            Write-Host "$(Get-UnicodeChar 0x1F50D) Ninja is installed: Version $version"
            return $version
        }
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) Ninja is not installed or not found in PATH." -ForegroundColor Yellow
    }
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

function Get-ClangToolVersion($tool) {
    try {
        $version = & $tool --version 2>$null | Select-String -Pattern "$tool version (\d+\.\d+\.\d+)" | ForEach-Object { $_.Matches.Groups[1].Value }
        if ($version) {
            Write-Host "$(Get-UnicodeChar 0x1F50D) $tool is installed: Version $version"
            return $version
        }
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) $tool is not installed or not found in PATH." -ForegroundColor Yellow
    }
    return "0.0.0"
}

function Get-LatestClangVersion {
    try {
        return (Invoke-RestMethod -Uri "https://api.github.com/repos/llvm/llvm-project/releases/latest" -UseBasicParsing).tag_name -replace "^llvmorg-", ""
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) Failed to fetch the latest Clang version from GitHub." -ForegroundColor Yellow
        return "0.0.0"
    }
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

function Get-ClangToolVersion($tool) {
    $version = & $tool --version 2>$null | Select-String -Pattern "$tool version (\d+\.\d+\.\d+)" | ForEach-Object { $_.Matches.Groups[1].Value }
    if ($version) {
        Write-Host "$(Get-UnicodeChar 0x1F50D) $tool is installed: Version $version"
        return $version
    }
    Write-Host "$(Get-UnicodeChar 0x26A0) $tool is not installed or not found in PATH." -ForegroundColor Yellow
    return "0.0.0"
}

function Test-IsAdmin {
    $identity = [System.Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object System.Security.Principal.WindowsPrincipal($identity)
    return $principal.IsInRole([System.Security.Principal.WindowsBuiltInRole]::Administrator)
}

function Install-ClangTool($tool) {
    Write-Host "$(Get-UnicodeChar 0x1F680) Installing latest $tool..."
    $latestVersion = Get-LatestClangVersion
    Write-Host "$(Get-UnicodeChar 0x1F50D) Latest $tool version found: $latestVersion"

    # Construct the direct URL to the standalone executable
    $url = "https://github.com/llvm/llvm-project/releases/download/llvmorg-$latestVersion/$tool.exe"
    Write-Host "$(Get-UnicodeChar 0x1F4E5) Downloading from: $url"

    try {
        $destination = "$env:ProgramFiles\LLVM\bin\$tool.exe"
        Invoke-WebRequest -Uri $url -OutFile $destination
        Write-Host "$(Get-UnicodeChar 0x2705) $tool installed successfully at $destination"
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) Failed to install $tool. Check your internet connection and try again." -ForegroundColor Red
    }
}

function Install-LLVM {
    Write-Host "$(Get-UnicodeChar 0x1F680) Installing latest LLVM (includes Clang-Format and Clang-Tidy)..."
    $latestVersion = Get-LatestClangVersion
    Write-Host "$(Get-UnicodeChar 0x1F50D) Latest LLVM version found: $latestVersion"

    $url = "https://github.com/llvm/llvm-project/releases/download/llvmorg-$latestVersion/LLVM-$latestVersion-win64.exe"
    Write-Host "$(Get-UnicodeChar 0x1F4E5) Downloading from: $url"

    Write-Host ""
    Write-Host "$(Get-UnicodeChar 0x1F681)  YOU MUST ADD TO YOUR SYSTEM PATH"
    Write-Host "$(Get-UnicodeChar 0x1F682)  YOU MUST ADD TO YOUR SYSTEM PATH"
    Write-Host "$(Get-UnicodeChar 0x1F683)  YOU MUST ADD TO YOUR SYSTEM PATH"
    Write-Host ""
    Write-Host "$(Get-UnicodeChar 0x1F684) click the radio button when prompted"
    Write-Host ""
    Write-Host "$(Get-UnicodeChar 0x1F685)  YOU MUST ADD TO YOUR SYSTEM PATH"
    Write-Host "$(Get-UnicodeChar 0x1F686)  YOU MUST ADD TO YOUR SYSTEM PATH"
    Write-Host "$(Get-UnicodeChar 0x1F687)  YOU MUST ADD TO YOUR SYSTEM PATH"
    Write-Host ""


    try {
        $installer = "$env:TEMP\LLVM-installer.exe"
        Invoke-WebRequest -Uri $url -OutFile $installer
        Start-Process -FilePath $installer -Wait #-ArgumentList "/S" -Wait
        Write-Host "$(Get-UnicodeChar 0x2705) LLVM installed successfully!"
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) Failed to install LLVM. Check your internet connection and try again." -ForegroundColor Red
    }
}

# Install Git pre-commit hook
$gitHooksDir = Join-Path $PSScriptRoot "../.git/hooks"
$preCommitHook = Join-Path $PSScriptRoot "../.githooks/pre-commit"

if (-Not (Test-Path "$gitHooksDir/pre-commit")) {
    Write-Host "Installing pre-commit hook..."
    Copy-Item -Path $preCommitHook -Destination "$gitHooksDir/pre-commit" -Force
    icacls "$gitHooksDir/pre-commit" /grant Everyone:RX > $null  # Ensure it's executable
    Write-Host "$(Get-UnicodeChar 0x2705) Installed pre-commit hook"
} else {
    Write-Host "$(Get-UnicodeChar 0x2705) Pre-commit hook already installed. Skipping..."
}

$CMakeInstalledVersion = Get-CMakeVersion
$CMakeLatestVersion = Get-LatestCMakeVersion
$needs_cmake_install = ([version]$CMakeInstalledVersion -lt [version]$CMakeLatestVersion)

$NinjaInstalledVersion = Get-NinjaVersion
$NinjaLatestVersion = Get-LatestNinjaVersion
$needs_ninja_install = ([version]$NinjaInstalledVersion -lt [version]$NinjaLatestVersion)

$ClangFormatInstalledVersion = Get-ClangToolVersion "clang-format"
$ClangTidyInstalledVersion = Get-ClangToolVersion "clang-tidy"
$ClangLatestVersion = Get-LatestClangVersion
$needs_clang_format_install = ([version]$ClangFormatInstalledVersion -lt [version]$ClangLatestVersion)
$needs_clang_tidy_install = ([version]$ClangTidyInstalledVersion -lt [version]$ClangLatestVersion)

if (-not $needs_cmake_install -and -not $needs_ninja_install -and -not $needs_clang_format_install -and -not $needs_clang_tidy_install) {
    Write-Host "$(Get-UnicodeChar 0x2705) CMake, Ninja, Clang-Format, and Clang-Tidy are already up to date!"
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

if ($needs_cmake_install) { Install-CMake }
if ($needs_ninja_install) { Install-Ninja }
if ($needs_clang_format_install -or $needs_clang_tidy_install) { Install-LLVM }

Write-Host "$(Get-UnicodeChar 0x2705) Installation completed successfully!"
Write-Host ""
Write-Host "$(Get-UnicodeChar 0x1F91D) Press any key to exit..." -ForegroundColor Cyan
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
exit
