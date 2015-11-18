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

NUM_OF_SPRITE_BYTES = 4

MASTER_CHEAF_HEIGHT = 3
MASTER_CHEAF_WIDTH  = 2
MASTER_CHEAF_FRAMES = 8
BYTES_PER_MASTER_CHEAF_FRAME = MASTER_CHEAF_HEIGHT * MASTER_CHEAF_WIDTH * NUM_OF_SPRITE_BYTES


; Byte 0 : Stores the y-coordinate of the top left of the sprite minus 1.
; Byte 1 : Index number of the sprite in the pattern tables.
; Byte 2 : Stores the attributes of the sprite.
;   o Bits 0-1 : Most significant two bits of the colour.
;   o Bits 5   : Indicates whether this sprite has priority over the background.
;   o Bits 6   : Indicates whether to filip the sprite horizontally.
;   o Bits 7   : Indicates wheteher to filip the sprite vertically.
; Byte 3 : X posit

; Y - 1, Tile index, Attribute, X
SPRITES_TABLE:
FRAME_0:
.byte	$20, $D0, $00, $20
.byte	$20, $D1, $00, $28
.byte	$28, $E0, $00, $20
.byte	$28, $E1, $00, $28
.byte	$30, $F0, $00, $20
.byte	$30, $F1, $00, $28

FRAME_1:
.byte	$20, $D0, $00, $20
.byte	$20, $D1, $00, $28
.byte	$28, $E0, $00, $20
.byte	$28, $E1, $00, $28		; 10
.byte	$30, $F2, $00, $20
.byte	$30, $F3, $00, $28

FRAME_2:
.byte	$20, $D0, $00, $20
.byte	$20, $D1, $00, $28
.byte	$28, $E4, $00, $20
.byte	$28, $E5, $00, $28
.byte	$30, $F4, $00, $20
.byte	$30, $F5, $00, $28

FRAME_3:
.byte	$20, $D0, $00, $20
.byte	$20, $D1, $00, $28		; 20
.byte	$28, $E4, $00, $20
.byte	$28, $E5, $00, $28
.byte	$30, $F6, $00, $20
.byte	$30, $F7, $00, $28

FRAME_4:
.byte	$20, $D0, $00, $20
.byte	$20, $D1, $00, $28
.byte	$28, $E8, $00, $20
.byte	$28, $E9, $00, $28
.byte	$30, $F8, $00, $20
.byte	$30, $F9, $00, $28		; 30

FRAME_5:
.byte	$20, $D0, $00, $20
.byte	$20, $D1, $00, $28
.byte	$28, $E8, $00, $20
.byte	$28, $E9, $00, $28
.byte	$30, $FA, $00, $20
.byte	$30, $FB, $00, $28

FRAME_6:
.byte	$20, $D0, $00, $20
.byte	$20, $D1, $00, $28
.byte	$28, $EC, $00, $20
.byte	$28, $ED, $00, $28		; 40
.byte	$30, $FC, $00, $20
.byte	$30, $FD, $00, $28

FRAME_7:
.byte	$20, $D0, $00, $20
.byte	$20, $D1, $00, $28
.byte	$28, $EC, $00, $20
.byte	$28, $ED, $00, $28
.byte	$30, $FE, $00, $20
.byte	$30, $FF, $00, $28

.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00		; 50
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00

.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00		; 60

.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00		; 64
.byte	$00, $00, $00, $00
.byte	$00, $00, $00, $00
