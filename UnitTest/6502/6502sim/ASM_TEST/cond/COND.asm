	PROCESSOR 6502
	ORG $8000
	
RESET
	cld				; clear decimal flag
	sei				; disable interrupt
	lda #-128
	cmp #1		; Compare A and #$20
	
	;lda #-128
	;adc #-1
	;adc				; overflow
	
	;lda #-128
	;sec
	;sbc #1			; overflow
	
LOOP_FOR_EVER
	jmp LOOP_FOR_EVER	; End of program.

NMI_ROUTINE
IRQ_ROUTINE
	rti

	ORG $FFFA
	dc.w IRQ_ROUTINE
	dc.w RESET
	dc.w NMI_ROUTINE