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

#include "ppu.h"

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "emulator.h"
#include "nes-hdr.h"
#include "debuger.h"
#include "palette.h"
#include "mapper.h"

//#define NES_ARM_LIB
#if defined NES_ARM_LIB
#include "graphics.h"
#endif

#if defined _WIN32 && !defined __GNUC__
#pragma comment(lib, "C:\\C++\\NES_DEV\\SDL\\lib\\x86\\SDL2.lib")
#pragma comment(lib, "C:\\C++\\NES_DEV\\SDL\\lib\\x86\\SDL2main.lib")
#endif

#define RED_COLOR		0x00FF0000
#define BLUE_COLOR		0x000000FF
#define GREEN_COLOR		0x0000FF00
#define BLACK_COLOR		0x00000000
#define WITHE_COLOR		0x00FFFFFF

#if defined _WIN32 || defined __linux__
SDL_Window* window;
SDL_Surface* surface;
#if defined DEBUG_PPU_NAMETABLE
SDL_Window* window_name_table;
SDL_Surface* surface_name_table;
#endif
#if defined DEBUG_PPU_PATTERNTABLE
SDL_Window* window_pattern_table;
SDL_Surface* surface_pattern_table;
int palette_number;
#endif
#endif

/// TODO: Replace thise declarations when moved to PPU status struct.
uint8_t pattern_table[PPU_PATTERN_TABLE_SIZE];
uint8_t name_table[PPU_NAME_TABLE_SIZE];
uint8_t palette[PPU_PALETTE_SIZE];
//struct PpuStatus ppu;

/// Callback functions for mappers.
void (*scanline_end)(int scanline) = NULL;
void (*scanline_start)(int scanline) = NULL;
void (*vblank_start)(void) = NULL;
void (*vblank_end)(void) = NULL;
void (*rendering_start)(void) = NULL;
void (*frame_end)(void) = NULL;

__inline static void ppu_data_reg_inc(pPpuStatus ppu)
{
	if (ppu->regs.ctrl & AUTO_INCREMNT)
		ppu->v += 32;	// Increment 32 by 32
	else
		ppu->v += 1;		// Increment 1 by 1
}

