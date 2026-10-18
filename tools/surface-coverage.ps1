<#
.SYNOPSIS
    Emits the runtime surface-coverage smoke suite from the generated bindings.

.DESCRIPTION
    The suite asserts one thing for every class and every member the generator
    wrote into generated_cpp/: that it is still reachable from script in the
    plugin that was built from that output. A class whose global went away, or a
    member the plugin no longer installs, fails the run with the class and the
    member named -- which is what the hand-written suites, covering a fraction of
    the surface, could not report.

    The suite is derived from the projects the generator just wrote, so it needs
    no baseline and follows the Qt 6.12 and Qt 6.8 LTS lines alike. It is a test
    artefact: test.ps1 writes it under logs/ and it is never committed.

    What it proves: emitted surface -> reachable surface, end to end through the
    plugin registration.
    What it does not prove: that the emitted surface is what Qt offers. A member
    the generator never wrote cannot be missing here; that loss is what the
    generator's reject log and the surface diff of the audit are for.

.PARAMETER GeneratedRoot
    Directory holding the generated binding projects (generated_cpp/).

.PARAMETER OutputPath
    The .js suite to write.

.PARAMETER Module
    Restrict to these binding modules (core, gui, network, ...). Default: every
    module found under GeneratedRoot.

.PARAMETER MinimumClassCount
    Refuse to write a suite that carries fewer classes than this: an extraction
    that read nothing must not pass as a green run. Default 400 (Qt 6.12: 457).

.PARAMETER MinimumMemberCount
    Same guard for members. Default 7000 (Qt 6.12: 8808).

.EXAMPLE
    ./tools/surface-coverage.ps1 -GeneratedRoot ./generated_cpp -OutputPath ./logs/surface-coverage-smoke.js
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)] [string] $GeneratedRoot,

    [Parameter(Mandatory = $true)] [string] $OutputPath,

    [string[]] $Module = @(),

    [int] $MinimumClassCount = 400,

    [int] $MinimumMemberCount = 7000
)

$ErrorActionPreference = "Stop"

$GeneratedRoot = [System.IO.Path]::GetFullPath($GeneratedRoot)
if (-not (Test-Path -LiteralPath $GeneratedRoot -PathType Container)) {
    throw "Generated projects not found: $GeneratedRoot"
}

# Reads the function-name table the generator writes at the top of every wrapper:
#
#   static const char * const qtscript_QPolygon_function_names[] = {
#   "QPolygon"
#   // static
#   , "fromVector"
#   // prototype
#   , "append"
#   };
#
# The first entry is the class name, the two sections are its static members and
# its prototype members. Wrappers under generator/curated/ that carry no such
# table (enum and flag namespaces) are reported and left out.
function Get-ExportedSurface {
    param(
        [Parameter(Mandatory = $true)] [string] $Root,
        [string[]] $Modules
    )

    $classes = New-Object System.Collections.Generic.List[object]
    $skipped = New-Object System.Collections.Generic.List[string]

    foreach ($file in Get-ChildItem -LiteralPath $Root -Recurse -Filter "qtscript_*.cpp" -File | Sort-Object FullName) {
        $moduleDirectory = Split-Path (Split-Path $file.FullName -Parent) -Leaf
        if ($moduleDirectory -notmatch '^com_trolltech_qt_(?<module>.+)$') {
            continue
        }
        $moduleName = $Matches['module']
        if ($Modules.Count -gt 0 -and $Modules -notcontains $moduleName) {
            continue
        }

        $className = $null
        $section = "class"
        $inTable = $false
        $statics = New-Object System.Collections.Generic.List[string]
        $prototypes = New-Object System.Collections.Generic.List[string]

        foreach ($line in [System.IO.File]::ReadAllLines($file.FullName)) {
            if (-not $inTable) {
                if ($line -match '^\s*(?:static\s+)?const char \* const qtscript_\w+_function_names\[\]\s*=\s*\{') {
                    $inTable = $true
                }
                continue
            }
            if ($line -match '^\s*}') {
                break
            }
            if ($line -match '^\s*//\s*static\b') {
                $section = "static"
                continue
            }
            if ($line -match '^\s*//\s*prototype\b') {
                $section = "prototype"
                continue
            }
            if ($line -match '^\s*//') {
                continue
            }
            if ($line -match '"([^"]*)"') {
                $member = $Matches[1]
                if ($section -eq "class") {
                    $className = $member
                } elseif ($section -eq "static") {
                    $statics.Add($member)
                } else {
                    $prototypes.Add($member)
                }
            }
        }

        if (-not $className) {
            $skipped.Add("$($file.Name) (no function-name table: curated wrapper)")
        } elseif ($className -notmatch '^[A-Za-z_][A-Za-z0-9_]*$') {
            $skipped.Add("$($file.Name) (class name is not an identifier: $className)")
        } else {
            $classes.Add([pscustomobject]@{
                Module = $moduleName
                Class = $className
                Statics = @($statics)
                Prototypes = @($prototypes)
            })
        }
    }

    return [pscustomobject]@{ Classes = $classes; Skipped = $skipped }
}

