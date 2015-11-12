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

; Register map
PPU_CTRL     = $2000
PPU_MASK     = $2001
PPU_STATUS   = $2002
PPU_OAM_ADDR = $2003
PPU_OAM_DATA = $2004
PPU_SCROLL   = $2005
PPU_ADDR     = $2006
PPU_DATA     = $2007
	
	mac PPU_SET_SCROLL_XY
	lda #{0}
	sta PPU_SCROLL
	lda #{1}
	sta PPU_SCROLL
	endm

	mac PPU_SET_ADDR
	lda >#{0}
	sta PPU_ADDR
	lda <#{0}
	sta PPU_ADDR
	endm
	
	mac PPU_SET_ADDR_A
	pha
	lda #{0}
	sta PPU_ADDR
	pla
	sta PPU_ADDR
	endm
	
	; Wait for v blank signales
WAIT_VBLANK SUBROUTINE
	lda PPU_STATUS
	bpl WAIT_VBLANK
	rts
	
WAIT_FRAMES SUBROUTINE
WAIT_FRAMES_LOOP
	jsr WAIT_VBLANK
	dex
	bne WAIT_FRAMES_LOOP
	rts
