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

PPU_CTRL	= $2000
PPU_MASK	= $2001
PPU_SCROLL	= $2005
PPU_ADDR	= $2006
PPU_DATA	= $2007

.segment "ZEROPAGE"
DATA_PTR = $00

.macro SET_DATA_PTR addr
	lda #<addr
	sta DATA_PTR
	lda #>addr
	sta DATA_PTR+1
.endmacro

.macro PPU_SET_ADDR addr
	lda #>addr
	sta PPU_ADDR
	lda #<addr
	sta PPU_ADDR
.endmacro

.segment "CODE"
NAME_STR:
.byte "Vahid Heidari", 0

.proc print
	ldy #0
PRINT_LOOP:
	lda (DATA_PTR),y
	beq EXIT
	sta PPU_DATA
	iny
	jmp PRINT_LOOP
EXIT:
	rts

.endproc
MAIN:
	sei				; Disable Interrupt
	ldx #0
	stx PPU_CTRL	; Disable NMI
	stx PPU_MASK	; Disable rendering

	dex				; Initialize stack pointer
	txs				; Stack pointer is initialized to $FF

	PPU_SET_ADDR $3F00
	
	lda #$0D
	sta PPU_DATA
	lda #$30
	sta PPU_DATA
	
	SET_DATA_PTR NAME_STR
	PPU_SET_ADDR $2081
	jsr print
	PPU_SET_ADDR $0000
	
	lda #$80
	sta PPU_CTRL
	lda #$8C
	sta PPU_MASK
	
LOOP_FOR_EVER:
	jmp LOOP_FOR_EVER

NMI_HANDLER:
IRQ_HANDLER:
	rti

.segment "VECTORS"
.word MAIN
.word MAIN
.word MAIN

.word IRQ_HANDLER
.word MAIN
.word NMI_HANDLER

.segment "CHARS"
.incbin "Resources\ASCII.chr"
