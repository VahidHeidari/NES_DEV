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

#ifndef P_6502_H
#define P_6502_H		/// Processor 6502

#include <stdint.h>

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/// P register masks.
#define C_FLAG		0x01
#define Z_FLAG		0x02
#define I_FLAG		0x04
#define D_FLAG		0x08
#define B_FLAG		0x10
/// Bit 5 not used
#define FLAG_1		0x20
#define V_FLAG		0x40
#define N_FLAG		0x80

/// Interrupt status
#define INTR_NONE		0x00		/// Interrupt none
#define INTR_IRQ		0x01		/// IRQ interrupt
#define INTR_RESET		0x02		/// Reset interrupt
#define INTR_NMI		0x04		/// None maskable interrupt

/**
 * Two byte registers which some times must access nibles separtelly.
 * Word data stored in 'w' and each nibels stored in 'b'.
 */
typedef union TwoByteReg
{
	struct byte_nibles
	{
		uint8_t l;		/// Low byte
		uint8_t h;		/// High byte
	} b;		/// Byte access to this register.

	uint16_t w;		/// Word access to this register.
} TwoByteReg, *pTwoByteReg;

/**
 * 6502 processor registers. This struct holds execution status of CPU.
 * This struct stores execution context of CPU.
 */
typedef struct P6502
{
	uint8_t p;						/// Status register
	uint8_t y;						/// Y Index register
	uint8_t x;						/// X index register
	uint8_t a;						/// Accumulator
	uint8_t sp;						/// Stack poiter
	TwoByteReg pc;					/// Program counter

	int cycle_counter;				/// CPU cycles
	int ins_cycles;					/// Last instruction cycles
	int interrupts;					/// State of Interrupts.
} P6502, *pP6502;

/**
 * Reads a byte from memory. This function called when CPU access RAM.
 *
 * @param addr Address of byte.
 * @return Returns byte.
 */
uint8_t read(uint16_t addr);

/**
 * Writes a byte to memory. This function called when CPU access RAM, I/O registers
 * or mapper registers access.
 *
 * @param addr Address to wirte.
 * @param value Value to wirte at address.
 */
void write(uint16_t addr, uint8_t value);
 
/**
 * Resets processor. This function called when RESET interrupt occurred.
 *
 * @param p Pointer to processor.
 */
void reset(pP6502 p);
 
/**
 * Power on processor at startup.
 *
 * @param p Pointer to processor.
 */
void power_on(pP6502 p);

/**
 * Fetchs and executes opcode, and automatically advances PC to next
 * instruction.  It  stores  last  instruction cycles in processor's
 * ins_cycles, and adds it to cycle_counter.
 *
 * @param p Pointer to processor.
 */
void fetch(pP6502 p);

/**
 * Clock cycle of CPU. Per each clock cycle of CPU, PPU clocks 3 times,
 * and APU clocks 1 times.
 */
void cpu_clock_cycle();

#ifdef __cplusplus
}
#endif

#endif