int ppu_init(pPpuStatus ppu)
{
#if defined _WIN32 || defined __linux__
	int i;
	SDL_DisplayMode mode;
	SDL_Window* w;
	SDL_Surface* s;

#if defined DEBUG_PPU_NAMETABLE
	SDL_Window* wn;
	SDL_Surface* sn;
#endif
#if defined DEBUG_PPU_PATTERNTABLE
	SDL_Window* wp;
	SDL_Surface* sp;
#endif
	
	memset(ppu, 0, sizeof(PpuStatus));
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 0;
	
	w = SDL_CreateWindow("NES Emulator", WINDOW_X_POS, WINDOW_Y_POS,
			WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (!w)
	{
		debug_message("SDL_CreateWindow() failed!");
		return 0;
	}
	
	mode.format = SDL_PIXELFORMAT_RGB888;
	mode.w = 256;
	mode.h = 240;
	mode.refresh_rate = 0;
	if (SDL_SetWindowDisplayMode(w, &mode) < 0)
	{
		debug_message("Setting display mode failed.!");
		return 0;
	}
	s = SDL_GetWindowSurface(w);
	debug_message("Byte per pixel : %d = '%s'   ", s->format->BytesPerPixel
			, SDL_GetPixelFormatName(s->format->format));
	// Clear surface.
	for (i = 0; i < s->w * s->h; ++i)
		((uint32_t*)(s->pixels))[i] = 0xFFFFFF;
	SDL_UpdateWindowSurface(w);
	window = w;
	surface = s;

#if defined DEBUG_PPU_NAMETABLE
	wn = SDL_CreateWindow("Name Table View", WINDOW_NAME_TABLE_X_POS, WINDOW_NAME_TABLE_Y_POS,
			WINDOW_NAME_TABLE_WIDTH, WINDOW_NAME_TABLE_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_SetWindowDisplayMode(wn, &mode);
	sn = SDL_GetWindowSurface(wn);
	// Clear surface.
	for (i = 0; i < sn->w * sn->h; ++i)
		((uint32_t*)(sn->pixels))[i] = 0x00FFFFFF;
	SDL_UpdateWindowSurface(wn);
	window_name_table = wn;
	surface_name_table = sn;

//	ppu_debug_name_table(ppu);
#endif
#if defined DEBUG_PPU_PATTERNTABLE
	wp = SDL_CreateWindow("Pattern Table View", WINDOW_PATTERN_TABLE_X_POS, WINDOW_PATTERN_TABLE_Y_POS, WINDOW_PATTERN_TABLE_WIDTH, WINDOW_PATTERN_TABLE_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_SetWindowDisplayMode(wp, &mode);
	sp = SDL_GetWindowSurface(wp);
	for (i = 0; i < sp->w * sp->h; ++i)
		((uint32_t*)(sp->pixels))[i] = 0x00FFFFFF;
	SDL_UpdateWindowSurface(wp);
	window_pattern_table = wp;
	surface_pattern_table = sp;

	palette[0] = 0x0E; palette[1] = 0x05; palette[2] = 0x12; palette[3] = 0x1A;
	ppu_debug_pattern_table(ppu);
#endif

#endif

	return 1;
}

void ppu_close(pPpuStatus ppu)
{
	(void)ppu;
#if defined _WIN32
	if (window)
		SDL_DestroyWindow(window);

#if defined DEBUG_PPU_NAMETABLE
	if (window_name_table)
		SDL_DestroyWindow(window_name_table);

#endif
	SDL_Quit();
#endif
}

void ppu_write(pPpuStatus ppu, uint16_t addr, uint8_t value)
{
	switch (addr & 0x2007)
	{
		case PPUCTRL_REG:		// $2000
			log_info("writing $2000 (PPU CTRL REG)      : 0x%02X at cc:%d", value, p.cycle_counter);
			ppu->regs.ctrl = value;
			ppu->t &= ~0x0C00;
			ppu->t |= (uint16_t)(value & 0x03) << 10;		// Copy nametable select bits to T latch.
			break;

		case PPUMASK_REG:		// $2001
			log_info("writing $2001 (PPU MASK REG)      : 0x%02X at cc:%d", value, p.cycle_counter);
			ppu->regs.mask = value;
			break;

		case OAMADDR_REG:		// $2003
			log_info("writing $2003 (OAM ADDR REG)      : 0x%02X at cc:%d", value, p.cycle_counter);
			ppu->regs.oam_address = value;
			break;
		
		case OAMDATA_REG:		// $2004
			log_info("writing $2004 (OAM DATA REG)      : 0x%02X at 0x%02X at cc:%d", value, ppu->regs.oam_address, p.cycle_counter);
			ppu->OAM.u.memory[ppu->regs.oam_address++] = value;
			break;

		case PPUSCROLL_REG:		// $2005
			log_info("%s writing $2005 (PPU SCROLL REG) : 0x%02X at cc:%d", (ppu->w ? "second" : "first"), value, p.cycle_counter);
			if (ppu->w == 0)		// First write
			{
				ppu->x = value & 0x07;		// Copy lower 3 bits.
				ppu->t &= ~0x001F;
				ppu->t |= (uint16_t)(value >> 3) & 0x001F;	// Copy 5 bits to D4-D0.
				ppu->w = 1;
			}
			else				// Second write
			{
				ppu->t &= ~0x73E0;
				ppu->t |= ((uint16_t)(value & 0x07) << 12)// Copy lower 3 bits to D14-D12
					| ((uint16_t)(value & 0xF8) << 2);	// Copy 5 bits to D9-D5
				ppu->w = 0;
			}
			log_info("                    x : 0x%02X       t : 0x%04X      v : 0x%04X at cc:%d", ppu->x, ppu->t, ppu->v, p.cycle_counter);
			break;

		case PPUADDR_REG:		// $2006
			log_info("%s writing $2006 (PPU ADDR REG)   : 0x%02X    v(addr) : 0x%04X          t : 0x%04X at cc:%d", (ppu->w ? "second" : "first "), value, ppu->v, ppu->t, p.cycle_counter);
			if (ppu->w == 0)		// First write
			{
													// Address bus is 14 bit then
													// top 2 bits is ignored.
													// Clear bit 14, and D8-D13
				ppu->t &= ~0xFF00;
				ppu->t |= (uint16_t)(value & 0x3F) << 8;	// D8-D13 is loaded with lower 6
															// bits of value.
				ppu->w = 1;
			}
			else				// Second write
			{
				ppu->t &= ~0x00FF;
				ppu->t |= (uint16_t)value;
				ppu->v = ppu->t;
				ppu->w = 0;
				ppu_debug_name_table(ppu, GREEN_COLOR);
			}
			log_info("             after writing addr :       v : 0x%04x          t : 0x%04X at cc:%d", ppu->v, ppu->t, p.cycle_counter);
			break;

		case PPUDATA_REG:		// $2007
			log_info("writing $2007 (PPU DATA REG) 0x%02X at 0x%04X at cc:%d", value, ppu->v, p.cycle_counter);
#if defined _WIN32 && !defined __GNUC__
			if (ppu->v >= 0 && ppu->v < 0x2000)
#elif defined __linux__ || defined __GNUC__
			if ((0 < ppu->v || ppu->v == 0) && ppu->v < 0x2000)
#else
			if ((0 < ppu->v || ppu->v == 0) && ppu->v < 0x2000)
#endif
			{
				if (!mapper_write_ppu(addr, value))				// Mapper 2 hase CHAR RAM.
					log_error("WRONG PPU ADDRESS! Writting 0x%02X at 0x%04X to pattern ROM!", value, ppu->v);
			}
			else if (ppu->v >= 0x2000 && ppu->v < 0x3F00)		// Nametable attribute.
			{
				int n_addr;
				switch (mirroring)
				{
					case MIRRORING_VERTICAL:
						n_addr = ppu->v & 0x07FF;
						name_table[n_addr] = name_table[n_addr + 0x0800] = value;
						break;

					case MIRRORING_HORIZONTAL:	// If MIRRORING_HORIZONTAL
						n_addr = ppu->v & 0x03FF;
						if (ppu->v < 0x2800)
							name_table[n_addr] = name_table[n_addr + 0x0400] = value;
						else
							name_table[n_addr + 0x0800] = name_table[n_addr + 0xC00] = value;
						break;

					case MIRRORING_ONE_SCREEN:
						n_addr = ppu->v & 0x3FF;
						name_table[n_addr] = name_table[n_addr | 0x0400]
								= name_table[n_addr | 0x0800] = name_table[n_addr | 0x0C00]
								= value;
						break;

					case MIRRORING_FOUR_SCREEN:
						n_addr = ppu->v & 0x1FFF;
						name_table[n_addr] = value;
						break;

					default:
						log_error("Unknown mirroring type : %d", mirroring);
						break;
				}
			}
			else if (ppu->v < 0x4000)							// Palettes
			{
				int addr = ppu->v & 0x001F;
				palette[addr] = value;
				if (addr > 0x0F && ((addr & 0x03) == 0))		// Background color
					palette[0] = value;
					/*palette[0x00] = palette[0x04] = palette[0x08] = palette[0x0C] =
					palette[0x10] = palette[0x14] = palette[0x18] = palette[0x1C] = value;*/
			}

			ppu_data_reg_inc(ppu);
			ppu->regs.data = value;
			break;
		default:
			log_error("writing 0x%02X at 0x%04X", value, ppu->v);
			break;
	}
}

uint8_t ppu_read(pPpuStatus ppu, uint16_t addr)
{
	int tmp;
	switch (addr)
	{
		case PPUSTATUS_REG:		// $2002
			log_info("reading $2002 (PPU STATUS REG) 0x%02X at cc:%d", ppu->regs.status, p.cycle_counter);
			ppu->w = 0;							// Clear first/second write toggle flage.
			tmp = ppu->regs.status;
			ppu->regs.status &= ~VBLANK_EVENT;	// Clear vblank event flag.
			return tmp;

		case OAMDATA_REG:		// $2004
			log_info("reading $2004 (OAM DATA REG)   0x%02X from 0x%02X at cc:%d", ppu->OAM.u.memory[ppu->regs.oam_address], ppu->regs.oam_address, p.cycle_counter);
			return ppu->OAM.u.memory[ppu->regs.oam_address];

		case PPUDATA_REG:		// $2007
			log_info("reading $2007 (PPU DATA REG)   0x%02X at cc:%d", ppu->regs.data, p.cycle_counter);
			addr = ppu->v & 0x3FFF;		// Current VRAM address
			if (addr < 0x3EFF)
			{
				tmp = ppu->regs.data;
				if (addr < 0x2000)
					ppu->regs.data = pattern_table[addr];
				else
					ppu->regs.data = name_table[addr - 0x2000];
			}
			else
				tmp = palette[addr & 0x00FF];		// Palette data

			ppu_data_reg_inc(ppu);
			return tmp;

		default:
			log_error("reading at 0x%04X at cc:%d", (int)ppu->v, p.cycle_counter);
			break;
	}

	log_error("Invalid PPU register read at address : 0x%04X at cc:%d", addr, p.cycle_counter);
	return 0;
}

void ppu_clock(pPpuStatus ppu)
{
	int i;

	// VINT period for 20 scanline (0..19)
	if (ppu->scanline == SCANLINE_VINT_START && ppu->clock_cycle == 0)
	{
		if (vblank_start)
			vblank_start();

		/*
		 * The VBlank flag of the PPU is set at tick 1 (the second tick) of scanline 241,
		 * where the VBLANK NMI also occurs. The PPU makes no memory accesses during
		 * these scanlines.
		 */
		if (ppu->regs.ctrl & NMI_ENABLE)
			ppu->vblank_flag = 1;
		ppu->regs.status |= VBLANK_EVENT;// | WRITE_TO_VRAM_ENABLE;
	}
	else if (ppu->scanline == SCANLINE_DUMMY && ppu->clock_cycle == 0)// Clear VBlank flag and status register.
	{
		if (vblank_end)
			vblank_end();
		if (rendering_start)
			rendering_start();

		ppu->obj_0_found = 0;
		ppu->vblank_flag = 0;
		ppu->regs.status &= 0x1F;
		//if (!IS_RENDERING_ENABLED(ppu))
			ppu->regs.status |= WRITE_TO_VRAM_ENABLE;
		ppu_debug_name_table(ppu, BLUE_COLOR);
		ppu_debug_pattern_table(ppu);
		log_info("Start of frame    t: $%04x     v: $%04x at cc:%d", ppu->t, ppu->v, p.cycle_counter);
	} 
	else if (IS_RENDERING_ENABLED(ppu))		// Rendering
	{	
		// At scanline 20 set VINT flag and start rendering dummy scanline
		if (ppu->scanline == SCANLINE_DUMMY)
		{			
			// On every odd frames this scanline is only 340 clock cycles.
			if (ppu->odd_even_frame && ppu->clock_cycle == PPU_CLOCK_PER_SCANLINE - 1)
				++ppu->clock_cycle;
			
			/*
			 * During pixels 280 through 304 of this scanline, the vertical scroll bits
			 * are reloaded if rendering is enabled.
			 */
			if (ppu->clock_cycle >= 279 && ppu->clock_cycle <= 303)
				VERT(ppu->v, ppu->t);
		}
		
		// Between scanlines 21..260 rendering 240 lines.SCANLINE_RENDERING_START
		if (ppu->scanline >= SCANLINE_DUMMY && ppu->scanline < SCANLINE_RENDERING_END)
		{
			/*
			 * The data for each tile is fetched during this phase. Each memory access
			 * takes 2 PPU cycles to complete, and 4 must be performed per tile:
			 *
			 * 1. Name table byte
			 * 2. Attribute table byte
			 * 3. Tile bitmap low
			 * 4. Tile bitmap high (+8 bytes from tile bitmap low)
			 */
			if (ppu->clock_cycle < 256
				|| (ppu->clock_cycle >= 320 && ppu->clock_cycle < 337)) // 0 .. 255 || 320 .. 335
			{
				switch (ppu->clock_cycle % 8)
				{
					case 0:
						/*
						 * Load new data to background shift registers.
						 * At the start of a new tile fetch phase (every 8 cc's), both latched pattern table
						 * bitmaps are loaded into the upper 8-bits of 2 16-bit shift registers.
						 *
						 * Reload shift registers at 8, 16, 24, ..., 256.
						 * NOTE: At the beginning of each scanline, the data for the first
						 * two tiles is already loaded into the shift registers, so the
						 * first tile that gets fetched is tile 3.
						 */
						if (ppu->clock_cycle != 0)
						{
							ppu->bg_shift_reg1.b.l = ppu->bg_data_low;
							ppu->bg_shift_reg2.b.l = ppu->bg_data_high;
							ppu->pal_latch1 = ppu->bg_attri & 1;
							ppu->pal_latch2 = (ppu->bg_attri & 2) >> 1;
						}
						break;
					case 1:
						// Name table byte
						ppu->tile_addr = name_table[TILE_ADDRESS(ppu->v)] * 16 + FIN_Y(ppu->v);
						break;
					case 3:
						// Attribute table byte
						ppu->bg_attri = name_table[ATTRIBUTE_ADDRESS(ppu->v)];
						if (ppu->v & 0x0002)
							ppu->bg_attri >>=2;
						if (ppu->v & 0x0040)
							ppu->bg_attri >>= 4;
						break;
					case 5:
						// Low background byte
						ppu->bg_data_low = pattern_table[ppu->tile_addr 
							+ (ppu->regs.ctrl & BACKGROUND_PATTERN_TABALE ? 0x1000 : 0)];
						break;
					case 7:
						// High background byte
						ppu->bg_data_high = pattern_table[ppu->tile_addr + 8
							+ ((ppu->regs.ctrl & BACKGROUND_PATTERN_TABALE) ? 0x1000 : 0)];
						INC_HORI(ppu->v);
						break;
				}
				if (ppu->clock_cycle == 255)
					INC_VERT(ppu->v);		// inc vert(v)
			}

			if (ppu->clock_cycle == 256)
			{
				//ppu->t = 0x2400;
				HORI(ppu->v, ppu->t);		// hori(v) = hori(t)
			}

			// Sprite evaluation for the next scanline done as a seperate process.
			if (ppu->clock_cycle < 64)
			{
				/*
				 * Secondary OAM (32-byte buffer for current sprites on scanline) in initialized
				 * to $FF - attempting to read $2004 will return $FF. Internally, the clear
				 * operation is implemented by reading from the OAM and writing into the secondary
				 * OAM as usual, only a signal is active that makes the read always return $FF.
				 */
				//memset(&ppu->OAM_secondary, 0xFF, sizeof(SecondarySpriteMemory));
				ppu->OAM_secondary.u.memory[ppu->clock_cycle / 2] = 0xFF;
			}
			else if (ppu->clock_cycle < 256)
			{
				// Reset the index
				if (ppu->clock_cycle == 64)
				{
					// Store number of in-range sprites
					ppu->sprites_found = ppu->OAM_secondary_index;
					// Reset indices.
					ppu->OAM_index = ppu->OAM_secondary_index = 0;
					ppu->phase = PPU_PHASE_Y_EVAL;
				}

				if (!(ppu->clock_cycle & 1))		// On odd clock cycles write.
				{
					switch (ppu->phase)
					{
						case PPU_PHASE_Y_EVAL:
							if ((ppu->scanline - ppu->OAM.u.memory[ppu->OAM_index] >= 21)
								&& (ppu->scanline - ppu->OAM.u.memory[ppu->OAM_index] < (21
								+ (ppu->regs.ctrl & SPRITE_SIZE ? 16 : 8))))
							{
								// In range sprite
								ppu->OAM_secondary.u.memory[ppu->OAM_secondary_index] =
									ppu->OAM.u.memory[ppu->OAM_index];

								if (ppu->OAM_index == 0)
									ppu->obj_0_found = 1;
								
								++ppu->OAM_secondary_index;
								++ppu->OAM_index;
								ppu->phase = PPU_PHASE_COPY;
								break;
							}
							ppu->OAM_index += 4;
							if (ppu->OAM_index >= 256)
								ppu->phase = PPU_PHASE_END;
							break;
						case PPU_PHASE_COPY:
							ppu->OAM_secondary.u.memory[ppu->OAM_secondary_index] =
								ppu->OAM.u.memory[ppu->OAM_index];
							++ppu->OAM_secondary_index;
							++ppu->OAM_index;
							if ((ppu->OAM_secondary_index & 0x03) == 0)
							{
								if (ppu->OAM_index >= 256)
									ppu->phase = PPU_PHASE_END;
								else if (ppu->OAM_secondary_index == 32)
									ppu->phase = PPU_PHASE_DROP;
								else
									ppu->phase = PPU_PHASE_Y_EVAL;
							}
							break;
						case PPU_PHASE_DROP:
							if ((ppu->scanline - ppu->OAM.u.memory[ppu->OAM_index] >= 21)
								&& (ppu->scanline - ppu->OAM.u.memory[ppu->OAM_index] < (21
								+ (ppu->regs.ctrl & SPRITE_SIZE) ? 16 : 8)))
							{
								// In range sprite
								ppu->regs.status |= SPRITE_OVERFLOW;
								ppu->phase = PPU_PHASE_DROP_COPY;
								ppu->next_entry = ppu->OAM_index + 4;
								++ppu->OAM_index;
							}
							else
							{
								ppu->OAM_index = ((ppu->OAM_index + 4) & ~0x03)
									| ((ppu->OAM_index + 1) & 0x03);
								if (ppu->OAM_index >= 256)
									ppu->phase = PPU_PHASE_END;
							}
							break;
						case PPU_PHASE_DROP_COPY:
							++ppu->OAM_index;
							if (ppu->OAM_index >= 256)
								ppu->phase = PPU_PHASE_END;
							else if (ppu->OAM_index == ppu->next_entry)
								ppu->phase = PPU_PHASE_DROP;
							break;
						case PPU_PHASE_END:
						default:
							break;
					}
				}
			}
			else if (ppu->clock_cycle < 320)
			{
				if (ppu->clock_cycle == 256)
					ppu->OAM_index = ppu->OAM_secondary_index / 4, ppu->OAM_secondary_index = 0;

				if (ppu->OAM_secondary_index != ppu->OAM_index)
				{
					int y;
					switch (ppu->clock_cycle % 8)
					{
						case 3:
							ppu->sp_attribute[ppu->OAM_secondary_index] =
								ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].attribute;
							ppu->sp_xpos[ppu->OAM_secondary_index] =
								ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].x;
							break;
						case 5:
							if (ppu->regs.ctrl & SPRITE_SIZE)		// 8x16sprite size
							{
								y = ppu->scanline - 21
									- ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].y;

								if (ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].attribute & SPRITE_FILIP_VERTICAL)
									y = 15 - y;

									y += 32 * (ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].index / 2)
										+ ((y < 8) ? 0 : 8);

								y += ((ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].index % 2 == 0) ? 0x0000 : 0x1000);

								ppu->sp_shift_reg_low[ppu->OAM_secondary_index] =
									pattern_table[y];
							}
							else									// 8x8 sprite size
							{
								ppu->sp_shift_reg_low[ppu->OAM_secondary_index] = /*0xFF;*/
									pattern_table[ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].index
									* 16
									+ (ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].attribute
									& SPRITE_FILIP_VERTICAL 
									? 7 - ppu->scanline + 21
									+ ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].y
									: ppu->scanline - 21
									- ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].y)
									+ ((ppu->regs.ctrl & SPRITE_PATTERN_TABLE) ? 0x1000 : 0x0000 )];
							}
							break;
						case 7:
							if (ppu->regs.ctrl & SPRITE_SIZE)		// 8x16 sprite size
							{
								y = ppu->scanline - 21
									- ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].y;

								if (ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].attribute & SPRITE_FILIP_VERTICAL)
									y = 15 - y;

									y += 32 * (ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].index / 2) + 8
										+ ((y < 8) ? 0 : 8);

								y += ((ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].index % 2 == 0) ? 0x0000 : 0x1000);

								ppu->sp_shift_reg_high[ppu->OAM_secondary_index] =
									pattern_table[y];
							}
							else									// 8x8 sprite size
							{
								ppu->sp_shift_reg_high[ppu->OAM_secondary_index] = /*0xFF;*/
									pattern_table[ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].index
									* 16 + 8
									+ (ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].attribute
									& SPRITE_FILIP_VERTICAL
									? 7 - ppu->scanline + 21
									+ ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].y
									: ppu->scanline - 21
									- ppu->OAM_secondary.u.sprite[ppu->OAM_secondary_index].y)
									+ ((ppu->regs.ctrl & SPRITE_PATTERN_TABLE) ? 0x1000 : 0x0000)];
							}
							++ppu->OAM_secondary_index;
							break;
					}
				}
			}

			if (ppu->scanline != SCANLINE_DUMMY)
				ppu_put_pixel(ppu);
		
			if ((ppu->clock_cycle >= 0 && ppu->clock_cycle < 256)
				|| (ppu->clock_cycle >= 320 && ppu->clock_cycle < 336))
			{
				// Shift background shift registers one bit to left.
				ppu->bg_shift_reg1.w <<= 1;
				ppu->bg_shift_reg2.w <<= 1;
				ppu->pal_shift_reg1 <<= 1;
				ppu->pal_shift_reg2 <<= 1;
				ppu->pal_shift_reg1 |= ppu->pal_latch1;
				ppu->pal_shift_reg2 |= ppu->pal_latch2;

				// Each cycle sprite x-conters decremented. If none zero do nothing, else
				// shift one bit for every cycle and prepar
				if (ppu->clock_cycle < 256)
				{
					// TODO: Ceck valid sprites only.
					for (i = 0; i < 8; ++i)
					{
						if (ppu->sp_xpos[i] == 0)		// Active sprite if counter is zero.
						{
							// Shift each cycle, pair registers.
							if (ppu->sp_attribute[i] & SPRITE_FILIP_HORIZONTAL)
							{
								ppu->sp_shift_reg_low[i] >>= 1;
								ppu->sp_shift_reg_high[i] >>= 1;
							}
							else
							{
								ppu->sp_shift_reg_low[i] <<= 1;
								ppu->sp_shift_reg_high[i] <<= 1;
							}
						}
						else
							--ppu->sp_xpos[i];
					}
				}
			}
		}
	}
	//else
	//{
	//	/// TODO: Add ppu rendering disabled color.
	//	if (ppu->scanline >= SCANLINE_RENDERING_START && ppu->scanline < SCANLINE_RENDERING_END)
	//			putpixel(ppu);
	//}
	
	// Adjust clock cycle and scanline counter.
	// 341 PPU's cc make up the time of a typical scanline (or 341/3 CPU cc's) 
	if ((++ppu->clock_cycle) == SCANLINE_CLOCK_END)
	{
		if (scanline_end)
			scanline_end(ppu->scanline);

		if (scanline_start)
				scanline_start(ppu->scanline);

		ppu->clock_cycle = SCANLINE_CLOCK_RESET;
		if ((++ppu->scanline) == SCANLINE_NUMBER_END)
		{
			if (frame_end)
				frame_end();

			ppu->scanline = SCANLINE_NUMBER_RESET;
			ppu->odd_even_frame ^= 1;		// Toggle frame odd/even flag.
			ppu_debug_name_table(ppu, RED_COLOR);
			log_info("End of frame      t: $%04x     v: $%04x at cc:%d\n", ppu->t, ppu->v, p.cycle_counter);
		}
	}
}

