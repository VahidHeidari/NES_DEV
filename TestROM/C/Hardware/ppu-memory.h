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

#ifndef PPU_MEMORY_H_
#define PPU_MEMORY_H_

#include "stdint.h"

/// Registers
#define PPU_CTRL				(*((uint8_t*)0x2000))
#define PPU_MASK				(*((uint8_t*)0x2001))
#define PPU_STATUS				(*((uint8_t*)0x2002))
#define PPU_OAM_ADDR			(*((uint8_t*)0x2003))
#define PPU_OAM_DATA			(*((uint8_t*)0x2004))
#define PPU_SCROLL				(*((uint8_t*)0x2005))
#define PPU_ADDR				(*((uint8_t*)0x2006))
#define PPU_DATA				(*((uint8_t*)0x2007))
#define PPU_DMA					(*((uint8_t*)0x4014))

/// Constants and sizes
#define PPU_PALETTE_SIZE		0x0020
#define PPU_NAME_TABLE_SIZE		0x1000
#define PPU_PATTERN_TABLE_SIZE	0x2000
 
/// Memory map addresses
/// Pattern tables
#define PPU_PATTERN_TABLE_0_START	0x0000
#define PPU_PATTERN_TABLE_0_END		0x0FFF
#define PPU_PATTERN_TABLE_1_START	0x1000
#define PPU_PATTERN_TABLE_1_END		0x1FFF
/// Nametables
#define PPU_NAME_TABLE_0_START		0x2000
#define PPU_NAME_TABLE_0_END		0x23FF
#define PPU_NAME_TABLE_1_START		0x2400
#define PPU_NAME_TABLE_1_END		0x27FF
#define PPU_NAME_TABLE_2_START		0x2800
#define PPU_NAME_TABLE_2_END		0x2BFF
#define PPU_NAME_TABLE_3_START		0x2C00
#define PPU_NAME_TABLE_3_END		0x2FFF
/// Palette RAM
#define PPU_PALETTE_START				0x3F00
#define PPU_PALETTE_END					0x3F1F
/// Background palette
#define PPU_PALETTE_BACKGROUND_START	0x3F00
#define PPU_PALETTE_BACKGROUND_END		0x3F0F
#define PPU_PALETTE_BACKGROUND_0_START	PPU_PALETTE_BACKGROUND_START
#define PPU_PALETTE_BACKGROUND_1_START	0x3F04
#define PPU_PALETTE_BACKGROUND_2_START	0x3F08
#define PPU_PALETTE_BACKGROUND_3_START	0x3F0C
/// Sprite palette
#define PPU_PALETTE_SPRITE_START		0x3F10
#define PPU_PALETTE_SPRITE_END			PPU_PALETTE_END
#define PPU_PALETTE_SPRITE_0_START		PPU_PALETTE_SPRITE_START
#define PPU_PALETTE_SPRITE_1_START		0x3F14
#define PPU_PALETTE_SPRITE_2_START		0x3F18
#define PPU_PALETTE_SPRITE_3_START		0x3F1C

/// Palette colors
#define PPU_COLOR_BLACK			0x0F
#define PPU_COLOR_WHITE			0x30
#define PPU_COLOR_BLUE			0x01

/// Functions
#define PPU_SET_ADDR(ADDR)		do {	\
	PPU_ADDR = ADDR >> 8;				\
	PPU_ADDR = ADDR;					\
} while (0)

#define PPU_SET_SCROLL_XY(X, Y)	do {	\
	PPU_SCROLL = X;						\
	PPU_SCROLL = Y;						\
} while (0)

#define PPU_SET_SCROLL(SCROLL)	do {	\
	PPU_SCROLL = SCROLL >> 8;			\
	PPU_SCROLL = SCROLL;				\
} while (0)

void wait_vblank(void);
void wait_frames(unsigned char frames);

#endif
 
