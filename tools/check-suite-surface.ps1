<#
.SYNOPSIS
    Checks a hand-written smoke suite against one generated binding tree, before CI.

.DESCRIPTION
    A smoke suite reaches into the bindings by name: it instantiates classes and
    calls members. When a name it uses is not emitted on the Qt line the suite is
    about to run on, the failure arrives 20 minutes later, from the CI runner, as
    "undefined is not a function" -- which is how two runs were spent on
    QUuid.createUuidV7 (Qt 6.9) and QBasicReadWriteLock (Qt 6.11), neither of
    which is a generator loss.

    This tool answers the same question locally, against a generated_cpp tree of
    the line in question (the Qt 6.8 LTS tree can be taken from the artifact the
    CI publishes). It reads the class and member tables every wrapper carries and
    looks up every reference the suite makes:

      * a class used outside a version guard must be emitted, or the class
        reference is reported;
      * X.member and instance members are looked up in that class's static list,
        prototype list, enums and enum keys; a member no class in the tree emits
        is reported;
      * a member another class emits is reported as a warning only, since
        inheritance and properties make that plausible and it needs a human.

    References inside an `if (typeof X ...)` guard are left alone: that is the
    house way of versioning a class that only the newer line has, and the guard is
    deliberate.

    What it proves: every name the suite uses is emitted by the tree, so the run
    will not fail for a missing name.
    What it does not prove: that the emitted member behaves, nor the reachability
    of what is emitted -- that is the surface coverage suite's job at run time.

    The table reader is a second copy of the one in surface-coverage.ps1, kept
    separate on purpose: that tool is validated on both CI lines and is not worth
    refactoring for this one.

.PARAMETER GeneratedRoot
    Directory holding the generated binding projects (generated_cpp/).

.PARAMETER Suite
    One or more smoke suites to check (tests/*-smoke.js).

.PARAMETER MinimumClassCount
    Refuse to judge anything from a tree that carries fewer classes than this: an
    unreadable or wrong tree must not produce a green run. Default 400 (Qt 6.12:
    457, Qt 6.8 LTS: 456).

.EXAMPLE
    ./tools/check-suite-surface.ps1 -GeneratedRoot ./generated_cpp -Suite ./tests/collections-smoke.js
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)] [string] $GeneratedRoot,

    [Parameter(Mandatory = $true)] [string[]] $Suite,

    [int] $MinimumClassCount = 400
)

$ErrorActionPreference = "Stop"

# Set-Location does not move the .NET process directory, and GetFullPath() follows
# that one: resolving a relative path with it silently read the tree of whichever
# checkout the shell was started in. Resolve against the PowerShell location.
function Resolve-InputPath {
    param([string] $Path)
    if ([System.IO.Path]::IsPathRooted($Path)) {
        return [System.IO.Path]::GetFullPath($Path)
    }
    return [System.IO.Path]::GetFullPath((Join-Path (Get-Location).Path $Path))
}

$GeneratedRoot = Resolve-InputPath -Path $GeneratedRoot
if (-not (Test-Path -LiteralPath $GeneratedRoot -PathType Container)) {
    throw "Generated projects not found: $GeneratedRoot"
}

# Reads one wrapper and returns the names it registers: the class itself, its
# static and prototype members, and for every enum it emits both the enum name
# (qtscript_QLineF_IntersectionType_keys) and the keys its values carry, which is
# what a script writes as QLineF.IntersectionType.BoundedIntersection.
function Get-WrapperSurface {
    param([string] $FilePath, [string] $FallbackClassName)

    $result = [pscustomobject]@{
        Class     = $FallbackClassName
        Base      = $null
        Statics   = New-Object System.Collections.Generic.List[string]
        Prototypes = New-Object System.Collections.Generic.List[string]
        Enums     = New-Object System.Collections.Generic.List[string]
        EnumKeys  = New-Object System.Collections.Generic.List[string]
    }

    $lines = [System.IO.File]::ReadAllLines($FilePath)
    $section = "class"
    $inTable = $false
    $inKeys = $false

    foreach ($line in $lines) {
        if ($inTable) {
            if ($line -match '^\s*}') { $inTable = $false; $section = "class"; continue }
            if ($line -match '^\s*//\s*static\b') { $section = "static"; continue }
            if ($line -match '^\s*//\s*prototype\b') { $section = "prototype"; continue }
            if ($line -match '^\s*//') { continue }
            if ($line -match '"([^"]*)"') {
                $name = $Matches[1]
                if ($section -eq "class") { $result.Class = $name }
                elseif ($section -eq "static") { $result.Statics.Add($name) }
                else { $result.Prototypes.Add($name) }
            }
            continue
        }

        if ($inKeys) {
            if ($line -match '^\s*}') { $inKeys = $false; continue }
            if ($line -match '"([^"]*)"') { $result.EnumKeys.Add($Matches[1]) }
            continue
        }

        if ($line -match 'qtscript_\w+_function_names\[\]\s*=\s*\{') {
            $inTable = $true
            continue
        }
        # The wrapper states the prototype chain it installs, which is the only
        # source of the base class inside the tree:
        #   proto.setPrototype(engine->defaultPrototype(qMetaTypeId<QBasicReadWriteLock*>()));
        if ($line -match 'setPrototype\(engine->defaultPrototype\(qMetaTypeId<(?<base>\w+)\*?>\(\)\)\)') {
            $result.Base = $Matches['base']
            continue
        }
        # static const char * const qtscript_QLineF_IntersectionType_keys[] = {
        if ($line -match 'qtscript_\w+_(?<enum>\w+)_keys\[\]\s*=\s*\{') {
            $result.Enums.Add($Matches['enum'])
            $inKeys = $true
            continue
        }
    }

    return $result
}

