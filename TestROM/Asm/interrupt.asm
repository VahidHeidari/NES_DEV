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

.segment "ZEROPAGE"

; Animation controler variables
ANIMATION_DELAY = 6
FRAME_COUNTER:   .res 1
ANIMATION_FRAME: .res 1

.segment "CODE"

.INCLUDE "spr.asm"

.macro INC_TICK_COUNT
	INC_INT	ONE_INC, tick_count
.endmacro

; NMI routines executes every frame.
NMI_ROUTINE:
	STORE_CONTEXT
	
	INC_TICK_COUNT

	lda FRAME_COUNTER				; Increment frame counter.
	clc
	adc #1
	sta FRAME_COUNTER
	cmp #ANIMATION_DELAY
	bne OAM_EXIT
	lda #0
	sta FRAME_COUNTER

	lda ANIMATION_FRAME
	clc
	adc #BYTES_PER_MASTER_CHEAF_FRAME
	cmp #MASTER_CHEAF_FRAMES * BYTES_PER_MASTER_CHEAF_FRAME
	bne STORE_ANIMATION_FRAME
	lda #0
STORE_ANIMATION_FRAME:
	sta ANIMATION_FRAME

COPY_MASTER_CHEAF_FRAME:
	tay
	; Number of bytes to copy
	ldx #BYTES_PER_MASTER_CHEAF_FRAME
	PPU_SET_OAM_ADDR 0
OAM_LOOP:
	lda SPRITES_TABLE,y
	sta PPU_OAM_DATA
	iny
	dex
	bne OAM_LOOP	
OAM_EXIT:

	RESTORE_CONTEXT

; IRQ routione
; Do nothing just for now!
IRQ_ROUTINE:
	rti

; Vector table
.segment "VECTORS"
.word 0
.word 0
.word 0
.word NMI_ROUTINE		; NMI routine
.word MAIN				; Main entry point
.word IRQ_ROUTINE		; IRQ routin
