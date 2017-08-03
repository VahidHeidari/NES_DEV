	PROCESSOR 6502
	ORG $8000
	
RESET
	lda #$01
	asl					; 0x01 << 1   = 0b00000010
	asl					; 0x01 << 2   = 0b00000100
	asl					; 0x01 << 3   = 0b00001000
	asl					; 0x01 << 4   = 0b00010000
	asl					; 0x01 << 5   = 0b00100000
	asl					; 0x01 << 6   = 0b01000000
	asl					; 0x01 << 7   = 0b10000000
	asl					; 0x01 << 8   = 0b00000000    C = 1, Z = 1

LOOP_FOR_EVER
	jmp LOOP_FOR_EVER	; End of program
	
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