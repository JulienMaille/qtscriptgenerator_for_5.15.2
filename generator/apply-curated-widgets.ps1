# Post-regen step: install the curated QtWidgets bindings.
#
# Almost the whole module regenerates cleanly against Qt 6; the typesystem
# carries the Qt 6 adaptations (protected-enum rejections, QMessageBox::Option,
# QToolTip/QWhatsThis ctor removal, QTextEdit regex-arg mapping).
#
# qtscriptshell_QDoubleSpinBox.{cpp,h} is the one file pair the fork's
# generator still cannot reproduce: its validate(QString&, int&) and
# fixup(QString&) shell overrides unpack the script result object
# ({state, input, pos} / string) back into the C++ out-arguments. The fork's
# shell emitter only emits the plain call, so JS overrides of
# validate/fixup would lose their outputs (interpretText would never see
# the validated text or cursor position). The hand-maintained pair keeps
# the unpacking. Copy both over the freshly generated tree (the header
# differs too: it keeps the Qt 6 enterEvent(QEnterEvent*) override the
# regen drops while QEnterEvent stays unbound).
#
# The .pri is rewritten on every regen run, so this must run after each
# widgets regen and before the DLL build.
$ErrorActionPreference = 'Stop'
$regenDir = 'D:\Codez\qtscriptgenerator_for_5.15.2\regen-test\generated_cpp\com_trolltech_qt_widgets'
$curatedDir = 'D:\Codez\qtscriptgenerator-qt6-bindings\generated_cpp\com_trolltech_qt_widgets'

if (-not (Test-Path $regenDir)) { throw "regen output missing: $regenDir" }
if (-not (Test-Path $curatedDir)) { throw "curated output missing: $curatedDir" }

$curatedFiles = @(
    'qtscriptshell_QDoubleSpinBox.cpp',
    'qtscriptshell_QDoubleSpinBox.h'
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

Write-Host ("curated QtWidgets bindings applied: " + ($curatedFiles -join ', '))
