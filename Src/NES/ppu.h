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

#ifndef PPU_H
#define PPU_H

#include <stdint.h>

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#include "..\\..\\3rdParty\\SDL2-2.0.5\\include\\SDL.h"
#elif defined __linux__
#include <SDL2/SDL.h>
#elif defined NES_ARM_LIB
#include "graphics.h"
#endif

#include "p6502.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WINDOW_X_POS					50
#define WINDOW_Y_POS					50
#define WINDOW_WIDTH					256
#define WINDOW_HEIGHT					240

#define WINDOW_NAME_TABLE_X_POS			350
#define WINDOW_NAME_TABLE_Y_POS			50
#define WINDOW_NAME_TABLE_WIDTH			(256 * 2)
#define WINDOW_NAME_TABLE_HEIGHT		(240 * 2)

#define WINDOW_PATTERN_TABLE_X_POS		50
#define WINDOW_PATTERN_TABLE_Y_POS		(WINDOW_Y_POS + WINDOW_HEIGHT + 50)
#define WINDOW_PATTERN_TABLE_WIDTH		(2 * 8 * 16)
#define WINDOW_PATTERN_TABLE_HEIGHT		(8 * 16)

/// CPU port address of PPU registers
#define PPU_CTRL_REG					0x2000
#define PPU_MASK_REG					0x2001
#define PPU_STATUS_REG					0x2002
#define OAM_ADDR_REG					0x2003
#define OAM_DATA_REG					0x2004
#define PPU_SCROLL_REG					0x2005
#define PPU_ADDR_REG					0x2006
#define PPU_DATA_REG					0x2007

#define PPU_PATTERN_TABLE_SIZE			0x2000
#define PPU_NAME_TABLE_SIZE				0x1000		/// All 4k of name tables.
#define PPU_PALETTE_SIZE				32

#define PPU_SCANLINES_PER_FRAME			262
#define PPU_CLOCK_PER_SCANLINE			341
#define PPU_CLOCK_PER_CPU_CLOCK			3
#define PPU_DMA_CLOCK					1536
#define CPU_DMA_CLOCK					512
#define PPU_MEMORY_ACCESS_PER_SCANLINE	170
#define PPU_VINT_PERIOD_SCANLINES		20
/// TODO: Fix reset values and boundaries.
#define SCANLINE_NUMBER_END				262
#define SCANLINE_NUMBER_RESET			0
#define SCANLINE_VINT_START				0
#define SCANLINE_VINT_END				19
#define SCANLINE_DUMMY					20
#define SCANLINE_RENDERING_START		21		/// TODO: Replace all hard coded ppu->scanline - 21 lines.
#define SCANLINE_RENDERING_END			261
#define SCANLINE_CLOCK_END				340
#define SCANLINE_CLOCK_RESET			0

/// PPU Control register 1 ($20 00) WRITE
#define NAME_TABLE_ADDRESS				0x03		/// Bits 0-1
#define AUTO_INCREMNT					0x04		/// Bit 2 (0:+1, 1:+32)
#define SPRITE_PATTERN_TABLE			0x08		/// Bit 3
#define BACKGROUND_PATTERN_TABALE		0x10		/// Bit 4
#define SPRITE_SIZE						0x20		/// Bit 5 (1:8x8, 0:8:16)
#define MASTER_SLAVE_MODE				0x40		/// Bit 6
#define NMI_ENABLE						0x80		/// Bit 7 (1:En, 0:Dis)

/// PPU Control Register 2 ($20 01) WRITE
#define COLOR_OR_MONOCHROME_MODE		0x01		/// Bit 0
#define CLIP_BACKGROUND					0x02		/// Bit 1 (0:Hide 1:Show)
#define CLIP_SPRITES					0x04		/// Bit 2 (0:Hide 1:Show)
#define BACKGROUND_DISPLAY_ENABLE		0x08		/// Bit 3
#define SPRITES_DISPLAY_ENABLE			0x10		/// Bit 4
#define COLOR_INTENSITY					0xE0		/// Bits 5-7

