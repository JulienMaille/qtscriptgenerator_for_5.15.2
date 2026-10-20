# Shared surface extraction for the tools that read the emitted bindings.
# Dot-source this file, then call Get-ExportedSurface -Root <generated_cpp>.
#
# It reads the function-name table the generator writes at the top of every
# wrapper:
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
