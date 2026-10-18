[CmdletBinding()]
param(
    [ValidateSet("Debug", "Release", "All")]
    [string] $Configuration = "All",

    [string] $QtPrefix = $env:QT_ROOT_DIR,

    [string] $QtScriptPrefix = $env:QTSCRIPT_PREFIX,

    [ValidateRange(1000, 600000)]
    [int] $EvaluatorTimeoutMilliseconds = 120000,

    # The surface coverage suite is built from generated_cpp/ on every run and
    # needs no switch. This drops it, for a quick local loop only: the emitted
    # class and member surface is then not verified.
    [switch] $SkipSurfaceCoverage
)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "build-config.ps1")
$QtPrefix = Resolve-QtPrefix $QtPrefix
$QtScriptPrefix = Resolve-QtScriptPrefix $QtScriptPrefix $QtPrefix
$env:PATH = "$QtScriptPrefix\bin;$QtPrefix\bin;$env:PATH"
$env:QT_QPA_PLATFORM = "offscreen"
$testRoot = $PSScriptRoot
$logRoot = Join-Path $testRoot "logs"
New-Item -ItemType Directory -Force -Path $logRoot | Out-Null

$sourceExtensions = @(".pro", ".pri", ".h", ".cpp")
$compatReferences = foreach ($sourceRoot in @(
    (Join-Path $PSScriptRoot "qtbindings"),
    (Join-Path $PSScriptRoot "generated_cpp"))) {
    Get-ChildItem -LiteralPath $sourceRoot -Recurse -File |
        Where-Object { $sourceExtensions -contains $_.Extension.ToLowerInvariant() } |
        Where-Object { $_.FullName -notmatch '\\(?:debug|release)\\' } |
        Select-String -Pattern "Core5Compat" -SimpleMatch
}
if ($compatReferences) {
    throw "Qt5Compat source dependency detected:`n$($compatReferences -join [Environment]::NewLine)"
}

$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path -LiteralPath $vswhere)) {
    throw "vswhere.exe is required to locate dumpbin.exe: $vswhere"
}
$dumpbin = & $vswhere -latest -products * -find "VC\Tools\MSVC\**\bin\Hostx64\x64\dumpbin.exe" |
    Select-Object -First 1
if (-not $dumpbin) {
    throw "Could not locate the x64 MSVC dumpbin.exe"
}

$configurations = switch ($Configuration) {
    "Debug" { @("debug") }
    "Release" { @("release") }
    default { @("release", "debug") }
}

# The smoke suites are discovered, not listed: adding tests/*-smoke.js is enough
# for it to run, and a suite can only be left out by naming it in
# $excludedSmokeTests, so an omission is a deliberate, visible edit. The list
# used to be written out here, which silently ignored every suite it did not
# name.
$excludedSmokeTests = @(
    # Empty on purpose: name a suite here only with the reason beside it.
)
$testDirectory = Join-Path $PSScriptRoot "tests"
$smokeTests = @(Get-ChildItem -LiteralPath $testDirectory -Filter "*-smoke.js" -File | Sort-Object Name)
foreach ($excluded in $excludedSmokeTests) {
    if (-not (Test-Path -LiteralPath (Join-Path $testDirectory $excluded))) {
        throw "Excluded smoke suite does not exist: tests\$excluded"
    }
}
$smokeTests = @($smokeTests | Where-Object { $excludedSmokeTests -notcontains $_.Name })
if ($smokeTests.Count -eq 0) {
    throw "No smoke suite found: $(Join-Path $testDirectory '*-smoke.js')"
}

# The surface coverage suite asserts that everything the generator wrote into
# generated_cpp/ is reachable from script in the bindings built from it. It is
# derived from the generated projects and written under logs/, so no generated
# file is committed and the suite follows the Qt version that was generated.
$surfaceCoverageSuite = $null
if ($SkipSurfaceCoverage) {
    Write-Warning "Surface coverage suite skipped: the emitted class and member surface is not verified."
} else {
    $generatedRoot = Join-Path $PSScriptRoot "generated_cpp"
    if (-not (Test-Path -LiteralPath $generatedRoot -PathType Container)) {
        throw "generated_cpp/ is missing and the surface coverage suite is built from it. Run generator\regen-all.ps1 first, or pass -SkipSurfaceCoverage to run without it."
    }
    $surfaceCoverageSuite = Join-Path $logRoot "surface-coverage-smoke.js"
    & (Join-Path $PSScriptRoot "tools\surface-coverage.ps1") -GeneratedRoot $generatedRoot -OutputPath $surfaceCoverageSuite
}

