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

NAMETABLESTATE=$00

	PROCESSOR 6502
	ORG $C000

	INCLUDE "palette.asm"
	INCLUDE "attrib.asm"
;	INCLUDE "spr.asm"

MAIN SUBROUTINE
	sei				; Disable interrupt
;	brk
;	nop
	
; Wait for v blank signales
WAIT_V_BLANK
	lda $2002
	bpl WAIT_V_BLANK
	
; Disable NMI
	ldx #$00
	stx $2000
	stx $2001

; Initalize stack pointer to $FF
	dex
	txs

; Clear RAM
	lda #$00
	ldx #$00
CLEAR_RAM
	sta $00,x
	sta $100,x
	sta $200,x
	sta $300,x
	sta $400,x
	sta $500,x
	sta $600,x
	sta $700,x
	dex
	bne CLEAR_RAM

; Set palette start address $3F00-$3F20
	ldx #$3F
	stx $2006
	ldx #$00
	stx $2006
	
	ldx #$00				; Index of palette array
	ldy #$20				; Number of palette entries
LOOP_PALETTE
	lda PALETTE_TABLE,x		; Get palette table color in Acc
	sta $2007				; Store Acc in PPU palette memory
	inx						; Next index
	dey						; Loop variable decrement
	bne LOOP_PALETTE

; Initialize attribute table start address of Attr.0 $23C0-$2400
	ldx #$23
	stx $2006
	ldx #$C0
	stx $2006
	
	ldx #$00				; Index of attribute table 0
	ldy #$40				; Number of entries
LOOP_ATTRIBUTE_TABLE_0
	lda ATTRIBUTE_TABLE_0,X	; Get attribute table value in Acc
	sta $2007				; Store Acc in PPU attribute table 0 memory
	inx						; Next index
	dey						; Loop varible decrement
	bne LOOP_ATTRIBUTE_TABLE_0

; Set up name table start address at $2000-23BF
	ldx #$20
	stx $2006
	ldx #$00
	stx $2006

; TODO: Use a map table to fill
	ldx #$00				; Start index of Map table
	ldy #$00				; 255 tile
	lda #$04				; Index of pattern
LOOP_MAP0_FILL
	sta $2007				; Stort Acc in name table 0 in PPU memory
	inx
	dey
	bne LOOP_MAP0_FILL
	
	ldx #$00				; Start index of Map table
	ldy #$00				; 255 tile
	lda #$02				; Index of pattern TODO: Use a map table to fill
LOOP_MAP1_FILL
	sta $2007				; Stort Acc in name table 0 in PPU memory
	inx
	dey
	bne LOOP_MAP1_FILL
	
	ldx #$00				; Start index of Map table
	ldy #$00				; 255 tile
	lda #$01				; Index of pattern TODO: Use a map table to fill
LOOP_MAP2_FILL
	sta $2007				; Stort Acc in name table 0 in PPU memory
	inx
	dey
	bne LOOP_MAP2_FILL
	
	ldx #$00				; Start index of Map table
	ldy #$C0				; 192 tile
	lda #$00				; Index of pattern TODO: Use a map table to fill
LOOP_MAP3_FILL
	sta $2007				; Stort Acc in name table 0 in PPU memory
	inx
	dey
	bne LOOP_MAP3_FILL

;================
; Initialize attribute table start address of Attr.0 $23C0-$2400
	ldx #$27
	stx $2006
	ldx #$C0
	stx $2006
	
	ldx #$00				; Index of attribute table 0
	ldy #$40				; Number of entries
LOOP_ATTRIBUTE_TABLE_10
	lda ATTRIBUTE_TABLE_0,X	; Get attribute table value in Acc
	sta $2007				; Store Acc in PPU attribute table 0 memory
	inx						; Next index
	dey						; Loop varible decrement
	bne LOOP_ATTRIBUTE_TABLE_10
	
; Set up name table start address at $2000-23BF
	ldx #$24
	stx $2006
	ldx #$00
	stx $2006

; TODO: Use a map table to fill
	ldx #$00				; Start index of Map table
	ldy #$00				; 255 tile
	lda #$03				; Index of pattern
LOOP_MAP10_FILL
	sta $2007				; Stort Acc in name table 0 in PPU memory
	inx
	dey
	bne LOOP_MAP10_FILL
	
	ldx #$00				; Start index of Map table
	ldy #$00				; 255 tile
	lda #$02				; Index of pattern TODO: Use a map table to fill
LOOP_MAP11_FILL
	sta $2007				; Stort Acc in name table 0 in PPU memory
	inx
	dey
	bne LOOP_MAP11_FILL
	
	ldx #$00				; Start index of Map table
	ldy #$00				; 255 tile
	lda #$01				; Index of pattern TODO: Use a map table to fill
LOOP_MAP12_FILL
	sta $2007				; Stort Acc in name table 0 in PPU memory
	inx
	dey
	bne LOOP_MAP12_FILL
	
	ldx #$00				; Start index of Map table
	ldy #$C0				; 192 tile
	lda #$00				; Index of pattern TODO: Use a map table to fill
LOOP_MAP13_FILL
	sta $2007				; Stort Acc in name table 0 in PPU memory
	inx
	dey
	bne LOOP_MAP13_FILL
;================
	lda #$00
	sta $2006
	lda #$00
	sta $2006
	lda #$1
	sta $2007

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
	
; Sprite 1 init
	lda #$04
	sta $2003		; OAM address

	lda #$08
	sta $2004		; Y

	lda #$01
	sta $2004		; Index

	;lda #$00		; No attribute
	;lda #$20		; Behind background
	;lda #$40		; Horizontal flip
	lda #$80		; Vertical flip
	;lda #$C0		; Vertical and horizontal flip
	sta $2004		; Attribute

	lda #$00
	sta $2004		; X

; bit 0-1 : base name table
; bit 2   : address increment 0 : add 1       1 : add 32
; bit 3   : sprite pattern table address
; bit 4   : background pattern table address
; bit 5   : sprite size 0 : 8x8      1 : 8x16
; bit 6   : PPU master slave
; bit 7   : generate NMI 0 : off     1 : on
; Re-enable v blank NMI
	lda #%10010000
	sta $2000
	sta $00				; Temporary location for storing nametable state

; bit 0 : monochrome
; bit 1 : background enable
; bit 2 : sprite enable
	lda	#%00011110
	sta $2001

	ldx #$00			; Load X scroll.
	ldy #$00			; Load Y scroll.
	lda #$07			; Load x position address of sprite 1 in PPU sprites.
; Do nothing just for now!
LOOP_FOR_EVER
	jmp LOOP_FOR_EVER

; NMI routines executes every frame.
; Moves sprite 1 one pixel to right and afte 256 steps
; overflows position to 0 agine and continue.
NMI_ROUTINE SUBROUTINE
	inx						; Increment background position.
	bne DONE 				; Check overflow.
	pha						; Save Acc
	lda $00					; Restore nametable state.
	eor #%00000001			; Toggle nametale.
	sta $2000				; Change PPU nametable state.
	sta $00					; Save new nametable state.
	pla						; Restore Acc.
DONE
	;iny					; Increment Y scroll.
	sta $2003				; Sprite address had been stored in Accumulator
	stx $2004				; X position of sprite 1

	stx $2005				; Scroll screen
	sty $2005
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
