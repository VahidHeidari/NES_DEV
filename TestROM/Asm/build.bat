@echo off

set DASM="C:\Documents and Settings\Delta\My Documents\ARM\DOCS\dasm32\dasm.exe"
set RESOURCE="Resources"

%DASM% intro-rom.asm -v0 -f3 -ointro-rom.bin && copy /b %RESOURCE%\Header+intro-rom.bin+%RESOURCE%\ascii.chr test.nes



pause