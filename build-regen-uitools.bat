@echo off
if "%VSDEVCMD%"=="" for /f "delims=" %%v in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -property installationPath') do set "VSDEVCMD=%%v\Common7\Tools\VsDevCmd.bat"
call "%VSDEVCMD%" -arch=amd64 >NUL 2>&1
if "%QTDIR%"=="" set QTDIR=D:\Qt\6.8.3\msvc2022_64
set PATH=%QTDIR%\bin;%PATH%
cd /d %~dp0regen-test\qtbindings\qtscript_uitools
%QTDIR%\bin\qmake.exe qtscript_uitools.pro -o Makefile 2>&1
nmake %*
