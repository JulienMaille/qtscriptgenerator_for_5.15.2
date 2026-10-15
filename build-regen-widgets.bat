@echo off
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >NUL 2>&1
set PATH=D:\Qt\6.8.3\msvc2022_64\bin;%PATH%
set QTDIR=D:\Qt\6.8.3\msvc2022_64
cd /d D:\Codez\qtscriptgenerator_for_5.15.2\regen-test\qtbindings\qtscript_widgets
D:\Qt\6.8.3\msvc2022_64\bin\qmake.exe qtscript_widgets.pro -o Makefile 2>&1
nmake %*
