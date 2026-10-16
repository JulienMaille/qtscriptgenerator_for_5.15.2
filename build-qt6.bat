@echo off
if "%VSDEVCMD%"=="" for /f "delims=" %%v in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -property installationPath') do set "VSDEVCMD=%%v\Common7\Tools\VsDevCmd.bat"
call "%VSDEVCMD%" -arch=amd64 >NUL 2>&1
cd /d %~dp0generator
nmake %*
