;-----------------------------------
; This is an 'NES' test program.
; Assembler is ca65.exe
; Date 2020/03/25 Wednesday 10:07
;
; NES_DEV is a cross-platform, portable, and hand-held NES emulator.
;
; Copyright (C) 2015-2020 Vahid Heidari (DeltaCode)
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
;-----------------------------------


; ******************************************************
; This file genrated at : Mon 03/24/2014  1:16:04.73
;
; Assembler   : DASM.exe
; File name   : add16.asm
; Description : Adding two 16 bit numbers at $20, $21
;     and $22, $23 and store result at $24, 25
; ******************************************************

	PROCESSOR 6502
	ORG $8000

; Main entry point of program.
RESET
; TODO: Write your code here!
	clc									; Clear carry
	lda $20								; Load low byte
	adc $22								; Add with low byte of second number
	sta $24								; Store result at low byte

	lda $21								; Load high byte
	adc $23								; Add with high byte of second number
	sta $25								; Store result at high byte

LOOP_FOR_EVER
	jmp LOOP_FOR_EVER					; End of program

; Interrupt routines
; Do nothing just for now!
NMI_ROUTINE
IRQ_ROUTINE
	rti

; Vector table
	ORG $FFFA
	dc.w NMI_ROUTINE
	dc.w RESET
	dc.w IRQ_ROUTINE
