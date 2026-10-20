<#
.SYNOPSIS
    Check the emitted binding surface against a versioned contract.

.DESCRIPTION
    The contract, generator/surface-contract.json, lists every class and every
    member (per module, split static/prototype) the bindings emitted at the
    last reviewed generation. The check reads the tree the generator just
    wrote and fails when a name the contract carries is gone:

    - contract class missing from the tree, or contract module gone;
    - contract member missing from the class (a member that moved between the
      static and prototype sections is one).

    Names that exist now but are not in the contract do not fail the check:
    they are new bindings, they are reported in the check log and they enter
    the contract visibly in git at the next -Update. This makes the contract
    the complement of tools/reject-budget.ps1, which watches the generator's
    rejections; the contract watches what the generator emitted, so a loss the
    generator does not log (a parser change, a curation error, a template
    alias shift) fails the run too.

    The flow is the one of the budget: the check runs in CI right after the
    generation step; the contract is updated consciously with

        ./tools/surface-contract.ps1 -GeneratedRoot ./generated_cpp -Update

    and reviewed through the git diff: additions are written back plainly, a
    loss only with -Force, deliberately, for the few cases where Qt itself
    dropped the member.

.PARAMETER GeneratedRoot
    Directory holding the generated binding projects (generated_cpp/). The
    check needs the full tree, all modules: a single-module regeneration must
    not be compared against (and must not silently shrink) a contract that
    covers the whole surface, so a partial tree is refused.

.PARAMETER Contract
    Contract file to work with. Default: generator\surface-contract.json next
    to this tool's repository.

.PARAMETER Update
    Write the measured surface back to the contract. Refused when it would
    remove names from an existing contract, unless -Force is given.

.PARAMETER Force
    With -Update, allow writing back a contract that loses names.

.EXAMPLE
    ./tools/surface-contract.ps1 -GeneratedRoot ./generated_cpp

.EXAMPLE
    ./tools/surface-contract.ps1 -GeneratedRoot ./generated_cpp -Update
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)] [string] $GeneratedRoot,

    [string] $Contract = "",

    [switch] $Update,

    [switch] $Force
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path $PSScriptRoot -Parent
if (-not $Contract) { $Contract = Join-Path $repoRoot "generator\surface-contract.json" }
$GeneratedRoot = [System.IO.Path]::GetFullPath($GeneratedRoot)

if (-not (Test-Path -LiteralPath $GeneratedRoot -PathType Container)) {
    throw "Generated projects not found: $GeneratedRoot"
}

. (Join-Path $PSScriptRoot "surface-scan.ps1")

# In step with the floor of surface-coverage.ps1: a partial or broken tree must
# not be compared against the contract at all.
$MinimumClassCount = 400

$surface = Get-ExportedSurface -Root $GeneratedRoot
$classCount = $surface.Classes.Count
$memberCount = [int](($surface.Classes | ForEach-Object { $_.Statics.Count + $_.Prototypes.Count } |
    Measure-Object -Sum).Sum)

if ($classCount -lt $MinimumClassCount) {
    throw "Only $classCount classes were read from $GeneratedRoot (expected at least $MinimumClassCount); the surface contract needs the full tree of all modules."
}

# Measure into ordered maps, sorted with ordinal rules, so that a given tree
# serializes to the same bytes no matter which PowerShell wrote the file.
$measured = [ordered]@{}
foreach ($byModule in ($surface.Classes | Group-Object -Property Module | Sort-Object -Property Name)) {
    $classes = [ordered]@{}
    foreach ($entry in ($byModule.Group | Sort-Object -Property Class)) {
        # Two wrappers could theoretically carry the same class name inside one
        # module; the last one read would win. Nothing of the sort is known
        # today, and -Update would make such a change visible in git.
        $statics = [string[]]@($entry.Statics)
        [array]::Sort($statics, [System.StringComparer]::Ordinal)
        $prototypes = [string[]]@($entry.Prototypes)
        [array]::Sort($prototypes, [System.StringComparer]::Ordinal)
        $classes[$entry.Class] = [ordered]@{ "static" = $statics; "prototype" = $prototypes }
    }
    $measured[$byModule.Name] = $classes
}

function Format-JsonString([string] $s) {
    # Members and class names come from C++ string literals the scanner already
    # read; escape the characters JSON needs, nothing more, so the bytes do not
    # depend on the PowerShell or the JSON serializer that wrote the file.
    $out = New-Object System.Text.StringBuilder
    foreach ($ch in $s.ToCharArray()) {
        $code = [int]$ch
        if ($ch -eq [char]92) { $null = $out.Append("\\") }
        elseif ($ch -eq [char]34) { $null = $out.Append('\"') }
        elseif ($code -lt 0x20) { $null = $out.Append(('\u{0:x4}' -f $code)) }
        else { $null = $out.Append($ch) }
    }
    return $out.ToString()
}

