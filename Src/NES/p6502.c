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

#include "p6502.h"

#include <string.h>

#include "cycles.h"
#include "emulator.h"
#include "io.h"
#include "debuger.h"
#include "bank-switch.h"
#include "mapper.h"

/// TODO: Add accurate tick counting to all memory access's.

/**
 * Utility macros for calculating effective address. This macros stores
 * effective address in addr variable, and uses tmp for temporary store
 * some calculations. In some ABS_X_C  ABS_Y_C  and  IND_Y_C ins_cycles 
 * correction had been performed.
 */
/// Immediate
/// TODO: #define GET_IMMEDIAE(addr) addr.w = p->pc.w++;
/// and change all IMM instructions.
#define GET_IMMEDIATE(val) val.w = read(p->pc.w++)

/// Z PAGE
#define GET_ZERO_PAGE(addr) addr.w = read(p->pc.w++)

/// Z PAGE, X
#define GET_ZERO_PAGE_X(addr) do {	\
	addr.w = read(p->pc.w++);		\
	addr.b.l += p->x;				\
} while (0)

/// Z PAGE, Y
#define GET_ZERO_PAGE_Y(addr) do {	\
	addr.w = read(p->pc.w++);		\
	addr.b.l += p->y;				\
} while (0)

/// Absolute address
#define GET_ABS(addr) do {		\
	addr.b.l = read(p->pc.w++);	\
	addr.b.h = read(p->pc.w++);	\
} while (0)

/// ABS, X - no correction
#define GET_ABS_X(addr) do {	\
	GET_ABS(addr);				\
	addr.w += p->x;				\
} while (0)

/**
 * ABS, X - cycle correction if boundary exceeds.
 * Add 1 if boundary crossed.
 */
#define GET_ABS_X_C(addr, tmp) do {	\
	GET_ABS(addr);					\
	tmp = addr;						\
	addr.w += p->x;					\
	if (tmp.b.h != addr.b.h)		\
		++ins_cycles;				\
} while (0)

/// ABS, Y - no correction
#define GET_ABS_Y(addr) do {	\
	GET_ABS(addr);				\
	addr.w += p->y;				\
} while (0)

/**
 * ABS, Y - cycle correction if boundary exceeds.
 * Add 1 if boundary crossed.
 */
#define GET_ABS_Y_C(addr, tmp) do {	\
	GET_ABS(addr);					\
	tmp = addr;						\
	addr.w += p->y;					\
	if (tmp.b.h != addr.b.h)		\
		++ins_cycles;				\
} while (0)

/// (ABSOLUTE INDIRECT) - only JMP
#define GET_IND(addr, tmp) do {	\
	GET_ABS(tmp);				\
	addr.b.l = read(tmp.w);		\
	++tmp.b.l;					\
	addr.b.h = read(tmp.w);		\
} while (0)

/// (INDIRECT, X) - only zero-page
/*#define GET_IND_X(addr, tmp) do {	\
//	tmp.w = read(p->pc.w++);		\
//	tmp.b.l += p->x;				\
//	addr.b.l = read(tmp.w++);		\
//	addr.b.h = read(tmp.w);			\
//} while (0)
*/// X6502 compatibility
#define GET_IND_X(addr, tmp) do {	\
	tmp.w = read(p->pc.w++);		\
	tmp.b.l += p->x;				\
	addr.b.l = read(tmp.w);			\
	++tmp.b.l;						\
	addr.b.h = read(tmp.w);			\
} while (0)

/// (INDIRECT), Y - only zero-page
/*#define GET_IND_Y(addr, tmp) do {	\
//	tmp.w = read(p->pc.w++);		\
//	addr.b.l = read(tmp.w++);		\
//	addr.b.h = read(tmp.w);			\
//	addr.w += p->y;					\
*///} while (0)
#define GET_IND_Y(addr, tmp) do {	\
	tmp.w = read(p->pc.w++);		\
	addr.b.l = read(tmp.w);			\
	++tmp.b.l;						\
	addr.b.h = read(tmp.w);			\
	addr.w += p->y;					\
} while (0)

/**
 * (INDIRECT), Y - cycle correction
 * Add 1 if boundary crossed.
 */
/*/#define GET_IND_Y_C(addr, tmp) do {	\
//	tmp.w = read(p->pc.w++);		\
//	addr.b.l = read(tmp.w++);		\
//	addr.b.h = read(tmp.w);			\
//	tmp = addr;						\
//	addr.w += p->y;					\
//	if (tmp.b.h != addr.b.h)		\
//		++ins_cycles;				\
*///} while (0)
#define GET_IND_Y_C(addr, tmp) do {	\
	tmp.w = read(p->pc.w++);		\
	addr.b.l = read(tmp.w);			\
	++tmp.b.l;						\
	addr.b.h = read(tmp.w);			\
	tmp = addr;						\
	addr.w += p->y;					\
	if (tmp.b.h != addr.b.h)		\
		++ins_cycles;				\
} while (0)

