;-----------------------------------
; This is an 'NES' test program.
; Assembler is ca65.exe
; Date 2015/11/17 Thursday 11:02
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

; Register map
PPU_CTRL     = $2000
PPU_MASK     = $2001
PPU_STATUS   = $2002
PPU_OAM_ADDR = $2003
PPU_OAM_DATA = $2004
PPU_SCROLL   = $2005
PPU_ADDR     = $2006
PPU_DATA     = $2007

.export _wait_vblank
.export _wait_frames

.macro PPU_SET_OAM_ADDR addr
	lda #addr
	sta PPU_OAM_ADDR
.endmacro

.macro PPU_SET_SCROLL_XY cX, cY
	lda #cX
	sta PPU_SCROLL
	lda #cY
	sta PPU_SCROLL
.endmacro

.macro PPU_SET_ADDR addr
	lda #>addr
	sta PPU_ADDR
	lda #<addr
	sta PPU_ADDR
.endmacro

.macro PPU_SET_ADDR_A addr_high_byte
	pha
	lda #addr_high_byte
	sta PPU_ADDR
	pla
	sta PPU_ADDR
.endmacro

	; Wait for v blank signales
_wait_vblank:
	lda PPU_STATUS
	bpl _wait_vblank
	rts

_wait_frames:
	tax
WAIT_FRAMES_LOOP:
	jsr _wait_vblank
	dex
	bne WAIT_FRAMES_LOOP
	rts
