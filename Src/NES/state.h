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

#ifndef STATE_H_
#define STATE_H_

#include <stdio.h>
#include <stdint.h>

#if defined __cplusplus
extern "C" {
#endif

#define NUM_OF_SLOTS			4
#define SLOT_PATH_SIZE			300

/// Version
#define STATE_VERSION_REV		0x00
#define STATE_VERSION_MAJ		0x01
#define STATE_VERSION_MIN		0x00
#define STATE_VERSION_PTH		0x00

/// Info tag id
#define STATE_STAT_TAG_ID		0x54415453
#define STATE_VER_TAG_ID		0x20524556
#define STATE_CMNT_TAG_ID		0x544e4d43

/// Snapshot tag id
#define STATE_SNAPSHOT_TAG_ID	0x50414e53

/// CPU tag id
#define STATE_CPU_TAG_ID		0x20555043
#define STATE_APU_TAG_ID		0x20555041
#define STATE_RAM_TAG_ID		0x204d4152

/// PPU tag id
#define STATE_PPU_TAG_ID		0x20555050
#define STATE_PALT_TAG_ID		0x544c4150
#define STATE_NTA_TAG_ID		0x2041544e
#define STATE_PTRN_TAG_ID		0x4e525450

/// Joypads tag id
#define STATE_JOY_TAG_ID		0x20594f4a

/// Mappers tag id
#define STATE_MAPR_TAG_ID		0x5250414d
#define STATE_NUM_TAG_ID		0x204d554e

/// Emulator tag id
#define STATE_EMU_TAG_ID		0x20554d45
#define STATE_PATH_TAG_ID		0x48544150
#define STATE_NESH_TAG_ID		0x4853454e
#define STATE_MROR_TAG_ID		0x524f524d

#if defined __GNUC__
#define ATTRIB_PACKED(STRU) __attribute__((__packed__)) STRU
#else
#define ATTRIB_PACKED(STRU)	STRU
#endif

#define STR_TO_ID(str)			((uint32_t)str[0] << 24	\
	| (uint32_t)str[1] << 16 | (uint32_t)str[2] << 8 | (uint32_t)str[3])
#define CHR_TO_ID(a, b, c, d)	((uint32_t)a << 24	\
	| (uint32_t)b << 16 | (uint32_t)c << 8 | (uint32_t)d)

extern char slot[NUM_OF_SLOTS][SLOT_PATH_SIZE];

#if _WIN32
#pragma pack(push, 1)
#endif
typedef struct BitmapHdr
{
	/// Bitmap file header
	uint16_t sig;
	uint32_t file_size;
	uint32_t reserved;
	uint32_t data_offset;

	/// Bitmap Header Info header
	uint32_t info_size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bit_count;
	uint32_t compression;
	uint32_t image_size;
	uint32_t x_pels_per_meter;
	uint32_t y_pels_per_meter;
	uint32_t clr_used;
	uint32_t clr_important;
} ATTRIB_PACKED(BitmapHdr), *pBitmapHdr;
#if _WIN32
#pragma pack(pop)
#endif

typedef struct ChunkTag
{
	union
	{
		char name[4];
		uint32_t id;
	} Sig;
	uint32_t len;
} ChunkTag, *pChunkTag;

typedef struct StateVersion
{
	uint8_t rev;
	uint8_t maj;
	uint8_t min;
	uint8_t pth;
} StateVersion, *pStateVersion;

extern const StateVersion state_version;

/// Info tags
extern const ChunkTag state_tag;
extern const ChunkTag version_tag;
extern const ChunkTag comment_tag;

/// CPU state
extern const ChunkTag cpu_tag;
extern const ChunkTag apu_tag;
extern const ChunkTag ram_tag;

/// PPU state
extern const ChunkTag ppu_tag;
extern const ChunkTag palette_tag;
extern const ChunkTag nta_tag;
extern const ChunkTag pattern_tag;

/// Joypads state
extern const ChunkTag joy_tag;

/// Emulator state
extern const ChunkTag emu_tag;
extern const ChunkTag path_tag;
extern const ChunkTag nes_hdr_tag;
extern const ChunkTag mirroring_tag;

int State_Init(const char* path);
int State_Close(void);
int State_Load(const char* path);
int State_Save(const char* path);
int State_SaveSlot(int i);
int State_LoadSlot(int i);

int State_WriteChunkTag(FILE* state, const ChunkTag* chunk, const void* data);
int State_ReadChunkTag(FILE* state, pChunkTag chunk);

#if defined __cplusplus
}
#endif

#endif