/**
 * Relative - cycle corection
 * Add 1 if branching occurres in same page;
 * add 2 if branching occurres in different page.
 */
//#ifdef _WIN32
/*/#define GET_REL(tmp) do {							\
//	tmp = p->pc;									\
//	p->pc.w += (int8_t)(read(p->pc.w++));			\
//	ins_cycles += (p->pc.b.h != tmp.b.h ? 2 : 1);	\
//} while (0)
*///#elif defined __linux__
#define GET_REL(tmp) do {							\
	tmp = p->pc;									\
	p->pc.w += (int8_t)read(p->pc.w) + 1;			\
	ins_cycles += (p->pc.b.h != tmp.b.h ? 2 : 1);	\
} while (0)
//#endif

/// Next instruction, Advances PC by one.
#define NEXT_INST() ++p->pc.w

/// Negative and zero flag
#define FLAG_N_Z(reg) do {							\
	p->p &= ~(N_FLAG | Z_FLAG);						\
	p->p |= (reg ? 0 : Z_FLAG) | (reg & N_FLAG);	\
} while (0)

/**
 * Add with carry utility macro. Use this macro after calculating effective
 * address.
 *
 * Operation : A + M + C -> A, C
 * Flags     : N Z C I D V
 *             / / / - - /
 */
#define ADC(val) do {														\
	result.w = p->a + val + (p->p & C_FLAG);								\
	p->p &= ~(N_FLAG | Z_FLAG | C_FLAG | V_FLAG);							\
	p->p |= (result.b.l & N_FLAG)											\
			| ((~(p->a ^ val) & (p->a ^ result.b.l) & 0x80) ? V_FLAG : 0)	\
			| (result.b.h ? C_FLAG : 0) | (result.b.l ? 0 : Z_FLAG);		\
	p->a = result.b.l;														\
} while (0)

/**
 * And utility macro. Use this macro after calculating effective address.
 *
 * Operation : A /\ M -> A
 * Flags     : N Z C I D V
 *             / / - - - -
 */
#define AND(val) do {	\
	p->a &= val;		\
	FLAG_N_Z(p->a);		\
} while (0)

/**
 * Shift left one bit memory or accumulator.
 *
 * Operation : C <-   D7-D0   <- 0
 * Flags     : N Z C I D V
 *             / / / - - -
 */
#define ASL(val) do {						\
	val.w <<= 1;							\
	p->p &= ~(N_FLAG | Z_FLAG | C_FLAG);	\
	p->p |= (val.b.l & N_FLAG) |			\
			(val.b.l ? 0 : Z_FLAG) |		\
			(val.b.h & C_FLAG);				\
} while (0)

/**
 * Test bits in memory with accumulator.
 *
 * Operation : A /\ M, M7 -> N, M6 -> V
 * Flags     : N Z C I D V
 *             M7/ - - - M6
 */
#define BIT(val) do {						\
	p->p &= ~(N_FLAG | Z_FLAG | V_FLAG);	\
	p->p |= (val & N_FLAG) | (val & 0x40)	\
	| ((p->a & val) ? 0 : Z_FLAG);			\
} while (0)

#define CMP(val) do {										\
	val.w = (uint16_t)(p->a) - val.w;						\
	p->p &= ~(N_FLAG | Z_FLAG | C_FLAG);					\
	p->p |= (val.b.l & N_FLAG) | (val.b.l ? 0 : Z_FLAG)		\
			| (val.b.h ? 0 : C_FLAG);						\
} while (0)

#define CPX(val) do {										\
	val.w = (uint16_t)(p->x) - val.w;						\
	p->p &= ~(N_FLAG | Z_FLAG | C_FLAG);					\
	p->p |= (val.b.l & N_FLAG) | (val.b.l ? 0 : Z_FLAG)		\
			| (val.b.h ? 0 : C_FLAG);						\
} while (0)

#define CPY(val) do {										\
	val.w = (uint16_t)(p->y) - val.w;						\
	p->p &= ~(N_FLAG | Z_FLAG | C_FLAG);					\
	p->p |= (val.b.l & N_FLAG) | (val.b.l ? 0 : Z_FLAG)		\
			| (val.b.h ? 0 : C_FLAG);						\
} while (0)

#define EOR(val) do {	\
	p->a ^= val;		\
	FLAG_N_Z(p->a);		\
} while (0)

#define LSR(val) do {						\
	p->p &= ~(N_FLAG | Z_FLAG | C_FLAG);	\
	p->p |= (val & C_FLAG);					\
	val >>= 1;								\
	p->p |=(val ? 0 : Z_FLAG);				\
} while (0)

#define PUSH(val) do {			\
	write(p->sp | 0x100, val);	\
	--p->sp;					\
} while (0)

