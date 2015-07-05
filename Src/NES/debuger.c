/**
 * NES_DEV is a cross-platform, portable, and hand-held NES emulator.
 *
 * Copyright (C) 2015  Vahid Heidari (DeltaCode)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "debuger.h"

#include <string.h>
#include <stdio.h>

#if defined _WIN32 && !defined __GNUC__
#include <varargs.h>
#elif defined __linux__ || defined __GNUC__
#include <stdarg.h>
#endif

#include "config.h"
#include "test-rom.h"
#include "nes-hdr.h"
#include "cycles.h"

char asm_m[256];
int show_sp = 1;
int show_bg = 1;

void print_regs(pP6502 p)
{
	printf("    A  : %02X            PC : %04X\n", p->a, p->pc.w);
	printf("    X  : %02X            P  : %02X\n", p->x, p->p);
	printf("    Y  : %02X            N V   B D I Z C\n", p->y);
	printf("    SP : %02X            %c %c %c %c %c %c %c %c\n",
			p->sp,
			p->p & N_FLAG ? '1' : '0',
			p->p & V_FLAG ? '1' : '0',
			p->p & FLAG_1 ? '1' : '0',
			p->p & B_FLAG ? '1' : '0',
			p->p & D_FLAG ? '1' : '0',
			p->p & I_FLAG ? '1' : '0',
			p->p & Z_FLAG ? '1' : '0',
			p->p & C_FLAG ? '1' : '0');
	printf("    cc : %d\n\n", p->cycle_counter);
}

uint8_t read_ram(uint16_t addr)
{
	return RAM[addr];
}

void write_ram(uint16_t addr, uint8_t value)
{
	RAM[addr] = value;
}

uint8_t read_rom(uint16_t addr)
{
	return ROM[addr - 0x8000];
}

void write_rom(uint16_t addr, uint8_t value)
{
	ROM[addr - 0x8000] = value;
}

void fill(uint16_t addr, size_t size, uint8_t value)
{
	memset(RAM + addr, value, size);
}

void write_p(pP6502 p, uint8_t value)
{
	p->p = value;
}

void write_a(pP6502 p, uint8_t value)
{
	p->a = value;
}

void write_x(pP6502 p, uint8_t value)
{
	p->x = value;
}

void write_y(pP6502 p, uint8_t value)
{
	p->y = value;
}

void write_sp(pP6502 p, uint8_t value)
{
	p->sp = value;
}

void write_pc(pP6502 p, uint16_t value)
{
	p->pc.w = value;
}

void write_cycle_counter(pP6502 p, int value)
{
	p->cycle_counter = value;
}

void irq_signal(pP6502 p)
{
	p->interrupts |= INTR_IRQ;
}

void reset_signal(pP6502 p)
{
	//p->interrupts |= RESET;
	reset(p);
}

void nmi_signal(pP6502 p)
{
	p->interrupts |= INTR_NMI;
}

int dissassemble(pP6502 p)
{
	int pc = p->pc.w;
	int instruction = read(p->pc.w);

	switch (instruction)
	{
		/* ADC */
		case 0x69: sprintf_s(asm_m, sizeof(asm_m), "ADC #$%02X", read(pc + 1)); break;
		case 0x65: sprintf_s(asm_m, sizeof(asm_m), "ADC $%02X", read(pc + 1)); break;
		case 0x75: sprintf_s(asm_m, sizeof(asm_m), "ADC $%02X,X", read(pc + 1)); break;
		case 0x6D: sprintf_s(asm_m, sizeof(asm_m), "ADC $%02X%02x", read(pc + 2), read(pc + 1)); break;
		case 0x7D: sprintf_s(asm_m, sizeof(asm_m), "ADC $%02X%02X,X", read(pc + 2), read(pc + 1));break;
		case 0x79: sprintf_s(asm_m, sizeof(asm_m), "ADC $%02X%02X,Y", read(pc + 2), read(pc + 1));break;
		case 0x61: sprintf_s(asm_m, sizeof(asm_m), "ADC ($%02X,X)", read(pc + 1)); break;
		case 0x71: sprintf_s(asm_m, sizeof(asm_m), "ADC ($%02X),Y", read(pc + 1)); break;

		/* AND */
		case 0x29: sprintf_s(asm_m, sizeof(asm_m), "AND #$%02X", read(pc + 1)); break;
		case 0x25: sprintf_s(asm_m, sizeof(asm_m), "AND $%02X", read(pc + 1)); break;
		case 0x35: sprintf_s(asm_m, sizeof(asm_m), "AND $%02X,X", read(pc + 1)); break;
		case 0x2D: sprintf_s(asm_m, sizeof(asm_m), "AND $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0x3D: sprintf_s(asm_m, sizeof(asm_m), "AND $%02X%02X,X", read(pc + 2), read(pc + 1)); break;
		case 0x39: sprintf_s(asm_m, sizeof(asm_m), "AND $%02X%02X,Y", read(pc + 2), read(pc + 1)); break;
		case 0x21: sprintf_s(asm_m, sizeof(asm_m), "AND ($%02X,X)", read(pc + 1)); break;
		case 0x31: sprintf_s(asm_m, sizeof(asm_m), "AND ($%02X),Y", read(pc + 1)); break;

		/* ASL */
		case 0x0A: sprintf_s(asm_m, sizeof(asm_m), "ASL A"); break;
		case 0x06: sprintf_s(asm_m, sizeof(asm_m), "ASL $%02X", read(pc + 1)); break;
		case 0x16: sprintf_s(asm_m, sizeof(asm_m), "ASL $%02X,X", read(pc + 1)); break;
		case 0x0E: sprintf_s(asm_m, sizeof(asm_m), "ASL $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0x1E: sprintf_s(asm_m, sizeof(asm_m), "ASL $%02X%02X,X", read(pc + 2), read(pc + 1)); break;

		/* BCC */
		case 0x90: sprintf_s(asm_m, sizeof(asm_m), "BCC $%02X -> 0x%04X"
						   , read(pc + 1), pc + (int8_t)read(pc + 1) + 2); break;

		/* BCS */
		case 0xB0: sprintf_s(asm_m, sizeof(asm_m), "BCS $%02X -> 0x%04X"
						   , read(pc + 1), pc + (int8_t)read(pc + 1) + 2); break;
		
		/* BEQ */
		case 0xF0: sprintf_s(asm_m, sizeof(asm_m), "BEQ $%02X -> 0x%04X"
						   , read(pc + 1), pc + (int8_t)read(pc + 1) + 2); break;
		
		/* BIT */
		case 0x24: sprintf_s(asm_m, sizeof(asm_m), "BIT $%02X", read(pc + 1)); break;
		case 0x2C: sprintf_s(asm_m, sizeof(asm_m), "BIT $%02X%02X", read(pc + 2), read(pc + 1)); break;

		/* BMI */
		case 0x30: sprintf_s(asm_m, sizeof(asm_m), "BMI $%02X -> 0x%04X"
						   , read(pc + 1), pc + (int8_t)read(pc + 1) + 2); break;

		/* BNE */
		case 0xD0: sprintf_s(asm_m, sizeof(asm_m), "BNE $%02X -> 0x%04X"
						   , read(pc + 1), pc + (int8_t)read(pc + 1) + 2); break;

		/* BPL */
		case 0x10: sprintf_s(asm_m, sizeof(asm_m), "BPL $%02X -> 0x%04X"
						   , read(pc + 1), pc + (int8_t)read(pc + 1) + 2); break;

		/* BRK */
		case 0x00: sprintf_s(asm_m, sizeof(asm_m), "BRK"); break;

		/* BVC */
		case 0x50: sprintf_s(asm_m, sizeof(asm_m), "BVC $%02X -> 0x%04X"
						   , read(pc + 1), pc + (int8_t)read(pc + 1) + 2); break;

		/* BVS */
		case 0x70: sprintf_s(asm_m, sizeof(asm_m), "BVS $%02X -> 0x%04X"
						   , read(pc + 1), pc + read(pc + 1)); break;

		/* CLC */
		case 0x18: sprintf_s(asm_m, sizeof(asm_m), "CLC"); break;

		/* CLD */
		case 0xD8: sprintf_s(asm_m, sizeof(asm_m), "CLD"); break;

		/* CLI */
		case 0x58: sprintf_s(asm_m, sizeof(asm_m), "CLI"); break;

		/* CLV */
		case 0xB8: sprintf_s(asm_m, sizeof(asm_m), "CLV"); break;

		/* CMP */
		case 0xC9: sprintf_s(asm_m, sizeof(asm_m), "CMP #$%02X", read(pc + 1)); break;
		case 0xC5: sprintf_s(asm_m, sizeof(asm_m), "CMP $%02X", read(pc + 1)); break;
		case 0xD5: sprintf_s(asm_m, sizeof(asm_m), "CMP $%02X,X", read(pc + 1)); break;
		case 0xCD: sprintf_s(asm_m, sizeof(asm_m), "CMP $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0xDD: sprintf_s(asm_m, sizeof(asm_m), "CMP $%02X%02X,X", read(pc + 2), read(pc + 1)); break;
		case 0xD9: sprintf_s(asm_m, sizeof(asm_m), "CMP $%02X%02X,Y", read(pc + 2), read(pc + 1)); break;
		case 0xC1: sprintf_s(asm_m, sizeof(asm_m), "CMP (%02X,X)", read(pc + 1)); break;
		case 0xD1: sprintf_s(asm_m, sizeof(asm_m), "CMP (%02X),Y", read(pc + 1)); break;

		/* CPX */
		case 0xE0: sprintf_s(asm_m, sizeof(asm_m), "CPX #$%02X", read(pc + 1)); break;
		case 0xE4: sprintf_s(asm_m, sizeof(asm_m), "CPX $%02X", read(pc + 1)); break;
		case 0xEC: sprintf_s(asm_m, sizeof(asm_m), "CPX $%02X%02X", read(pc + 2), read(pc + 1)); break;
		
		/* CPY */
		case 0xC0: sprintf_s(asm_m, sizeof(asm_m), "CPY #$%02X", read(pc + 1)); break;
		case 0xC4: sprintf_s(asm_m, sizeof(asm_m), "CPY $%02X", read(pc + 1)); break;
		case 0xCC: sprintf_s(asm_m, sizeof(asm_m), "CPY $%02X%02X", read(pc + 2), read(pc + 1)); break;
		
		/* DEC */
		case 0xC6: sprintf_s(asm_m, sizeof(asm_m), "DEC $%02X", read(pc + 1)); break;
		case 0xD6: sprintf_s(asm_m, sizeof(asm_m), "DEC $%02X,X", read(pc + 1)); break;
		case 0xCE: sprintf_s(asm_m, sizeof(asm_m), "DEC $%02X%02x", read(pc + 2), read(pc + 1)); break;
		case 0xDE: sprintf_s(asm_m, sizeof(asm_m), "DEC $%02X%02x,X", read(pc + 2), read(pc + 1)); break;

		/* DEX */
		case 0xCA: sprintf_s(asm_m, sizeof(asm_m), "DEX"); break;

		/* DEY */
		case 0x88: sprintf_s(asm_m, sizeof(asm_m), "DEY"); break;

		/* EOR */
		case 0x49: sprintf_s(asm_m, sizeof(asm_m), "EOR #$%02X", read(pc + 1)); break;
		case 0x45: sprintf_s(asm_m, sizeof(asm_m), "EOR $%02X", read(pc + 1)); break;
		case 0x55: sprintf_s(asm_m, sizeof(asm_m), "EOR $%02X,X", read(pc + 1)); break;
		case 0x4D: sprintf_s(asm_m, sizeof(asm_m), "EOR $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0x5D: sprintf_s(asm_m, sizeof(asm_m), "EOR $%02X%02X,X", read(pc + 2), read(pc + 1)); break;
		case 0x59: sprintf_s(asm_m, sizeof(asm_m), "EOR $%02X%02X,Y", read(pc + 2), read(pc + 1)); break;
		case 0x41: sprintf_s(asm_m, sizeof(asm_m), "EOR ($%02X,X)", read(pc + 1)); break;
		case 0x51: sprintf_s(asm_m, sizeof(asm_m), "EOR ($%02X),Y", read(pc + 1)); break;

		/* INC */
		case 0xE6: sprintf_s(asm_m, sizeof(asm_m), "INC $%02X", read(pc + 1)); break;
		case 0xF6: sprintf_s(asm_m, sizeof(asm_m), "INC $%02X,X", read(pc + 1)); break;
		case 0xEE: sprintf_s(asm_m, sizeof(asm_m), "INC $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0xFE: sprintf_s(asm_m, sizeof(asm_m), "INC $%02X%02X,X", read(pc + 2), read(pc + 1)); break;
		
		/* INX */
		case 0xE8: sprintf_s(asm_m, sizeof(asm_m), "INX"); break;
		
		/* INY */
		case 0xC8: sprintf_s(asm_m, sizeof(asm_m), "INY"); break;

		/* JMP */
		case 0x4C: sprintf_s(asm_m, sizeof(asm_m), "JMP $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0x6C: sprintf_s(asm_m, sizeof(asm_m), "JMP ($%02X%02X)", read(pc + 2), read(pc + 1)); break;

		/* JSR */
		case 0x20: sprintf_s(asm_m, sizeof(asm_m), "JSR $%02X%02X", read(pc + 2), read(pc + 1)); break;

		/* LDA */
		case 0xA9: sprintf_s(asm_m, sizeof(asm_m), "LDA #$%02X", read(pc + 1)); break;
		case 0xA5: sprintf_s(asm_m, sizeof(asm_m), "LDA $%02X", read(pc + 1)); break;
		case 0xB5: sprintf_s(asm_m, sizeof(asm_m), "LDA $%02X,X", read(pc + 1)); break;
		case 0xAD: sprintf_s(asm_m, sizeof(asm_m), "LDA $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0xBD: sprintf_s(asm_m, sizeof(asm_m), "LDA $%02X%02X,X", read(pc + 2), read(pc + 1)); break;
		case 0xB9: sprintf_s(asm_m, sizeof(asm_m), "LDA $%02X%02X,Y", read(pc + 2), read(pc + 1)); break;
		case 0xA1: sprintf_s(asm_m, sizeof(asm_m), "LDA ($%02X,X)", read(pc + 1)); break;
		case 0xB1: sprintf_s(asm_m, sizeof(asm_m), "LDA ($%02X),Y", read(pc + 1)); break;

		/* LDX */
		case 0xA2: sprintf_s(asm_m, sizeof(asm_m), "LDX #$%02X", read(pc + 1)); break;
		case 0xA6: sprintf_s(asm_m, sizeof(asm_m), "LDX $%02X", read(pc + 1)); break;
		case 0xB6: sprintf_s(asm_m, sizeof(asm_m), "LDX $%02X,Y", read(pc + 1)); break;
		case 0xAE: sprintf_s(asm_m, sizeof(asm_m), "LDX $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0xBE: sprintf_s(asm_m, sizeof(asm_m), "LDX $%02X%02X,Y", read(pc + 2), read(pc + 1)); break;

		/* LDY */
		case 0xA0: sprintf_s(asm_m, sizeof(asm_m), "LDY #$%02X", read(pc + 1)); break;
		case 0xA4: sprintf_s(asm_m, sizeof(asm_m), "LDY $%02X", read(pc + 1)); break;
		case 0xB4: sprintf_s(asm_m, sizeof(asm_m), "LDY $%02X,X", read(pc + 1)); break;
		case 0xAC: sprintf_s(asm_m, sizeof(asm_m), "LDY $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0xBC: sprintf_s(asm_m, sizeof(asm_m), "LDY $%02X%02X,X", read(pc + 2), read(pc + 1)); break;

		/* LSR */
		case 0x4A: sprintf_s(asm_m, sizeof(asm_m), "LSR A"); break;
		case 0x46: sprintf_s(asm_m, sizeof(asm_m), "LSR $%02X", read(pc + 1)); break;
		case 0x56: sprintf_s(asm_m, sizeof(asm_m), "LSR $%02X,X", read(pc + 1)); break;
		case 0x4E: sprintf_s(asm_m, sizeof(asm_m), "LSR $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0x5E: sprintf_s(asm_m, sizeof(asm_m), "LSR $%02X%02X,X", read(pc + 2), read(pc + 1)); break;

		/* NOP */
		case 0xEA: sprintf_s(asm_m, sizeof(asm_m), "NOP"); break;

		/* ORA */
		case 0x09: sprintf_s(asm_m, sizeof(asm_m), "ORA #$%02X", read(pc + 1)); break;
		case 0x05: sprintf_s(asm_m, sizeof(asm_m), "ORA $%02X", read(pc + 1)); break;
		case 0x15: sprintf_s(asm_m, sizeof(asm_m), "ORA $%02X,X", read(pc + 1)); break;
		case 0x0D: sprintf_s(asm_m, sizeof(asm_m), "ORA $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0x1D: sprintf_s(asm_m, sizeof(asm_m), "ORA $%02X%02X,X", read(pc + 2), read(pc + 1)); break;
		case 0x19: sprintf_s(asm_m, sizeof(asm_m), "ORA $%02X%02X,Y", read(pc + 2), read(pc + 1)); break;
		case 0x01: sprintf_s(asm_m, sizeof(asm_m), "ORA ($%02X,X)", read(pc + 1)); break;
		case 0x11: sprintf_s(asm_m, sizeof(asm_m), "ORA ($%02X),Y", read(pc + 1)); break;

		/* PHA */
		case 0x48: sprintf_s(asm_m, sizeof(asm_m), "PHA"); break;

		/* PHP */
		case 0x08: sprintf_s(asm_m, sizeof(asm_m), "PHP"); break;
		
		/* PLA */
		case 0x68: sprintf_s(asm_m, sizeof(asm_m), "PLA"); break;
		
		/* PLP */
		case 0x28: sprintf_s(asm_m, sizeof(asm_m), "PLP"); break;

		/* ROL */
		case 0x2A: sprintf_s(asm_m, sizeof(asm_m), "ROL A"); break;
		case 0x26: sprintf_s(asm_m, sizeof(asm_m), "ROL $%02X", read(pc + 1)); break;
		case 0x36: sprintf_s(asm_m, sizeof(asm_m), "ROL $%02X,X", read(pc + 1)); break;
		case 0x2E: sprintf_s(asm_m, sizeof(asm_m), "ROL $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0x3E: sprintf_s(asm_m, sizeof(asm_m), "ROL $%02X%02X,X", read(pc + 2), read(pc + 1)); break;

		/* ROR */
		case 0x6A: sprintf_s(asm_m, sizeof(asm_m), "ROL A"); break;
		case 0x66: sprintf_s(asm_m, sizeof(asm_m), "ROL $%02X", read(pc + 1)); break;
		case 0x76: sprintf_s(asm_m, sizeof(asm_m), "ROL $%02X,X", read(pc + 1)); break;
		case 0x6E: sprintf_s(asm_m, sizeof(asm_m), "ROL $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0x7E: sprintf_s(asm_m, sizeof(asm_m), "ROL $%02X%02X,X", read(pc + 2), read(pc + 1)); break;

		/* RTI */
		case 0x40: sprintf_s(asm_m, sizeof(asm_m), "RTI"); break;

		/* RTS */
		case 0x60: sprintf_s(asm_m, sizeof(asm_m), "RTS"); break;

		/* SBC */
		case 0xE9: sprintf_s(asm_m, sizeof(asm_m), "SBC #$%02X", read(pc + 1)); break;
		case 0xE5: sprintf_s(asm_m, sizeof(asm_m), "SBC $%02X", read(pc + 1)); break;
		case 0xF5: sprintf_s(asm_m, sizeof(asm_m), "SBC $%02X,X", read(pc + 1)); break;
		case 0xED: sprintf_s(asm_m, sizeof(asm_m), "SBC $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0xFD: sprintf_s(asm_m, sizeof(asm_m), "SBC $%02X%02X,X", read(pc + 2), read(pc + 1)); break;
		case 0xF9: sprintf_s(asm_m, sizeof(asm_m), "SBC $%02X%02X,Y", read(pc + 2), read(pc + 1)); break;
		case 0xE1: sprintf_s(asm_m, sizeof(asm_m), "SBC ($%02X,X)", read(pc + 1)); break;
		case 0xF1: sprintf_s(asm_m, sizeof(asm_m), "SBC ($%02X),Y", read(pc + 1)); break;

		/* SEC */
		case 0x38: sprintf_s(asm_m, sizeof(asm_m), "SEC"); break;

		/* SED */
		case 0xF8: sprintf_s(asm_m, sizeof(asm_m), "SED"); break;

		/* SEI */
		case 0x78: sprintf_s(asm_m, sizeof(asm_m), "SEI"); break;

		/* STA */
		case 0x85: sprintf_s(asm_m, sizeof(asm_m), "STA $%02X", read(pc + 1)); break;
		case 0x95: sprintf_s(asm_m, sizeof(asm_m), "STA $%02X,X", read(pc + 1)); break;
		case 0x8D: sprintf_s(asm_m, sizeof(asm_m), "STA $%02X%02X", read(pc + 2), read(pc + 1)); break;
		case 0x9D: sprintf_s(asm_m, sizeof(asm_m), "STA $%02X%02X,X", read(pc + 2), read(pc + 1)); break;
		case 0x99: sprintf_s(asm_m, sizeof(asm_m), "STA $%02X%02X,Y", read(pc + 2), read(pc + 1)); break;
		case 0x81: sprintf_s(asm_m, sizeof(asm_m), "STA ($%02X,X)", read(pc + 1)); break;
		case 0x91: sprintf_s(asm_m, sizeof(asm_m), "STA ($%02X),Y", read(pc + 1)); break;

		/* STX */
		case 0x86: sprintf_s(asm_m, sizeof(asm_m), "STX $%02X", read(pc + 1)); break;
		case 0x96: sprintf_s(asm_m, sizeof(asm_m), "STX $%02X,Y", read(pc + 1)); break;
		case 0x8E: sprintf_s(asm_m, sizeof(asm_m), "STX $%02X%02X", read(pc + 2), read(pc + 1)); break;
	
		/* STY */
		case 0x84: sprintf_s(asm_m, sizeof(asm_m), "STY $%02X", read(pc + 1)); break;
		case 0x94: sprintf_s(asm_m, sizeof(asm_m), "STY $%02X,X", read(pc + 1)); break;
		case 0x8C: sprintf_s(asm_m, sizeof(asm_m), "STY $%02X%02X", read(pc + 2), read(pc + 1)); break;

		/* TAX */
		case 0xAA: sprintf_s(asm_m, sizeof(asm_m), "TAX"); break;

		/* TAY */
		case 0xA8: sprintf_s(asm_m, sizeof(asm_m), "TAY"); break;

		/* TSX */
		case 0xBA: sprintf_s(asm_m, sizeof(asm_m), "TSX"); break;

		/* TXA */
		case 0x8A: sprintf_s(asm_m, sizeof(asm_m), "TXA"); break;

		/* TXS */
		case 0x9A: sprintf_s(asm_m, sizeof(asm_m), "TXS"); break;

		/* TYA */
		case 0x98: sprintf_s(asm_m, sizeof(asm_m), "TYA"); break;

		default: sprintf_s(asm_m, sizeof(asm_m), "???    %02X", instruction);
	}

	// return (op_size[instruction] ? op_size[instruction] : 1);// Return 1 for undefined instructions for mass dissassembling insted of byte by byte.
	return op_size[instruction];
}

void init_test_rom(NesHeader* hdr, uint8_t* rom, uint8_t* chr_rom)
{
	memset(hdr, 0, sizeof(NesHeader));

	// Magic numbers of NES header
	hdr->nes_str[0] = 'N';
	hdr->nes_str[1] = 'E';
	hdr->nes_str[2] = 'S';
	hdr->magic_number = MAGIC_NUMBER;
	// ROM banks.
	hdr->num_of_rom_banks = 1;
	// CHAR ROMs
	hdr->num_of_vrom_banks = 1;
	// Mapper low nibble
	hdr->rom_control_1 = 0x01;
	// Mapper high nibble
	hdr->rom_control_2 = 0x00;

	// Only one rom at $C000
	memcpy(rom + ROM_SIZE / 2, TEST_ROM_bin, TEST_ROM_bin_len);
	// Pattern table
	memcpy(chr_rom, sp_bin, sp_bin_len);
	memcpy(chr_rom + PPU_PATTERN_TABLE_SIZE / 2, bg_bin, bg_bin_len);
	// Vector table
	memcpy(rom + ROM_SIZE - 6, vect_bin, vect_bin_len);
	mirroring = GET_MIRRORING_TYPE(hdr);
	image_path = "Embedded test ROM image!";
}

int dump_test_rom(const char* path)
{
	int i;
	int blank_space;
	NesHeader hdr;
	FILE* dump = NULL;

#if defined _WIN32 && !defined __GNUC__
	fopen_s(&dump, path, "wb");
#else
	dump = fopen(path, "wb");
#endif

	if (!dump)
		return 0;

	// Initialize iNES header.
	memset(&hdr, 0, sizeof(NesHeader));

	// Magic numbers of NES header
	hdr.nes_str[0] = 'N';
	hdr.nes_str[1] = 'E';
	hdr.nes_str[2] = 'S';
	hdr.magic_number = MAGIC_NUMBER;
	// ROM banks.
	hdr.num_of_rom_banks = 1;
	// CHAR ROMs
	hdr.num_of_vrom_banks = 1;
	// Mapper low nibble
	hdr.rom_control_1 = 0x01;
	// Mapper high nibble
	hdr.rom_control_2 = 0x00;

	// Dump iNES header.
	if (fwrite(&hdr, sizeof(NesHeader), 1, dump) != 1)
		return 0;

	// Dump ROM image.
	if (fwrite(TEST_ROM_bin, TEST_ROM_bin_len, 1, dump) != 1)
		return 0;

	blank_space = hdr.num_of_rom_banks * 16 * 1024 - TEST_ROM_bin_len - vect_bin_len;
	if (blank_space > 0)
		for (i = 0; i < blank_space; ++i)
			fputc(0, dump);

	// Dump vector table.
	if (fwrite(vect_bin, vect_bin_len, 1, dump) != 1)
		return 0;

	// Dump pattern image.
	if (fwrite(sp_bin, sp_bin_len, 1, dump) != 1)
		return 0;

	blank_space = 4 * 1024 - sp_bin_len;
	if (blank_space > 0)
		for (i = 0; i < blank_space; ++i)
			fputc(0, dump);

	if (fwrite(bg_bin, bg_bin_len, 1, dump) != 1)
		return 0;

	blank_space = 4 * 1024 - bg_bin_len;
	if (blank_space > 0)
		for (i = 0; i < blank_space; ++i)
			fputc(0, dump);

	return 1;
}

void log_info(const char* str, ...)
{
#ifdef DEBUG_LOGGING
	va_list args;
	FILE* f = fopen("log.txt", "a");
	if (!f)
		return;
	fprintf(f, "INFO    : ");
	va_start(args, str);
	vfprintf(f, str, args);
	va_end(args);
	fprintf(f, "\n");
	fclose(f);
#else
	(void)str;
#endif
}

void log_warning(const char* str, ...)
{
#ifdef DEBUG_LOGGING
	va_list args;
	FILE* f = fopen("log.txt", "a");
	if (!f)
		return;
	va_start(args, str);
	fprintf(f, "WARNING : ");
	vfprintf(f, str, args);
	fprintf(f, "\n");
	va_end(args);
	fclose(f);
#else
	(void)str;
#endif
}

void log_error(const char* str, ...)
{
#ifdef DEBUG_LOGGING
	va_list args;
	FILE* f = fopen("log.txt", "a");
	if (!f)
		return;
	va_start(args, str);
	fprintf(f, "ERROR   : ");
	vfprintf(f, str, args);
	fprintf(f, "\n");
	va_end(args);
	fclose(f);
#else
	(void)str;
#endif
}

void debug_message(const char* str, ...)
{
#if defined DEBUG_MODE
	va_list args;

	va_start(args, str);
	vprintf(str, args);
	putchar('\n');
	va_end(args);
#endif
}

