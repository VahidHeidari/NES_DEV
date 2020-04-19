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

#include "io.h"

#include <stdio.h>
#include <string.h>

#include "emulator.h"

#if defined _WIN32 || defined __linux__
#define IO_BASE		0x2000
uint8_t IO[IO_SIZE];
#endif

/**
 * TODO: More checking and mirroring, Mirroring does not handled!
 *
 * But I/O registers in $4000 - $4020 range does not have any
 * mirroring.
 */
void IO_Write(uint16_t addr, uint8_t val)
{
	int i;

	if (addr < 0x4000)
		PPU_Write(&ppu, addr, val);
	else if (addr < 0x4014)
		APU_Write(&apu, addr, val);		// TODO: Only $4015 is writeable. Remove this.
	else if (addr == 0x4014) {		// Sprite DMA
		memcpy(ppu.OAM.u.memory, RAM + val * 0x100, 256);
		p.cycle_counter += CPU_DMA_CLOCK;
		for (i = 0; i < CPU_DMA_CLOCK; ++i) {
			// PPU is 3 times faster than CPU.
			PPU_Clock(&ppu);
			PPU_Clock(&ppu);
			PPU_Clock(&ppu);

			APU_Clock(&apu);
		}
	} else if (addr == 0x4015)
		APU_Write(&apu, addr, val);
	else if (addr == 0x4016) {				// Joypad strobe
		if (val == 0x01)
			joypad.strobe = val;
		else if (val == 0x00 && joypad.strobe == 1)
			JOYPAD_RESET(&joypad);
	} else if (addr == 0x4017)
		APU_Write(&apu, addr, val);
#if defined _WIN32 || defined __linux__
	else
		IO[addr - IO_BASE] = val;
#endif
}

/// read or wirte functions must implement its mirroring itself.
uint8_t IO_Read(uint16_t addr)
{
	int tmp;
	if (addr < 0x4000)
		return PPU_Read(&ppu, addr);
	else if (addr == 0x4015)
		return APU_Read(&apu, addr);
	else if (addr == 0x4016) {						// Joypad 1
		tmp = !!(joypad.buttons1.buttons & joypad.buttons1.read_index);
		joypad.buttons1.read_index <<= 1;
		return tmp;
	} else if (addr == 0x4017) {							// Joypad 2
		tmp = !!(joypad.buttons2.buttons & joypad.buttons2.read_index);
		joypad.buttons2.read_index <<= 1;
		return tmp;
	}

#if defined _WIN32 || defined __linux__
	return IO[addr - IO_BASE];
#else
	return 0;
#endif
}

