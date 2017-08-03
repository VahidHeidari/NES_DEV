; ******************************************************
; This file genrated at : Wed 03/26/2014 20:18:30.32
; 
; Assembler   : DASM.exe
; File name   : unlist.asm
; Description :
; 
; ******************************************************

	PROCESSOR 6502
	ORG $8000

; Main entry point of program.
RESET
; TODO: Write your code here!

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
