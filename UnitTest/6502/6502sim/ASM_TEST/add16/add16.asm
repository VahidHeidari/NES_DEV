; ******************************************************
; This file genrated at : Mon 03/24/2014  1:16:04.73
; 
; Assembler   : DASM.exe
; File name   : add16.asm
; Description : Adding two 16 bit numbers at $20, $21
;     and $22, $23 and store result at $24, 25
; ******************************************************

	PROCESSOR 6502
	ORG $8000

; Main entry point of program.
RESET
; TODO: Write your code here!
	clc									; Clear carry
	lda $20								; Load low byte
	adc $22								; Add with low byte of second number
	sta $24								; Store result at low byte
	
	lda $21								; Load high byte
	adc $23								; Add with high byte of second number
	sta $25								; Store result at high byte

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
