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

#include "ppu-memory.h"

#define INTRODUCTION_DELAY	60
#define FAID_DELAY			5

/// Global variables
int i;
char* str;

void print_string()
{
	while (*str) {
		PPU_DATA = *str;
		++str;
	}
}

void set_palette()
{
	// Black background
	PPU_SET_ADDR(PPU_PALETTE_START);
	for (i = 0; i < PPU_PALETTE_SIZE; ++i)
		PPU_DATA = PPU_COLOR_BLACK;

	// White forground text
	PPU_SET_ADDR(PPU_PALETTE_START + 1);
	PPU_DATA = PPU_COLOR_WHITE;
	PPU_DATA = PPU_COLOR_WHITE;
	PPU_DATA = PPU_COLOR_WHITE;
}

void print_introduction()
{
	str = "Copyright\x27""2015,Vahid Heidiari";
	PPU_SET_ADDR(0x2042);
	print_string();

	str = "(DeltaCode)";
	PPU_SET_ADDR(0x206A);
	print_string();

	str = "Powered by CC65 compiler";
	PPU_SET_ADDR(0x20A5);
	print_string();

	str = "You can download sources at:";
	PPU_SET_ADDR(0x2202);
	print_string();

	str = "https://github.com/VahidHeidari/";
	PPU_SET_ADDR(0x2220);
	print_string();

	str = "NES_DEV";
	PPU_SET_ADDR(0x2246);
	print_string();
}

void main()
{
	wait_vblank();

	// Disable rendering.
	PPU_CTRL = 0;
	PPU_MASK = 0;

	set_palette();
	print_introduction();

	wait_vblank();
	PPU_CTRL = 0x80;		// Enable NMI.
	PPU_MASK = 0x08;		// Enable background rendering.
	PPU_SET_ADDR(0);		// Set rendering address.


	while (1) {
		// Faid out.
		wait_frames(INTRODUCTION_DELAY);
		for (i = 0; i < 3; ++i) {
			PPU_SET_ADDR(PPU_PALETTE_START + 1);
			PPU_DATA = 0x0D | ((3 - i) << 4);
			PPU_SET_ADDR(0);
			wait_frames(FAID_DELAY);
		}

		// Faid in.
		wait_frames(INTRODUCTION_DELAY);
		for (i = 0; i < 3; ++i) {
			PPU_SET_ADDR(PPU_PALETTE_START + 1);
			PPU_DATA = 0x0D | (i << 4);
			PPU_SET_ADDR(0);
			wait_frames(FAID_DELAY);
		}

		set_palette();
		PPU_SET_ADDR(0);
	}

	while (1)		// Loop for ever.
		continue;
}
