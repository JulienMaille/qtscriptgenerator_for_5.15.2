# Post-regen step: register curated (hand-maintained, non-generatable)
# Core bindings in the fresh generator output.
#
# The generator parses the Qt 6 masterinclude (QtCore/QtGui/...), so:
#  - QtScript's QRegExp (QtCore in Qt 5, QtScript in Qt 6) is invisible to it;
#  - QStringConverter is generated as an abstract class, while the shipped
#    product exposes it as a namespace object holding only the Encoding enum.
# Both bindings are hand-carried here (the bindings repo owns the curated
# layer) and re-attached after every core regen, before the DLL build. The
# .pri and _init.cpp are rewritten from scratch on each regen run, so this
# must run after each core regen.
$ErrorActionPreference = 'Stop'
$coreDir = 'D:\Codez\qtscriptgenerator_for_5.15.2\regen-test\generated_cpp\com_trolltech_qt_core'
$coreInclude = 'D:\Codez\qtscriptgenerator_for_5.15.2\regen-test\qtbindings\qtscript_core\include'
$curatedDir = 'D:\Codez\qtscriptgenerator-qt6-bindings\generated_cpp\com_trolltech_qt_core'
$bindingsInclude = 'D:\Codez\qtscriptgenerator-qt6-bindings\qtbindings\include'

# 1. Curated sources (tracked in the bindings repo).
foreach ($f in @('qtscript_QStringConverter.cpp', 'qtscript_QRegExp.cpp')) {
  $dest = Join-Path $coreDir $f
  Copy-Item (Join-Path $curatedDir $f) $dest -Force
  # Copy-Item keeps the source timestamp, which may be older than the object
  # file nmake built on the previous run; touch so the TU is recompiled.
  (Get-Item $dest).LastWriteTime = Get-Date
}

# 2. Compat headers the curated sources include (<qregexp.h>).
Copy-Item (Join-Path $bindingsInclude 'qregexp.h') (Join-Path $coreInclude 'qregexp.h') -Force
if (Test-Path (Join-Path $bindingsInclude 'QRegExp')) {
  Copy-Item (Join-Path $bindingsInclude 'QRegExp') (Join-Path $coreInclude 'QRegExp') -Recurse -Force
}

# 3. .pri: compile the curated QRegExp source, drop the generated
#    QStringConverter shell (the curated binding replaces its class form).
$pri = Join-Path $coreDir 'com_trolltech_qt_core.pri'
$lines = [System.IO.File]::ReadAllLines($pri)
$out = @()
$haveQRegExp = $false
foreach ($l in $lines) {
  $t = $l.Trim()
  if ($t -eq '$$PWD/qtscriptshell_QStringConverter.h \') { continue }
  if ($t -eq '$$PWD/qtscriptshell_QStringConverter.cpp \') { continue }
  if ($t -eq '$$PWD/qtscript_QRegExp.cpp \') { continue }
  $out += $l
  if ($t -eq '$$PWD/qtscript_QStringConverter.cpp \') {
    $out += '           $$PWD/qtscript_QRegExp.cpp \'
    $haveQRegExp = $true
  }
}
if (-not $haveQRegExp) { throw 'pri anchor not found' }
[System.IO.File]::WriteAllLines($pri, $out)

# 4. _init.cpp: declare, name, and register the curated class in parallel
#    (arrays must stay aligned).
$init = Join-Path $coreDir 'com_trolltech_qt_core_init.cpp'
$lines = [System.IO.File]::ReadAllLines($init)
$applied = $false
foreach ($l in $lines) { if ($l.Trim() -eq 'QScriptValue qtscript_create_QRegExp_class(QScriptEngine *engine);') { $applied = $true } }
if (-not $applied) {
  $out = @()
  foreach ($l in $lines) {
    $out += $l
    $t = $l.Trim()
    if ($t -eq 'QScriptValue qtscript_create_QStringConverter_class(QScriptEngine *engine);') {
      $out += 'QScriptValue qtscript_create_QRegExp_class(QScriptEngine *engine);'
    } elseif ($t -eq ', "QStringConverter"') {
      $out += '    , "QRegExp"'
    } elseif ($t -eq ', qtscript_create_QStringConverter_class') {
      $out += '    , qtscript_create_QRegExp_class'
    }
  }
  [System.IO.File]::WriteAllLines($init, $out)

  # 5. _init.cpp: extend the hardcoded class count by the curated additions.
  $t = [System.IO.File]::ReadAllText($init)
  if ($t -match 'for \(int i = 0; i < (\d+); \+\+i\) \{') {
    $n = [int]$Matches[1]
    $t = $t.Replace($Matches[0], ('for (int i = 0; i < ' + ($n + 1) + '; ++i) {'))
    [System.IO.File]::WriteAllText($init, $t)
  } else {
    throw 'init count not found'
  }
}

# 6. Verify the three arrays stayed parallel.
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
Write-Output ('curated Core registration applied and verified: ' + $decls.Count + ' classes')
