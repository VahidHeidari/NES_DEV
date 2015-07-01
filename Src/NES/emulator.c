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

#include "emulator.h"

#include <string.h>

#include "bank-switch.h"
#include "mapper.h"
#include "state.h"
#include "debuger.h"

#if defined NES_ARM_LIB
#include "joystick.h"
#endif

uint8_t ROM[ROM_SIZE];
uint8_t RAM[NOT_MIRRORED_RAM_SIZE];

P6502 p;					// 6502 processor
PpuStatus ppu;
Apu apu;
NesHeader hdr;				/// ROM image header
Joypad joypad;
int finished_emulation;		/// Emulation run or stop
char* image_path = NULL;	/// ROM image path
FILE* image = NULL;			/// ROM image
Mirroring mirroring;

void read_joypad(void);

int emulator_init(void)
{
	finished_emulation = 0;
	
#if defined _WIN32
	if (state_init("C:\\C++\\NES_DEV\\state\\") != 1)
#else
	if (state_init("../state/") != 1)
#endif
		return 0;

	if (joypad_init(&joypad) != 1)
		return 0;

	if (ppu_init(&ppu) != 1)
		return 0;

	if (apu_init(&apu) != 1)
		return 0;

	power_on(&p);

	frame_end = read_joypad;

	return 1;
}

void emulator_close(void)
{
	ppu_close(&ppu);
	apu_close(&apu);
	joypad_close(&joypad);

	if (image)
	{
		fclose(image);
		image = NULL;
	}
}

/// TODO: Write portable reading ROM image.
int read_rom_image(char* path)
{
	FILE* f;
#if defined _WIN32 && !defined __GNUC__
	fopen_s(&f, path, "rb");
#elif defined __linux__
	f = fopen(path, "rb");
#endif

	if (!f)
	{
		debug_message("Emulation failed because file '%s' could not open.", path);
		return 0;
	}
	memset(&hdr, 0, sizeof(hdr));
	if (fread(&hdr, sizeof(hdr), 1, f) != 1)		// Read NES header
	{
		debug_message("Reading NES header failed!");
		return 0;
	}

	nes_hdr_info(&hdr);

	// Check ROM image parameters supported by this emulator.
	switch (GET_MAPPER(&hdr))
	{
		case 0:
			if ((GET_NUM_OF_ROM_BANKS(&hdr) > 2) || (GET_NUM_OF_VROM_BANKS(&hdr) != 1))
			{
				debug_message("Mapper 0, invalid number of ROMS or CHR ROM");
				return 0;
			}
			if (IS_TRAINER(&hdr))
				fseek(f, 512, SEEK_CUR);

			if (GET_NUM_OF_ROM_BANKS((&hdr)) == 1)		// Read only PROG ROM part of image
			{
				if (fread(ROM, sizeof(ROM) / 2, 1, f) != 1)
				{
					debug_message("Reading ROM part from NES image file failed!");
					return 0;
				}
				memcpy(ROM + 0x4000, ROM, 0x4000);		// Copy 1st PRG bank to 2nd bank 
			}
			else
				if (fread(ROM, sizeof(ROM), 1, f) != 1)			// Copy all 2 PRG banks.
				{
					debug_message("Reading ROM part from NES image file failed!");
					return 0;
				}
			if (fread(pattern_table, sizeof(pattern_table), 1, f) != 1)		// Read 1st 8K pattern table
			{
				debug_message("Reading CHR ROM part from NES image file failed!");
				return 0;
			}
			break;
		case 1:
			/// TODO: Support trainer ram.
			if (IS_TRAINER(&hdr))
			{
				debug_message("Mapper 1 hase trainer, this type of ROMs not supported yet!");
				return 0;
			}
			init_mapper1(f);
			break;
		case 2:
			if (bank_switch_16k(0, ROM, f) != 1)
				return 0;
			if (bank_switch_16k(GET_NUM_OF_ROM_BANKS(&hdr) - 1, ROM + (ROM_SIZE / 2), f) != 1)
				return 0;
			break;
		case 3:
			if (fread(ROM, GET_NUM_OF_ROM_BANKS(&hdr) * (ROM_SIZE / 2), 1, f) != 1)
			{
				debug_message("Mapepr 3 reading ROM part from NES image file failed!");
				return 0;
			}
			if (bank_switch_8k(2 * GET_NUM_OF_ROM_BANKS(&hdr), pattern_table, f) != 1)// Skip 16Kb ROM banks.
				return 0;
			break;
		case 4:
			if (IS_TRAINER(&hdr))
				return 0;
			init_mapper4(f);
			break;

		default:
			return 0;
	}
	image_path = path;
	image = f;
	mirroring = GET_MIRRORING_TYPE(&hdr);

	// Dump pattern table
	/*f = fopen("C:\\mario.bin", "wb");
	fwrite(pattern_table, sizeof(pattern_table), 1, f);
	*/

	return 1;
}

