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
	
.macro INC_DATA_PTR value, result
	INC_INT value, result
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

.macro CLEAR_NAME_TABLE addr
	PPU_SET_ADDR addr
	ldx #0
	lda #0
	ldy #4
CLEAR_NAME_TABLE_LOOP:
	sta PPU_DATA
	dex
	bne CLEAR_NAME_TABLE_LOOP
	dey
	bne CLEAR_NAME_TABLE_LOOP
.endmacro

.macro INC_INT addr, result
	clc							; Clear carry flag
	lda addr					; Low nible of left value
	adc	result					; Add with low nible fo result
	sta result					; Store into low byte of result
	lda addr+1					; High byte of left value
	adc result+1				; Add with high byte of result
	sta result+1				; Store into high byte of result
.endmacro

.macro SHIFT_LEFT times
.repeat times
	lsr
.endrepeat
.endmacro
