[CmdletBinding()]
param(
    [string] $QtPrefix = $env:QT_ROOT_DIR,

    [ValidateSet("Release", "Debug")]
    [string] $Configuration = "Release",

    # Force a from-scratch rebuild (drops every object, moc and qrc output).
    [switch] $Clean,

    [string] $VsDevCmd = ""
)

# Builds generator/ against a Qt 6 prefix.
#
# The generator is a qmake/nmake project. Two things matter:
#   * qmake must run here, so Makefile.qt6 always matches the Qt prefix in use;
#     build-qt6.bat only calls nmake and silently reuses a stale Makefile.
#   * every object must be compiled against the *same* Qt headers as the
#     Qt6Core.dll loaded at run time. Mixing objects from two Qt versions
#     (e.g. recompiling only a few files after switching 6.8 -> 6.12) links
#     cleanly but corrupts memory at run time: the generator then dies with an
#     access violation inside QString/QByteArray, on any input.
# The stamp below tracks which prefix the objects came from and forces a full
# rebuild when it changes.

$ErrorActionPreference = "Stop"
$generatorRoot = $PSScriptRoot

if (-not $QtPrefix) { $QtPrefix = $env:QTDIR }
if (-not $QtPrefix) { throw "Specify -QtPrefix or set QT_ROOT_DIR/QTDIR." }
$QtPrefix = [System.IO.Path]::GetFullPath($QtPrefix)
if (-not (Test-Path -LiteralPath $QtPrefix -PathType Container)) {
    throw "Qt prefix does not exist: $QtPrefix"
}
$qmake = Join-Path $QtPrefix "bin\qmake.exe"
if (-not (Test-Path -LiteralPath $qmake)) { throw "qmake.exe not found: $qmake" }

if (-not $VsDevCmd) {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path -LiteralPath $vswhere)) { throw "vswhere.exe not found: $vswhere" }
    $installationPath = & $vswhere -latest -products * `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -property installationPath
    if (-not $installationPath) { throw "Visual Studio with the MSVC x64 toolchain was not found" }
    $VsDevCmd = Join-Path $installationPath "Common7\Tools\VsDevCmd.bat"
}
if (-not (Test-Path -LiteralPath $VsDevCmd)) { throw "VsDevCmd.bat not found: $VsDevCmd" }

$environment = cmd.exe /d /c "call `"$VsDevCmd`" -arch=amd64 -host_arch=amd64 >nul 2>&1 && set"
if ($LASTEXITCODE -ne 0) { throw "MSVC environment setup failed: $VsDevCmd" }
foreach ($line in $environment) {
    if ($line -match '^([^=]+)=(.*)$') {
        [Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], "Process")
    }
}

# The generator resolves its Qt include directories from QTDIR at run time and
# needs the Qt DLLs on PATH.
$env:QTDIR = $QtPrefix
$env:PATH = (Join-Path $QtPrefix "bin") + ";" + $env:PATH

$releaseDir = Join-Path $generatorRoot "release"
$stampPath = Join-Path $releaseDir ".generator-build.stamp"
$stampValue = "$QtPrefix|$Configuration"

if ($Clean) {
    Write-Host "Clean rebuild requested."
} elseif (Test-Path -LiteralPath $stampPath) {
    $previous = (Get-Content -LiteralPath $stampPath -Raw).Trim()
    if ($previous -ne $stampValue) {
        Write-Host "Qt prefix changed since the last build ($previous -> $stampValue): full rebuild."
        $Clean = $true
    }
} elseif ((Get-ChildItem -LiteralPath $releaseDir -Filter *.obj -File -ErrorAction SilentlyContinue |
           Measure-Object).Count -gt 0) {
    Write-Host "Objects of unknown origin in release/: full rebuild."
    $Clean = $true
}

if ($Clean) {
    New-Item -ItemType Directory -Force -Path $releaseDir | Out-Null
    foreach ($pattern in @('*.obj', 'moc_*.cpp', 'qrc_generator.cpp', '*.exe', '*.exp', '*.lib',
                          '*.pdb', '*.ilk', '*.idb', '*.res')) {
        Get-ChildItem -LiteralPath $releaseDir -Filter $pattern -File -ErrorAction SilentlyContinue |
            Remove-Item -Force
    }
}

Push-Location $generatorRoot
try {
    Write-Host "qmake -o Makefile.qt6 generator.pro ($QtPrefix)"
    & $qmake -o Makefile.qt6 generator.pro
    if ($LASTEXITCODE -ne 0) { throw "qmake failed with exit code $LASTEXITCODE" }

    Write-Host "nmake /f Makefile.qt6 $($Configuration.ToLowerInvariant())"
    & nmake /nologo /f Makefile.qt6 $Configuration.ToLowerInvariant()
    if ($LASTEXITCODE -ne 0) { throw "nmake failed with exit code $LASTEXITCODE" }
} finally {
    Pop-Location
}

$exe = Join-Path $releaseDir "generator.exe"
if (-not (Test-Path -LiteralPath $exe)) { throw "generator.exe was not produced: $exe" }
Set-Content -LiteralPath $stampPath -Value $stampValue
Write-Host "Generator built: $exe"