void read_joypad(void)
{
#if (defined _WIN32 || defined __linux__) && READ_SDL_JOYPAD == 1
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
			case SDL_WINDOWEVENT:
				if (evnt.window.event == SDL_WINDOWEVENT_CLOSE)
					finished_emulation = 1;
				break;

#if defined _WIN32 || defined __linux__
#if defined DEBUG_PPU_PATTERNTABLE
			case SDL_MOUSEBUTTONDOWN:
				if (evnt.window.windowID == SDL_GetWindowID(window_pattern_table))
				{
					if (evnt.button.button == 1)			// Next palette
						palette_number = (palette_number + 1) % 8;
					else if (evnt.button.button == 3)		// Previouse palette
						palette_number = (palette_number + 7) % 8;
					else if (evnt.button.button == 2)		// Reset palette nubmer
						palette_number = 0;
				}
				break;
#endif
#endif
			case SDL_QUIT:
				finished_emulation = 1;
				break;

			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						finished_emulation = 1;
						break;
					case SDLK_UP:
						joypad.joypad_1 |= UP_BOTTUN;
						break;
					case SDLK_DOWN:
						joypad.joypad_1 |= DOWN_BOTTUN;
						break;
					case SDLK_LEFT:
						joypad.joypad_1 |= LEFT_BOTTUN;
						break;
					case SDLK_RIGHT:
						joypad.joypad_1 |= RIGHT_BOTTUN;
						break;
					case SDLK_KP_1:
					case SDLK_s:
						joypad.joypad_1 |= A_BOTTUN;
						break;
					case SDLK_KP_2:
					case SDLK_a:
						joypad.joypad_1 |= B_BOTTUN;
						break;
					case SDLK_KP_7:
					case SDLK_RETURN:
						joypad.joypad_1 |= START_BOTTUN;
						break;
					case SDLK_KP_8:
					case SDLK_RCTRL:
						joypad.joypad_1 |= SELECT_BOTTUN;
						break;

					case SDLK_p:
						show_sp ^= 1;
						break;
					case SDLK_b:
						show_bg ^= 1;
						break;

					// Save state.
					case SDLK_F1: state_save_slot(0); break;
					case SDLK_F2: state_save_slot(1); break;
					case SDLK_F3: state_save_slot(2); break;
					case SDLK_F4: state_save_slot(3); break;

					// Load state.
					case SDLK_F5: state_load_slot(0); break;
					case SDLK_F6: state_load_slot(1); break;
					case SDLK_F7: state_load_slot(2); break;
					case SDLK_F8: state_load_slot(3); break;
				}
				break;

			case SDL_KEYUP:
				switch (evnt.key.keysym.sym)
				{
					case SDLK_UP:
						joypad.joypad_1 &= ~UP_BOTTUN;
						break;
					case SDLK_DOWN:
						joypad.joypad_1 &= ~DOWN_BOTTUN;
						break;
					case SDLK_LEFT:
						joypad.joypad_1 &= ~LEFT_BOTTUN;
						break;
					case SDLK_RIGHT:
						joypad.joypad_1 &= ~RIGHT_BOTTUN;
						break;
					case SDLK_KP_1:
					case SDLK_s:
						joypad.joypad_1 &= ~A_BOTTUN;
						break;
					case SDLK_KP_2:
					case SDLK_a:
						joypad.joypad_1 &= ~B_BOTTUN;
						break;
					case SDLK_KP_7:
					case SDLK_RETURN:
						joypad.joypad_1 &= ~START_BOTTUN;
						break;
					case SDLK_KP_8:
					case SDLK_RCTRL:
						joypad.joypad_1 &= ~SELECT_BOTTUN;
						break;
				}
				break;
		}
	}
	SDL_UpdateWindowSurface(window);
#elif defined NES_ARM_LIB
	int key_state = joystick_get_key_state();

	if (JOYSTICK_IS_LEFT(key_state))
		joypad.joypad_1 |= LEFT_BOTTON;
	else
		joypad.joypad_1 &= ~LEFT_BOTTON;

	if (JOYSTICK_IS_RIGHT(key_state))
		joypad.joypad_1 |= RIGHT_BOTTON;
	else
		joypad.joypad_1 &= ~RIGHT_BOTTON;

	if (JOYSTICK_IS_UP(key_state))
		joypad.joypad_1 |= UP_BOTTON;
	else
		joypad.joypad_1 &= ~UP_BOTTON;

	if (JOYSTICK_IS_DOWN(key_state))
		joypad.joypad_1 |= DOWN_BOTTON;
	else
		joypad.joypad_1 &= ~DOWN_BOTTON;

	if (JOYSTICK_IS_A(key_state))
		joypad.joypad_1 |= A_BOTTON;
	else
		joypad.joypad_1 &= ~A_BOTTON;

	if (JOYSTICK_IS_B(key_state))
		joypad.joypad_1 |= B_BOTTON;
	else
		joypad.joypad_1 &= ~B_BOTTON;

	if (JOYSTICK_IS_SELECT(key_state))
		joypad.joypad_1 |= SELECT_BOTTON;
	else
		joypad.joypad_1 &= ~SELECT_BOTTON;

	if (JOYSTICK_IS_START(key_state))
		joypad.joypad_1 |= START_BOTTON;
	else
		joypad.joypad_1 &= ~START_BOTTON;
#endif
}

void step(void)
{
	int i;
	
	if (ppu.vblank_flag && (ppu.regs.status & VBLANK_EVENT) && (ppu.regs.ctrl & NMI_ENABLE))
	{
		p.interrupts = INTR_NMI;
		ppu.vblank_flag = 0;
	}

	fetch(&p);

	for (i = 0; i < p.ins_cycles; ++i)
	{
		// PPU is 3 times faster.
		ppu_clock(&ppu);
		ppu_clock(&ppu);
		ppu_clock(&ppu);

		apu_clock(&apu);
	}

	//read_joypad();
}

void run(void)
{
	while (!finished_emulation)
		step();
}

void run_cycles(int cycles)
{
	long long start_cycles = p.cycle_counter;

	while (p.cycle_counter - start_cycles < cycles && !finished_emulation)
		step();
}

