@echo off
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >NUL 2>&1
cd /d D:\Codez\qtscriptgenerator_for_5.15.2\generator
nmake %*
