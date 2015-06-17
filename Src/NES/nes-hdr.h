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

#ifndef NES_HDR_H_
#define NES_HDR_H_

#include <stdint.h>

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/// String for indicating file format
#define NES_STR					"NES"

/// Magic number for indicating file format
#define MAGIC_NUMBER			0x1A

/// ROM Control Byte 1 offset bits
#define MIRRORING				0x01		/// Bit 0
#define BATTERY_BACKED_RAM		0x02		/// Bit 1
#define MEMORY_TRAINER			0x04		/// Bit 2
#define MIRRORING_OVERRIDE		0x08		/// Bit 3
#define FOUR_SCREEN				0x08		/// Bit 3 other name
#define MEMORY_MAPPER_LOW_BYTE	0xF0		/// Bits 4-7

/// ROM Control Byte 2
#define RESERVED				0x0F		/// Bits 0-3
#define MEMORY_MAPPER_HIGH_BYTE	0xF0		/// Bits 4-7

/// Utility macros
#define GET_NUM_OF_ROM_BANKS(pHdr)		((pHdr)->num_of_rom_banks)
#define GET_NUM_OF_VROM_BANKS(pHdr)		((pHdr)->num_of_vrom_banks)
#define GET_MAPPER(pHdr)				((((pHdr)->rom_control_1 & MEMORY_MAPPER_LOW_BYTE) >> 4)	\
		| ((pHdr)->rom_control_2 & MEMORY_MAPPER_HIGH_BYTE))
#define GET_MIRRORING_TYPE(pHdr)		(((pHdr)->rom_control_1 & MIRRORING) ?						\
												MIRRORING_VERTICAL : MIRRORING_HORIZONTAL)
#define IS_BATTRY_BACKED(pHdr)			((pHdr)->rom_control_1 & BATTERY_BACKED_RAM)
#define IS_MIRRORING_OVERRIDE(pHdr)		((pHdr)->rom_control_1 & MIRRORING_OVERRIDE)
#define IS_TRAINER(pHdr)				((pHdr)->rom_control_1 & MEMORY_TRAINER)

typedef enum Mirroring
{
	MIRRORING_NONE = 0,
	MIRRORING_VERTICAL,
	MIRRORING_HORIZONTAL,
	MIRRORING_OVERRIDEN,
	MIRRORING_ONE_SCREEN,
	MIRRORING_FOUR_SCREEN
} Mirroring;

typedef struct nes_hdr
{
	uint8_t nes_str[3];
	uint8_t magic_number;		/// NES constant header

	uint8_t num_of_rom_banks;	/// Size of PROG ROM in 16KB
	uint8_t num_of_vrom_banks;	/// Size of CHR ROM in 8KB (0 = CHAE RAM) 

	uint8_t rom_control_1;		/// Low byte of mapper and some flags
	uint8_t rom_control_2;		/// High byte of mapper
	uint8_t num_of_prg_ram;		/// Size of PRG RAM in 8KB(0 = 8KB)
	uint8_t tv_system;			/// TV system
	uint8_t flags_10;			/// (unofficial flag) TV system & RAM in $6000-$7FFF & bus conflicts

	uint8_t reserved[5];		/// Reserved for future expantion
} NesHeader, *pNesHeader;

void nes_hdr_info(pNesHeader hdr);

#ifdef __cplusplus
}
#endif

#endif

