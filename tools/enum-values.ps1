#requires -Version 5.1
<#
.SYNOPSIS
    Check every enum value the bindings expose against what Qt itself says.

.DESCRIPTION
    The generated tree carries, for each bound enum, a `_values[]` array whose
    entries name Qt constants (`QSizePolicy::Fixed`) and a `_keys[]` array with
    the script-visible names. Both are read here, then:

      1. a small C++ program is generated from the same entries, compiled
         against the Qt headers and run - it prints the value Qt gives to each
         constant;
      2. the script-visible constants are read at runtime from the built
         bindings (`Class.Key`, the flattened form QtScript uses) and compared.

    The oracle is Qt, not the generator: a value the generator resolved wrongly
    (the F7 and F8 failures) shows up as a mismatch here, and a constant that Qt
    does not define any more fails the oracle compilation.

    Output is a generated smoke suite under logs/, like the surface coverage
    suite, so no generated file is committed.

.PARAMETER GeneratedRoot
    The generated projects to read. Default: generated_cpp next to the
    repository.

.PARAMETER QtPrefix
    Qt installation the oracle is compiled against (the same one the bindings
    were built with).

.PARAMETER OutputPath
    Where the generated smoke suite is written. Default:
    logs/enum-values-smoke.js.

.PARAMETER Report
    Only print what was found; do not compile or write the suite.
#>
[CmdletBinding()]
param(
    [string] $GeneratedRoot = "",
    [string] $QtPrefix = "",
    [string] $OutputPath = "",
    [switch] $Report
)

$ErrorActionPreference = "Stop"
$repoRoot = Split-Path $PSScriptRoot -Parent
if (-not $GeneratedRoot) { $GeneratedRoot = Join-Path $repoRoot "generated_cpp" }
if (-not $OutputPath) { $OutputPath = Join-Path $repoRoot "logs\enum-values-smoke.js" }
$GeneratedRoot = [System.IO.Path]::GetFullPath($GeneratedRoot)

if (-not (Test-Path -LiteralPath $GeneratedRoot -PathType Container)) {
    throw "Generated projects not found: $GeneratedRoot"
}

function Read-EnumBlocks([string] $root) {
    $blocks = New-Object System.Collections.ArrayList
    $script:structuralErrors = New-Object System.Collections.ArrayList
    $files = @(Get-ChildItem -LiteralPath $root -Recurse -Filter "qtscript_*.cpp" -File)
    foreach ($file in $files) {
        $text = [System.IO.File]::ReadAllText($file.FullName)
        $module = $file.Directory.Name
        # static const <Type> qtscript_<Class>_<Enum>_values[] = { ... };
        foreach ($m in [regex]::Matches($text, 'static const\s+([^\r\n]+?)\s+(qtscript_\w+_values)\[\]\s*=\s*\{(.*?)\};', 'Singleline')) {
            $valuesName = $m.Groups[2].Value
            $valuesBody = $m.Groups[3].Value

            $keysName = $valuesName.Substring(0, $valuesName.Length - "_values".Length) + "_keys"
            $keysMatch = [regex]::Match($text, 'static const char \* const\s+' + [regex]::Escape($keysName) + '\[\]\s*=\s*\{(.*?)\};', 'Singleline')
            if (-not $keysMatch.Success) { continue }

            $values = @()
            foreach ($part in $valuesBody -split ',') {
                $v = $part.Trim()
                if ($v.Length -gt 0) { $values += $v }
            }
            $keys = @()
            foreach ($k in [regex]::Matches($keysMatch.Groups[1].Value, '"((?:[^"\\]|\\.)*)"')) {
                $keys += $k.Groups[1].Value
            }

            # The array name says where the binding attaches the constants:
            # `qtscript_<Class>_<Enum>_values`. The declared type names the C++
            # enum, which is not always the script class (the global enums land
            # on `Global`, and an enum inherited from a base keeps the derived
            # class). A values/keys count mismatch is a structural error, not a
            # reason to skip: the F8 failure reduced the values array to one
            # entry while the keys kept all of them.
            $stem = $valuesName.Substring("qtscript_".Length, $valuesName.Length - "qtscript_".Length - "_values".Length)
            $sep = $stem.IndexOf("_")
            if ($sep -le 0) {
                Write-Warning ("{0}: cannot read the class from {1}, skipped" -f $module, $valuesName)
                continue
            }
            $owner = $stem.Substring(0, $sep)
            $enumName = $stem.Substring($sep + 1)

            if ($values.Count -ne $keys.Count) {
                $null = $structuralErrors.Add([pscustomobject]@{
                    Module = $module
                    Class  = $owner
                    Enum   = $enumName
                    Detail = ("{0} values for {1} keys" -f $values.Count, $keys.Count)
                })
                continue
            }
            if ($values.Count -eq 0) { continue }

            $null = $blocks.Add([pscustomobject]@{
                Module   = $module
                Class    = $owner
                Enum     = $enumName
                Values   = $values
                Keys     = $keys
            })
        }
    }
    return $blocks
}

