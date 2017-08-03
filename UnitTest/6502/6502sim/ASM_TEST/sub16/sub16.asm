; ******************************************************
; This file genrated at : Mon 03/24/2014  2:37:39.90
; 
; Assembler   : DASM.exe
; File name   : sub16.asm
; Description : Subtracts two 16 bit numbers at $20,21
;     and $22,23 and store result at $24,25
; ******************************************************

	PROCESSOR 6502
	ORG $8000

; Main entry point of program.
RESET
; TODO: Write your code here!
	sec							; Set carry flag
	lda $20						; Get the low byte of the first number
	sbc $22						; Subtract form the low byte of the second number
	sta $24						; Store the result to low byte of the result
	
	lda $21						; Get the high byte of the first number
	sbc $23						; Subtract from the highe byte of the second number
	lda $25						; Store the result to high byte of the result
	
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