/// PPU Status Register ($20 02) READ
#define WRITE_TO_VRAM_ENABLE			0x10		/// Bit 4 (if 0 writing ignored)
#define SPRITE_OVERFLOW					0x20		/// Bit 5
#define SPRITE_0_HIT_FLAG				0x40		/// Bit 6
#define VBLANK_EVENT					0x80		/// Bit 7

#define SPRITE_PALETTE					0x03
#define SPRITE_PRIORITY					0x20		/// 0 : ing front of background		1 : behind background
#define SPRITE_FILIP_HORIZONTAL			0x40
#define SPRITE_FILIP_VERTICAL			0x80

#define IS_RENDERING_ENABLED(ppu)	((ppu->regs.mask & (BACKGROUND_DISPLAY_ENABLE | SPRITES_DISPLAY_ENABLE)))
#define IS_BACKGROUND_ENABLED(ppu)	(ppu->regs.mask & BACKGROUND_DISPLAY_ENABLE)
#define IS_SPRITE_ENABLED(ppu)		(ppu->regs.mask & SPRITES_DISPLAY_ENABLE)

#define HORI(v, t)		do {	\
	v &= ~0x041F;				\
	v |= t & 0x041F;			\
} while (0)

#define VERT(v, t)		do {	\
	v &= ~0x7BE0;				\
	v |= t & 0x7BE0;			\
} while (0)

/// Coarse Y increment
#define INC_VERT(v)		do {			\
	if ((v & 0x7000) != 0x7000)			\
		v += 0x1000;					\
	else {								\
		v ^= 0x7000;					\
		if ((v & 0x03E0) == 0x03A0)		\
			v ^= 0x0BA0;				\
		else if ((v & 0x03E0) == 0x03E0)\
			v ^= 0x03E0;				\
		else							\
			v += 0x0020;				\
	}									\
} while (0)

/// Coarse X increment
#define INC_HORI(v)		do {	\
	if ((v & 0x001F) == 31) {	\
		v &= ~0x001F;			\
		v ^= 0x0400;			\
	} else						\
		++v;					\
} while (0)

//#define TILE_ADDRESS_IO(v)			(0x2000 | (v & 0x0FFF))
//#define ATTRIBUTE_ADDRESS_IO(v)		(0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07))
#define TILE_ADDRESS(v)				(v & 0x0FFF)
#define ATTRIBUTE_ADDRESS(v)		(0x3C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07))
#define FIN_Y(v)					((v >> 12) & 0x7)
#define COARSE_X(v)					(v & 0x001F)
#define COARSE_Y(v)					((v >> 5) & 0x001F)

#if defined _WIN32 || defined __linux__
extern SDL_Window* window;
extern SDL_Surface* surface;

/// PPU debugging level name table
extern SDL_Window* window_name_table;
extern SDL_Surface* surface_name_table;
extern int is_window_name_table_shown;

/// PPU debugging level pattern table
extern SDL_Window* window_pattern_table;
extern SDL_Surface* surface_pattern_table;
extern int is_window_pattern_table_shown;
extern int palette_number;
#endif

/// TODO: Replace thise declarations when moved to PPU status struct.
extern uint8_t pattern_table[PPU_PATTERN_TABLE_SIZE];
extern uint8_t name_table[PPU_NAME_TABLE_SIZE];
extern uint8_t palette[PPU_PALETTE_SIZE];
extern struct PpuStatus ppu;

/// Callback functions for mappers.
extern void (*scanline_end)(int scanline);
extern void (*scanline_start)(int scanline);
extern void (*vblank_start)(void);
extern void (*vblank_end)(void);
extern void (*rendering_start)(void);
extern void (*frame_end)(void);

typedef enum
{
	PPU_PHASE_Y_EVAL,
	PPU_PHASE_COPY,
	PPU_PHASE_DROP,
	PPU_PHASE_DROP_COPY,
	PPU_PHASE_END
} PpuPhase_t;

