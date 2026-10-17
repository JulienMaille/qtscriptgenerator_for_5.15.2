@echo off
if "%VSDEVCMD%"=="" for /f "delims=" %%v in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -property installationPath') do set "VSDEVCMD=%%v\Common7\Tools\VsDevCmd.bat"
call "%VSDEVCMD%" -arch=amd64 >NUL 2>&1
if "%QTDIR%"=="" set "QTDIR=%QT_ROOT_DIR%"
if "%QTDIR%"=="" ( echo Set QTDIR or QT_ROOT_DIR to the Qt 6 prefix. & exit /b 2 )
set PATH=%QTDIR%\bin;%PATH%
cd /d %~dp0regen-test\qtbindings\qtscript_xml
%QTDIR%\bin\qmake.exe qtscript_xml.pro -o Makefile 2>&1
nmake %*
