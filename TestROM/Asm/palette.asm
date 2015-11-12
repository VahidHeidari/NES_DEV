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

PALETTE_TABLE:
IMAGE_PALETTE:
	.byte $0D
	.byte $30
	.byte $12
	.byte $05
	.byte $0D
	.byte $15
	.byte $09
	.byte $19
	.byte $0D
	.byte $0C
	.byte $1C
	.byte $0E
	.byte $0D
	.byte $1F
	.byte $20
	.byte $30

SPRITE_PALETTE:	
	.byte $0D		; Background color
	.byte $22
	.byte $32
	.byte $25
	.byte $0D
	.byte $35
	.byte $29
	.byte $39
	.byte $0D
	.byte $2C
	.byte $3C
	.byte $2D
	.byte $0D
	.byte $3D
	.byte $2F
	.byte $3F
