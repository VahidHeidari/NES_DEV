@ECHO off

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