#define POP(val) do {			\
	++p->sp;					\
	val = read(p->sp | 0x100);	\
} while (0)

#define ROL(val) do {									\
	val.w <<= 1;										\
	val.b.l |= (p->p & C_FLAG);							\
	p->p &= ~(N_FLAG | Z_FLAG | C_FLAG);				\
	p->p |= (val.b.l & N_FLAG)| (val.b.l ? 0 : Z_FLAG)	\
			| (val.b.h & C_FLAG);						\
} while (0)

#define ROR(val) do {										\
	val.b.h = (p->p & C_FLAG);								\
	p->p &= ~(N_FLAG | Z_FLAG | C_FLAG);					\
	p->p |= (val.b.l & C_FLAG);								\
	val.w >>= 1;											\
	p->p |= (val.b.l & N_FLAG) | (val.b.l ? 0 : Z_FLAG);	\
} while (0)

#define SBC(val) do {														\
	result.w = p->a - val - (~p->p & C_FLAG);								\
	p->p &= ~(N_FLAG | V_FLAG | Z_FLAG | C_FLAG);							\
	p->p |= (result.b.l & N_FLAG)											\
			| (((p->a ^ result.b.l) & (p->a ^ val) & 0x80) ? V_FLAG : 0)	\
			| (result.b.h ? 0 : C_FLAG) | (result.b.l ? 0 : Z_FLAG);		\
	p->a = result.b.l;														\
} while (0)

uint8_t read(uint16_t addr)
{
	// TODO: add more checkings like I/O registers and memory mappers.
	if (addr < 0x2000)		// Internal RAM
		return RAM[addr & 0x07FF];
	else if (addr < 0x8000)
		return read_io(addr);
	else
		return ROM[addr - 0x8000];		// External program ROM or Mapper
}

void write(uint16_t addr, uint8_t value)
{
	if (addr < (uint16_t)0x2000)		// 3 Mirrors of $0000-$0800
		RAM[addr & 0x07FF] = value;
	else if (addr < (uint16_t)0x8000)
		write_io(addr, value);
	else if (!mapper_write(addr, value))
		log_error("Unkown mapper write value $%02X to address $%04X", value, addr);
}

void reset(pP6502 p)
{
	log_info("Reset processor");
	memset(p, 0, sizeof(P6502));	// Clear all fields.
	//p->p = FLAG_1 | I_FLAG;			// Bit 5 is always 1, and interrupt mask is set
	p->p = FLAG_1 | Z_FLAG;
	p->sp = 0xFF;					// Stack pointer
	p->pc.b.l = read(0xFFFC);		// Reset vector low byte
	p->pc.b.h = read(0xFFFD);		// Reset vector high byte
}

void power_on(pP6502 p)
{
	log_info("Power on processor");

	reset(p);
	memset(RAM, 0xFF, sizeof(RAM));
}