function ConvertTo-ContractJsonText {
    param(
        [System.Collections.IDictionary] $Modules,
        [string] $Comment
    )

    $text = New-Object System.Text.StringBuilder
    $null = $text.AppendLine("{")
    $null = $text.AppendLine('  "$comment": "' + (Format-JsonString $Comment) + '",')

    $moduleOrder = @($Modules.Keys)
    for ($m = 0; $m -lt $moduleOrder.Count; $m++) {
        $module = $moduleOrder[$m]
        $classes = $Modules[$module]
        $classOrder = @($classes.Keys)
        $null = $text.AppendLine('  "' + (Format-JsonString $module) + '": {')
        for ($c = 0; $c -lt $classOrder.Count; $c++) {
            $classKey = $classOrder[$c]
            $sections = $classes[$classKey]
            $null = $text.AppendLine('    "' + (Format-JsonString $classKey) + '": {')
            for ($s = 0; $s -lt 2; $s++) {
                # One name per line: a member that appears or disappears sits on
                # its own line, so the git diff of an -Update reads like the log
                # of what entered or left the surface.
                $section = @("static", "prototype")[$s]
                $names = @($sections[$section])
                $sep = $(if ($c -lt $classOrder.Count - 1 -or $s -eq 0) { "," } else { "" })
                if ($names.Count -eq 0) {
                    $null = $text.AppendLine('      "' + $section + '": []' + $sep)
                    continue
                }
                $null = $text.AppendLine('      "' + $section + '": [')
                for ($i = 0; $i -lt $names.Count; $i++) {
                    $comma = $(if ($i -lt $names.Count - 1) { "," } else { "" })
                    $null = $text.Append('        "' + (Format-JsonString $names[$i]) + '"' + $comma)
                    if ($i -lt $names.Count - 1) { $null = $text.AppendLine("") }
                }
                $null = $text.AppendLine("")
                $null = $text.Append("      ]")
                if ($s -eq 0) { $null = $text.AppendLine(",") } else { $null = $text.AppendLine("") }
            }
            if ($c -lt $classOrder.Count - 1) { $null = $text.AppendLine("    },") } else { $null = $text.AppendLine("    }") }
        }
        $tail = $(if ($m -lt $moduleOrder.Count - 1) { "," } else { "" })
        $null = $text.AppendLine("  }" + $tail)
    }
    $null = $text.AppendLine("}")
    return $text.ToString()
}

$commentText = @(
    "The emitted surface of the last reviewed generation: every class and member (static and",
    "prototype sections) the generator wrote, per module, read by tools/surface-scan.ps1.",
    "tools/surface-contract.ps1 checks it in CI right after the generation step: a name in this",
    "file missing from the freshly regenerated tree fails the run. This is the complement of",
    "reject-budget.json, which watches the generator's rejections, while this one watches what",
    "the generator emitted. New names are allowed and reported; adopt them by running",
    "./tools/surface-contract.ps1 -GeneratedRoot ./generated_cpp -Update and reviewing the git",
    "diff. A loss is written back only with -Update -Force, deliberately. A single-module",
    "regeneration must not be fed here: a tree with too few classes is refused."
) -join " "

