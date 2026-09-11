$ErrorActionPreference = "Stop"


# ============================================================
# WINDOWS BUILD
# ============================================================

$RepoRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
$BuildDir = Join-Path $RepoRoot "build_win"


# ============================================================
# ENVIRONMENT
# ============================================================

$env:PATH = "$BuildDir;C:\msys64\mingw64\bin;" + $env:PATH


# ============================================================
# CONFIGURE
# ============================================================

Write-Host "Configuring Windows build in $BuildDir..." -ForegroundColor Cyan

cmake `
    -S $RepoRoot `
    -B $BuildDir `
    -G "Ninja" `
    -DCMAKE_BUILD_TYPE=Debug `
    -DPROJECT_PLATFORM=WINDOWS

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    exit 1
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

    & (Join-Path $BuildDir "MIYOO_SQUARE.exe")

}
else {

    Write-Host "Build failed!" -ForegroundColor Red

    exit 1

}