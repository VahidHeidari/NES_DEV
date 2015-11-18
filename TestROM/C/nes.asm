;-----------------------------------
; This is an 'NES' test program.
; Assembler is ca65.exe
; Date 2015/11/13 Thursday 19:10
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

.import initlib
.import push0,zerobss,copydata
.import _main,_wait_vblank

.export _exit,__STARTUP__:absolute=1

PPU_CTRL = $2000
PPU_MASK = $2001

.segment "HEADER"
.byte "NES", $1A	; Signiture
.byte $2			; Number of 16KB ROM banks
.byte $1			; Number of 8KB CHR ROM banks
.byte $0			; Contorl byte 1
.byte $0			; Control byte 2
.byte $0			; Number of 8K RAM Banks
.byte $0, $0		; Reserverd
.byte $0, $0		; Reserverd
.byte $0, $0		; Reserverd
.byte $0			; Reserverd

.segment "STARTUP"
start:
_exit:
	sei					; Disable IRQ.
	ldx #$FF			; Initialize stack pointer.
	txs

	inx					; Clear system RAM.
	lda #0
	sta PPU_CTRL		; Disable NMI.
	sta PPU_MASK		; Disable rendering.

clear_RAM:
	sta $00,x
	sta $100,x
	sta $200,x
	sta $300,x
	sta $400,x
	sta $500,x
	sta $600,x
	sta $700,x
	inx
	bne clear_RAM

	jsr zerobss
	jsr copydata
	jsr _main

.segment "CODE"
_nmi_handler:
_irq_handler:
	rti

.segment "VECTORS"
.word 0
.word 0
.word 0

.word _nmi_handler
.word start
.word _irq_handler

.segment "CHARS"
.incbin "ASCII.chr"