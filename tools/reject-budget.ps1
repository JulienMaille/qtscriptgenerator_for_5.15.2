<#
.SYNOPSIS
    Compares the per-module reject logs of a regen with the checked-in budget.

.DESCRIPTION
    The generator reports every member it does not bind, with the reason, into
    mjb_rejected_functions.log in its working directory. The file is rewritten on
    every run, so a nine-module regen used to keep only the last module's copy and
    a rejection that appeared in an earlier module was invisible afterwards.
    regen-all.ps1 now moves the logs into <output root>\logs\<module>\ before the
    next module starts, and this tool turns those nine directories into one number
    per module and per reason.

    The budget is an upper bound, one entry per module and reason. It is checked
    in (generator/reject-budget.json) and seeded with -Update from both Qt lines,
    keeping the larger count so the same file serves Qt 6.12 and Qt 6.8 LTS.
    A regen whose count exceeds the budget fails: the new rejection is a member
    the bindings just stopped having. A count below the budget is reported as a
    loose bound, not as a failure -- a fix shrinks the log and the budget should
    follow, by running -Update.

    Counts, not names, on purpose: the reasons are stable categories and the
    counts survive a reordering of the log, while a name list would fail on every
    member that moves from one reason to another. The failure lists the section
    and the delta, and the log it points at still carries every name.

    What it proves: no module rejected more members than the recorded bound, and a
    reason that did not exist when the budget was written is refused outright.
    What it does not prove: that the members inside the budget behave, or that a
    rejection recorded as intentional still is -- the audit review does that.

.PARAMETER LogRoot
    Directory holding <module>\mjb_rejected_functions.log, i.e.
    <output root>\logs as written by regen-all.ps1.

.PARAMETER Budget
    Budget file to check against. Default: generator\reject-budget.json next to
    this tool's repository.

.PARAMETER Update
    Raise the budget to the measured counts and write it back. Only ever raises:
    an entry that is absent is added, an entry that is too low is raised. Run it
    against one Qt line, then against the other, and the result covers both.

.PARAMETER Report
    Print the measured counts per module and reason without judging them.

.EXAMPLE
    ./tools/reject-budget.ps1 -LogRoot ./regen-test/logs

.EXAMPLE
    ./tools/reject-budget.ps1 -LogRoot ./regen-test/logs -Report

.EXAMPLE
    ./tools/reject-budget.ps1 -LogRoot ./regen-test/logs -Update
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)] [string] $LogRoot,

    [string] $Budget = "",

    [switch] $Update,

    [switch] $Report
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path $PSScriptRoot -Parent
if (-not $Budget) { $Budget = Join-Path $repoRoot "generator\reject-budget.json" }
$LogRoot = [System.IO.Path]::GetFullPath($LogRoot)

if (-not (Test-Path -LiteralPath $LogRoot -PathType Container)) {
    throw "Log root does not exist: $LogRoot (run generator\regen-all.ps1 first)"
}

# Reads one reject log into an ordered map: reason -> number of entries.
# The file repeats this shape for every reason:
#     *******************************************************************
#     Unmatched return type
#     *******************************************************************
#     (blank)
#      - QFoo::bar(...)
function Read-RejectLog([string] $Path) {
    $counts = [ordered]@{}
    $reason = $null
    $afterRule = $false
    foreach ($line in [System.IO.File]::ReadLines($Path)) {
        if ($line -match '^\*+$') {
            $afterRule = $true
            continue
        }
        if ($afterRule) {
            $title = $line.Trim()
            if ($title -eq "") { continue }   # blank line between the two rules
            $reason = $title
            if (-not $counts.Contains($reason)) { $counts[$reason] = 0 }
            $afterRule = $false
            continue
        }
        if ($null -ne $reason -and $line -match '^\s+-\s') {
            $counts[$reason] = $counts[$reason] + 1
        }
    }
    return $counts
}

# A module whose logs are missing must not read as "no rejections": that is the
# failure mode this whole mechanism exists to stop.
$measured = [ordered]@{}
foreach ($dir in (Get-ChildItem -LiteralPath $LogRoot -Directory | Sort-Object Name)) {
    $logPath = Join-Path $dir.FullName "mjb_rejected_functions.log"
    if (-not (Test-Path -LiteralPath $logPath)) {
        throw "Module '$($dir.Name)' has no mjb_rejected_functions.log in $($dir.FullName)"
    }
    $measured[$dir.Name] = Read-RejectLog $logPath
}

if ($measured.Count -eq 0) {
    throw "No module directory under $LogRoot (expected <module>\mjb_rejected_functions.log)"
}

