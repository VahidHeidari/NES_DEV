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

#include <stdio.h>

#include "M6502.h"
#include "p6502.h"

uint8_t MRAM[NOT_MIRRORED_RAM_SIZE];
int finished = 0;

uint8_t m_read_io(uint16_t addr)
{
	printf("Read invalid io at addr : $%04x.\n", addr);
	return 0;
}

void m_write_io(uint16_t addr, uint8_t val)
{
	printf("Invalid write val : $%02x at addr : $%04x\n", val, addr);
}

void Wr6502(register word Addr,register byte Value)
{
	if (Addr < (uint16_t)0x2000)		// 3 Mirrors of $0000-$0800
		MRAM[Addr & 0x07FF] = Value;
	else if (Addr < (uint16_t)0x8000)
		m_write_io(Addr, Value);
	else
	{
		printf("M6502 Write val $%02x to addr : $%04x\n", Value, Addr);
		if (Addr == 0xEAEA)
			finished = 1;
	}
}

byte Rd6502(register word Addr)
{
	if (Addr < 0x2000)		// Internal RAM
		return MRAM[Addr & 0x07FF];
	else if (Addr < 0x8000)
		return m_read_io(Addr);
	else
		return ROM[Addr - 0x8000];		// External program ROM or Mapper
}
//byte Op6502(register word Addr) { return Rd6502(Addr); }

byte Loop6502(register M6502 *R)
{
	(void)R;
	return INT_NONE;
}

byte Patch6502(register byte Op,register M6502 *R)
{
	printf("Patch6502 Op : 0x%02x at PC : 0x%04x\n", Op, R->PC.W);
	return 1;
}

void Print(M6502 *R)
{
	printf("PC:%04x A:%02x P:%02x X:%02x Y:%02x S:%02x\n"
		, R->PC.W, R->A, R->P, R->X, R->Y, R->S);
}

void print(pP6502 p)
{
	printf("pc:%04x a:%02x p:%02x x:%02x y:%02x s:%02x\n"
		, p->pc.w, p->a, p->p, p->x, p->y, p->sp);
}
