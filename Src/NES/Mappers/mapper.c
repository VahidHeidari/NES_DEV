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

#include "mapper.h"

#include <string.h>

#include "config.h"
#include "emulator.h"
#include "bank-switch.h"

/// Reset signal affects ctl_reg
#define MAPPER_1_RESET_TMP(mapper, image) do {		\
	(mapper)->idx = 0;								\
	(mapper)->tmp_reg = 0;							\
	(mapper)->ctl_reg |= 0x0C;						\
	Mapper1ROMSwitch((mapper), image);				\
} while (0)

Mapper1Regs mapper1;
Mapper2Regs mapper2;
Mapper3Regs mapper3;
Mapper4Regs mapper4;

const ChunkTag m1_tag = { {{'M', 'M', 'C', '1'}}, sizeof(Mapper1Regs) };
const ChunkTag m2_tag = { {{'U', 'R', 'O', 'M'}}, sizeof(Mapper2Regs) };
const ChunkTag m3_tag = { {{'C', 'R', 'O', 'M'}}, sizeof(Mapper3Regs) };
const ChunkTag m4_tag = { {{'M', 'M', 'C', '3'}}, sizeof(Mapper4Regs) };

int Mapper_IsSupported(int num)
{
	switch (num) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			return 1;

		default:
			break;
	}
	return 0;
}

int Mapper_HasChrRAM(int num)
{
	switch (num) {
		case 2:
			return 1;

		default:
			break;
	}
	return 0;
}

int Mapper_Init(void)
{
	return 1;
}

static __inline void Mapper1ROMSwitch(pMapper1Regs mapper, FILE* image)
{
	if ((mapper->ctl_reg & MAPPER_1_PRG_32_16KB_BIT) == 0)
		BankSwitch32K(((mapper->prg_reg & MAPPER_1_PRG_MASK) >> 1), ROM, image);
	else if (mapper->ctl_reg & MAPPER_1_PRG_SWITCH_BIT) {
		// 16K swap
		BankSwitch16K(GET_NUM_OF_ROM_BANKS(&hdr) - 1, ROM + (ROM_SIZE / 2), image);
		BankSwitch16K(mapper->prg_reg & MAPPER_1_PRG_MASK, ROM, image);
	} else {
		// 16K swap
		BankSwitch16K(0, ROM, image);
		BankSwitch16K(mapper->prg_reg & MAPPER_1_PRG_MASK, ROM + (ROM_SIZE / 2), image);
	}
}

static __inline void Mapper1ChrSwitch(pMapper1Regs mapper, FILE* image)
{
	if (mapper->ctl_reg & MAPPER_1_CHR_4_8KB_BIT) {
		BankSwitch4K((GET_NUM_OF_ROM_BANKS(&hdr) * 4) + mapper->chr_reg0, pattern_table, image);
		BankSwitch4K((GET_NUM_OF_ROM_BANKS(&hdr) * 4) + mapper->chr_reg1, pattern_table + (PPU_PATTERN_TABLE_SIZE / 2), image);
	} else
		BankSwitch8K((GET_NUM_OF_ROM_BANKS(&hdr) * 2) + mapper->chr_reg0, pattern_table, image);		// Each 16KB ROMS has 2 8KB
}

static __inline int Mapper1Write(pMapper1Regs mapper, uint16_t addr, uint8_t value)
{
	if (value & MAPPER_1_RESET_BIT)
		MAPPER_1_RESET_TMP(mapper, image);			// Reset Signal
	else {
		mapper->tmp_reg >>= 1;
		mapper->tmp_reg |= (value & MAPPER_1_DATA_BIT ? 0x10 : 0);

		if (++mapper->idx == 5) {
			if (addr < 0xA000) {									// reg0
				mapper->ctl_reg = mapper->tmp_reg;

				// Mirroring bits check.
				if ((mapper->ctl_reg & MAPPER_1_ONE_SCREEN_BIT) == 0)
					mirroring = MIRRORING_ONE_SCREEN;
				else if (mapper->ctl_reg & MAPPER_1_MIRRORING_BIT)
					mirroring = MIRRORING_HORIZONTAL;
				else
					mirroring = MIRRORING_VERTICAL;
			} else if (addr < 0xC000)								// reg1
				mapper->chr_reg0 = mapper->tmp_reg;
			else if (addr < 0xE000)									// reg2
				mapper->chr_reg1 = mapper->tmp_reg;
			else													// reg3
				mapper->prg_reg = mapper->tmp_reg;

			Mapper1ROMSwitch(mapper, image);
			Mapper1ChrSwitch(mapper, image);
			mapper->idx = mapper->tmp_reg = 0;					// Reset idx and tmp.
		}
	}
	return 1;
}