/// TODO: Add accurate ticks of PPU and pAPU to instaructions.
void fetch(pP6502 p)
{
	uint8_t src;				// Source data temporary.
	TwoByteReg result, addr;	// Temporary result of operations and effective address.
	uint8_t instruction;		// Fetch opcode and set PC to next instruction automatically.
	int ins_cycles = 0;			// Last instruction cycles.

	// Check interrupts.
	if (p->interrupts != INTR_NONE)
	{
		if (p->interrupts & INTR_NMI)		// NMI
		{
			ins_cycles = 7;
			PUSH(p->pc.b.h);			// Save return address.
			PUSH(p->pc.b.l);
			PUSH(p->p);					// Don't change P flag in NMI
			p->pc.b.l = read(0xFFFA);	// Load NMI vector.
			p->pc.b.h = read(0xFFFB);
			p->interrupts = INTR_NONE;	// Clear NMI it is edge sensitive.
		}
		else if ((p->interrupts & INTR_IRQ) && (~p->p & I_FLAG))		// IRQ
		{
			ins_cycles = 7;
			PUSH(p->pc.b.h);			// Save return address.
			PUSH(p->pc.b.l);
			p->p &= ~B_FLAG;			// Clear B (break) flag.
			p->p |= I_FLAG;				// Disable interrupts.
			PUSH(p->p);
			p->pc.b.l = read(0xFFFE);	// Load IRQ vector.
			p->pc.b.h = read(0xFFFF);
										// Don't clear IRQ it is level sensitive. Some
										// mappers have ACK signal to disable IRQ. IRQ
										// flag must be disabled in those places.
		}
		else if (p->interrupts & INTR_RESET)
		{
			reset(p);
			p->interrupts = INTR_NONE;	// Clear reset signal.
		}
	}

	instruction = read(p->pc.w++);
	ins_cycles += cycles[instruction];

	// Execute opcode.
	switch (instruction)
	{
		case 0x69:										 // ADC #Oper - Immadiate
			GET_IMMEDIATE(result); src = result.b.l; ADC(src); break;
		case 0x65:										// ADC Oper - Zero-page
			GET_ZERO_PAGE(addr); src = read(addr.w); ADC(src); break;
		case 0x75:										// ADC Oper, X - Zero-page, X
			GET_ZERO_PAGE_X(addr); src = read(addr.w); ADC(src); break;
		case 0x6D:										// ADC absolute
			GET_ABS(addr); src = read(addr.w); ADC(src); break;
		case 0x7D:										// ADC abs, X
			GET_ABS_X_C(addr, result); src = read(addr.w); ADC(src); break;
		case 0x79:										// ADC abs, Y
			GET_ABS_Y_C(addr, result); src = read(addr.w); ADC(src); break;
		case 0x61:										// ADC (indirect, X)
			GET_IND_X(addr, result); src = read(addr.w); ADC(src); break;
		case 0x71:										// ADC (indirect), Y
			GET_IND_Y_C(addr, result); src = read(addr.w); ADC(src); break;

		case 0x29: GET_IMMEDIATE(result);				// AND #Opr - immadiate
			AND(result.b.l); break;
		case 0x25: GET_ZERO_PAGE(addr);					// AND Opr - zero page
			src = read(addr.w); AND(src); break;
		case 0x35: GET_ZERO_PAGE_X(addr);				// AND Opr, X - zero page, x
			src = read(addr.w); AND(src); break;
		case 0x2D: GET_ABS(addr);						// AND Opr - Absolute
			src = read(addr.w); AND(src); break;
		case 0x3D: GET_ABS_X_C(addr, result);			// AND Opr, X - Absolute, X
			src = read(addr.w); AND(src); break;
		case 0x39: GET_ABS_Y_C(addr, result);			// AND Opr, Y - Absolute, Y
			src = read(addr.w); AND(src); break;
		case 0x21: GET_IND_X(addr, result);				// AND (Opr, X) - (indirect, X)
			src = read(addr.w); AND(src); break;
		case 0x31: GET_IND_Y_C(addr, result);			// AND (Opr), Y - (indirect), Y
			src = read(addr.w); AND(src); break;

		case 0x0A:	// ASL A - Accumulator
			result.w = p->a; ASL(result); p->a = result.b.l; break;
		case 0x06: GET_ZERO_PAGE(addr);					// ASL Opr - Zero page
			result.w = read(addr.w); ASL(result); write(addr.w, result.b.l); break;
		case 0x16: GET_ZERO_PAGE_X(addr);				// ASL Opr, X - Zero page, X
			result.w = read(addr.w); ASL(result); write(addr.w, result.b.l); break;
		case 0x0E: GET_ABS(addr);						// ASL Opr - Absolute
			result.w = read(addr.w); ASL(result); write(addr.w, result.b.l); break;
		case 0x1E: GET_ABS_X(addr);						// ASL Opr, X - Absolute, X
			result.w = read(addr.w); ASL(result); write(addr.w, result.b.l); break;

		case 0x90: if (p->p & C_FLAG) NEXT_INST();				// BCC Opr - Relative
			else GET_REL(addr); break;
		case 0xB0: if (p->p & C_FLAG) GET_REL(addr);			// BCS Opr - Relative
			else NEXT_INST(); break;
		case 0xF0: if (p->p & Z_FLAG) GET_REL(addr);			// BEQ Opr - Relative
			else NEXT_INST(); break;

		case 0x24:												// BIT Opr - Zero page
			GET_ZERO_PAGE(addr); src = read(addr.w); BIT(src); break;
		case 0x2C:												// BIT Opr - Absolute
			 GET_ABS(addr); src = read(addr.w); BIT(src); break;

		case 0x30: if (p->p & N_FLAG) GET_REL(addr);			// BMI Opr - Relative
			else NEXT_INST(); break;
		case 0xD0: if (p->p & Z_FLAG) NEXT_INST();				// BNE Opr - Relative
			else GET_REL(addr); break;
		case 0x10: if (p->p & N_FLAG) NEXT_INST();				// BPL Opr - Relative
			else GET_REL(addr); break;

		case 0x00:												// BRK
			++p->pc.w;
			PUSH(p->pc.b.h);
			PUSH(p->pc.b.l);
			PUSH(p->p | B_FLAG | FLAG_1);			// Set B (break) flag.
			p->p |= I_FLAG;							// Disable interrupt.
			p->pc.b.l = read(0xFFFE);
			p->pc.b.h = read(0xFFFF);
			break;

		case 0x50: if (!(p->p & V_FLAG)) GET_REL(addr);			// BVC Opr - Relative
			else NEXT_INST(); break;
		case 0x70: if (p->p & V_FLAG) GET_REL(addr);			// BVS Opr -Relative
			else NEXT_INST(); break;

		case 0x18: p->p &= ~C_FLAG; break;						// CLC - Implied
		case 0xD8: p->p &= ~D_FLAG; break;						// CLD - Implied
		case 0x58: p->p &= ~I_FLAG; break;						// CLI - Implied
		case 0xB8: p->p &= ~V_FLAG; break;						// CLV - Implied

		case 0xC9: GET_IMMEDIATE(result); CMP(result); break;	// CMP #Opr - Immediate
		case 0xC5: GET_ZERO_PAGE(addr);							// CMP Oper - Zero page
			result.w = read(addr.w); CMP(result); break;
		case 0xD5: GET_ZERO_PAGE_X(addr);						// CMP Oper, X - Zero page, X
			result.w = read(addr.w); CMP(result); break;
		case 0xCD: GET_ABS(addr);								// CMP Oper - Absolute
			result.w = read(addr.w); CMP(result); break;
		case 0xDD: GET_ABS_X_C(addr, result);					// CMP Oper, X - Absolute, X
			result.w = read(addr.w); CMP(result); break;
		case 0xD9: GET_ABS_Y_C(addr, result);					// CMP Oper, Y - Absolute, Y
			result.w = read(addr.w); CMP(result); break;
		case 0xC1: GET_IND_X(addr, result);						// CMP (Oper, X)
			result.w = read(addr.w); CMP(result); break;
		case 0xD1: GET_IND_Y_C(addr, result);					// CMP (Oper), Y
			result.w = read(addr.w); CMP(result); break;

		case 0xE0: GET_IMMEDIATE(result); CPX(result); break;	// CPX *Oper - Immadiate
		case 0xE4: GET_ZERO_PAGE(addr);							// CPX Oper - Zero page
			result.w = read(addr.w); CPX(result); break;
		case 0xEC: GET_ABS(addr);								// CPX Oper - Absolute
			result.w = read(addr.w); CPX(result); break;

		case 0xC0: GET_IMMEDIATE(result); CPY(result); break;	// CPY *Oper - Immediate
		case 0xC4: GET_ZERO_PAGE(addr);							// CPY Oper - Zero page
			result.w = read(addr.w); CPY(result); break;
		case 0xCC: GET_ABS(addr);								// CPY Oper - Absolute
			result.w = read(addr.w); CPY(result); break;

		case 0xC6: GET_ZERO_PAGE(addr);							// DEC Oper - Zero page
			src = read(addr.w); --src; FLAG_N_Z(src); write(addr.w, src); break;
		case 0xD6: GET_ZERO_PAGE_X(addr);						// DEC Oper, X - Zero page, X
			src = read(addr.w); --src; FLAG_N_Z(src); write(addr.w, src); break;
		case 0xCE: GET_ABS(addr);								// DEC Oper - Absolute
			src = read(addr.w); --src; FLAG_N_Z(src); write(addr.w, src); break;
		case 0xDE: GET_ABS_X(addr);								// DEC Oper, X - Absolute, X
			src = read(addr.w); --src; FLAG_N_Z(src); write(addr.w, src); break;

		case 0xCA: --p->x; FLAG_N_Z(p->x); break;	// DEX - Implied
		case 0x88: --p->y; FLAG_N_Z(p->y); break;	// DEY - Implied

		case 0x49:GET_IMMEDIATE(result); EOR(result.b.l); break;// EOR #Oper - Immediate
		case 0x45: GET_ZERO_PAGE(addr);							// EOR Oper - Zero page
			src = read(addr.w); EOR(src); break;
		case 0x55: GET_ZERO_PAGE_X(addr);						// EOR Oper, X - Zero page
			src = read(addr.w); EOR(src); break;
		case 0x4D: GET_ABS(addr);								// EOR Oper - Absolute
			src = read(addr.w); EOR(src); break;
		case 0x5D: GET_ABS_X_C(addr, result);					// EOR Oper, X - Absolute, X
			src = read(addr.w); EOR(src); break;
		case 0x59: GET_ABS_Y_C(addr, result);					// EOR Oper, Y - Absolute, Y
			src = read(addr.w); EOR(src); break;
		case 0x41: GET_IND_X(addr, result);					// EOR (Oper, X) - (Indirect, X)
			src = read(addr.w); EOR(src); break;
		case 0x51: GET_IND_Y_C(addr, result);				// EOR (Oper), Y - (Indirect), Y
			src = read(addr.w); EOR(src); break;

		case 0xE6: GET_ZERO_PAGE(addr);							// INC Oper - Zero page
			src = read(addr.w); ++src; FLAG_N_Z(src); write(addr.w, src); break;
		case 0xF6: GET_ZERO_PAGE_X(addr);						// INC Oper, X - Zero page
			src = read(addr.w); ++src; FLAG_N_Z(src); write(addr.w, src); break;
		case 0xEE: GET_ABS(addr);								// INC Oper - Absolute
			src = read(addr.w); ++src; FLAG_N_Z(src); write(addr.w, src); break;
		case 0xFE: GET_ABS_X(addr);								// INC Oper, X - Absolute, X
			src = read(addr.w); ++src; FLAG_N_Z(src); write(addr.w, src); break;

		case 0xE8: ++p->x; FLAG_N_Z(p->x); break;			// INX - Implied
		case 0xC8: ++p->y; FLAG_N_Z(p->y); break;			// INY - Implied
		case 0x4C: GET_ABS(addr); p->pc = addr; break;		// JMP Oper - Absolute
		case 0x6C: GET_IND(addr, result); p->pc = addr;break;// JMP (Oper) - Absolute Indirect

		case 0x20:											// JSR Oper - Absolute
			addr.b.l = read(p->pc.w++);
			addr.b.h = read(p->pc.w);
			PUSH(p->pc.b.h);
			PUSH(p->pc.b.l);
			p->pc = addr;
			break;

		case 0xA9: GET_IMMEDIATE(result);					// LDA #Oper - Immediate
			p->a = result.b.l; FLAG_N_Z(p->a); break;
		case 0xA5: GET_ZERO_PAGE(addr);						// LDA Oper - Zero page
			p->a = read(addr.w); FLAG_N_Z(p->a); break;
		case 0xB5: GET_ZERO_PAGE_X(addr);					// LDA Oper, X - Zero page, X
			p->a = read(addr.w); FLAG_N_Z(p->a); break;
		case 0xAD: GET_ABS(addr);							// LDA Oper - Absolute
			p->a = read(addr.w); FLAG_N_Z(p->a); break;
		case 0xBD: GET_ABS_X_C(addr, result);				// LDA Oper, X - Absolute, X
			p->a = read(addr.w); FLAG_N_Z(p->a); break;
		case 0xB9: GET_ABS_Y_C(addr, result);				// LDA Oper, Y - Absolute, Y
			p->a = read(addr.w); FLAG_N_Z(p->a); break;
		case 0xA1: GET_IND_X(addr, result);					// LDA (Oper, X) - (Indirect, X)
			p->a = read(addr.w); FLAG_N_Z(p->a); break;
		case 0xB1: GET_IND_Y_C(addr, result);				// LDA (Oper), Y - (Indirect), Y
			p->a = read(addr.w); FLAG_N_Z(p->a); break;

		case 0xA2: GET_IMMEDIATE(result);					// LDX #Oper - Immediate
			p->x = result.b.l; FLAG_N_Z(p->x); break;
		case 0xA6: GET_ZERO_PAGE(addr);						// LDX Oper - Zero page
			p->x = read(addr.w); FLAG_N_Z(p->x); break;
		case 0xB6: GET_ZERO_PAGE_Y(addr);					// LDX Oper, Y - Zero page, Y
			p->x = read(addr.w); FLAG_N_Z(p->x); break;
		case 0xAE: GET_ABS(addr);							// LDX Oper - Absolute
			p->x = read(addr.w); FLAG_N_Z(p->x); break;
		case 0xBE: GET_ABS_Y_C(addr, result);				// LDX Oper, Y - Absolute, Y
			p->x = read(addr.w); FLAG_N_Z(p->x); break;

		case 0xA0: GET_IMMEDIATE(result);					// LDY #Oper - Immediate
			p->y = result.b.l; FLAG_N_Z(p->y); break;
		case 0xA4: GET_ZERO_PAGE(addr);						// LDY Oper - Zero page
			p->y = read(addr.w); FLAG_N_Z(p->y); break;
		case 0xB4: GET_ZERO_PAGE_X(addr);					// LDY Oper, X - Zero page, X
			p->y = read(addr.w); FLAG_N_Z(p->y); break;
		case 0xAC: GET_ABS(addr);							// LDY Oper - Absolute
			p->y = read(addr.w); FLAG_N_Z(p->y); break;
		case 0xBC: GET_ABS_X_C(addr, result);				// LDY Oper, X - Absolute, X
			p->y = read(addr.w); FLAG_N_Z(p->y); break;

		case 0x4A: LSR(p->a); break;						// LSR A - Accumulator
		case 0x46: GET_ZERO_PAGE(addr);						// LSR Oper - Zero page
			src = read(addr.w); LSR(src); write(addr.w, src); break;
		case 0x56: GET_ZERO_PAGE_X(addr);					// LSR Oper, X - Zero page
			src = read(addr.w); LSR(src); write(addr.w, src); break;
		case 0x4E: GET_ABS(addr);							// LSR Oper - Absolute
			src = read(addr.w); LSR(src); write(addr.w, src); break;
		case 0x5E: GET_ABS_X(addr);							// LSR Oper, X - Absolute, X
			src = read(addr.w); LSR(src); write(addr.w, src); break;

		case 0xEA: break;									// NOP - Implied

		case 0x09: GET_IMMEDIATE(result);					// ORA #Oper - Immediate
			p->a |= result.b.l; FLAG_N_Z(p->a); break;
		case 0x05: GET_ZERO_PAGE(addr);						// ORA Oper - Zero page
			p->a |= read(addr.w); FLAG_N_Z(p->a); break;
		case 0x15: GET_ZERO_PAGE_X(addr);					// ORA Oper, X - Zero page, X
			p->a |= read(addr.w); FLAG_N_Z(p->a); break;
		case 0x0D: GET_ABS(addr);							// ORA Oper - Absolute
			p->a |= read(addr.w); FLAG_N_Z(p->a); break;
		case 0x1D: GET_ABS_X_C(addr, result);				// ORA Oper, X - Absolute, X
			p->a |= read(addr.w); FLAG_N_Z(p->a); break;
		case 0x19: GET_ABS_Y_C(addr, result);				// ORA Oper, Y - Absolute, Y
			p->a |= read(addr.w); FLAG_N_Z(p->a); break;
		case 0x01: GET_IND_X(addr, result);					// ORA (Oper, X) - (Indirect, X)
			p->a |= read(addr.w); FLAG_N_Z(p->a); break;
		case 0x11: GET_IND_Y_C(addr, result);				// ORA (Oper), Y - (Indirect), Y
			p->a |= read(addr.w); FLAG_N_Z(p->a); break;

		case 0x48: PUSH(p->a);break;							// PHA - Implied
		case 0x08: PUSH(p->p | B_FLAG | FLAG_1);break;			// PHP - Implied
		case 0x68: POP(p->a); FLAG_N_Z(p->a);break;				// PLA - Implied
		case 0x28: POP(p->p);break;								// PLP - Implied

		case 0x2A: result.w = p->a;								// ROL A - Accumulator
			ROL(result); p->a = result.b.l;	break;
		case 0x26: GET_ZERO_PAGE(addr);							// ROL Oper - Zero page
			result.w = read(addr.w); ROL(result); write(addr.w, result.b.l); break;
		case 0x36: GET_ZERO_PAGE_X(addr);						// ROL Oper, X - Zero page, X
			result.w = read(addr.w); ROL(result); write(addr.w, result.b.l); break;
		case 0x2E: GET_ABS(addr);								// ROL Oper - Absolute
			result.w = read(addr.w); ROL(result); write(addr.w, result.b.l); break;
		case 0x3E: GET_ABS_X(addr);								// ROL Oper, X - Absolute, X
			result.w = read(addr.w); ROL(result); write(addr.w, result.b.l); break;

		case 0x6A: result.w = p->a;								// ROR A - Accumulator
			ROR(result); p->a = result.b.l; break;
		case 0x66: GET_ZERO_PAGE(addr);					// ROR Oper - Zero page
			result.w = read(addr.w); ROR(result); write(addr.w, result.b.l); break;
		case 0x76: GET_ZERO_PAGE_X(addr);				// ROR Oper, X - Zero page, X
			result.w = read(addr.w); ROR(result); write(addr.w, result.b.l); break;
		case 0x6E: GET_ABS(addr);						// ROR Oper - Absolute
			result.w = read(addr.w); ROR(result); write(addr.w, result.b.l); break;
		case 0x7E: GET_ABS_X(addr);						// ROR Oper, X - Absolute, X
			result.w = read(addr.w); ROR(result); write(addr.w, result.b.l); break;

		case 0x40:												// RTI - Implied
			POP(p->p);
			POP(p->pc.b.l);
			POP(p->pc.b.h);
			break;

		case 0x60:												// RTS - Implied
			POP(p->pc.b.l);
			POP(p->pc.b.h);
			NEXT_INST();
			break;

		case 0xE9: GET_IMMEDIATE(result);					// SBC #Oper - Immediate
			src = result.b.l; SBC(src); break;
		case 0xE5: GET_ZERO_PAGE(addr);						// SBC Oper - Zero page
			src = read(addr.w); SBC(src); break;
		case 0xF5: GET_ZERO_PAGE_X(addr);					// SBC Oper, X - Zero page, X
			src = read(addr.w); SBC(src); break;
		case 0xED: GET_ABS(addr);							// SBC Oper - Absolute
			src = read(addr.w); SBC(src); break;
		case 0xFD: GET_ABS_X_C(addr, result);				// SBC Oper, X - Absolute, X
			src = read(addr.w); SBC(src); break;
		case 0xF9: GET_ABS_Y_C(addr, result);				// SBC Oper, Y - Absolute, Y
			src = read(addr.w); SBC(src); break;
		case 0xE1: GET_IND_X(addr, result);					// SBC (Oper, X) - (Indirect, X)
			src = read(addr.w); SBC(src); break;
		case 0xF1: GET_IND_Y_C(addr, result);				// SBC (Oper), Y - (Indirect), Y
			src = read(addr.w); SBC(src); break;

		case 0x38: p->p |= C_FLAG;								break; // SEC - Implied
		case 0xF8: p->p |= D_FLAG;								break; // SED - Implied
		case 0x78: p->p |= I_FLAG;								break; // SEI - Implied

		case 0x85: GET_ZERO_PAGE(addr);			// STA Oper - Zero page
			write(addr.w, p->a); break;
		case 0x95: GET_ZERO_PAGE_X(addr);		// STA Oper, X - Zero page, X
			write(addr.w, p->a); break;
		case 0x8D: GET_ABS(addr);				// STA Oper - Absolute
			write(addr.w, p->a); break;
		case 0x9D: GET_ABS_X(addr);				// STA Oper, X - Absolute, X
			write(addr.w, p->a); break;
		case 0x99: GET_ABS_Y(addr);				// STA Oper, Y - Absolute, Y
			write(addr.w, p->a); break;
		case 0x81: GET_IND_X(addr, result);		// STA (Oper, X) - (Indirect, X)
			write(addr.w, p->a); break;
		case 0x91: GET_IND_Y(addr, result);		// STA (Oper), Y - (Indirect), Y
			write(addr.w, p->a); break;

		case 0x86: GET_ZERO_PAGE(addr);			// STX Oper - Zero page
			write(addr.w, p->x); break;
		case 0x96: GET_ZERO_PAGE_Y(addr);		// STX Oper, Y - Zero page, Y
			write(addr.w, p->x); break;
		case 0x8E: GET_ABS(addr);				// STX Oper - Absolute
			write(addr.w, p->x); break;

		case 0x84: GET_ZERO_PAGE(addr);			// STY Oper - Zero page
			write(addr.w, p->y); break;
		case 0x94: GET_ZERO_PAGE_X(addr);		// STY Oper, X - Zero page, X
			write(addr.w, p->y); break;
		case 0x8C: GET_ABS(addr);				// STY Oper - Absolute
			write(addr.w, p->y); break;

		case 0xAA: p->x = p->a; FLAG_N_Z(p->x);					break; // TAX - Implied
		case 0xA8: p->y = p->a; FLAG_N_Z(p->y);					break; // TAY - Implied
		case 0xBA: p->x = p->sp; FLAG_N_Z(p->x);				break; // TSX - Implied
		case 0x8A: p->a = p->x; FLAG_N_Z(p->a);					break; // TXA - Implied
		case 0x9A: p->sp = p->x;								break; // TXS - Implied
		case 0x98: p->a = p->y; FLAG_N_Z(p->a);					break; // TYA - Implied

		/// TODO: Implement un-official opcodes.
#if defined UN_OFFICIALS
		// One byte opcode lenght
		case 0x1A:		case 0x3A:		case 0x5A:		case 0x7A:		case 0xDA:
		case 0xFA:
			break;

		// Two byte opcode lenght
		case 0x80:
		
		case 0x12:		case 0x32:		case 0x52:		case 0x72:		case 0x92:
		case 0xB2:		case 0xD2:		case 0xF2:

		case 0x07:		case 0x17:		case 0x27:		case 0x37:		case 0x47:
		case 0x57:		case 0x67:		case 0x77:		case 0x87:		case 0x97:
		case 0xA7:		case 0xB7:		case 0xC7:		case 0xD7:		case 0xE7:
		case 0xF7:

		case 0x04:		case 0x14:		case 0x34:		case 0x64:		case 0x74:

		case 0x89:
			p->pc.w += 1; break;

		// Three byte opcode legnth.
		case 0x0C:		case 0x1C:		case 0x3C:		case 0x7C:		case 0x9C:

		case 0x5C:		case 0xDC:		case 0xFC:

		case 0x9E:

		case 0x93:

		case 0x1B:		case 0x3B:		case 0x5B:		case 0x7B:		case 0xDB:
		case 0xFB:

		case 0xEB:		case 0xE3:

		case 0x0F:		case 0x1F:		case 0x2F:		case 0x3F:		case 0x4F:
		case 0x5F:		case 0x6F:		case 0x7F:		case 0x8F:		case 0x9F:
		case 0xAF:		case 0xBF:		case 0xCF:		case 0xDF:		case 0xEF:
		case 0xFF:
			p->pc.w += 2; break;
#endif

		default:
			log_error("Undefined opcode at 0x%04X : 0x%02X at cc : %d", p->pc.w, instruction, p->cycle_counter);
			// Log undefined opcode.
			debug_message("\nUndefined opcode at 0x%04X : 0x%02X at cc : %d", p->pc.w - 1, instruction, p->cycle_counter);
			break;
	}

	p->cycle_counter += ins_cycles;
	p->ins_cycles = ins_cycles;
}

