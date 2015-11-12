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

	PROCESSOR 6502
	ORG $C000

NAMETABLESTATE EQU $00
STR_PTR EQU $01
DATA_PTR EQU $03
	
	INCLUDE "macros.asm"
	INCLUDE "palette.asm"
	INCLUDE "attrib.asm"
	INCLUDE "strings.asm"
	INCLUDE "ppu.asm"
	INCLUDE "dlogo.asm"
	
MAIN SUBROUTINE
	DISABLE_INTERRUPT
	
	jsr WAIT_VBLANK			; Wait untile VBlank period
	
	DISABLE_NMI
	INITIALIZE_STACK_POINTER
	CLEAR_RAM
	PPU_SET_ADDR $3F00		; Set palette start address $3F00-$3F20
	
	ldx #$00				; Index of palette array
	ldy #$20				; Number of palette entries
LOOP_PALETTE
	lda PALETTE_TABLE,x		; Get palette table color in Acc
	sta $2007				; Store Acc in PPU palette memory
	inx						; Next index
	dey						; Loop variable decrement
	bne LOOP_PALETTE

; TODO: Load sprites from a table or DMA operation.
; sprites placement
	ldx #$00				; Counter
	ldy #$FF				; Y position
	lda #$00
	sta $2003				; Start address
LOOP_SPRITES_INIT:
	sty $2004				; Y-1 is $FF so hides
	sta $2004				; Tile index
	sta $2004				; Attribute
	sta $2004				; X
	dex
	bne LOOP_SPRITES_INIT

	; Print introduction string.
	PPU_SET_ADDR $2041
	SET_STR_PTR INTRO_STR_1
	jsr PRINT_STR
	
	PPU_SET_ADDR $206A
	SET_STR_PTR INTRO_STR_2
	jsr PRINT_STR
	
	PPU_SET_ADDR $20A7
	SET_STR_PTR INTRO_STR_3
	jsr PRINT_STR

PUT_LOGO
	PPU_SET_ADDR $210A
	SET_DATA_PTR DELTA_LOGO
	ldx #DELTA_LOGO_HEIGHT
	lda #$EA
	pha
LOGO_OUTER_LOOP
	pla
	clc
	adc #$20				; Next line of logo
	PPU_SET_ADDR_A $21
	pha

	ldy #0
LOGO_LOOP
	lda (DATA_PTR),y
	sta $2007
	iny
	cpy #DELTA_LOGO_WIDTH
	bne LOGO_LOOP
	INC_DATA_PTR DELTA_LOGO_WIDTH
	dex
	bne LOGO_OUTER_LOOP
	
	PPU_SET_SCROLL_XY 0,0
	
; bit 0-1 : base name table
; bit 2   : address increment 0 : add 1       1 : add 32
; bit 3   : sprite pattern table address
; bit 4   : background pattern table address
; bit 5   : sprite size 0 : 8x8      1 : 8x16
; bit 6   : PPU master slave
; bit 7   : generate NMI 0 : off     1 : on
; Re-enable v blank NMI
	lda #%10000000
	sta $2000
	sta $00				; Temporary location for storing nametable state

; bit 0 : monochrome
; bit 1 : background enable
; bit 2 : sprite enable
	lda	#%00011110
	sta $2001

FAID_FRAME_DELAY = 10
NUMBER_OF_FAID_COLOR = 5
PALETTE_FAID_COLORS
	dc.b $30, $3D, $2D, $1D, $0D

	SET_DATA_PTR PALETTE_FAID_COLORS
LOOP_FOR_EVER
	ldy #0
FAID_OUT_LOOP
	ldx #FAID_FRAME_DELAY
	jsr WAIT_FRAMES
	PPU_SET_ADDR $3F01
	lda (DATA_PTR),y
	sta PPU_DATA
	PPU_SET_ADDR 0
	iny
	cpy #NUMBER_OF_FAID_COLOR
	bne FAID_OUT_LOOP

	ldy #NUMBER_OF_FAID_COLOR-1
FAID_IN_LOOP
	ldx #FAID_FRAME_DELAY
	jsr WAIT_FRAMES
	PPU_SET_ADDR $3F01
	lda (DATA_PTR),y
	sta PPU_DATA
	PPU_SET_ADDR 0
	dey
	bne FAID_IN_LOOP

	jmp LOOP_FOR_EVER

; NMI routines executes every frame.
NMI_ROUTINE SUBROUTINE
	rti

; IRQ routione
; Do nothing just for now!
IRQ_ROUTINE SUBROUTINE
	rti
	; Vector table
	ORG $FFFA
	dc.w NMI_ROUTINE		; NMI routine
	dc.w MAIN				; Main entry point
	dc.w IRQ_ROUTINE		; IRQ routine