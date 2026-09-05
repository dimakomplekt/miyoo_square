$ErrorActionPreference = "Stop"

$ProjectDir = $PSScriptRoot
$BuildDir = Join-Path $ProjectDir "build_lin"
$MiyooPath = "\\192.168.0.103\__sdcard\App\MIYOO_SQUARE"


Write-Host ""
Write-Host "========================================"
Write-Host "MIYOO WIFI DEPLOYMENT"
Write-Host "========================================"

Write-Host "Project:"
Write-Host "  $ProjectDir"

Write-Host "Build:"
Write-Host "  $BuildDir"

Write-Host "Miyoo:"
Write-Host "  $MiyooPath"


# ====================================================
# CHECK BUILD
# ====================================================

Write-Host ""
Write-Host "Checking build..."

if (-not (Test-Path -LiteralPath $BuildDir)) {
    throw "Build directory not found: $BuildDir"
}

Write-Host "OK: build directory found."


# ====================================================
# CHECK MIYOO
# ====================================================

Write-Host ""
Write-Host "Checking Miyoo connection..."

if (-not (Test-Path -LiteralPath $MiyooPath)) {
    throw "Miyoo directory not found: $MiyooPath"
}

Write-Host "OK: Miyoo found."


# ====================================================
# SHOW BUILD CONTENT
# ====================================================

Write-Host ""
Write-Host "Build content:"

$BuildItems = Get-ChildItem -LiteralPath $BuildDir -Force

foreach ($Item in $BuildItems) {
    Write-Host "  $($Item.Name)"
}


# ====================================================
# CLEAR MIYOO DIRECTORY
# ====================================================

Write-Host ""
Write-Host "Clearing Miyoo application directory..."

$Shell = New-Object -ComObject Shell.Application

$ParentPath = Split-Path -Parent $MiyooPath
$FolderName = Split-Path -Leaf $MiyooPath

$ParentFolder = $Shell.Namespace($ParentPath)
$Folder = $ParentFolder.ParseName($FolderName)

$Folder.InvokeVerb("delete")

Start-Sleep -Milliseconds 500

Write-Host "OK: target directory cleared."


# ====================================================
# CREATE MIYOO DIRECTORY
# ====================================================

if (-not (Test-Path -LiteralPath $MiyooPath)) {
    New-Item -ItemType Directory -Path $MiyooPath | Out-Null
}


# ====================================================
# COPY BUILD
# ====================================================

Write-Host ""
Write-Host "Copying build to Miyoo..."

foreach ($Item in $BuildItems) {

    Write-Host "  Copying: $($Item.Name)"

    Copy-Item `
        -LiteralPath $Item.FullName `
        -Destination $MiyooPath `
        -Recurse `
        -Force
}


# ====================================================
# DONE
# ====================================================

Write-Host ""
Write-Host "========================================"
Write-Host "DEPLOYMENT COMPLETE"
Write-Host "========================================"

Write-Host "Miyoo application:"
Write-Host "  $MiyooPath"

Write-Host "Deployed files:"

$DeployedItems = Get-ChildItem -LiteralPath $MiyooPath -Force

foreach ($Item in $DeployedItems) {
    Write-Host "  $($Item.Name)"
}
