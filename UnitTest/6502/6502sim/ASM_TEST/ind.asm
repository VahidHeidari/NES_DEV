; ******************************************************
; This file genrated at : Sun 03/30/2014 22:08:05.70
; 
; Assembler   : DASM.exe
; File name   : idxind.asm
; Description :
; 
; ******************************************************

	PROCESSOR 6502
	ORG $8000

; Main entry point of program.
RESET
; TODO: Write your code here!
	lda #$CF
	sta $00
	lda #$30
	bit $00
	cld
	ldy #$01
	lda ($80,x)
	lda ($10),Y
;	brk
;	nop

LOOP_FOR_EVER
	jmp LOOP_FOR_EVER					; End of program

; Interrupt routines
; Do nothing just for now!
NMI_ROUTINE
IRQ_ROUTINE
	rti

; Vector table
	ORG $FFFA
	dc.w NMI_ROUTINE
	dc.w RESET
	dc.w IRQ_ROUTINE