$blocks = Read-EnumBlocks $GeneratedRoot
$structuralErrors = @($script:structuralErrors)
$count = 0
foreach ($b in $blocks) { $count += $b.Values.Count }
Write-Host ("Enum values: {0} enums, {1} constants, {2} modules" -f $blocks.Count, $count, (@($blocks | ForEach-Object { $_.Module } | Sort-Object -Unique)).Count)
if ($structuralErrors.Count -ne 0) {
    foreach ($e in $structuralErrors) { Write-Warning ("{0}: {1}::{2} has {3}" -f $e.Module, $e.Class, $e.Enum, $e.Detail) }
}

if ($Report) {
    foreach ($b in $blocks) {
        Write-Host ("  {0,-4} {1}::{2} - {3} values" -f "", $b.Class, $b.Enum, $b.Values.Count)
    }
    return
}

# --- Oracle -----------------------------------------------------------------
# One translation unit with every Qt module the tree covers. The constants are
# copied verbatim from the generated tree, so the oracle compiles exactly what
# the bindings compiled; a constant Qt removed since would fail here.

if (-not $QtPrefix) { $QtPrefix = $env:QT_ROOT_DIR }
if (-not $QtPrefix) { throw "QtPrefix is required to compile the enum oracle." }
$QtPrefix = [System.IO.Path]::GetFullPath($QtPrefix)

$work = Join-Path ([System.IO.Path]::GetTempPath()) ("qtscript-enum-oracle-" + [guid]::NewGuid().ToString("N").Substring(0, 8))
New-Item -ItemType Directory -Force -Path $work | Out-Null
$oracleCpp = Join-Path $work "enum-oracle.cpp"
$oracleExe = Join-Path $work "enum-oracle.exe"

$modules = @($blocks | ForEach-Object { $_.Module } | Sort-Object -Unique)
$includeLines = New-Object System.Collections.ArrayList
foreach ($em in @("QtCore", "QtGui", "QtWidgets", "QtNetwork", "QtSql", "QtXml", "QtSvg", "QtPrintSupport", "QtUiTools")) {
    $header = Join-Path $QtPrefix ("include\" + $em + "\" + $em)
    if (Test-Path -LiteralPath $header) { $null = $includeLines.Add("#include <$em/$em>") }
}

$sb = New-Object System.Text.StringBuilder
$null = $sb.AppendLine("// Generated by tools/enum-values.ps1 - enum oracle, not part of the build.")
foreach ($line in $includeLines) { $null = $sb.AppendLine($line) }
$null = $sb.AppendLine("#include <cstdio>")
$null = $sb.AppendLine("int main()")
$null = $sb.AppendLine("{")
foreach ($b in $blocks) {
    for ($i = 0; $i -lt $b.Values.Count; ++$i) {
        $label = $b.Module + "|" + $b.Class + "|" + $b.Enum + "|" + $b.Keys[$i]
        $expr = $b.Values[$i]
        $null = $sb.AppendLine(('    std::printf("%s\t%lld\n", "' + $label + '", (long long)(' + $expr + '));'))
    }
}
$null = $sb.AppendLine("    return 0;")
$null = $sb.AppendLine("}")
[System.IO.File]::WriteAllText($oracleCpp, $sb.ToString(), (New-Object System.Text.UTF8Encoding($false)))

$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path -LiteralPath $vswhere)) { throw "vswhere.exe not found; a Visual Studio installation is required to compile the oracle." }
$vsRoot = (& $vswhere -latest -property installationPath).Trim()
$vcvars = Join-Path $vsRoot "VC\Auxiliary\Build\vcvars64.bat"
if (-not (Test-Path -LiteralPath $vcvars)) { throw "vcvars64.bat not found under $vsRoot" }

