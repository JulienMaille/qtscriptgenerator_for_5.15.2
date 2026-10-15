# Post-regen step: install the curated QtPrintSupport bindings.
#
# Almost the whole module regenerates cleanly against Qt 6; the typesystem
# carries the Qt 5 compatibility surface (numCopies/setOrientation/orientation,
# setPageSizeMM/setPageMargins, isOptionEnabled) as prototype inject-code.
#
# qtscript_QPrinter.cpp is the one file the fork's generator still cannot
# reproduce: Qt 6 dropped the QPrinter::Orientation enum (it now lives on
# QPageLayout) and QPrinter derives from QPagedPaintDevice, so the generated
# class loses both the synthetic Orientation enum and the QPaintDevice*
# conversion that QPainter::begin(printer) needs. The hand-maintained file
# keeps them. Copy it over the freshly generated tree.
#
# The .pri is rewritten on every regen run, so this must run after each
# printsupport regen and before the DLL build.
$ErrorActionPreference = 'Stop'
$regenDir = 'D:\Codez\qtscriptgenerator_for_5.15.2\regen-test\generated_cpp\com_trolltech_qt_printsupport'
$curatedDir = 'D:\Codez\qtscriptgenerator-qt6-bindings\generated_cpp\com_trolltech_qt_printsupport'

if (-not (Test-Path $regenDir)) { throw "regen output missing: $regenDir" }
if (-not (Test-Path $curatedDir)) { throw "curated output missing: $curatedDir" }

$curatedFiles = @(
    'qtscript_QPrinter.cpp'
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

Write-Host ("curated QtPrintSupport bindings applied: " + ($curatedFiles -join ', '))
