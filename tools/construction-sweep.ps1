#requires -Version 5.1
<#
.SYNOPSIS
    Emits the runtime construction sweep from the generated bindings.

.DESCRIPTION
    Three things are asserted here for every class the generator emitted:

      1. the class is registered in script (`typeof Class === "function"`);
      2. it carries a prototype object (its instance members have somewhere to
         live);
      3. when the generated constructor accepts no argument, `new Class()` builds
         an object instead of throwing, which exercises the constructor body, the
         shell class of a QObject with virtuals, and the ownership handover the
         wrapper performs.

    The class list and the 0-argument constructor detection are both read from
    generated_cpp/: a class is constructed only when the generator itself wrote
    an `argumentCount() == 0` branch after the "did you forget to construct with
    'new'" guard, so no class is asked to build something the bindings never
    promised. That is what makes the suite follow the Qt 6.12 and Qt 6.8 LTS
    lines without a baseline.

    What it proves: every emitted class is registered, and every emitted
    no-argument constructor runs.
    What it does not prove: that a constructor with arguments works, nor that a
    constructed object behaves.

    The suite is a test artefact: test.ps1 writes it under logs/, never commits
    it. Since a bad constructor can abort the process instead of throwing, the
    suite keeps a progress file beside it, with the class being built on its last
    line, so a native failure names a class instead of an exit code.

.PARAMETER GeneratedRoot
    Directory holding the generated binding projects (generated_cpp/).

.PARAMETER OutputPath
    The .js suite to write.

.PARAMETER MinimumClassCount
    Refuse to write a suite that carries fewer classes than this: an extraction
    that read nothing must not pass as a green run. Default 400 (Qt 6.12: 457).

.EXAMPLE
    ./tools/construction-sweep.ps1 -GeneratedRoot ./generated_cpp -OutputPath ./logs/construction-smoke.js
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)] [string] $GeneratedRoot,

    [Parameter(Mandatory = $true)] [string] $OutputPath,

    [int] $MinimumClassCount = 400
)

$ErrorActionPreference = "Stop"

$GeneratedRoot = [System.IO.Path]::GetFullPath($GeneratedRoot)
if (-not (Test-Path -LiteralPath $GeneratedRoot -PathType Container)) {
    throw "Generated projects not found: $GeneratedRoot"
}

# The class list comes from the same function-name table the surface tools read,
# so the sweep cannot disagree with them about which classes exist.
. (Join-Path $PSScriptRoot "surface-scan.ps1")
$surface = Get-ExportedSurface -Root $GeneratedRoot
if ($surface.Classes.Count -lt $MinimumClassCount) {
    throw "Only $($surface.Classes.Count) classes were read from $GeneratedRoot (expected at least $MinimumClassCount): the generated projects are incomplete or unreadable."
}

# The constructor is emitted in `qtscript_<Class>_static_call`, case 0: the
# "did you forget to construct with 'new'" guard comes first, then one branch per
# overload, and the case ends with `break;`. The search must stop at that break:
# the cases that follow are static member functions, and QItemSelection::maxSize
# (case 2) takes no argument, which made a naive window report a constructor
# that does not exist. A class with no such guard is not constructible from
# script and is only checked for registration.
#
# A namespace class (Qt, QSsl, QSql) is registered as a callable object built on
# an empty prototype on purpose: it carries static members and enums, no
# instances, so it has no prototype to check.
$entries = New-Object System.Collections.Generic.List[object]
$withoutConstructor = 0
foreach ($entry in ($surface.Classes | Sort-Object Module, Class)) {
    $source = Join-Path $GeneratedRoot ("com_trolltech_qt_" + $entry.Module + "\qtscript_" + $entry.Class + ".cpp")
    if (-not (Test-Path -LiteralPath $source -PathType Leaf)) {
        throw "Class $($entry.Class) is listed under $($entry.Module) but $source is missing."
    }
    $text = [System.IO.File]::ReadAllText($source)
    $namespace = $text.Contains('QScriptValue proto = QScriptValue();')
    $constructible = $false
    $staticCall = [regex]::Match($text, 'static QScriptValue qtscript_\w+_static_call\(QScriptContext')
    if ($staticCall.Success) {
        $caseZero = [regex]::Match(
            $text.Substring($staticCall.Index),
            'case 0:\s*([\s\S]*?)\n\s*break;')
        if ($caseZero.Success) {
            $body = $caseZero.Groups[1].Value
            $constructible = $body.Contains('Did you forget to construct') -and
                $body.Contains('argumentCount() == 0')
        }
    }
    if (-not $constructible) { ++$withoutConstructor }
    $entries.Add([pscustomobject]@{
        Module        = $entry.Module
        Class         = $entry.Class
        Constructible = $constructible
        Namespace     = $namespace
    })
}