if ($Report) {
    foreach ($module in $measured.Keys) {
        Write-Host ("{0}" -f $module)
        foreach ($reason in $measured[$module].Keys) {
            Write-Host ("    {0,-4} {1}" -f $measured[$module][$reason], $reason)
        }
    }
    return
}

$recorded = [ordered]@{}
if (Test-Path -LiteralPath $Budget) {
    $raw = Get-Content -LiteralPath $Budget -Raw | ConvertFrom-Json
    foreach ($property in $raw.PSObject.Properties) {
        if ($property.Name.StartsWith('$')) { continue }
        $entries = [ordered]@{}
        foreach ($reason in $property.Value.PSObject.Properties) {
            $entries[$reason.Name] = [int] $reason.Value
        }
        $recorded[$property.Name] = $entries
    }
}

if ($Update) {
    # Raise only: the file is the bound, and a single Qt line must not lower what
    # the other line needs. $comment from the existing file is preserved.
    $defaultComment = "Upper bound of mjb_rejected_functions.log entries, per module and reason. Seeded with tools/reject-budget.ps1 -Update from Qt 6.12 and Qt 6.8.3; the larger count wins so one file serves both lines. A count that grows is a member the bindings lost and fails the check; a count that shrinks means the bound is loose and should be tightened with -Update."
    $next = [ordered]@{}
    $next['$comment'] = if ($recorded.Count -gt 0 -and $raw.'$comment') { $raw.'$comment' } else { $defaultComment }
    $modules = @($recorded.Keys) + @($measured.Keys) | Sort-Object -Unique
    foreach ($module in $modules) {
        $entries = [ordered]@{}
        $old = if ($recorded.Contains($module)) { $recorded[$module] } else { [ordered]@{} }
        $new = if ($measured.Contains($module)) { $measured[$module] } else { [ordered]@{} }
        $reasons = @($old.Keys) + @($new.Keys) | Sort-Object -Unique
        foreach ($reason in $reasons) {
            $a = if ($old.Contains($reason)) { $old[$reason] } else { 0 }
            $b = if ($new.Contains($reason)) { $new[$reason] } else { 0 }
            $entries[$reason] = [Math]::Max($a, $b)
        }
        $next[$module] = $entries
    }
    $json = $next | ConvertTo-Json -Depth 4
    # ConvertTo-Json escapes the apostrophes and ampersands of the reason titles
    # as \u0027 / \u0026 (its JSON writer escapes them for HTML safety). The file
    # is meant to be read and diffed, so undo that on the two characters the
    # reason titles actually contain.
    $json = $json -replace '\\u0027', "'" -replace '\\u0026', '&'
    [System.IO.File]::WriteAllText($Budget, $json + "`n", (New-Object System.Text.UTF8Encoding($false)))
    Write-Host "Budget written: $Budget"
    return
}

$failures = New-Object 'System.Collections.Generic.List[string]'
$loose = New-Object 'System.Collections.Generic.List[string]'

foreach ($module in $measured.Keys) {
    if (-not $recorded.Contains($module)) {
        $failures.Add("module '$module' has reject logs but no budget entry")
        continue
    }
    foreach ($reason in $measured[$module].Keys) {
        if (-not $recorded[$module].Contains($reason)) {
            $failures.Add(("{0}: reason '{1}' is not in the budget ({2} entries)" -f `
                $module, $reason, $measured[$module][$reason]))
            continue
        }
        $actual = $measured[$module][$reason]
        $bound = $recorded[$module][$reason]
        if ($actual -gt $bound) {
            $failures.Add(("{0}: {1} went from {2} to {3} (+{4})" -f `
                $module, $reason, $bound, $actual, ($actual - $bound)))
        }
        elseif ($actual -lt $bound) {
            $loose.Add(("{0}: {1} is {2}, budget {3}" -f $module, $reason, $actual, $bound))
        }
    }
    foreach ($reason in $recorded[$module].Keys) {
        if (-not $measured[$module].Contains($reason)) {
            $loose.Add(("{0}: reason '{1}' no longer appears, budget {2}" -f `
                $module, $reason, $recorded[$module][$reason]))
        }
    }
}

foreach ($module in $recorded.Keys) {
    if (-not $measured.Contains($module)) {
        $failures.Add("module '$module' is in the budget but produced no reject logs")
    }
}

foreach ($line in $loose) { Write-Host "loose: $line" }

if ($failures.Count -gt 0) {
    foreach ($line in $failures) { Write-Host "FAIL: $line" }
    throw ("{0} reject log entr(y|ies) outside the budget (logs: {1})" -f $failures.Count, $LogRoot)
}

Write-Host ("Reject budget respected: {0} modules, {1} reason(s) (logs: {2})" -f `
    $measured.Count, (($measured.Values | ForEach-Object { $_.Keys } | Sort-Object -Unique).Count), $LogRoot)