function ConvertTo-JavaScriptStringArray {
    param([string[]] $Values)

    $quoted = foreach ($value in $Values) {
        '"' + $value.Replace('\', '\\').Replace('"', '\"') + '"'
    }
    return "[" + ($quoted -join ",") + "]"
}

$surface = Get-ExportedSurface -Root $GeneratedRoot -Modules $Module
$classCount = $surface.Classes.Count
$memberCount = [int](($surface.Classes | ForEach-Object { $_.Statics.Count + $_.Prototypes.Count } |
    Measure-Object -Sum).Sum)

if ($classCount -lt $MinimumClassCount) {
    throw "Only $classCount classes were read from $GeneratedRoot (expected at least $MinimumClassCount): the generated projects are incomplete or unreadable."
}
if ($memberCount -lt $MinimumMemberCount) {
    throw "Only $memberCount members were read from $GeneratedRoot (expected at least $MinimumMemberCount): the generated projects are incomplete or unreadable."
}

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add(@'
// Runtime surface coverage: every class and every member the generator emitted
// into generated_cpp/ must be reachable from script in the built bindings.
//
// Generated by tools/surface-coverage.ps1 -- do not edit by hand; the file is
// written under logs/ on every test.ps1 run and is never committed.
//
// What this proves: the emitted surface is instantiated by the binding plugin.
// What it does not prove: that the emitted surface is what Qt 6 offers; a member
// the generator never wrote cannot be missing here.
'@)
$lines.Add("// Classes: $classCount. Members: $memberCount.")
$lines.Add("")
$lines.Add("function check(condition, message) {")
$lines.Add("    if (!condition)")
$lines.Add("        throw new Error(message);")
$lines.Add("}")
$lines.Add("")
$lines.Add("// [module, class, static members, prototype members]")
$lines.Add("var SURFACE = [")
foreach ($group in ($surface.Classes | Group-Object Module | Sort-Object Name)) {
    $lines.Add("// " + $group.Name)
    foreach ($entry in ($group.Group | Sort-Object Class)) {
        $statics = ConvertTo-JavaScriptStringArray -Values $entry.Statics
        $prototypes = ConvertTo-JavaScriptStringArray -Values $entry.Prototypes
        $lines.Add('["' + $group.Name + '","' + $entry.Class + '",' + $statics + ',' + $prototypes + '],')
    }
}
$lines.Add("];")
$lines.Add("")
$lines.Add(@'
// A suite built from nothing must not pass as a green run: the floor is far
// below what a complete generated tree carries, so only a broken extraction or
// a truncated generation trips it.
check(SURFACE.length >= __MIN_CLASSES__, "the suite carries " + SURFACE.length
    + " classes, below the expected floor of __MIN_CLASSES__");

var global = (function () { return this; })();
var classCount = 0;
var memberCount = 0;
var missing = [];

for (var i = 0; i < SURFACE.length; ++i) {
    var entry = SURFACE[i];
    var module = entry[0];
    var name = entry[1];
    var statics = entry[2];
    var prototypes = entry[3];
    var cls = global[name];
    if (cls === undefined || cls === null) {
        missing.push(module + ":" + name + " (class)");
        continue;
    }
    ++classCount;
    var proto = (typeof cls === "function" && cls.prototype) ? cls.prototype : null;
    var j;
    var member;
    for (j = 0; j < statics.length; ++j) {
        member = statics[j];
        if (cls[member] !== undefined)
            ++memberCount;
        else
            missing.push(module + ":" + name + "." + member + " (static)");
    }
    for (j = 0; j < prototypes.length; ++j) {
        member = prototypes[j];
        // A namespace class (Qt, QSsl, QSql) is registered as a plain object and
        // keeps its members on the object itself, without a prototype.
        if (proto !== null && proto[member] !== undefined)
            ++memberCount;
        else if (cls[member] !== undefined)
            ++memberCount;
        else
            missing.push(module + ":" + name + "." + member);
    }
}

if (missing.length > 0)
    throw new Error("surface coverage: " + classCount + "/" + SURFACE.length + " classes and "
        + memberCount + " members reachable, " + missing.length + " missing: "
        + missing.slice(0, 60).join(", ")
        + (missing.length > 60 ? " (+" + (missing.length - 60) + " more)" : ""));
'@)

$text = ($lines -join "`n")
$text = $text.Replace('__MIN_CLASSES__', $MinimumClassCount)
$directory = Split-Path -Parent ([System.IO.Path]::GetFullPath($OutputPath))
if ($directory -and -not (Test-Path -LiteralPath $directory -PathType Container)) {
    New-Item -ItemType Directory -Force -Path $directory | Out-Null
}
[System.IO.File]::WriteAllText($OutputPath, $text + "`n", (New-Object System.Text.UTF8Encoding($false)))

Write-Host ("Surface coverage suite: {0} classes, {1} members, {2} modules -> {3}" -f `
    $classCount, $memberCount, ($surface.Classes | Group-Object Module).Count, $OutputPath)
foreach ($entry in $surface.Skipped) {
    Write-Host "  not covered: $entry"
}
