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

#ifndef MAPPER_H_
#define MAPPER_H_

#include <stdint.h>
#include <stdio.h>

#include "state.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Mapper 1
/// State tag
#define MAPPER_1_TAG_ID						0x31434d4d
/// Tmp reg
#define MAPPER_1_RESET_BIT					0x80
#define MAPPER_1_DATA_BIT					0x01
/// CTL reg
#define MAPPER_1_MIRRORING_BIT				0x01
#define MAPPER_1_ONE_SCREEN_BIT				0x02
#define MAPPER_1_PRG_SWITCH_BIT				0x04
#define MAPPER_1_PRG_32_16KB_BIT			0x08
#define MAPPER_1_CHR_4_8KB_BIT				0x10
/// PRG reg
#define MAPPER_1_PRG_MASK					0x0F
#define MAPPER_1_RAM_ENABLE_BIT				0x10
/// CHR reg0/1
#define MAPPER_1_CHR_REG_MASK				0x1F

/// Mapper 2
/// State tag
#define MAPPER_2_TAG_ID						0x4d4f5255

/// Mapper 3
/// State tag
#define MAPPER_3_TAG_ID						0x4d4f5243

/// Mapper 4
/// State tag
#define MAPPER_4_TAG_ID						0x33434d4d
/// Registers mask
#define MAPPER_4_MASK						0xE001
#define MAPPER_4_CTL_ADDR					0x8000
#define MAPPER_4_DATA_ADDR					0x8001
#define MAPPER_4_MIRRORING_ADDR				0xA000
#define MAPPER_4_RAM_CTL_ADDR				0xA001
#define MAPPER_4_IRQ_VALUE_ADDR				0xC000
#define MAPPER_4_IRQ_CLEAR_ADDR				0xC001
#define MAPPER_4_IRQ_ACK_ADDR				0xE000
#define MAPPER_4_IRQ_ENABLE_ADDR			0xE001
/// ctl
#define MAPPER_4_CHR_MODE_SELECT_BIT		0x80
#define MAPPER_4_PRG_MODE_SELECT_BIT		0x40
#define MAPPER_4_ADDRESS_MASK				0x07
/// Mirroing reg
#define MAPPER_4_MIRRORING_BIT				0x01
#define MAPPER_4_MIRRORING_HORZ				0x01
#define MAPPER_4_WRAM_WIRTE_DISABLE_BIT		0x40
/// RAM ctl reg
#define MAPPER_4_WRAM_MASK					0xC0
#define MAPPER_4_WRAM_ENABLE_BIT			0x80

typedef struct Mapper1Regs
{
	int idx;
	uint8_t tmp_reg;
	uint8_t ctl_reg;
	uint8_t chr_reg0;
	uint8_t chr_reg1;
	uint8_t prg_reg;
} Mapper1Regs, *pMapper1Regs;

/// Mapper 2 is simple, it has CHR RAM and only 16K bank switch at $8000.
typedef struct Mapper2Regs
{
	uint8_t rom_bank;
} Mapper2Regs, *pMapper2Regs;

/// Mapper 3 is is simple, it is Fixed ROM banks and only swaps CHR ROMs.
typedef struct Mapper3Regs
{
	uint8_t chr_bank;
} Mapper3Regs, *pMapper3Regs;

typedef struct Mapper4Regs
{
	uint8_t ctl_reg;
	union
	{
		struct
		{
			uint8_t r0_reg;
			uint8_t r1_reg;
			uint8_t r2_reg;
			uint8_t r3_reg;
			uint8_t r4_reg;
			uint8_t r5_reg;
			uint8_t r6_reg;
			uint8_t r7_reg;
		} Raw;
		uint8_t data_reg[8];
	} DataReg;
	uint8_t mirroring_reg;
	uint8_t ram_ctl_reg;
	uint8_t irq_value_reg;
	uint8_t irq_counter;
	int irq_enable_reg;
} Mapper4Regs, *pMapper4Regs;

int mapper_is_supported(int num);
int mapper_has_chr_ram(int num);

int init_mappers(void);
int mapper_write(uint16_t addr, uint8_t value);
int mapper_write_ppu(uint16_t addr, uint8_t value);
uint8_t mapper_read(uint16_t addr);
int mapper_save_state(FILE* state);
int mapper_load_state(FILE* state, const ChunkTag* tag);

int init_mapper1(FILE* image);
int init_mapper4(FILE* image);

#ifdef __cplusplus
}
#endif

#endif

