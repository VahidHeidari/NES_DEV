@ECHO off

REM NES_DEV is a cross-platform, portable, and hand-held NES emulator.
REM
REM Copyright (C) 2015-2020 Vahid Heidari (DeltaCode)
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
REM
REM
REM ***************************************************************************
REM *    This is batch file script to assemble input file and                 *
REM *    create binary output file.                                           *
REM *                                                                         *
REM *    DATE      : 2014/03/24 Monday                                        *
REM *    ASSEMBLER : DASM.exe                                                 *
REM ***************************************************************************

REM Change only file name
SET FILE_NAME=shift

REM Assembler path
SET ASM_PATH="C:\Documents and Settings\Delta\My Documents\ARM\DOCS\dasm32\DASM.exe"

REM Relative address path
REM SET ASM_PATH=..\..\..\dasm32\DASM.exe

REM Assembler options
SET ASM_OPTIONS= -f3 -o%FILE_NAME%.bin

REM Source path
SET SOURCE_PATH= %FILE_NAME%.asm

REM Assemble code
SET EXET_ASM=%ASM_PATH%%SOURCE_PATH%%ASM_OPTIONS%

REM Delete already assembled file
del /q %FILE_NAME%.bin

REM Execute assembler and cereate new binary output file
REM ECHO %EXET_ASM%
%EXET_ASM%

REM Print last executed command exit status.
REM ECHO %?%

REM Pause for viewing output of assembler.
pause
