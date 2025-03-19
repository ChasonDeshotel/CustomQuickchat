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

function Get-ClangVersion($tool) {
    try {
        $version = clang-format --version 2>$null | Select-String -Pattern "clang-format version (\d+\.\d+\.\d+)" | ForEach-Object { $_.Matches.Groups[1].Value }
        if ($version) {
            Write-Host "$(Get-UnicodeChar 0x1F50D) LLVM is installed: Version $version"
            return $version
        }
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) $tool is not installed or not found in PATH." -ForegroundColor Yellow
    }
    return "0.0.0"
}

function Get-LatestClangVersion {
    try {
        $releases = Invoke-RestMethod -Uri "https://api.github.com/repos/llvm/llvm-project/releases" -UseBasicParsing
        foreach ($release in $releases) {
            if ($release.assets | Where-Object { $_.name -match "LLVM-.*-win64.exe" }) {
                return $release.tag_name -replace "^llvmorg-", ""
            }
        }
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) Failed to fetch the latest Clang version from GitHub." -ForegroundColor Yellow
        return "0.0.0"
    }
}

function Get-GitVersion {
    try {
        $version = git --version 2>$null | Select-String -Pattern "git version (\d+\.\d+\.\d+).*" | ForEach-Object { $_.Matches.Groups[1].Value }
        if ($version) {
            Write-Host "$(Get-UnicodeChar 0x1F50D) Git is installed: Version $version"
            return $version
        }
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) Git is not installed or not found in PATH." -ForegroundColor Yellow
    }
    return "0.0.0"
}

function Install-GitBash {
    Write-Host "$(Get-UnicodeChar 0x1F680) Installing Git Bash..."
    $GitInstallerUrl = "https://github.com/git-for-windows/git/releases/download/v2.49.0.windows.1/Git-2.49.0-64-bit.exe"
    $GitInstallerPath = "$env:TEMP\GitInstaller.exe"

    Invoke-WebRequest -Uri $GitInstallerUrl -OutFile $GitInstallerPath

    Write-Host "$(Get-UnicodeChar 0x1F4E5) Running Git Bash installer..."
    Start-Process -FilePath $GitInstallerPath

    # Find Git installation path
    $GitInstallPath = "C:\Program Files\Git\bin"
    if (!(Test-Path $GitInstallPath)) {
        $GitInstallPath = "C:\Program Files (x86)\Git\bin"
    }

    if (Test-Path $GitInstallPath) {
        Write-Host "$(Get-UnicodeChar 0x2705) Git installed successfully at $GitInstallPath"

        # Add to PATH
        $env:Path += ";$GitInstallPath"
        [System.Environment]::SetEnvironmentVariable("Path", $env:Path, [System.EnvironmentVariableTarget]::Machine)

        Write-Host "$(Get-UnicodeChar 0x1F4A1) Added Git to PATH. You may need to restart your terminal for changes to take effect."
    } else {
        Write-Host "$(Get-UnicodeChar 0x26A0) WARNING: Git installation path not found! Add it to PATH manually." -ForegroundColor Yellow
    }
}

function Get-BoostVersion {
    try {
        $BoostIncludePath = "C:\local\boost-*"
        $installedBoost = Get-ChildItem -Path "C:\local" -Filter "boost-*" -Directory | Sort-Object Name -Descending | Select-Object -First 1
        if ($installedBoost) {
            Write-Host "$(Get-UnicodeChar 0x1F50D) Boost is installed: $($installedBoost.Name)"
            return $installedBoost.Name -replace "boost-", ""
        }
    } catch {
        Write-Host "$(Get-UnicodeChar 0x26A0) Boost is not installed." -ForegroundColor Yellow
    }
    return "0.0.0"
}

function Install-Boost {
    # Download Boost
    $BoostUrl = "https://github.com/boostorg/boost/releases/download/boost-1.87.0/boost-1.87.0-b2-nodocs.zip"
    $BoostZipPath = "$env:TEMP\boost-1.87.0-b2-nodocs.zip"
    Write-Host "$(Get-UnicodeChar 0x1F4E5) Downloading Boost from $BoostUrl"
    Invoke-WebRequest -Uri $BoostUrl -OutFile $BoostZipPath

    $CLocalPath = "C:\local"

    Write-Host "$(Get-UnicodeChar 0x1F4E6) Extracting Boost to $CLocalPath..."
    Expand-Archive -Path $BoostZipPath -DestinationPath $CLocalPath -Force

    $BoostExtractPath = "C:\local\boost-1.87.0"

    cd $BoostExtractPath

    # Run Bootstrap to set up b2
    Write-Host "$(Get-UnicodeChar 0x1F527) Running Bootstrap..."
    Start-Process -FilePath "$BoostExtractPath\bootstrap.bat" -NoNewWindow -Wait
    
    # Build & Install Boost
    Write-Host "$(Get-UnicodeChar 0x1F6E0) Building Boost..."
    Start-Process -FilePath "$BoostExtractPath\b2.exe" -ArgumentList "toolset=msvc runtime-link=shared variant=release link=shared address-model=64 define=_ITERATOR_DEBUG_LEVEL=0 --prefix=$BoostExtractPath install" -NoNewWindow -Wait

    cd -

    Write-Host "$(Get-UnicodeChar 0x2705) Boost installed successfully!"
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

function Test-IsAdmin {
    $identity = [System.Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object System.Security.Principal.WindowsPrincipal($identity)
    return $principal.IsInRole([System.Security.Principal.WindowsBuiltInRole]::Administrator)
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

$ClangInstalledVersion = Get-ClangVersion
$ClangLatestVersion = Get-LatestClangVersion
$needs_clang_install = ([version]$ClangInstalledVersion -lt [version]$ClangLatestVersion)

$GitInstalledVersion = Get-GitVersion
$needs_git_install = ([version]$GitInstalledVersion -lt [version]"2.49.0")

$BoostInstalledVersion = Get-BoostVersion
$needs_boost_install = ([version]$BoostInstalledVersion -lt [version]"1.84.0")

if (-not $needs_cmake_install -and -not $needs_ninja_install -and -not $needs_clang_install -and -not $needs_git_install -and -not $needs_boost_install) {
    Write-Host "$(Get-UnicodeChar 0x2705) CMake, Ninja, Clang-Format, Clang-Tidy, Git Bash, and Boost are already up to date!"
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
if ($needs_clang_install) { Install-LLVM }
if ($needs_git_install)   { Install-GitBash }
if ($needs_boost_install) { Install-Boost }

Write-Host "$(Get-UnicodeChar 0x2705) Installation completed successfully!"
Write-Host ""
Write-Host "$(Get-UnicodeChar 0x1F91D) Press any key to exit..." -ForegroundColor Cyan
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
exit