void ppu_put_pixel(pPpuStatus ppu)
{	
	if (ppu->clock_cycle >= 1 && ppu->clock_cycle < 257)
	{
		int i;
		uint32_t color;
		int color_index = 0, sp_index = 0;
		int selector = 0x80 >> ppu->x;
		
		// Background color
		color_index = ppu->bg_shift_reg1.b.h & selector ? 0x01 : 0;
		color_index |= ppu->bg_shift_reg2.b.h & selector ? 0x02 : 0;

		for (i = 0; i < ppu->sprites_found; ++i)
		{
			if (ppu->sp_xpos[i] == 0)
			{
				if (ppu->sp_attribute[i] & SPRITE_FILIP_HORIZONTAL)
				{
					sp_index = ppu->sp_shift_reg_low[i] & 0x01 ? 1 : 0;
					sp_index |= ppu->sp_shift_reg_high[i] & 0x01 ? 2 : 0;
				}
				else
				{
					sp_index = ppu->sp_shift_reg_low[i] & 0x80 ? 1 : 0;
					sp_index |= ppu->sp_shift_reg_high[i] & 0x80 ? 2 : 0;
				}

				// Check is non-transparent color?
				if (sp_index != 0)
					break;
			}
		}
		
		if ((i == 0) && (ppu->obj_0_found == 1)
			&& (color_index != 0) && (sp_index != 0))
		{
			ppu->regs.status |= SPRITE_0_HIT_FLAG;
			ppu->obj_0_found = 0;
		}

		// Pixel multiplaxer
		if (show_sp && show_bg)
		{
			if (sp_index != 0 && ((color_index == 0) || (i != 8 && color_index != 0
				&& !(ppu->sp_attribute[i] & SPRITE_PRIORITY))))
			{
				// Sprite color
				color_index = sp_index | ((ppu->sp_attribute[i] & SPRITE_PALETTE) << 2);
				color_index += PPU_PALETTE_SIZE / 2;
			}
			else if (color_index != 0 && ((sp_index == 0) || (i != 8 && sp_index != 0
				&& (ppu->sp_attribute[i] & SPRITE_PRIORITY))))
			{
				// Background color
				color_index |= ppu->pal_shift_reg1 & selector ? 0x04 : 0;
				color_index |= ppu->pal_shift_reg2 & selector ? 0x08 : 0;
			}
		}
		else
		{
			if (show_sp && sp_index)
			{
				// Sprite color
				color_index = sp_index | ((ppu->sp_attribute[i] & SPRITE_PALETTE) << 2);
				color_index += PPU_PALETTE_SIZE / 2;
			}
			else if (show_bg)
			{
				// Background color
				if (color_index)
				{
					color_index |= ppu->pal_shift_reg1 & selector ? 0x04 : 0;
					color_index |= ppu->pal_shift_reg2 & selector ? 0x08 : 0;
				}
			}
			else
				color_index = 0;
		}

		color_index = palette[color_index];
		if (ppu->regs.mask & COLOR_OR_MONOCHROME_MODE)
			color_index &= 0x30;

		color = colors[color_index];

#if defined _WIN32 || defined __linux__
		// Put color to surface.
		((uint32_t*)(surface->pixels))[((ppu->scanline - 21) * 256)
				+ (ppu->clock_cycle - 1)] = color;
#else
		p.x = ppu->clock_cycle - 1;
		p.y = ppu->scanline;
		//printf("p(%3d, %3d)\r\n", p.x, p.y);
		set_pixel(p, color);
#endif
	}
/*
	// End of scanline rendering.
	if (ppu->clock_cycle == 256)
	{
#if defined _WIN32 || defined __linux__
		// Update surface.
		// For speed up rendering only update at the end of rendering scanline.
		if (ppu->scanline == SCANLINE_RENDERING_END - 1)	
			SDL_UpdateWindowSurface(window);
#endif
	}*/
}

