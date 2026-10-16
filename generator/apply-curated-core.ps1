# Post-regen step: register curated (hand-maintained, non-generatable)
# Core bindings in the fresh generator output.
#
# QStringConverter is generated as an abstract class, while the shipped
# product exposes it as a namespace object holding only the Encoding enum.
# It is hand-carried here (the bindings repo owns the curated layer) and
# re-attached after every core regen, before the DLL build. The .pri and
# _init.cpp are rewritten from scratch on each regen run, so this must run
# after each core regen.
param(
    [string] $ForgeRoot = (Split-Path $PSScriptRoot -Parent),
    [string] $BindingsRoot = (Join-Path (Split-Path $ForgeRoot -Parent) 'qtscriptgenerator-qt6-bindings')
)
$ErrorActionPreference = 'Stop'
$coreDir = Join-Path $ForgeRoot 'regen-test\generated_cpp\com_trolltech_qt_core'
$coreInclude = Join-Path $ForgeRoot 'regen-test\qtbindings\qtscript_core\include'
$curatedDir = Join-Path $BindingsRoot 'generated_cpp\com_trolltech_qt_core'
$bindingsInclude = Join-Path $BindingsRoot 'qtbindings\include'

# 1. Curated sources (tracked in the bindings repo).
foreach ($f in @('qtscript_QStringConverter.cpp')) {
  $dest = Join-Path $coreDir $f
  Copy-Item (Join-Path $curatedDir $f) $dest -Force
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
