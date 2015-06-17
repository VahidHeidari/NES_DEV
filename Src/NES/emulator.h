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

#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>
#include <stdio.h>

#include "config.h"

#include "p6502.h"
#include "joypad.h"
#include "ppu.h"
#include "apu.h"
#include "nes-hdr.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NES_NTSC_CLK_FREQ_F			21.47727
#define NES_NTSC_CLK_FREQ			21477270
#define NES_NTSC_CLK_12TH_FREQ_F	1.7897725
#define NES_NTSC_CLK_12TH_FREQ		1789773

#define ROM_SIZE				0x8000		/// 32KB
#define RAM_SIZE				0x2000		/// 8KB
#define NOT_MIRRORED_RAM_SIZE	0x0800		/// 2KB

extern uint8_t ROM[ROM_SIZE];
extern uint8_t RAM[NOT_MIRRORED_RAM_SIZE];

extern P6502 p;
extern PpuStatus ppu;
extern Apu apu;
extern NesHeader hdr;
extern Joypad joypad;
extern int finished_emulation;
extern char* image_path;
extern FILE* image;
extern Mirroring mirroring;

int emulator_init(void);
void emulator_close(void);
void emulator_save_state(const char* path);
int emulator_load_state(const char* path);
int read_rom_image(char* path);
void step(void);
void run(void);
void run_cycles(int cycles);

#ifdef __cplusplus
}
#endif

#endif

