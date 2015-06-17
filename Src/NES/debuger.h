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

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#include "config.h"
#include "p6502.h"
#include "emulator.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char asm_m[256];		// Assembley menomonic.
extern int show_sp;
extern int show_bg;

/// RAM
uint8_t read_ram(uint16_t addr);
void write_ram(uint16_t addr, uint8_t value);
void fill(uint16_t addr, size_t size, uint8_t value);

/// ROM
uint8_t read_rom(uint16_t addr);
void write_rom(uint16_t addr, uint8_t value);

/// TEST ROMs
void init_test_rom(NesHeader* hdr, uint8_t* rom, uint8_t* chr_rom);

/// Processor
void print_regs(pP6502 p);
void write_p(pP6502 p, uint8_t value);
void write_a(pP6502 p, uint8_t value);
void write_x(pP6502 p, uint8_t value);
void write_y(pP6502 p, uint8_t value);
void write_sp(pP6502 p, uint8_t value);
void write_pc(pP6502 p, uint16_t value);
void write_cycle_counter(pP6502 p, int value);
void irq_signal(pP6502 p);
void reset_signal(pP6502 p);
void nmi_signal(pP6502 p);

void log_info(const char* str, ...);
void log_warning(const char* str, ...);
void log_error(const char* str, ...);

void debug_message(const char* str, ...);

/**
 * Disassembler
 * Disassembles current address which program counter pointing to.
 *
 * @param p Processor which want to dissassemble next executing instaruction.
 * @return Returns instruction length in byte.
 */
int dissassemble(pP6502 p);

#ifdef __cplusplus
}
#endif

#endif

