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

; Byte 0 : Stores the y-coordinate of the top left of the sprite minus 1.
; Byre 1 : Index number of the sprite in the pattern tables.
; Byte 2 : Stores the attributes of the sprite.
;   o Bits 0-1 : Most significant two bits of the colour.
;   o Bits 5   : Indicates whether this sprite has priority over the background.
;   o Bits 6   : Indicates whether to filip the sprite horizontally.
;   o Bits 7   : Indicates wheteher to filip the sprite vertically.
; Byte 3 : X posit

; Y - 1, Tile index, Attribute, X
SPRITES_TABLE:
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
; 10
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
; 20
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
; 30
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
; 40
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
; 50
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
; 60
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
; 64