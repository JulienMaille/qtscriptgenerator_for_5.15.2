@echo off
rem Regenerate one Qt 6 module into the git-ignored regen-test workspace.
rem Usage: regen-module.bat <module>   (e.g. core, gui, network, sql, ...)
rem Prereq: generator\release\generator.exe is built (build-qt6.bat).
setlocal
set MOD=%~1
if "%MOD%"=="" (
  echo Usage: regen-module.bat ^<module^>
  exit /b 2
)
if "%QTDIR%"=="" set QTDIR=D:\Qt\6.8.3\msvc2022_64
set PATH=%QTDIR%\bin;%PATH%
cd /d %~dp0
release\generator.exe qtscript_masterinclude_qt6.h build_%MOD%.txt --output-directory=%~dp0..\regen-test || exit /b 1
endlocal
