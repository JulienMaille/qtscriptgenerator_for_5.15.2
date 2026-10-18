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

# ... and from INCLUDE, which it hands to the legacy preprocessor (main.h).
# build-generator.ps1 exports the MSVC and Windows SDK INCLUDE into its caller,
# so a regen run in the same shell as a build resolves the STL headers Qt pulls
# in (<string>, <type_traits>, ...). rpp then produces an 11.7 MB stream instead
# of the 7.1 MB one and silently loses four QtCore classes - QByteArray,
# QBitArray, QStringMatcher, QStringConverter - i.e. 80 classes instead of 84,
# with nothing in the log to say so. The generation recipe is Qt only, and the
# CI runs the two steps in separate shells, so clear the variable: a regen must
# not depend on what built the generator.
$savedInclude = $env:INCLUDE
$env:INCLUDE = ''

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
    $env:INCLUDE = $savedInclude
}

if ($PublishRoot) {
    $PublishRoot = [System.IO.Path]::GetFullPath($PublishRoot)
    $generatedTarget = Join-Path $PublishRoot "generated_cpp"
    # Replace, do not merge. Copy-Item leaves every file that is not part of this
    # regen in place, so a class the generator did not produce - or no longer
    # produces - survived in generated_cpp from an earlier run and was compiled
    # and shipped as if it were current. That is what hid a class the generator
    # had dropped: the build stayed green because the stale file was still
    # there. The output root is wiped for the same reason.
    if (Test-Path -LiteralPath $generatedTarget) {
        Write-Host "Replacing $generatedTarget"
        Remove-Item -LiteralPath $generatedTarget -Recurse -Force
    }
    New-Item -ItemType Directory -Force -Path $generatedTarget | Out-Null
    Copy-Item -Path (Join-Path $OutputRoot "generated_cpp\*") -Destination $generatedTarget -Recurse -Force
    Write-Host "Published the generated projects to $generatedTarget"
}

Write-Host "Generated $($Modules.Count) modules into $OutputRoot"
