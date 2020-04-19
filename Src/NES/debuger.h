/**
 * NES_DEV is a cross-platform, portable, and hand-held NES emulator.
 *
 * Copyright (C) 2015-2020 Vahid Heidari (DeltaCode)
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
uint8_t ReadRAM(uint16_t addr);
void WriteRAM(uint16_t addr, uint8_t value);
void Fill(uint16_t addr, size_t size, uint8_t value);

/// ROM
uint8_t ReadROM(uint16_t addr);
void WriteROM(uint16_t addr, uint8_t value);

/// TEST ROMs
void InitTestROM(NesHeader* hdr, uint8_t* rom, uint8_t* chr_rom);
int DumpTestROM(const char* path);

/// Processor
void PrintRegs(pP6502 p);
void WriteP(pP6502 p, uint8_t value);
void WriteA(pP6502 p, uint8_t value);
void WriteX(pP6502 p, uint8_t value);
void WriteY(pP6502 p, uint8_t value);
void WriteSP(pP6502 p, uint8_t value);
void WritePC(pP6502 p, uint16_t value);
void WriteCycleCounter(pP6502 p, int value);
void IRQSignal(pP6502 p);
void ResetSignal(pP6502 p);
void NMISignal(pP6502 p);

void LogInfo(const char* str, ...);
void LogWarning(const char* str, ...);
void LogError(const char* str, ...);

void DebugMessage(const char* str, ...);

/**
 * Disassembler
 * Disassembles current address which program counter pointing to.
 *
 * @param p Processor which want to dissassemble next executing instaruction.
 * @return Returns instruction length in byte.
 */
int Dissassemble(pP6502 p);

#ifdef __cplusplus
}
#endif

#endif

