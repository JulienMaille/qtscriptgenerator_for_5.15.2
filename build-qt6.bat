@echo off
rem Builds generator\ against a Qt 6 prefix.
rem
rem Delegates to generator\build-generator.ps1, which runs qmake itself (so
rem Makefile.qt6 always matches the prefix in use) and rebuilds from scratch
rem whenever the Qt prefix changed. Linking objects compiled against two Qt
rem versions builds fine but corrupts memory at run time.
rem
rem Usage: build-qt6.bat [qt-prefix] [clean]
setlocal
if not "%~1"=="" set "QTDIR=%~1"
if "%QTDIR%"=="" set "QTDIR=%QT_ROOT_DIR%"
if "%QTDIR%"=="" (
  echo Set QTDIR or QT_ROOT_DIR to the Qt 6 prefix, or pass it as the first argument.
  exit /b 2
)
set "EXTRA="
if /i "%~2"=="clean" set "EXTRA=-Clean"
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0generator\build-generator.ps1" -QtPrefix "%QTDIR%" -Configuration Release %EXTRA%
exit /b %ERRORLEVEL%
