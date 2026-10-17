# Post-regen step: register curated (hand-maintained, non-generatable)
# Core bindings in the fresh generator output.
#
# QStringConverter is generated as an abstract class, while the shipped
# product exposes it as a namespace object holding only the Encoding enum.
# The curated file lives in generator/curated/, in this repository, so a regen
# plus curated pass reproduces the shipped core bindings without any other
# checkout. The .pri and _init.cpp are rewritten from scratch on each regen
# run, so this must run after each core regen and before the DLL build.
param(
    [string] $ForgeRoot = (Split-Path $PSScriptRoot -Parent),
    [string] $RegenRoot = (Join-Path (Split-Path $PSScriptRoot -Parent) 'regen-test'),
    [string] $CuratedRoot = (Join-Path $PSScriptRoot 'curated')
)
$ErrorActionPreference = 'Stop'
$coreDir = Join-Path $RegenRoot 'generated_cpp\com_trolltech_qt_core'
$curatedDir = Join-Path $CuratedRoot 'com_trolltech_qt_core'

if (-not (Test-Path $coreDir)) { throw "regen output missing: $coreDir" }

# 1. Curated sources (tracked in this repository).
foreach ($f in @('qtscript_QStringConverter.cpp')) {
  $source = Join-Path $curatedDir $f
  if (-not (Test-Path $source)) { throw "curated file missing: $source" }
  $dest = Join-Path $coreDir $f
  Copy-Item $source $dest -Force
  # Copy-Item keeps the source timestamp, which may be older than the object
  # file nmake built on the previous run; touch so the TU is recompiled.
  (Get-Item $dest).LastWriteTime = Get-Date
}

# 2. .pri: drop the generated QStringConverter shell (the curated binding
#    replaces its class form; same class slot, so the init arrays are
#    untouched).
$pri = Join-Path $coreDir 'com_trolltech_qt_core.pri'
$lines = [System.IO.File]::ReadAllLines($pri)
$out = @()
foreach ($l in $lines) {
  $t = $l.Trim()
  if ($t -eq '$$PWD/qtscriptshell_QStringConverter.h \') { continue }
  if ($t -eq '$$PWD/qtscriptshell_QStringConverter.cpp \') { continue }
  $out += $l
}
[System.IO.File]::WriteAllLines($pri, $out)
Write-Output 'curated Core QStringConverter applied'
