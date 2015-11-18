;-----------------------------------
; This is an 'NES' test program.
; Assembler is ca65.exe
; Date 2015/11/17 Thursday 11:02
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
	
.macro DISABLE_INTERRUPT
	sei
.endmacro
	
.macro ENABLE_INTERRUPT
	cli
.endmacro
	
.macro DISABLE_NMI
	ldx #$00
	stx $2000
	stx $2001
.endmacro
	
.macro INITIALIZE_STACK_POINTER
	dex
	txs
.endmacro
	
.macro CLEAR_RAM
	lda #$00
	ldx #$00
CLEAR:
	sta $00,x
	sta $100,x
	sta $200,x
	sta $300,x
	sta $400,x
	sta $500,x
	sta $600,x
	sta $700,x
	dex
	bne CLEAR
.endmacro

.macro SET_DATA_PTR addr
	lda #<addr
	sta DATA_PTR
	lda #>addr
	sta DATA_PTR+1
.endmacro
	
.macro INC_DATA_PTR value
	lda #value
	clc
	adc DATA_PTR
	sta DATA_PTR
.endmacro

.macro STORE_CONTEXT
	plp
	pha								; Store registers.
	txa
	pha
	tya
	pha
.endmacro

.macro RESTORE_CONTEXT
	pla								; Restore registers.
	tay
	pla
	tax
	pla
	php
.endmacro
