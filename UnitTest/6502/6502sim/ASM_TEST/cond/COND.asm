;-----------------------------------
; This is an 'NES' test program.
; Assembler is ca65.exe
; Date 2020/03/25 Wednesday 10:01
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
	cld				; clear decimal flag
	sei				; disable interrupt
	lda #-128
	cmp #1		; Compare A and #$20

	;lda #-128
	;adc #-1
	;adc				; overflow

	;lda #-128
	;sec
	;sbc #1			; overflow

LOOP_FOR_EVER
	jmp LOOP_FOR_EVER	; End of program.

NMI_ROUTINE
IRQ_ROUTINE
	rti

	ORG $FFFA
	dc.w IRQ_ROUTINE
	dc.w RESET
	dc.w NMI_ROUTINE
