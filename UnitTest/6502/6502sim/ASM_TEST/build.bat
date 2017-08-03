@ECHO off

REM ***************************************************************************
REM *    This is batch file script to assemble input file and                 *
REM *    create binary output file. Only change the 'FILE_NAME' to desiered   *
REM *    file name, And then run it. This script create source code for you,  *
REM *    , assemble source and generats output file! :D                       *
REM *                                                                         *
REM *    DATE      : 2014/03/24 Monday                                        *
REM *    ASSEMBLER : DASM.exe                                                 *
REM ***************************************************************************

REM Change only file name
SET FILE_NAME=ind

REM Absolute assembler path
SET ASM_PATH="C:\Documents and Settings\Delta\My Documents\ARM\DOCS\dasm32\DASM.exe"

REM Relative address path
REM SET ASM_PATH=..\..\..\dasm32\DASM.exe

REM Assembler options
SET ASM_OPTIONS= -f3 -o%FILE_NAME%.bin -v4

REM Source path
SET SOURCE_PATH= %FILE_NAME%.asm

REM Uncomment for debug only
REM DEL /q %SOURCE_PATH%

REM Check input file exists or not?
REM If not exists, then create a template source for lazy developer! :D
IF NOT EXIST %SOURCE_PATH% (
ECHO ; ******************************************************> %SOURCE_PATH%
ECHO ; This file genrated at : %date% %time%>>  %SOURCE_PATH%
ECHO ; >> %SOURCE_PATH%
ECHO ; Assembler   : DASM.exe>> %SOURCE_PATH%
ECHO ; File name   : %FILE_NAME%.asm>> %SOURCE_PATH%
ECHO ; Description :>> %SOURCE_PATH%
ECHO ; >> %SOURCE_PATH%
ECHO ; ******************************************************>> %SOURCE_PATH%
ECHO.>> %SOURCE_PATH%

ECHO 	PROCESSOR 6502>> %SOURCE_PATH%
ECHO 	ORG $8000>> %SOURCE_PATH%
ECHO.>> %SOURCE_PATH%

ECHO ; Main entry point of program.>> %SOURCE_PATH%
ECHO RESET>> %SOURCE_PATH%
ECHO ; TODO: Write your code here!>> %SOURCE_PATH%
ECHO.>> %SOURCE_PATH%

ECHO LOOP_FOR_EVER>> %SOURCE_PATH%
ECHO 	jmp LOOP_FOR_EVER					; End of program>> %SOURCE_PATH%
ECHO.>> %SOURCE_PATH%

ECHO ; Interrupt routines>> %SOURCE_PATH%
ECHO ; Do nothing just for now!>> %SOURCE_PATH%
ECHO NMI_ROUTINE>> %SOURCE_PATH%
ECHO IRQ_ROUTINE>> %SOURCE_PATH%
ECHO 	rti>> %SOURCE_PATH%
ECHO.>> %SOURCE_PATH%

ECHO ; Vector table>> %SOURCE_PATH%
ECHO 	ORG $FFFA>> %SOURCE_PATH%
ECHO 	dc.w NMI_ROUTINE>> %SOURCE_PATH%
ECHO 	dc.w RESET>> %SOURCE_PATH%
ECHO 	dc.w IRQ_ROUTINE>> %SOURCE_PATH%
)

REM Create assembling command.
SET EXEC_ASM=%ASM_PATH%%SOURCE_PATH%%ASM_OPTIONS%

REM Delete already assembled file
DEL /q %FILE_NAME%.bin

REM Execute assembler and cereate new binary output file
REM ECHO %EXEC_ASM%
SET ERRORLEVEL=
%EXEC_ASM%

REM TODO: Check exit code of assembler, and if not succeded, then delete binary file
IF NOT %ERRORLEVEL% == 0 ( 
DEL /q %FILE_NAME%.bin 
)

REM Pause for viewing output of assembler.
PAUSE