void ppu_print_status(pPpuStatus ppu)
{
#define EOL "\r\n"
	printf("Control 1    (PPUCTRL)   : %02X" EOL, ppu->regs.ctrl);
	printf("Control 2    (PPUMASK)   : %02X" EOL, ppu->regs.mask);
	printf("status       (PPUSTATUS) : %02X" EOL, ppu->regs.status);
	printf("spr ram addr (OAMADDR)   : %02X" EOL, ppu->regs.oam_address);
	printf("spr ram io   (OAMADATA)  : %02X" EOL, ppu->regs.oam_data);
	printf("vram addr 1  (PPUSCROLL) : %02X" EOL, ppu->regs.scroll);
	printf("vram addr 2  (PPUADDR)   : %02X" EOL, ppu->regs.address);
	printf("vram io      (PPUDATA)   : %02X" EOL EOL, ppu->regs.data);

	printf("scanline                 : %d" EOL, ppu->scanline);
	printf("clock cycle              : %d" EOL, ppu->clock_cycle);
	printf("odd even frame           : %d" EOL, ppu->odd_even_frame);
	printf("VBLANK flag              : %d" EOL, ppu->vblank_flag);
	printf("v                        : $%04X" EOL, ppu->v);
	printf("t                        : $%04X" EOL, ppu->t);
	printf("x                        : %d" EOL, ppu->x);
	printf("w                        : %d" EOL, ppu->w);

	printf("bg shift reg 1           : %04X" EOL, ppu->bg_shift_reg1.w);
	printf("bg shift reg 2           : %04X" EOL, ppu->bg_shift_reg2.w);
	printf("pal shift reg 1          : %02X" EOL, ppu->pal_shift_reg1);
	printf("pal shift reg 2          : %02X" EOL, ppu->pal_shift_reg2);
	printf("NT Address               : $%04X" EOL, TILE_ADDRESS(ppu->v));
	printf("BG ATTRIBUTE             : $%04X" EOL, ATTRIBUTE_ADDRESS(ppu->v));
	
	/// TODO: Sprite data goes here! Dump primary and secondary OAM data here.
#undef EOL
}

