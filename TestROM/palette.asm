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

PALETTE_TABLE
IMAGE_PALETTE
	dc.b $0D
	dc.b $02
	dc.b $12
	dc.b $05
	dc.b $0D
	dc.b $15
	dc.b $09
	dc.b $19
	dc.b $0D
	dc.b $0C
	dc.b $1C
	dc.b $0E
	dc.b $0D
	dc.b $1F
	dc.b $20
	dc.b $30

SPRITE_PALETTE	
	dc.b $0D
	dc.b $22
	dc.b $32
	dc.b $25
	dc.b $0D
	dc.b $35
	dc.b $29
	dc.b $39
	dc.b $0D
	dc.b $2C
	dc.b $3C
	dc.b $2D
	dc.b $0D
	dc.b $3D
	dc.b $2F
	dc.b $3F

	