$includeArgs = New-Object System.Collections.ArrayList
$null = $includeArgs.Add(('"' + (Join-Path $QtPrefix "include") + '"'))
foreach ($em in @("QtCore", "QtGui", "QtWidgets", "QtNetwork", "QtSql", "QtXml", "QtSvg", "QtPrintSupport", "QtUiTools")) {
    $inc = Join-Path $QtPrefix ("include\" + $em)
    if (Test-Path -LiteralPath $inc) { $null = $includeArgs.Add(('"' + $inc + '"')) }
}
# Q_OBJECT-bearing headers pull in QMetaObject, but nothing is instantiated that
# needs a Qt library at link time: only constants are read.
$cmd = 'call "' + $vcvars + '" >nul 2>&1 && cl /nologo /std:c++17 /Zc:__cplusplus /permissive- /utf-8 /EHsc /W0 ' +
       (($includeArgs | ForEach-Object { "/I$_" }) -join ' ') +
       ' /Fe:"' + $oracleExe + '" /Fo:"' + (Join-Path $work "oracle.obj") + '" "' + $oracleCpp + '"' +
       ' /link /LIBPATH:"' + (Join-Path $QtPrefix "lib") + '" Qt6Core.lib'
$compileLog = Join-Path $work "compile.log"
$null = & cmd.exe /d /c ($cmd + ' > "' + $compileLog + '" 2>&1')
if (-not (Test-Path -LiteralPath $oracleExe)) {
    $tail = @(Get-Content -LiteralPath $compileLog -ErrorAction SilentlyContinue | Select-Object -Last 30)
    throw ("The enum oracle does not compile against {0}:{1}{2}" -f $QtPrefix, [Environment]::NewLine, ($tail -join [Environment]::NewLine))
}

$oracleOut = Join-Path $work "oracle.tsv"
& $oracleExe > $oracleOut
if ($LASTEXITCODE -ne 0) { throw "The enum oracle failed to run (exit $LASTEXITCODE)." }

# --- Generated suite --------------------------------------------------------
$entries = @()
foreach ($line in Get-Content -LiteralPath $oracleOut -Encoding UTF8) {
    if (-not $line) { continue }
    $parts = $line -split "`t"
    if ($parts.Count -ne 2) { throw "Unexpected oracle line: $line" }
    $entries += ($parts[0] + "|" + $parts[1])
}
Write-Host ("Oracle: {0} constants compiled against {1}" -f $entries.Count, (Split-Path $QtPrefix -Leaf))

$js = New-Object System.Text.StringBuilder
$null = $js.AppendLine("// Generated by tools/enum-values.ps1 - do not edit; see generator/AUDIT-qt6.md (F7, F8).")
$null = $js.AppendLine("// Every constant the generated tree exposes is compared with the value Qt")
$null = $js.AppendLine("// compiles for it: the oracle is built from the Qt headers, not from the generator.")
$null = $js.AppendLine("var entries = [")
for ($i = 0; $i -lt $entries.Count; ++$i) {
    $comma = ","
    if ($i -eq $entries.Count - 1) { $comma = "" }
    $null = $js.AppendLine('  "' + $entries[$i] + '"' + $comma)
}
$null = $js.AppendLine("];")
$null = $js.AppendLine("// A mismatch between the number of values and of keys means the generator")
$null = $js.AppendLine("// dropped enumerators: F8 reduced QLocalSocket's array to one entry.")
$null = $js.AppendLine("var structural = [")
for ($i = 0; $i -lt $structuralErrors.Count; ++$i) {
    $e = $structuralErrors[$i]
    $comma = ","
    if ($i -eq $structuralErrors.Count - 1) { $comma = "" }
    $null = $js.AppendLine('  "' + $e.Module + '|' + $e.Class + '|' + $e.Enum + '|' + $e.Detail + '"' + $comma)
}
$null = $js.AppendLine("];")
$null = $js.AppendLine(@'
if (structural.length !== 0) {
    var broken = [];
    for (var i = 0; i < structural.length; ++i) {
        var s = structural[i].split("|");
        broken.push(s[0] + " : " + s[1] + "::" + s[2] + " (" + s[3] + ")");
    }
    throw new Error("enum arrays disagree with their keys: " + broken.join("; "));
}
var checked = 0;
var missing = [];
var mismatch = [];
for (var i = 0; i < entries.length; ++i) {
    var f = entries[i].split("|");
    var module = f[0], className = f[1], enumName = f[2], key = f[3], expected = Number(f[4]);
    var holder = null;
    try { holder = this[className]; } catch (e) { holder = null; }
    var actual;
    if (holder) {
        if (holder[key] !== undefined) {
            actual = holder[key];
        } else if (holder[enumName] !== undefined && holder[enumName][key] !== undefined) {
            actual = holder[enumName][key];
        }
    }
    if (actual === undefined) {
        missing.push(module + " : " + className + "." + key);
        continue;
    }
    if (Number(actual) !== expected) {
        mismatch.push(module + " : " + className + "." + key + " = " + Number(actual) + ", Qt says " + expected);
    }
    ++checked;
}
if (missing.length !== 0)
    throw new Error("enum constants not reachable from script: " + missing.join("; "));
if (mismatch.length !== 0)
    throw new Error("enum values differ from Qt: " + mismatch.join("; "));
if (checked !== entries.length)
    throw new Error("only " + checked + " of " + entries.length + " enum constants were checked");
'@)

$dir = Split-Path $OutputPath -Parent
if (-not (Test-Path -LiteralPath $dir)) { New-Item -ItemType Directory -Force -Path $dir | Out-Null }
[System.IO.File]::WriteAllText($OutputPath, $js.ToString(), (New-Object System.Text.UTF8Encoding($false)))

Remove-Item -LiteralPath $work -Recurse -Force -ErrorAction SilentlyContinue
Write-Host ("Enum values suite written: {0} constants -> {1}" -f $entries.Count, $OutputPath)
