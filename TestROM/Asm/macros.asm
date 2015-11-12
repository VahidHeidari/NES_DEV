;-----------------------------------
; This is an 'NES' test program.
; Assembler is DASM.exe
; Date 2014/02/20 Thursday 21:49
;
; NES_DEV is a cross-platform, portable, and hand-held NES emulator.
;
; Copyright (C) 2015  Vahid Heidari (DeltaCode)
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
	
	mac DISABLE_INTERRUPT
	sei
	endm
	
	mac ENABLE_INTERRUPT
	cli
	endm
	
	mac DISABLE_NMI
	ldx #$00
	stx $2000
	stx $2001
	endm
	
	mac INITIALIZE_STACK_POINTER
	dex
	txs
	endm
	
	mac CLEAR_RAM
	lda #$00
	ldx #$00
.CLEAR
	sta $00,x
	sta $100,x
	sta $200,x
	sta $300,x
	sta $400,x
	sta $500,x
	sta $600,x
	sta $700,x
	dex
	bne .CLEAR
	endm

	mac SET_DATA_PTR
	lda <#{0}
	sta DATA_PTR
	lda >#{0}
	sta DATA_PTR+1
	endm
	
	mac INC_DATA_PTR
	lda #{0}
	clc
	adc DATA_PTR
	sta DATA_PTR
;	bcc .END
;	lda #0
;	adc DATA_PTR+1
;	adc
;.END
	endm
	