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

PRINT_STR:
	ldy #0
PRINT_LOOP:
	lda (STR_PTR),y
	beq PRINT_STR_END
	iny
	sta $2007
	jmp PRINT_LOOP
PRINT_STR_END:
	rts