static __inline void Mapper4ROMSwitch(pMapper4Regs mapper, FILE* image)
{
	if (mapper->ctl_reg & MAPPER_4_PRG_MODE_SELECT_BIT) {
		BankSwitch8K((GET_NUM_OF_ROM_BANKS(&hdr) * 2) - 2, ROM, image);
		BankSwitch8K(mapper->DataReg.Raw.r7_reg, ROM + BANK_SIZE_8K, image);
		BankSwitch8K(mapper->DataReg.Raw.r6_reg, ROM + BANK_SIZE_16K, image);
	} else {
		BankSwitch8K(mapper->DataReg.Raw.r6_reg, ROM, image);
		BankSwitch8K(mapper->DataReg.Raw.r7_reg, ROM + BANK_SIZE_8K, image);
		BankSwitch8K((GET_NUM_OF_ROM_BANKS(&hdr) * 2) - 2, ROM + BANK_SIZE_16K, image);
	}
}

static __inline void Mapper4ChrSwitch(pMapper4Regs mapper, FILE* image)
{
	int roms_1k = GET_NUM_OF_ROM_BANKS(&hdr) * 16;
	int roms_2k = GET_NUM_OF_ROM_BANKS(&hdr) * 8;
	if (mapper->ctl_reg & MAPPER_4_CHR_MODE_SELECT_BIT) {
		BankSwitch1K(roms_1k + mapper->DataReg.Raw.r2_reg, pattern_table, image);
		BankSwitch1K(roms_1k + mapper->DataReg.Raw.r3_reg, pattern_table + 0x0400, image);
		BankSwitch1K(roms_1k + mapper->DataReg.Raw.r4_reg, pattern_table + 0x0800, image);
		BankSwitch1K(roms_1k + mapper->DataReg.Raw.r5_reg, pattern_table + 0x0C00, image);
		BankSwitch2K(roms_2k + mapper->DataReg.Raw.r0_reg / 2, pattern_table + 0x1000, image);
		BankSwitch2K(roms_2k + mapper->DataReg.Raw.r1_reg / 2, pattern_table + 0x1800, image);
	} else {
		BankSwitch2K(roms_2k + mapper->DataReg.Raw.r0_reg / 2, pattern_table, image);
		BankSwitch2K(roms_2k + mapper->DataReg.Raw.r1_reg / 2, pattern_table + 0x0800, image);
		BankSwitch1K(roms_1k + mapper->DataReg.Raw.r2_reg, pattern_table + 0x1000, image);
		BankSwitch1K(roms_1k + mapper->DataReg.Raw.r3_reg, pattern_table + 0x1400, image);
		BankSwitch1K(roms_1k + mapper->DataReg.Raw.r4_reg, pattern_table + 0x1800, image);
		BankSwitch1K(roms_1k + mapper->DataReg.Raw.r5_reg, pattern_table + 0x1C00, image);
	}

	PPU_DebugNameTable(&ppu, 0x00FFFFFF);
	PPU_DebugPatternTable(&ppu);

	// Mapper4 debug
	//roms_1k = 1;
	//roms_2k = 9;
	//while (roms_2k--)
	//	while (roms_1k--)
	//		continue;
}

static __inline int Mapper4Write(pMapper4Regs mapper, uint16_t addr, uint8_t value)
{
	switch (addr & MAPPER_4_MASK) {
		case MAPPER_4_CTL_ADDR:
			// CHR switching
			if ((mapper->ctl_reg & MAPPER_4_CHR_MODE_SELECT_BIT) != (value & MAPPER_4_CHR_MODE_SELECT_BIT))
				Mapper4ChrSwitch(&mapper4, image);
			// PRG switching
			if ((mapper->ctl_reg & MAPPER_4_PRG_MODE_SELECT_BIT) != (value & MAPPER_4_PRG_MODE_SELECT_BIT))
				Mapper4ROMSwitch(&mapper4, image);
			mapper->ctl_reg = value;
			break;

		case MAPPER_4_DATA_ADDR:
			if (value != mapper->DataReg.data_reg[mapper->ctl_reg & MAPPER_4_ADDRESS_MASK]) {
				mapper->DataReg.data_reg[mapper->ctl_reg & MAPPER_4_ADDRESS_MASK] = value;
				if ((mapper->ctl_reg & MAPPER_4_ADDRESS_MASK) == 6
						|| (mapper->ctl_reg & MAPPER_4_ADDRESS_MASK) == 7)
					Mapper4ROMSwitch(&mapper4, image);
				else
					Mapper4ChrSwitch(&mapper4, image);
			}
			break;

		case MAPPER_4_MIRRORING_ADDR:
			mapper->mirroring_reg = value & MAPPER_4_MIRRORING_BIT;
			if (mirroring != MIRRORING_FOUR_SCREEN)
				mirroring = value & MAPPER_4_MIRRORING_HORZ ? MIRRORING_HORIZONTAL : MIRRORING_VERTICAL;
			break;

		case MAPPER_4_RAM_CTL_ADDR:
			mapper->ram_ctl_reg = value & MAPPER_4_WRAM_MASK;
			break;

		case MAPPER_4_IRQ_VALUE_ADDR:
			mapper->irq_value_reg = value;
			break;

		case MAPPER_4_IRQ_CLEAR_ADDR:
			mapper->irq_counter = 0;
			break;

		case MAPPER_4_IRQ_ACK_ADDR:
			mapper->irq_enable_reg = 0;
			p.interrupts = INTR_NONE;		// Disable IRQ counter.
			break;

		case MAPPER_4_IRQ_ENABLE_ADDR:
			mapper->irq_enable_reg = 1;		// Enable IRQ counter.
			break;
	}
	return 1;
}

