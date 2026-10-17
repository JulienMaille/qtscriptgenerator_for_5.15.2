[CmdletBinding()]
param(
    [string] $QtPrefix = $env:QT_ROOT_DIR,

    # Root the nine generated projects are written to. The generator writes
    # <root>\generated_cpp\<module> and <root>\qtbindings\qtscript_<module>.
    # Defaults to <forge>\regen-test, the historical generation workspace.
    [string] $OutputRoot = "",

    [string[]] $Modules = @(
        "core", "gui", "network", "printsupport", "sql", "svg", "uitools", "widgets", "xml"
    ),

    # Optional: copy the freshly generated projects into <publish root>\generated_cpp
    # so the CMake build (which reads <repo>\generated_cpp) picks them up.
    [string] $PublishRoot = "",

    # Re-attach the curated (hand-maintained, non-generatable) files after the regen.
    [switch] $ApplyCurated,

    # Keep an existing output tree instead of starting from scratch.
    [switch] $NoClean
)

# Runs the freshly built generator over every module's typesystem and produces
# the nine C++ binding projects.
#
# The generator is a plain Qt application: it needs QTDIR to resolve the Qt
# headers it parses, and the Qt DLLs on PATH. No compiler is involved, so the
# MSVC environment of build-generator.ps1 is not required here.
#
# The output root is wiped by default: a module regen rewrites its own files but
# never deletes a class that a previous Qt version or typesystem produced, so a
# stale file would silently stay in the project that is built and shipped.

$ErrorActionPreference = "Stop"
$generatorRoot = $PSScriptRoot
$forgeRoot = Split-Path $generatorRoot -Parent

if (-not $QtPrefix) { $QtPrefix = $env:QTDIR }
if (-not $QtPrefix) { throw "Specify -QtPrefix or set QT_ROOT_DIR/QTDIR." }
$QtPrefix = [System.IO.Path]::GetFullPath($QtPrefix)
if (-not (Test-Path -LiteralPath $QtPrefix -PathType Container)) {
    throw "Qt prefix does not exist: $QtPrefix"
}

$generator = Join-Path $generatorRoot "release\generator.exe"
if (-not (Test-Path -LiteralPath $generator)) {
    throw "The generator is not built yet: $generator (run generator\build-generator.ps1 first)"
}

if (-not $OutputRoot) { $OutputRoot = Join-Path $forgeRoot "regen-test" }
$OutputRoot = [System.IO.Path]::GetFullPath($OutputRoot)

if (-not $NoClean) {
    if ($OutputRoot -eq [System.IO.Path]::GetFullPath($forgeRoot)) {
        throw "Refusing to clean a checkout: $OutputRoot"
    }
    if (Test-Path -LiteralPath $OutputRoot) {
        Write-Host "Cleaning $OutputRoot"
        Remove-Item -LiteralPath $OutputRoot -Recurse -Force
    }
}
New-Item -ItemType Directory -Force -Path $OutputRoot | Out-Null

# The generator reads its include directories from QTDIR and loads the Qt DLLs
# from PATH.
$env:QTDIR = $QtPrefix
$env:PATH = (Join-Path $QtPrefix "bin") + ";" + $env:PATH

$masterInclude = "qtscript_masterinclude_qt6.h"
Push-Location $generatorRoot
try {
    foreach ($module in $Modules) {
        $buildFile = "build_$module.txt"
        if (-not (Test-Path -LiteralPath (Join-Path $generatorRoot $buildFile))) {
            throw "Typesystem file not found: $buildFile"
        }

        Write-Host "--- generating $module ---"
        # Invoke through cmd.exe with stderr merged there: the generator reports
        # every preprocessor notice on stderr, and a caller that redirects
        # streams (2>&1) while $ErrorActionPreference is 'Stop' turns the first
        # notice into a terminating error. Merging inside cmd keeps the notices
        # in the log and leaves only the exit code to act on.
        $commandLine = '"{0}" {1} {2} --output-directory="{3}" 2>&1' -f `
            $generator, $masterInclude, $buildFile, $OutputRoot
        & cmd.exe /d /c $commandLine
        if ($LASTEXITCODE -ne 0) {
            throw "The generator failed for module '$module' (exit code $LASTEXITCODE)"
        }

        $moduleDir = Join-Path $OutputRoot "generated_cpp\com_trolltech_qt_$module"
        if (-not (Test-Path -LiteralPath $moduleDir)) {
            throw "The generator produced nothing for module '$module': $moduleDir"
        }
    }

    if ($ApplyCurated) {
        foreach ($module in @("core", "printsupport", "svg", "xml")) {
            $curatedScript = Join-Path $generatorRoot "apply-curated-$module.ps1"
            Write-Host "--- curated $module ---"
            & $curatedScript -ForgeRoot $forgeRoot -RegenRoot $OutputRoot
            if ($LASTEXITCODE -ne 0) {
                throw "The curated $module step failed (exit code $LASTEXITCODE)"
            }
        }
    }
} finally {
    Pop-Location
}

if ($PublishRoot) {
    $PublishRoot = [System.IO.Path]::GetFullPath($PublishRoot)
    $generatedTarget = Join-Path $PublishRoot "generated_cpp"
    New-Item -ItemType Directory -Force -Path $generatedTarget | Out-Null
    Copy-Item -Path (Join-Path $OutputRoot "generated_cpp\*") -Destination $generatedTarget -Recurse -Force
    Write-Host "Published the generated projects to $generatedTarget"
}

Write-Host "Generated $($Modules.Count) modules into $OutputRoot"