# --- build the index -------------------------------------------------------

$classes = @{}
$memberOwners = @{}

foreach ($file in Get-ChildItem -LiteralPath $GeneratedRoot -Recurse -Filter "qtscript_*.cpp" -File | Sort-Object FullName) {
    $fallback = $file.BaseName -replace '^qtscript_', ''
    $wrapper = Get-WrapperSurface -FilePath $file.FullName -FallbackClassName $fallback
    $name = $wrapper.Class
    if (-not $name) { $name = $fallback }
    if ($classes.ContainsKey($name)) { continue }
    $classes[$name] = $wrapper

    foreach ($member in @($wrapper.Statics) + @($wrapper.Prototypes) + @($wrapper.Enums) + @($wrapper.EnumKeys)) {
        if (-not $memberOwners.ContainsKey($member)) {
            $memberOwners[$member] = New-Object System.Collections.Generic.List[string]
        }
        if (-not $memberOwners[$member].Contains($name)) {
            $memberOwners[$member].Add($name)
        }
    }
}

if ($classes.Count -lt $MinimumClassCount) {
    throw "Only $($classes.Count) classes were read from $GeneratedRoot (expected at least $MinimumClassCount): the generated projects are incomplete or unreadable."
}

# Which tree was read is part of the result: an index built from the wrong
# checkout would otherwise be indistinguishable from a clean one.
Write-Host ("Generated tree: {0} ({1} classes)" -f $GeneratedRoot, $classes.Count)

# The prototype chain the wrapper installs is the tree's own answer to "is this
# member inherited": QReadWriteLock reaches lockForRead through
# QBasicReadWriteLock, QWizard reaches close() through QWidget.
function Get-PrototypeChain {
    param([string] $ClassName, [hashtable] $Index)

    $chain = New-Object System.Collections.Generic.List[string]
    $cursor = $ClassName
    $guard = 0
    while ($cursor -and $Index.ContainsKey($cursor) -and $guard -lt 32) {
        $chain.Add($cursor)
        $cursor = $Index[$cursor].Base
        ++$guard
    }
    return $chain
}

function Test-PrototypeMember {
    param([string] $ClassName, [string] $Member, [hashtable] $Index)

    foreach ($name in (Get-PrototypeChain -ClassName $ClassName -Index $Index)) {
        if (@($Index[$name].Prototypes) -contains $Member) { return $name }
    }
    return $null
}

# A property is not in the tables: the meta-object exposes it because the class
# carries accessors, so `wizard.currentId` is justified by currentId() (or by
# setCurrentId()/isCurrentId() when only those are emitted).
function Test-AccessorProperty {
    param([string] $ClassName, [string] $Member, [hashtable] $Index)

    $capital = $Member.Substring(0, 1).ToUpperInvariant() + $Member.Substring(1)
    foreach ($name in (Get-PrototypeChain -ClassName $ClassName -Index $Index)) {
        $prototypes = @($Index[$name].Prototypes)
        if (($prototypes -contains ("set" + $capital)) -or ($prototypes -contains ("is" + $capital))) {
            return $true
        }
    }
    return $false
}

# --- read the suites -------------------------------------------------------

$findings = 0
$warnings = 0