#if defined DEBUG_PPU_NAMETABLE
static void put_name_table(pPpuStatus ppu, int nt)
{
#if defined _WIN32 || __linux__
#if defined DEBUG_PPU_NAMETABLE
	int x, y;
	int i;
	int idx = 0;
	uint8_t low, high;
	uint32_t pixel;
	
	for (x = 0; x < 32 * 30; ++x)
	{
		const int tile_addr = x + nt * 0x0400;

		for (y = 0; y < 8; ++y)
		{
			if (ppu->regs.ctrl & BACKGROUND_PATTERN_TABALE)
			{
				  low = pattern_table[name_table[tile_addr] * 16     + y + 0x1000];
				 high = pattern_table[name_table[tile_addr] * 16 + 8 + y + 0x1000];
			}
			else
			{
				 low = pattern_table[name_table[tile_addr] * 16     + y];
				high = pattern_table[name_table[tile_addr] * 16 + 8 + y];
			}
			
			idx = (((x / 32) * 8) + y) * WINDOW_NAME_TABLE_WIDTH + (x % 32) * 8;

			     if (nt == 1) idx += 256;
			else if (nt == 2) idx += 30 * 8 * WINDOW_NAME_TABLE_WIDTH;
			else if (nt == 3) idx += 30 * 8 * WINDOW_NAME_TABLE_WIDTH + 256;
			
			for (i = 0; i < 8; ++i)
			{
				/**
				 * Extracting Attributes from tile address is calculated from the fallowing calculations:
				 * (It can be used for fast rendering in low end and slow devices)
				 *
				 * For example NameTable_0 addressings is:
				 *    NameTable_0    ->  $2000 to $23BF : $3C0 = 960 Bytes
				 *    AttribTable_0  ->  $23C0 to $2400 : $40  = 64  Bytes
				 *
				 * NameTable is a 32x30 tiles witch is maped to a 8x8 grid of AttribTable:
				 *
				 *    32 x 30         32 x 32     Each   4 bytes in NameTable -> Attrib += 1
				 *                                Each 128 bytes in NameTable -> Attrib += 8
				 *       | Map   ~>      | Map
				 *       v               v
				 *
				 *     8 x 8           8 x 8
				 *
				 *
				 * NameTable_0:
				 *
				 *           +---+---++---+---+  +---+---++---+---+--
				 *    $2000  | 0 | 1 || 2 | 3 |  | 4 | 5 || 6 | 7 |
				 *           +---0---++---1---+  +---+---++---+---+--
				 *    $2020  | 32| 33|| 34| 35|  | 36| 37|| 38| 39|
				 *           +===+===++===+===+  +===+===++===+===+--
				 *    $2040  | 64| 65|| 66| 67|  | 68| 69|| 70| 71|
				 *           +---2---++---3---+  +---+---++---+---+--
				 *    $2060  | 96| 97|| 98| 99|  |100|101||102|103|
				 *           +---+---++---+---+  +---+---++---+---+--
				 *
				 *           +---+---++---+---+  +---+---++---+---+--
				 *    $2080  |128|129||130|131|  |132|133||134|135|
				 *           +---+---++---+---+  +---+---++---+---+--
				 *    $20A0  |160|161||162|163|  |164|165||166|167|
				 *           +---+---++---+---+  +---+---++---+---+--
				 *    $20B0  |192|193||194|195|  |196|197||198|199|
				 *           +---+---++---+---+  +---+---++---+---+--
				 *    $20C0  |224|225||226|227|  |228|229||230|231|
				 *           +---+---++---+---+  +---+---++---+---+--
				 *           |   |   ||   |   |  |   |   ||   |   |
				 *
				 *
				 * AttribTable_0:
				 *
				 *           3  2  1  0            3  2  1  0
				 *    $23C0 [|][|][|][|]    $23C1 [|][|][|][|]    . . .
				 *    $23C8 [|][|][|][|]    $23C9 [|][|][|][|]    . . .
				 *    $23D0 [|][|][|][|]    $23D1 [|][|][|][|]    . . .
				 *
				 * We can extract AttribTable address from NameTable tile address as:
				 *
				 *                 +---+---+---+---+---+---+---+---+ +---+---+---+---+---+---+---+---+
				 * tile_addr $     | F | E | D | C | B | A | 9 | 8 | | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
				 *                 +---+---+---+---+---+---+---+---+ +---+---+---+---+---+---+---+---+
				 *                                           ^         ^   A       ^        ^  A
				 *                                           |_________|   |       |________|  |
				 *                                               Row       |         Column    |
				 *                                                      Shifting            Shifting
				 *
				 * Attribute Address = (Row * 8) + Column = ((tile_addr & 0x0380) >> 4) | ((tile_addr & 0x001C) >> 2)
				 *    Row    = ((tile_addr & 0x0380) >> 7) << 3) = (tile_addr & 0x0380) >> 4
				 *    Column = ((tile_addr & 0x001C) >> 2)
				 *
				 * Shifting = (((tile_addr & 0x0002) >> 1) | ((tile_addr & 0x0040) >> 5)) * 2
				 *     = (tile_addr & 0x0002) | ((tile_addr & 0x0040) >> 4)
				 */
				int attrib;
				const int attrib_addr = nt * 0x0400 | 0x03C0 |
					((tile_addr & 0x0380) >> 4) |
					((tile_addr & 0x001C) >> 2);
				const int shifting = (tile_addr & 0x02) | ((tile_addr & 0x40) >> 4);

				pixel  = ( low & (0x80 >> i)) ? 0x01 : 0;
				pixel |= (high & (0x80 >> i)) ? 0x02 : 0;

				attrib = name_table[attrib_addr];
				attrib >>= shifting;
				attrib &= 0x03;
				attrib <<= 2;		// High bits of color

				pixel |= attrib;

				if ((pixel & 0x03) == 0)
					pixel = 0;
				pixel = palette[pixel];
				((uint32_t*)(surface_name_table->pixels))[idx++] = (colors[pixel]);
			}
			//idx += 256 * 2 - 8;
		}
	}
#else
	(void)ppu;
	(void)nt;
#endif
#endif
}
#endif

