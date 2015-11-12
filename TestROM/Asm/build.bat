@echo off

set DASM="C:\Documents and Settings\Delta\My Documents\ARM\DOCS\dasm32\dasm.exe"
set RESOURCE="Resources"

%DASM% intro-rom.asm -v2 -f3 -ointro-rom.bin && copy /b intro-rom.bin+%RESOURCE%\ascii.chr test.nes

REM %RESOURCE%\Header+

pause