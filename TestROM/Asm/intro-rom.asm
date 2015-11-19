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

.segment "HEADER"
.byte "NES", $1A	; Signiture
.byte $2			; Number of 16KB ROM banks
.byte $1			; Number of 8KB CHR ROM banks
.byte $0			; Contorl byte 1
.byte $0			; Control byte 2
.byte $0			; Number of 8K RAM Banks
.byte $0, $0		; Reserverd
.byte $0, $0		; Reserverd
.byte $0, $0		; Reserverd
.byte $0			; Reserverd

.segment "ZEROPAGE"
NAMETABLESTATE:	.res 1
STR_PTR:		.res 2
DATA_PTR:		.res 2

.segment "CODE"

.INCLUDE "strings.asm"
.INCLUDE "dlogo.asm"
.INCLUDE "macros.asm"
.INCLUDE "palette.asm"
.INCLUDE "ppu.asm"

FAID_FRAME_DELAY		= 5
NUMBER_OF_FAID_COLOR	= 5

PALETTE_FAID_COLORS:
.byte $30, $3D, $2D, $1D, $0D

MAIN:
	DISABLE_INTERRUPT
	
	jsr _wait_vblank			; Wait untile VBlank period
	
	DISABLE_NMI
	INITIALIZE_STACK_POINTER
	CLEAR_RAM
	CLEAR_NAME_TABLE $2000

	PPU_SET_ADDR $3F00		; Set palette start address $3F00-$3F20
	
	ldx #$00				; Index of palette array
	ldy #$20				; Number of palette entries
LOOP_PALETTE:
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
	
	PPU_SET_ADDR $20A4
	SET_STR_PTR INTRO_STR_3
	jsr PRINT_STR

	PPU_SET_ADDR $22A2
	SET_STR_PTR INTRO_STR_4
	jsr PRINT_STR

	PPU_SET_ADDR $22C0
	SET_STR_PTR INTRO_STR_5
	jsr PRINT_STR

	PPU_SET_ADDR $22E4
	SET_STR_PTR INTRO_STR_6
	jsr PRINT_STR

PUT_LOGO:
	PPU_SET_ADDR $210A
	SET_DATA_PTR DELTA_LOGO
	ldx #DELTA_LOGO_HEIGHT
	lda #$EA
	pha
LOGO_OUTER_LOOP:
	pla
	clc
	adc #$20				; Next line of logo
	PPU_SET_ADDR_A $21
	pha

	ldy #0
LOGO_LOOP:
	lda (DATA_PTR),y
	sta $2007
	iny
	cpy #DELTA_LOGO_WIDTH
	bne LOGO_LOOP
	INC_DATA_PTR DELTA_LOGO_WIDTH
	dex
	bne LOGO_OUTER_LOOP
	
	PPU_SET_ADDR $23D2
	lda #$55
	sta PPU_DATA
	sta PPU_DATA
	sta PPU_DATA
	sta PPU_DATA

	PPU_SET_SCROLL_XY 0,0
	
; bit 0-1 : base name table
; bit 2   : address increment 0 : add 1       1 : add 32
; bit 3   : sprite pattern table address
; bit 4   : background pattern table address
; bit 5   : sprite size 0 : 8x8      1 : 8x16
; bit 6   : PPU master slave
; bit 7   : generate NMI 0 : off     1 : on
; Re-enable v blank NMI
	lda #%10001000
	sta $2000
	sta $00				; Temporary location for storing nametable state

; bit 0 : monochrome
; bit 1 : background enable
; bit 2 : sprite enable
	lda	#%00011110
	sta $2001

	SET_DATA_PTR PALETTE_FAID_COLORS
LOOP_FOR_EVER:
	ldy #0
FAID_OUT_LOOP:
	lda #FAID_FRAME_DELAY
	jsr _wait_frames
	PPU_SET_ADDR $3F01
	lda (DATA_PTR),y
	sta PPU_DATA
	PPU_SET_ADDR 0
	iny
	cpy #NUMBER_OF_FAID_COLOR
	bne FAID_OUT_LOOP

	ldy #NUMBER_OF_FAID_COLOR-1
FAID_IN_LOOP:
	lda #FAID_FRAME_DELAY
	jsr _wait_frames
	PPU_SET_ADDR $3F01
	lda (DATA_PTR),y
	sta PPU_DATA
	PPU_SET_ADDR 0
	dey
	cpy #$FF
	bne FAID_IN_LOOP

	lda #FAID_FRAME_DELAY*4
	jsr _wait_frames

	jmp LOOP_FOR_EVER

.INCLUDE "interrupt.asm"
.segment "CHARS"
.INCBIN "Resources/ASCII.chr"