#if defined DEBUG_PPU_NAMETABLE
static void plot_vertical_line(pPpuStatus ppu, int x, uint32_t color)
{
#if defined _WIN32 || __linux__
	int i;

	if (x < 0 || x >= WINDOW_NAME_TABLE_WIDTH)
		return;

	// Plot Y line.
	for (i = 0; i < WINDOW_NAME_TABLE_HEIGHT; ++i)
		((uint32_t*)(surface_name_table->pixels))[i * 256 * 2 + x] = color;
#else
	(void)x;
	(void)color;
#endif
	(void)ppu;
}

#if defined DEBUG_PPU_NAMETABLE
static void plot_horizontal_line(pPpuStatus ppu, int y, uint32_t color)
{
#if defined _WIN32 || __linux__
	int i;

	if (y < 0 || y >= WINDOW_NAME_TABLE_HEIGHT)
		return;

	// Plot X line.
	for (i = 0; i < WINDOW_NAME_TABLE_WIDTH; ++i)
		((uint32_t*)(surface_name_table->pixels))[i + 256 * 2 * y] = color;
#else
	(void)y;
	(void)color;
#endif
#endif
	(void)ppu;
}
#endif

void ppu_debug_name_table(pPpuStatus ppu, uint32_t color)
{
#if defined _WIN32 || __linux__
#if defined DEBUG_PPU_NAMETABLE
	
	// Return if not initialized or closed.
	if (!window_name_table)
		return;

	put_name_table(ppu, 0);
	put_name_table(ppu, 1);
	put_name_table(ppu, 2);
	put_name_table(ppu, 3);

	plot_vertical_line(ppu, ((COARSE_X(ppu->v) << 3) + ppu->x), color);
	plot_horizontal_line(ppu, COARSE_Y(ppu->v), color);

	SDL_UpdateWindowSurface(window_name_table);
#else
	(void)color;
#endif
#endif
	(void)ppu;
}

