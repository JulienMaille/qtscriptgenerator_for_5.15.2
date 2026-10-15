# Post-regen step: register curated (hand-maintained, non-generatable)
# GUI bindings in the fresh generator output. The generator cannot produce
# these because their Qt 6 C++ sources are gone (QMatrix) or need compat
# shims; the .pri/_init.cpp below are regenerated from scratch on every
# regen run, so this must run after each GUI regen, before the DLL build.
$ErrorActionPreference = 'Stop'
$guiDir = 'D:\Codez\qtscriptgenerator_for_5.15.2\regen-test\generated_cpp\com_trolltech_qt_gui'
$curatedDir = 'D:\Codez\qtscriptgenerator-qt6-bindings\generated_cpp\com_trolltech_qt_gui'

# 1. Curated sources (tracked in the bindings repo).
foreach ($f in @('qtscript_QMatrix.cpp')) {
  Copy-Item (Join-Path $curatedDir $f) (Join-Path $guiDir $f) -Force
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