if ($Update) {
    if (Test-Path -LiteralPath $Contract) {
        $recordedJson = ConvertFrom-Json -InputObject ([System.IO.File]::ReadAllText($Contract))

        $losses = New-Object System.Collections.Generic.List[string]
        foreach ($moduleProp in ($recordedJson.PSObject.Properties | Where-Object { $_.Name -ne '$comment' })) {
            $module = $moduleProp.Name
            if (-not $measured.Contains($module)) {
                $losses.Add("$module : whole module missing from the tree")
                continue
            }
            foreach ($classProp in $moduleProp.Value.PSObject.Properties) {
                $classKey = $classProp.Name
                if (-not $measured[$module].Contains($classKey)) {
                    $losses.Add("$module : whole class missing from the tree")
                    continue
                }
                foreach ($section in @("static", "prototype")) {
                    $now = New-Object 'System.Collections.Generic.HashSet[string]'
                    $null = $now.UnionWith([string[]]@($measured[$module][$classKey][$section]))
                    foreach ($name in @($classProp.Value.$section)) {
                        if (-not $now.Contains($name)) {
                            $losses.Add("$module : $classKey.$name")
                        }
                    }
                }
            }
        }

        if ($losses.Count -gt 0 -and -not $Force) {
            $shown = $losses | Select-Object -First 40
            $more = $(if ($losses.Count -gt 40) { ", and " + ($losses.Count - 40) + " more" } elseif ($losses.Count -gt 1) { ", and 1 more" } else { "" })
            $hint = $(if ($losses.Count -gt 1) { "names" } else { "name" })
            throw ("Refusing to write the contract: {0} {1} of the current contract {2} gone from {3}: {4}{5}Review the regeneration first; only write the loss back with -Update -Force after a deliberate decision (e.g. Qt removed the member)." -f `
                $losses.Count, $hint, $(if ($losses.Count -gt 1) { "are" } else { "is" }), $GeneratedRoot, (($shown -join "; ") + $more + ". "), "")
        }
    } elseif ($Force) {
        throw "-Force is only meaningful with an existing contract (there is nothing to lose yet)."
    }

    $jsonText = ConvertTo-ContractJsonText -Modules $measured -Comment $commentText
    $utf8NoBom = New-Object System.Text.UTF8Encoding($false)
    [System.IO.File]::WriteAllText($Contract, $jsonText, $utf8NoBom)
    Write-Host ("Surface contract written: {0} classes, {1} members, {2} modules -> {3}" -f `
        $classCount, $memberCount, $measured.Keys.Count, $Contract)
    exit 0
}

# --- check mode -------------------------------------------------------------
if (-not (Test-Path -LiteralPath $Contract)) {
    throw "Contract not found: $Contract (run with -Update against a reviewed tree to write it first)"
}

$recordedJson = ConvertFrom-Json -InputObject ([System.IO.File]::ReadAllText($Contract))

$moduleCount = 0
$contractLosses = New-Object System.Collections.Generic.List[string]
$contractAdditions = New-Object System.Collections.Generic.List[string]
$contractClassCount = 0
$contractMemberCount = 0

foreach ($moduleProp in ($recordedJson.PSObject.Properties | Where-Object { $_.Name -ne '$comment' })) {
    $module = $moduleProp.Name
    $moduleCount++
    if (-not $measured.Contains($module)) {
        $contractLosses.Add("$module : whole module missing from the tree")
        continue
    }
    foreach ($classProp in $moduleProp.Value.PSObject.Properties) {
        $classKey = $classProp.Name
        $contractClassCount++
        if (-not $measured[$module].Contains($classKey)) {
            $contractLosses.Add("$module : $classKey (whole class)")
            continue
        }
        foreach ($section in @("static", "prototype")) {
            $names = @($classProp.Value.$section)
            $contractMemberCount += $names.Count
            $now = New-Object 'System.Collections.Generic.HashSet[string]'
            $null = $now.UnionWith([string[]]@($measured[$module][$classKey][$section]))
            foreach ($name in $names) {
                if (-not $now.Contains($name)) {
                    $contractLosses.Add("$module : $classKey.$name")
                }
            }
            foreach ($newName in $now) {
                $known = New-Object 'System.Collections.Generic.HashSet[string]'
                $null = $known.UnionWith([string[]]$names)
                if (-not $known.Contains($newName)) {
                    $contractAdditions.Add("$module : $classKey.$newName")
                }
            }
        }
    }
    # Classes that exist now but are not in the contract.
    foreach ($classKey in $measured[$module].Keys) {
        $inContract = $false
        foreach ($classProp in $moduleProp.Value.PSObject.Properties) {
            if ($classProp.Name -eq $classKey) { $inContract = $true; break }
        }
        if (-not $inContract) {
            $n = (@($measured[$module][$classKey]["static"]).Count + @($measured[$module][$classKey]["prototype"]).Count)
            $contractAdditions.Add("$module : $classKey (new class, $n members)")
        }
    }
}

Write-Host ("Surface contract: {0} classes, {1} members, {2} modules measured; contract {3} classes, {4} members, {5} modules" -f `
    $classCount, $memberCount, $measured.Keys.Count, $contractClassCount, $contractMemberCount, $moduleCount)

if ($contractLosses.Count -gt 0) {
    # Whole-class and whole-module entries already say why; name members first so
    # the log opens on the exact losses, keep at most 40 lines readable in CI.
    $shown = $contractLosses | Select-Object -First 40
    $more = $(if ($contractLosses.Count -gt 40) { " ... and " + ($contractLosses.Count - 40) + " more" } else { "" })
    $hint = $(if ($contractLosses.Count -gt 1) { "names" } else { "name" })
    throw ("Surface contract violated: {0} {1} the bindings emitted at the last review {2} missing from {3}: {4}" -f `
        $contractLosses.Count, $hint, $(if ($contractLosses.Count -gt 1) { "are" } else { "is" }), $GeneratedRoot, (($shown -join "; ") + $more))
}

if ($contractAdditions.Count -gt 0) {
    $shown = $contractAdditions | Select-Object -First 40
    Write-Host ("  {0} name(s) not in the contract (reported, not failed - adopt with -Update):" -f $contractAdditions.Count)
    $shown | ForEach-Object { Write-Host ("    + " + $_) }
    if ($contractAdditions.Count -gt 40) {
        Write-Host ("    + ... and " + ($contractAdditions.Count - 40) + " more")
    }
}
exit 0
