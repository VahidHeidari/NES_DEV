@echo off

REM NES_DEV is a cross-platform, portable, and hand-held NES emulator.
REM 
REM Copyright (C) 2015  Vahid Heidari (DeltaCode)
REM 
REM This program is free software: you can redistribute it and/or modify
REM it under the terms of the GNU General Public License as published by
REM the Free Software Foundation, either version 3 of the License, or
REM (at your option) any later version.
REM 
REM This program is distributed in the hope that it will be useful,
REM but WITHOUT ANY WARRANTY; without even the implied warranty of
REM MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
REM GNU General Public License for more details.
REM 
REM You should have received a copy of the GNU General Public License
REM along with this program.  If not, see <http://www.gnu.org/licenses/>.

REM Binary path
set BIN=C:\C++\NES_DEV\git\TestROM\C\cc65\win\bin

REM Assembler path
set ASM=%BIN%\ca65.exe
set FLAGS="-v --cpu 6502"

REM Linker path
set LD=%BIN%\ld65.exe
set LD_FLAGS="-Ccc65-test.cfg"

REM Source file without extention
set PRG="intro-rom"

REM Building commands
echo %ASM%
%ASM% %FLAGS% %PRG%.asm && %LD% %LD_FLAGS% %PRG%.o -o %PRG%.nes

pause