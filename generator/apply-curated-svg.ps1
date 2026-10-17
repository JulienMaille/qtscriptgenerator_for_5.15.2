# Post-regen step: install the curated QtSvg bindings.
#
# The classes QSvgRenderer, QSvgGenerator and QSvgWidget regenerate cleanly
# against Qt 6. QGraphicsSvgItem cannot: its script prototype must chain to
# the QGraphicsItem prototype (so inherited view methods like boundingRect
# resolve) while carrying the QGraphicsObject pointer for its own methods.
# The fork's generator only chains to the direct C++ base (QGraphicsObject),
# hiding the item API. The hand-maintained files, kept in generator/curated/,
# preserve the product shape:
#   - qtscript_QtSvg.cpp            : the QtSvg namespace object plus the QSvg
#                                     alias, with QtSvg::Option/Options;
#   - qtscript_QSvgGenerator.cpp     : the SvgVersion constructor and version
#                                     surface over the scoped enum the parser
#                                     cannot see (no Q_ENUM in Qt 6.8);
#   - qtscript_QGraphicsSvgItem.cpp : item-chained prototype with renderer,
#                                     element, caching and bounds methods;
#   - qtscript_svg_shared.h         : shared conversion helpers;
#   - com_trolltech_qt_svg_init.cpp : initializes the namespace first, then
#                                     registers the generated classes alongside
#                                     the curated item class.
# The .pri is rewritten on every regen run, so this must run after each svg
# regen and before the DLL build.
param(
    [string] $ForgeRoot = (Split-Path $PSScriptRoot -Parent),
    [string] $RegenRoot = (Join-Path (Split-Path $PSScriptRoot -Parent) 'regen-test'),
    [string] $CuratedRoot = (Join-Path $PSScriptRoot 'curated')
)
$ErrorActionPreference = 'Stop'
$regenDir = Join-Path $RegenRoot 'generated_cpp\com_trolltech_qt_svg'
$curatedDir = Join-Path $CuratedRoot 'com_trolltech_qt_svg'

if (-not (Test-Path $regenDir)) { throw "regen output missing: $regenDir" }
if (-not (Test-Path $curatedDir)) { throw "curated output missing: $curatedDir" }

$curatedFiles = @(
    'qtscript_QtSvg.cpp',
    'qtscript_QSvgGenerator.cpp',
    'qtscript_QGraphicsSvgItem.cpp',
    'qtscript_svg_shared.h',
    'com_trolltech_qt_svg_init.cpp'
)

foreach ($name in $curatedFiles) {
    $source = Join-Path $curatedDir $name
    if (-not (Test-Path $source)) { throw "curated file missing: $source" }
    $dest = Join-Path $regenDir $name
    Copy-Item -LiteralPath $source -Destination $dest -Force
    # The repo copies keep the source timestamp, which can be older than the
    # object file nmake already built; touch them so nmake recompiles.
    (Get-Item -LiteralPath $dest).LastWriteTime = Get-Date
}

# .pri: the shared header must be visible to the build.
$pri = Join-Path $regenDir 'com_trolltech_qt_svg.pri'
$lines = [System.IO.File]::ReadAllLines($pri)
$out = @()
$done = $false
foreach ($l in $lines) {
    if (-not $done -and $l.Trim() -eq '$$PWD/qtscriptshell_QGraphicsSvgItem.h \') {
        $out += '           $$PWD/qtscript_svg_shared.h \'
        $done = $true
    }
    $out += $l
}
if (-not $done) { throw 'pri anchor not found' }
[System.IO.File]::WriteAllLines($pri, $out)

Write-Host ("curated QtSvg bindings applied: " + ($curatedFiles -join ', '))