/// TODO: Clear some unused registers.
typedef struct PpuRegs
{
	uint8_t ctrl;			/// $20 00		PPUCTRL
	uint8_t mask;			/// $20 01		PPUMASK
	uint8_t status;			/// $20 02		PPUSTATUS
	uint8_t oam_address;	/// $20 03		OAMADDR
	uint8_t oam_data;		/// $20 04		OAMDATA & $E3
	uint8_t scroll;			/// $20 05		PPUSCROLL (scroll register X)
	uint8_t address;		/// $20 06		PPUADDR (scroll address)
	uint8_t data;			/// $20 07		PPUDATA
} PpuRegs, *pPpuRegs;

typedef struct Sprite
{
	uint8_t y;
	uint8_t index;
	uint8_t attribute;
	uint8_t x;
} Sprite, *pSprite;

typedef struct PrimarySpriteMemory
{
	union
	{
		Sprite sprite[64];			/// Named memory
		uint8_t memory[64 * 4];		/// Raw memory
	} u;
} PrimarySpriteMemory, *pPrimarySpriteMemory;

typedef struct SecondarySpriteMemory
{
	union
	{
		Sprite sprite[8];			/// Named memory
		uint8_t memory[8 * 4];		/// Raw memory
	} u;
} SecondarySpriteMemory, *pSecondarySpriteMemory;

typedef struct PpuStatus
{
	struct PpuRegs regs;			/// PPU I/O registers, wich CPU can access.

	/// Internal registers used for internal operations.
	int scanline;
	int clock_cycle;
	int odd_even_frame;
	int vblank_flag;				/// VBlank flag

	/// Background
	/// TODO: Change v and t types to TwoByteReg for some performance improvemnets.
	uint16_t v;						/// VRAM address
	uint16_t t;						/// Temp VRAM address
	int x;							/// Fine x scroll
	int w;							/// First/Second toggle
	TwoByteReg bg_shift_reg1;		/// Background shift register for bitmap 1
	TwoByteReg bg_shift_reg2;		/// Background shift register for bitmap 2
	uint8_t bg_data_low;
	uint8_t bg_data_high;

	uint8_t pal_shift_reg1;			/// Palette shift register 1
	uint8_t pal_shift_reg2;			/// Palette shift register 2
	uint8_t pal_latch1;
	uint8_t pal_latch2;
	uint8_t bg_attri;
	uint16_t tile_addr;

	// Sprites
	PrimarySpriteMemory OAM;				/// Primary OAM
	SecondarySpriteMemory OAM_secondary;	/// Secondary OAM
	int OAM_index;							/// Index to current evaluating sprite
	int OAM_secondary_index;				/// Index to current sprite (4-bit counter)
	int sprites_found;						/// Number of in-range sprites found
	PpuPhase_t phase;						/// PPU sprite evaluation phase
	int next_entry;							/// PPU drop phase next entry counter
	int obj_0_found;						/// 1 bit memory
	int sp_shift_reg_low[8];				/// 8 paires of 8-bit shift registers
	int sp_shift_reg_high[8];
	uint8_t sp_attribute[8];				/// 8 latches
	int sp_xpos[8];							/// 8 counters contains the x position of sprites
} PpuStatus, *pPpuStatus;

int PPU_Init(pPpuStatus ppu);
void PPU_Close(pPpuStatus ppu);
void PPU_Write(pPpuStatus ppu, uint16_t addr, uint8_t value);
uint8_t PPU_Read(pPpuStatus ppu, uint16_t addr);
void PPU_Clock(pPpuStatus ppu);
void PPU_PutPixel(pPpuStatus ppu);

/// Debuging
void PPU_PrintStatus(pPpuStatus ppu);
void PPU_DebugNameTable(pPpuStatus ppu, uint32_t color);
void PPU_DebugPatternTable(pPpuStatus ppu);

#ifdef __cplusplus
}
#endif

#endif

