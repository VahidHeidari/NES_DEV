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

//#include <nes.h>

#include "ppu-memory.h"

#define INTRODUCTION_DELAY	60
#define FAID_DELAY			2

/// Global variables
int i;

extern void waitvblank(void);

void print_string(const char* str)
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
	PPU_SET_ADDR(PPU_PALETTE_START + 3);
	PPU_DATA = PPU_COLOR_WHITE;
}

void print_introduction()
{
	PPU_SET_ADDR(0x2041);
	print_string(" Copyright`2015, Vahid Heidari");
	PPU_SET_ADDR(0x2061);
	print_string("         (DeltaCode)");
	PPU_SET_ADDR(0x20A1);
	print_string("  Powered by CC65 compiler");
}

void wait_frames(int frames)
{
	while (frames--)
		waitvblank();
}

void main()
{
	waitvblank();
	
	// Disable rendering.
	PPU_CTRL = 0;
	PPU_MASK = 0;
	PPU_SET_SCROLL_XY(0, 0);
	
	set_palette();
	print_introduction();
	
	PPU_CTRL = 0x80;		// Enable NMI.
	PPU_MASK = 0x08;		// Enable background rendering.
	
	while (1) {
		// Faid out.
		wait_frames(INTRODUCTION_DELAY);
		for (i = 0; i < 3; ++i) {
			PPU_SET_ADDR(PPU_PALETTE_START + 3);
			PPU_DATA = 0x0D | ((3 - i) << 4);
			PPU_SET_SCROLL(0);
			wait_frames(FAID_DELAY);
		}
		
		// Faid in.
		wait_frames(INTRODUCTION_DELAY);
		for (i = 0; i < 3; ++i) {
			PPU_SET_ADDR(PPU_PALETTE_START + 3);
			PPU_DATA = 0x0D | (i << 4);
			PPU_SET_SCROLL(0);
			wait_frames(FAID_DELAY);
		}
		set_palette();
		PPU_SET_SCROLL(0);
	}
	
	while (1)		// Loop for ever.
		continue;
}
