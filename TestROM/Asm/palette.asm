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

PALETTE_TABLE:
IMAGE_PALETTE:
.byte $0D, $30, $12, $05
.byte $0D, $28, $09, $19
.byte $0D, $0C, $1C, $0E
.byte $0D, $1F, $20, $30

SPRITE_PALETTE:	
.byte $0D, $1C, $2B, $39		; Background color
.byte $0D, $35, $29, $39
.byte $0D, $2C, $3C, $2D
.byte $0D, $3D, $2F, $3F
