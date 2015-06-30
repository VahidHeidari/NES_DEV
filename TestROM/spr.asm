; Byte 0 : Stores the y-coordinate of the top left of the sprite minus 1.
; Byre 1 : Index number of the sprite in the pattern tables.
; Byte 2 : Stores the attributes of the sprite.
;   o Bits 0-1 : Most significant two bits of the colour.
;   o Bits 5   : Indicates whether this sprite has priority over the background.
;   o Bits 6   : Indicates whether to filip the sprite horizontally.
;   o Bits 7   : Indicates wheteher to filip the sprite vertically.
; Byte 3 : X posit

; Y - 1, Tile index, Attribute, X
SPRITES_TABLE
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
; 10
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
; 20
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
; 30
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
; 40
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
; 50
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
; 60
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
	dc.b	$00, $00, $00, $00
; 64