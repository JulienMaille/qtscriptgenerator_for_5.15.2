# Post-regen step: register curated (hand-maintained, non-generatable)
# GUI bindings in the fresh generator output. The generator cannot produce
# these because their Qt 6 C++ sources are gone (QMatrix) or need compat
# shims; the .pri/_init.cpp below are regenerated from scratch on every
# regen run, so this must run after each GUI regen, before the DLL build.
param(
    [string] $ForgeRoot = (Split-Path $PSScriptRoot -Parent),
    [string] $BindingsRoot = (Join-Path (Split-Path $ForgeRoot -Parent) 'qtscriptgenerator-qt6-bindings')
)
$ErrorActionPreference = 'Stop'
$guiDir = Join-Path $ForgeRoot 'regen-test\generated_cpp\com_trolltech_qt_gui'
$guiInclude = Join-Path $ForgeRoot 'regen-test\qtbindings\qtscript_gui\include'
$curatedDir = Join-Path $BindingsRoot 'generated_cpp\com_trolltech_qt_gui'

# 1. Curated sources (tracked in the bindings repo).
foreach ($f in @('qtscript_QMatrix.cpp')) {
  Copy-Item (Join-Path $curatedDir $f) (Join-Path $guiDir $f) -Force
}

# 1b. Shared header: the generated QTextDocument::find(QRegularExpression)
#     overloads are scripted as QRegExp and convert through these helpers,
#     which the module's __package_shared.h must provide (the product header
#     carries the same block). Append once, guarded by a marker.
$shared = Join-Path $guiInclude '__package_shared.h'
$text = [System.IO.File]::ReadAllText($shared)
if ($text -notmatch 'qtscriptRegularExpression') {
  $text += @'

// Qt 6: QRegularExpression replaced QRegExp as the regex argument type (e.g.
// QTextDocument::find). The script surface stays QRegExp, so the generated
// bindings convert a JavaScript RegExp or a QRegExp object through these
// helpers and hand the native call a QRegularExpression.
#include <QtCore/QRegularExpression>
#include <QtScript/QRegExp>
#include <QtScript/QScriptValue>

inline bool qtscriptIsRegExp(const QScriptValue &value)
{
    return value.isRegExp()
        || qMetaTypeId<QRegExp>() == value.toVariant().userType();
}

inline QRegExp qtscriptToRegExp(const QScriptValue &value)
{
    if (value.isRegExp())
        return value.toRegExp();
    return qvariant_cast<QRegExp>(value.toVariant());
}

inline QRegularExpression qtscriptRegularExpression(const QRegExp &expression)
{
    QRegularExpression::PatternOptions options;
    if (expression.caseSensitivity() == Qt::CaseInsensitive)
        options |= QRegularExpression::CaseInsensitiveOption;
    return QRegularExpression(expression.pattern(), options);
}
'@
  [System.IO.File]::WriteAllText($shared, $text)
}

# 2. .pri: compile the curated source.
$pri = Join-Path $guiDir 'com_trolltech_qt_gui.pri'
$lines = [System.IO.File]::ReadAllLines($pri)
$out = @()
$done = $false
foreach ($l in $lines) {
  if (-not $done -and $l.Trim() -eq '$$PWD/qtscript_QMatrix3x3.cpp \') {
    $out += '           $$PWD/qtscript_QMatrix.cpp \'
    $done = $true
  }
  $out += $l
}
if (-not $done) { throw 'pri anchor not found' }
[System.IO.File]::WriteAllLines($pri, $out)

# 3. _init.cpp: declare, name, and register the curated class in parallel
# with its QMatrix3x3 neighbor (arrays must stay aligned).
$init = Join-Path $guiDir 'com_trolltech_qt_gui_init.cpp'
$lines = [System.IO.File]::ReadAllLines($init)
$out = @()
foreach ($l in $lines) {
  $out += $l
  $t = $l.Trim()
  if ($t -eq 'QScriptValue qtscript_create_QMatrix3x3_class(QScriptEngine *engine);') {
    $out += 'QScriptValue qtscript_create_QMatrix_class(QScriptEngine *engine);'
  } elseif ($t -eq ', "QMatrix3x3"') {
    $out += '    , "QMatrix"'
  } elseif ($t -eq ', qtscript_create_QMatrix3x3_class') {
    $out += '    , qtscript_create_QMatrix_class'
  }
}
[System.IO.File]::WriteAllLines($init, $out)

# 4. _init.cpp: extend the hardcoded class count by the curated additions.
$t = [System.IO.File]::ReadAllText($init)
if ($t -match 'for \(int i = 0; i < (\d+); \+\+i\) \{') {
  $n = [int]$Matches[1]
  $t = $t.Replace($Matches[0], ('for (int i = 0; i < ' + ($n + 1) + '; ++i) {'))
  [System.IO.File]::WriteAllText($init, $t)
} else {
  throw 'init count not found'
}

# 5. Verify the three arrays stayed parallel.
$lines = [System.IO.File]::ReadAllLines($init)
$decls = @(); $names = @(); $creates = @()
$inNames = $false; $inCreates = $false
foreach ($l in $lines) {
  $t = $l.Trim()
  if ($t -match '^QScriptValue qtscript_create_(\w+)_class\(QScriptEngine \*engine\);$') { $decls += $Matches[1] }
  if ($t -match 'class_names') { $inNames = $true; continue }
  if ($inNames) {
    if ($t -match '"(\w+)"') { $names += $Matches[1] }
    elseif ($t -match '^\};') { $inNames = $false; $inCreates = $true; continue }
  } elseif ($inCreates) {
    if ($t -match '(qtscript_create_\w+_class)') { $creates += $Matches[1] }
    elseif ($t -match '^\};') { $inCreates = $false }
  }
}
if ($decls.Count -ne $names.Count -or $decls.Count -ne $creates.Count) {
  throw ('array length mismatch: decls=' + $decls.Count + ' names=' + $names.Count + ' creates=' + $creates.Count)
}
for ($i = 0; $i -lt $decls.Count; $i++) {
  if ($names[$i] -ne $decls[$i] -or $creates[$i] -ne ('qtscript_create_' + $decls[$i] + '_class')) {
    throw ('array misalignment at ' + $i)
  }
}
Write-Output ('curated GUI registration applied and verified: ' + $decls.Count + ' classes')
