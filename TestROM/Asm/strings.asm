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
	
.macro SET_STR_PTR addr
	lda #<addr
	sta STR_PTR
	lda #>addr
	sta STR_PTR+1
.endmacro
	
INTRO_STR_1:
.byte "Copyright ' 2015,Vahid Heidari", 0
INTRO_STR_2:
.byte "(DeltaCode)", 0
INTRO_STR_3:
.byte "Powered by CC65 Compiler", 0
INTRO_STR_4:
.byte "You can find source codes at:",0
INTRO_STR_5:
.byte "https://github.com/VahidHeidari/",0
INTRO_STR_6:
.byte "NES_DEV",0

.proc _print_str
	ldy #0
PRINT_LOOP:
	lda (STR_PTR),y
	beq PRINT_STR_END
	iny
	sta $2007
	jmp PRINT_LOOP
PRINT_STR_END:
	rts
.endproc

.proc _print_hex_a
	pha								; Save A reg.
	ldy #0
	SHIFT_LEFT 4					; A >>= 4.
	tax								; X reg. used as index.
	lda HEX_NUMBERS,x
	sta (STR_PTR),y					; Store high nibble into string buffer.
	iny
	
	pla								; Restore A reg.
	and #$0F						; A &= 0x0F.
	tax								; X reg. used as index.
	lda HEX_NUMBERS,x
	sta (STR_PTR),y					; Store low nibble into string buffer.
	iny
	
	lda #0							; Trailing null character
	sta (STR_PTR),y
	rts
HEX_NUMBERS:
.byte "0123456789ABCDEF"
.endproc

.proc _str_len
	ldy #0
@STR_LEN_LOOP:
	lda (STR_PTR),y
	beq @STR_LEN_EXIT
@STR_LEN_INCREMENT:
	iny
	bne @STR_LEN_LOOP
@STR_LEN_EXIT:
	rts
.endproc

.proc _str_rev
	ldy #0
@STR_REV_LOOP:
	lda (STR_PTR),y
	beq @STR_REV_LOOP_EXIT
	pha
	iny
	bne @STR_REV_LOOP
@STR_REV_LOOP_EXIT:
	tya
	tax
	ldy #0
@REV_LOOP:
	pla
	sta (STR_PTR),y
	iny
	dex
	bne @REV_LOOP
	rts
.endproc