$constructibleCount = $entries.Count - $withoutConstructor

# The progress file is written by the suite itself, through QFile, so a class
# whose constructor aborts the process is named even though no exception is
# raised. It lives beside the suite, under logs/.
$progress = [System.IO.Path]::ChangeExtension([System.IO.Path]::GetFullPath($OutputPath), ".progress.txt")
$progressJs = $progress.Replace('\', '/').Replace('"', '\"')

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add(@'
// Runtime construction sweep: every class the generator emitted must be
// registered in script, carry a prototype, and - when the generator wrote a
// no-argument constructor - build an object from `new Class()`.
//
// Generated by tools/construction-sweep.ps1 -- do not edit by hand; the file is
// written under logs/ on every test.ps1 run and is never committed.
//
// A constructor that aborts the process cannot throw, so the class being built
// is appended to the progress file named below before each attempt: read its
// last line to learn which class took the process down.
'@)
$lines.Add("")
$lines.Add("var PROGRESS = `"$progressJs`";")
$lines.Add("")
$lines.Add(@'
function check(condition, message) {
    if (!condition)
        throw new Error(message);
}

// [module, class, constructible, namespace]
var SWEEP = [
'@)
foreach ($group in ($entries | Group-Object Module | Sort-Object Name)) {
    $lines.Add("// " + $group.Name)
    foreach ($entry in $group.Group) {
        $flag = "false"
        if ($entry.Constructible) { $flag = "true" }
        $namespace = "false"
        if ($entry.Namespace) { $namespace = "true" }
        $lines.Add('["' + $group.Name + '","' + $entry.Class + '",' + $flag + ',' + $namespace + '],')
    }
}
$lines.Add("];")
$lines.Add("")
$lines.Add(@'
check(SWEEP.length >= __MIN_CLASSES__, "the suite carries " + SWEEP.length
    + " classes, below the expected floor of __MIN_CLASSES__");

var global = (function () { return this; })();
var progress = new QFile(PROGRESS);
if (!progress.open(QIODevice.WriteOnly | QIODevice.Truncate))
    throw new Error("construction sweep cannot write its progress file: " + PROGRESS);

function mark(text) {
    progress.write(new QByteArray(text + "\n"));
    progress.flush();
}

var failures = [];
var registered = 0;
var constructed = 0;

for (var i = 0; i < SWEEP.length; ++i) {
    var module = SWEEP[i][0];
    var name = SWEEP[i][1];
    var constructible = SWEEP[i][2];
    var namespace = SWEEP[i][3];
    var cls = global[name];
    if (typeof cls !== "function") {
        failures.push(module + ":" + name + " is not a constructor function (" + typeof cls + ")");
        continue;
    }
    ++registered;
    if (!namespace && (cls.prototype === undefined || cls.prototype === null)) {
        failures.push(module + ":" + name + " has no prototype");
        continue;
    }
    if (!constructible)
        continue;
    mark(name);
    var object = null;
    try {
        object = new cls();
    } catch (e) {
        failures.push(module + ":" + name + " new " + name + "() threw: " + e);
        continue;
    }
    if (object === undefined || object === null) {
        failures.push(module + ":" + name + " new " + name + "() returned " + object);
        continue;
    }
    ++constructed;
}

progress.close();
if (failures.length > 0)
    throw new Error("construction sweep: " + registered + "/" + SWEEP.length + " registered, "
        + constructed + " constructed, " + failures.length + " failed: "
        + failures.slice(0, 40).join("; ")
        + (failures.length > 40 ? " (+" + (failures.length - 40) + " more)" : ""));
'@)

$text = ($lines -join "`n").Replace('__MIN_CLASSES__', $MinimumClassCount)
$directory = Split-Path -Parent ([System.IO.Path]::GetFullPath($OutputPath))
if ($directory -and -not (Test-Path -LiteralPath $directory -PathType Container)) {
    New-Item -ItemType Directory -Force -Path $directory | Out-Null
}
[System.IO.File]::WriteAllText($OutputPath, $text + "`n", (New-Object System.Text.UTF8Encoding($false)))

Write-Host ("Construction sweep suite: {0} classes, {1} with a no-argument constructor -> {2}" -f `
    $entries.Count, $constructibleCount, $OutputPath)
