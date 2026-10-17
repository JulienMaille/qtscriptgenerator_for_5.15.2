# Post-regen step: register curated (hand-maintained, non-generatable)
# QtXml bindings in the fresh generator output.
#
# Qt 6 dropped the SAX classes and moved the DOM parse API around. The
# hand-maintained pieces the generator cannot produce on its own live in
# generator/curated/:
#   - qtscript_QtXml.cpp           : the QtXml namespace object and the
#                                    QDomDocument::ParseOption/ParseOptions
#                                    surface;
#   - qtscript_QDomDocument.cpp    : the Dom parse overloads (QIODevice,
#                                    QByteArray, QString, QXmlStreamReader,
#                                    ParseOptions) and importNode;
#   - qtscript_QDomNode.cpp        : normalizes a derived DOM wrapper
#                                    (QDomElement, QDomText, ...) to QDomNode
#                                    for both `this` and arguments, plus the
#                                    TypeError null checks for QTextStream
#                                    references;
#   - com_trolltech_qt_xml_init.cpp: registers base classes before derived
#                                    ones so the wrapper prototypes inherit
#                                    the Qt 6 hierarchy, and injects the QtXml
#                                    namespace.
# The .pri is rewritten from scratch on each regen run, so this must run
# after each xml regen and before the DLL build.
param(
    [string] $ForgeRoot = (Split-Path $PSScriptRoot -Parent),
    [string] $RegenRoot = (Join-Path (Split-Path $PSScriptRoot -Parent) 'regen-test'),
    [string] $CuratedRoot = (Join-Path $PSScriptRoot 'curated')
)
$ErrorActionPreference = 'Stop'
$xmlDir = Join-Path $RegenRoot 'generated_cpp\com_trolltech_qt_xml'
$curatedDir = Join-Path $CuratedRoot 'com_trolltech_qt_xml'

if (-not (Test-Path $xmlDir)) { throw "regen output missing: $xmlDir" }
if (-not (Test-Path $curatedDir)) { throw "curated output missing: $curatedDir" }

# 1. Curated sources (tracked in this repository).
$curated = @(
  'qtscript_QtXml.cpp',
  'qtscript_QDomDocument.cpp',
  'qtscript_QDomNode.cpp',
  'com_trolltech_qt_xml_init.cpp'
)
foreach ($f in $curated) {
  $source = Join-Path $curatedDir $f
  if (-not (Test-Path $source)) { throw "curated file missing: $source" }
  $dest = Join-Path $xmlDir $f
  Copy-Item $source $dest -Force
  # The repo copies keep the source timestamp, which can be older than the
  # object file nmake already built; touch them so nmake recompiles.
  (Get-Item $dest).LastWriteTime = Get-Date
}

# 2. .pri: compile the QtXml namespace source.
$pri = Join-Path $xmlDir 'com_trolltech_qt_xml.pri'
$lines = [System.IO.File]::ReadAllLines($pri)
$out = @()
$done = $false
foreach ($l in $lines) {
  if (-not $done -and $l.Trim() -eq '$$PWD/qtscript_QDomDocument.cpp \') {
    $out += $l
    $out += '           $$PWD/qtscript_QtXml.cpp \'
    $done = $true
    continue
  }
  $out += $l
}
if (-not $done) { throw 'qtscript_QDomDocument.cpp entry not found in .pri' }
[System.IO.File]::WriteAllLines($pri, $out)

Write-Host 'curated QtXml registration applied and verified'
