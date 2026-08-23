$ErrorActionPreference = "Stop"


# ============================================================
# WINDOWS BUILD
# ============================================================

$BuildDir = "build_win"


# ============================================================
# ENVIRONMENT
# ============================================================

$env:PATH = "$(Get-Location)\$BuildDir;C:\msys64\mingw64\bin;" + $env:PATH


# ============================================================
# CONFIGURE
# ============================================================

if (-not (Test-Path $BuildDir)) {

    Write-Host "Configuring Windows build..." -ForegroundColor Cyan

    cmake `
        -S . `
        -B $BuildDir `
        -G "Ninja" `
        -DCMAKE_BUILD_TYPE=Debug `
        -DPROJECT_PLATFORM=WINDOWS

}
else {

    Write-Host "Windows build directory already exists." -ForegroundColor Green

}


# ============================================================
# BUILD
# ============================================================

Write-Host "Building MIYOO_SQUARE for Windows..." -ForegroundColor Yellow

cmake --build $BuildDir


# ============================================================
# RUN
# ============================================================

if ($LASTEXITCODE -eq 0) {

    Write-Host "Build successful!" -ForegroundColor Green

    & ".\$BuildDir\MIYOO_SQUARE.exe"

}
else {

    Write-Host "Build failed!" -ForegroundColor Red

    exit 1

}