function Invoke-Evaluator {
    param(
        [Parameter(Mandatory = $true)] [string] $Evaluator,
        [Parameter(Mandatory = $true)] [string[]] $Arguments,
        [Parameter(Mandatory = $true)] [string] $Label,
        [int] $TimeoutMilliseconds = 30000
    )

    $safeLabel = $Label -replace '[^A-Za-z0-9_.-]', '_'
    $stdoutPath = Join-Path $logRoot "$safeLabel.stdout.log"
    $stderrPath = Join-Path $logRoot "$safeLabel.stderr.log"
    Remove-Item -LiteralPath $stdoutPath, $stderrPath -Force -ErrorAction SilentlyContinue

    # ProcessStartInfo receives one command-line string on Windows. Quote each
    # argument so test paths remain valid when the checkout is under a path
    # containing spaces.
    $quotedArguments = @($Arguments | ForEach-Object {
        '"' + $_.Replace('"', '\"') + '"'
    })
    $startInfo = New-Object System.Diagnostics.ProcessStartInfo
    $startInfo.FileName = $Evaluator
    $startInfo.Arguments = $quotedArguments -join ' '
    $startInfo.WorkingDirectory = $testRoot
    $startInfo.UseShellExecute = $false
    $startInfo.CreateNoWindow = $true
    $startInfo.RedirectStandardOutput = $true
    $startInfo.RedirectStandardError = $true

    $process = New-Object System.Diagnostics.Process
    $process.StartInfo = $startInfo
    [void]$process.Start()
    $stdoutTask = $process.StandardOutput.ReadToEndAsync()
    $stderrTask = $process.StandardError.ReadToEndAsync()

    if (-not $process.WaitForExit($TimeoutMilliseconds)) {
        & taskkill.exe /PID $process.Id /T /F | Out-Null
        [void]$process.WaitForExit(5000)
        $stdout = $stdoutTask.Result
        $stderr = $stderrTask.Result
        [System.IO.File]::WriteAllText($stdoutPath, $stdout)
        [System.IO.File]::WriteAllText($stderrPath, $stderr)
        $process.Dispose()
        throw "$Label timed out after $TimeoutMilliseconds ms; see $stderrPath`n$stdout$stderr"
    }
    # WaitForExit() without a timeout drains asynchronous output handlers.
    $process.WaitForExit()
    $stdout = $stdoutTask.Result
    $stderr = $stderrTask.Result
    [System.IO.File]::WriteAllText($stdoutPath, $stdout)
    [System.IO.File]::WriteAllText($stderrPath, $stderr)
    $exitCode = $process.ExitCode
    $process.Dispose()

    if ($exitCode -ne 0) {
        throw "$Label failed with exit code $($exitCode).`n$stdout$stderr"
    }
}

foreach ($name in $configurations) {
    $evaluator = Join-Path $PSScriptRoot "qtbindings\qs_eval\$name\qs_eval.exe"
    if (-not (Test-Path -LiteralPath $evaluator)) {
        throw "Evaluator is missing for $name. Run build.ps1 first: $evaluator"
    }

    $debugBuild = $name -eq "debug"
    $pluginBinaries = Get-ChildItem -LiteralPath (Join-Path $PSScriptRoot "plugins\script") -Filter "qtscript_*.dll" |
        Where-Object { ($_.BaseName -match 'd$') -eq $debugBuild }
    foreach ($binary in @($pluginBinaries) + @(Get-Item -LiteralPath $evaluator)) {
        $dependencies = & $dumpbin /nologo /dependents $binary.FullName
        if ($dependencies -match "Qt6Core5Compatd?\.dll") {
            throw "Qt5Compat binary dependency detected in $($binary.FullName)"
        }
    }
    Write-Host "No Qt5Compat binary dependencies: $name"

    Invoke-Evaluator -Evaluator $evaluator -Arguments @("--scripttools-smoke") `
        -Label "$name-scripttools-smoke" -TimeoutMilliseconds $EvaluatorTimeoutMilliseconds
    Write-Host "QtScriptTools debugger smoke test passed: $name"

    if ($surfaceCoverageSuite) {
        Invoke-Evaluator -Evaluator $evaluator -Arguments @($surfaceCoverageSuite) `
            -Label "$name-surface-coverage-smoke" -TimeoutMilliseconds $EvaluatorTimeoutMilliseconds
        Write-Host "surface-coverage-smoke.js passed: $name (every emitted class and member is reachable)"
    }

    foreach ($smokeTest in $smokeTests) {
        Invoke-Evaluator -Evaluator $evaluator -Arguments @($smokeTest.FullName) `
            -Label "$name-$($smokeTest.BaseName)" `
            -TimeoutMilliseconds $EvaluatorTimeoutMilliseconds
        Write-Host "$($smokeTest.Name) passed: $name"
    }
    Write-Host "All $($smokeTests.Count) discovered smoke suites passed: $name"
}