static void Mapper4ScanlineStart(int scanline)
{
	(void)scanline;		// Not used.

	if (mapper4.irq_counter == 0)
		mapper4.irq_counter = mapper4.irq_value_reg;		// Reload IRQ counter.
	else {
		--mapper4.irq_counter;								// Decrement IRQ counter.
		if (mapper4.irq_counter == 0 && mapper4.irq_enable_reg)
			p.interrupts = INTR_IRQ;
	}
}

static void Mapper4RenderingStart(void)
{
	Mapper4ScanlineStart(0);
}

int Mapper_Write(uint16_t addr, uint8_t value)
{
	switch (GET_MAPPER(&hdr)) {
		case 1:
			return Mapper1Write(&mapper1, addr, value);

		case 2:
			mapper2.rom_bank = value & 0x0F;
			BankSwitch16K(mapper2.rom_bank, ROM, image);
			return 1;

		case 3:
			mapper3.chr_bank = value;
			BankSwitch8K(2 * GET_NUM_OF_ROM_BANKS(&hdr) + value, pattern_table, image);
			return 1;

		case 4:
			return Mapper4Write(&mapper4, addr, value);

		case 0:
		default:
			break;
	}

	return 0;		// Not implemented mapper.
}

int Mapper_WritePPU(uint16_t addr, uint8_t value)
{
	(void)addr;
	if (GET_MAPPER(&hdr) == 2) {
		pattern_table[ppu.v] = value;
		return 1;
	}

	return 0;		// Not implemented mapper.
}

uint8_t Mapper_Read(uint16_t addr)
{
	(void)addr;
	return 0;
}

int Mapper_SaveState(FILE* state)
{
	int mapper_no = GET_MAPPER(&hdr);

	if (Mapper_IsSupported(mapper_no) != 1)
		return 0;

	switch (mapper_no)
	{
		case 1: return State_WriteChunkTag(state, &m1_tag, &mapper1);
		case 2: return State_WriteChunkTag(state, &m2_tag, &mapper2);
		case 3: return State_WriteChunkTag(state, &m3_tag, &mapper3);
		case 4: return State_WriteChunkTag(state, &m4_tag, &mapper4);

		default:
			break;
	}
	return 0;
}

int Mapper_LoadState(FILE* state, const ChunkTag* tag)
{
	switch (tag->Sig.id) {
		case MAPPER_1_TAG_ID:
			if (fread(&mapper1, sizeof(Mapper1Regs), 1, state) != 1)
				return 0;

			Mapper1ROMSwitch(&mapper1, image);
			Mapper1ChrSwitch(&mapper1, image);
			break;

		case MAPPER_2_TAG_ID:
			if (fread(&mapper2, sizeof(Mapper2Regs), 1, state) != 1)
				return 0;

			BankSwitch16K(mapper2.rom_bank, ROM, image);
			break;

		case MAPPER_3_TAG_ID:
			if (fread(&mapper3, sizeof(Mapper3Regs), 1, state) != 1)
				return 0;

			break;

		case MAPPER_4_TAG_ID:
			if (fread(&mapper4, sizeof(Mapper4Regs), 1, state) != 1)
				return 0;

			Mapper4ROMSwitch(&mapper4, image);
			Mapper4ChrSwitch(&mapper4, image);
			break;

		default:
			return 0;
	}
	return 1;
}

int InitMapper1(FILE* image)
{
	memset(&mapper1, 0, sizeof(Mapper1Regs));
	MAPPER_1_RESET_TMP(&mapper1, image);
	return 1;
}

int InitMapper4(FILE* image)
{
	// Reset struct.
	memset(&mapper4, 0, sizeof(Mapper4Regs));

	// Last PRG bank is fixed at $C000.
	BankSwitch8K(GET_NUM_OF_ROM_BANKS(&hdr) * 2 - 1, ROM + BANK_SIZE_8K * 3, image);

	// We can delay this switching to mapper first initialization, but I did it that now.
	Mapper4ROMSwitch(&mapper4, image);
	Mapper4ChrSwitch(&mapper4, image);

	// Set callback for IRQ counter.
	scanline_start = Mapper4ScanlineStart;
	rendering_start = Mapper4RenderingStart;		// Dummy line rendering
	return 1;
}

