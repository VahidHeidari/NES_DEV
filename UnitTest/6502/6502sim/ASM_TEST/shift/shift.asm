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


	PROCESSOR 6502
	ORG $8000

RESET
	lda #$01
	asl					; 0x01 << 1   = 0b00000010
	asl					; 0x01 << 2   = 0b00000100
	asl					; 0x01 << 3   = 0b00001000
	asl					; 0x01 << 4   = 0b00010000
	asl					; 0x01 << 5   = 0b00100000
	asl					; 0x01 << 6   = 0b01000000
	asl					; 0x01 << 7   = 0b10000000
	asl					; 0x01 << 8   = 0b00000000    C = 1, Z = 1

LOOP_FOR_EVER
	jmp LOOP_FOR_EVER	; End of program

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