foreach ($path in $Suite) {
    $full = Resolve-InputPath -Path $path
    if (-not (Test-Path -LiteralPath $full)) { throw "Suite not found: $full" }

    $typed = @{}                 # variable name -> class, for `var x = new QClass(...)`
    $seenClasses = New-Object 'System.Collections.Generic.HashSet[string]'
    $guardDepth = New-Object System.Collections.Generic.List[int]
    $depth = 0
    $classRefs = 0
    $memberRefs = 0
    $propertyRefs = 0
    $guarded = 0
    $lineNumber = 0

    foreach ($line in [System.IO.File]::ReadAllLines($full)) {
        ++$lineNumber
        $trimmed = $line.Trim()
        $inGuard = $guardDepth.Count -gt 0
        $isGuardLine = $trimmed -match 'if\s*\(\s*typeof\s+[\w\.]+'

        # A comment may name a class the suite no longer uses; and a reference on
        # the guard line itself is part of the guard.
        $skip = $inGuard -or $isGuardLine -or $trimmed.StartsWith('//')
        if ($inGuard) { ++$guarded }

        if (-not $skip) {
            # var polygon = new QPolygon([...]) -> polygon is a QPolygon
            foreach ($m in [regex]::Matches($line, '(?:var|let|const)?\s*(\w+)\s*=\s*new\s+(Q\w+)\s*\(')) {
                $typed[$m.Groups[1].Value] = $m.Groups[2].Value
            }

            # String literals carry messages, not code: an error message that names a
            # class must not become a reference to it.
            $code = [regex]::Replace($line, '"(?:[^"\\]|\\.)*"', '""')

            $references = New-Object System.Collections.Generic.List[object]

            # A class used on its own: typeof QBasicReadWriteLock, new QPolygon(...).
            foreach ($m in [regex]::Matches($code, '\b(Q[A-Z]\w*)\b')) {
                $references.Add([pscustomobject]@{
                    Class    = $m.Groups[1].Value
                    Member   = $null
                    Instance = $false
                })
            }

            # QClass.member  (static member, enum name, enum key, prototype)
            foreach ($m in [regex]::Matches($code, '\b(Q\w+)\s*\.\s*([A-Za-z_]\w*)')) {
                $references.Add([pscustomobject]@{
                    Class    = $m.Groups[1].Value
                    Member   = $m.Groups[2].Value
                    Instance = $false
                })
            }

            # typedInstance.member
            foreach ($m in [regex]::Matches($code, '\b(\w+)\s*\.\s*([A-Za-z_]\w*)')) {
                $receiver = $m.Groups[1].Value
                if ($typed.ContainsKey($receiver)) {
                    $references.Add([pscustomobject]@{
                        Class    = $typed[$receiver]
                        Member   = $m.Groups[2].Value
                        Instance = $true
                    })
                }
            }

            foreach ($reference in $references) {
                $className = $reference.Class
                $member = $reference.Member
                if ($null -ne $member -and $member -eq "prototype") { continue }

                if (-not $classes.ContainsKey($className)) {
                    if ($seenClasses.Add($className)) {
                        ++$classRefs
                        ++$findings
                        $what = if ($null -ne $member) { "$className.$member" } else { $className }
                        Write-Host ("  !! {0}:{1} {2} -- class {3} is not emitted by this tree" -f `
                            (Split-Path $full -Leaf), $lineNumber, $what, $className)
                    }
                    continue
                }

                if ($seenClasses.Add($className)) { ++$classRefs }
                if ($null -eq $member) { continue }
                ++$memberRefs

                if ($reference.Instance) {
                    if (Test-PrototypeMember -ClassName $className -Member $member -Index $classes) { continue }
                    if (Test-AccessorProperty -ClassName $className -Member $member -Index $classes) {
                        ++$propertyRefs
                        continue
                    }
                } else {
                    $wrapper = $classes[$className]
                    $own = @($wrapper.Statics) + @($wrapper.Prototypes) + @($wrapper.Enums) +
                           @($wrapper.EnumKeys)
                    if ($own -contains $member) { continue }
                }

                if ($memberOwners.ContainsKey($member)) { $owners = @($memberOwners[$member]) } else { $owners = @() }
                if ($owners.Count -gt 0) {
                    ++$warnings
                    $shown = @($owners | Select-Object -First 3) -join ", "
                    if ($owners.Count -gt 3) { $shown += " (+" + ($owners.Count - 3) + " more)" }
                    Write-Host ("  ~  {0}:{1} {2}.{3} -- not emitted by {2}, emitted by {4}" -f `
                        (Split-Path $full -Leaf), $lineNumber, $className, $member, $shown)
                } else {
                    ++$findings
                    Write-Host ("  !! {0}:{1} {2}.{3} -- no class in this tree emits {3}" -f `
                        (Split-Path $full -Leaf), $lineNumber, $className, $member)
                }
            }
        }

        $depth += ([regex]::Matches($line, '\{')).Count - ([regex]::Matches($line, '\}')).Count
        if ($isGuardLine -and ([regex]::Matches($line, '\{')).Count -gt 0) {
            $guardDepth.Add($depth)
        }
        while ($guardDepth.Count -gt 0 -and $depth -lt $guardDepth[$guardDepth.Count - 1]) {
            $guardDepth.RemoveAt($guardDepth.Count - 1)
        }
    }

    Write-Host ("{0}: {1} classes, {2} member references ({3} through a property accessor), {4} lines under a version guard" -f `
        (Split-Path $full -Leaf), $classRefs, $memberRefs, $propertyRefs, $guarded)
}

if ($findings -gt 0) {
    throw "$findings reference(s) the tree does not emit; $warnings warning(s)."
}

Write-Host ("No unsatisfied references; {0} warning(s) to read." -f $warnings)