void ppu_debug_pattern_table(pPpuStatus ppu)
{
#if defined _WIN32 || defined __linux__
#if defined DEBUG_PPU_PATTERNTABLE
	int i, x, y;
	uint32_t pixel;

	// Return if not initialized or closed.
	if (!window_pattern_table)
		return;

	// Left pattern table
	for (i = 0; i < 256; ++i)
	{
		for (y = 0; y < 8; ++y)
		{
			for (x = 0; x < 8; ++x)
			{
				int idx = ((i % 16) * 8 + x)
						+ (((i / 16 * 8) + y) * WINDOW_PATTERN_TABLE_WIDTH);

				pixel  = pattern_table[i * 16     + y] & (0x80 >> x) ? 0x01 : 0;
				pixel |= pattern_table[i * 16 + 8 + y] & (0x80 >> x) ? 0x02 : 0;
				pixel = palette[palette_number * 4 + pixel];
				((uint32_t*)(surface_pattern_table->pixels))[idx] = colors[pixel];
			}
		}
	}

	// Right pattern table
	for (i = 0; i < 256; ++i)
	{
		for (y = 0; y < 8; ++y)
		{
			for (x = 0; x < 8; ++x)
			{
				int idx = ((i % 16) * 8 + x)
						+ (((i / 16 * 8) + y) * WINDOW_PATTERN_TABLE_WIDTH)
						+ (WINDOW_PATTERN_TABLE_WIDTH / 2);

				pixel  = pattern_table[i * 16     + y + (PPU_PATTERN_TABLE_SIZE / 2)] & (0x80 >> x) ? 0x01 : 0;
				pixel |= pattern_table[i * 16 + 8 + y + (PPU_PATTERN_TABLE_SIZE / 2)] & (0x80 >> x) ? 0x02 : 0;
				pixel = palette[palette_number * 4 + pixel];
				((uint32_t*)(surface_pattern_table->pixels))[idx] = colors[pixel];
			}
		}
	}
	SDL_UpdateWindowSurface(window_pattern_table);
#endif
#endif
	(void)ppu;
}

