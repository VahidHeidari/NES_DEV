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

#include "state.h"

#include <string.h>

#include "config.h"
#include "emulator.h"
#include "nes-hdr.h"
#include "debuger.h"
#include "mapper.h"

#define SNAPSHOT_WIDTH		256
#define SNAPSHOT_HEIGHT		240
#define SNAPSHOT_SIZE		(SNAPSHOT_WIDTH * SNAPSHOT_HEIGHT)

char slot[NUM_OF_SLOTS][SLOT_PATH_SIZE];
char bmps[NUM_OF_SLOTS][SLOT_PATH_SIZE];

const StateVersion state_version =
{
	STATE_VERSION_REV,
	STATE_VERSION_MAJ,
	STATE_VERSION_MIN,
	STATE_VERSION_PTH
};

/// Info tags
const ChunkTag state_tag		= { {{'S', 'T', 'A', 'T'}}, 0 };
const ChunkTag version_tag		= { {{'V', 'E', 'R', ' '}}, 4 };
const ChunkTag comment_tag		= { {{'C', 'M', 'N', 'T'}}, 0 };

/// CPU state
const ChunkTag cpu_tag			= { {{'C', 'P', 'U', ' '}}, sizeof(P6502) };
const ChunkTag apu_tag			= { {{'A', 'P', 'U', ' '}}, sizeof(Apu) };
const ChunkTag ram_tag			= { {{'R', 'A', 'M', ' '}}, NOT_MIRRORED_RAM_SIZE };

/// PPU state
const ChunkTag ppu_tag			= { {{'P', 'P', 'U', ' '}}, sizeof(PpuStatus) };
const ChunkTag palette_tag		= { {{'P', 'A', 'L', 'T'}}, PPU_PALETTE_SIZE };
const ChunkTag nta_tag			= { {{'N', 'T', 'A', ' '}}, PPU_NAME_TABLE_SIZE };

/// Joypads state
const ChunkTag joy_tag			= { {{'J', 'O', 'Y', ' '}}, sizeof(Joypad) };

/// Mapper tags
const ChunkTag mapper_tag		= { {{'M', 'A', 'P', 'R'}}, 0 };
const ChunkTag num_tag			= { {{'N', 'U', 'M', ' '}}, 1 };
const ChunkTag pattern_tag		= { {{'P', 'T', 'R', 'N'}}, PPU_PATTERN_TABLE_SIZE };

/// Emulator state
const ChunkTag emu_tag			= { {{'E', 'M', 'U', ' '}}, 0 };
const ChunkTag path_tag			= { {{'P', 'A', 'T', 'H'}}, 0 };
const ChunkTag nes_hdr_tag		= { {{'N', 'E', 'S', 'H'}}, sizeof(NesHeader) };
const ChunkTag mirroring_tag	= { {{'M', 'R', 'O', 'R'}}, 1 };

int State_Init(const char* path)
{
	int i;
	int path_len = strlen(path);

	if (path_len + 8 > SLOT_PATH_SIZE)
		return 0;

	for (i = 0; i < NUM_OF_SLOTS; ++i) {
#if defined _WIN32 && !defined __GNUC__
		sprintf_s(&slot[i][0], SLOT_PATH_SIZE, "%s/SLOT%d.SVT", path, i);
		sprintf_s(&bmps[i][0], SLOT_PATH_SIZE, "%s/SLOT%d.bmp", path, i);
#else
		snprintf(&slot[i][0], SLOT_PATH_SIZE, "%s/SLOT%d.SVT", path, i);
		snprintf(&bmps[i][0], SLOT_PATH_SIZE, "%s/SLOT%d.bmp", path, i);
#endif
	}
	return 1;
}

int State_Close()
{
	return 1;
}

int State_Load(const char* path)
{
	FILE* s;
	int file_size;
	int idx;
	char comment[101];
	ChunkTag st_tag, cr_tag;
	StateVersion ver;

#if defined _WIN32 && !defined __GNUC__
	fopen_s(&s, path, "rb");
#else
	s = fopen(path, "rb");
#endif

	if (!s) {
		DebugMessage("Could not load state from '%s'!", path);
		return 0;
	}

	file_size = State_ReadChunkTag(s, &st_tag) - sizeof(ChunkTag);
	if (st_tag.Sig.id != state_tag.Sig.id) {
		printf("Saved state formate error!");
		return 0;
	}

	while (file_size) {
		int tag_size = State_ReadChunkTag(s, &cr_tag);
		switch (cr_tag.Sig.id) {
			// Info state
			case STATE_VER_TAG_ID:
				if (fread(&ver, sizeof(StateVersion), 1, s) != 1)
					return 0;
				if (ver.rev != STATE_VERSION_REV
						|| ver.maj != STATE_VERSION_MAJ
						|| ver.min != STATE_VERSION_MIN
						|| ver.pth != STATE_VERSION_PTH) {
					DebugMessage("State version is not compatible!");
					DebugMessage("   App ver:%u.%u.%u.%u      state ver:%u.%u.%u.%u"
						, STATE_VERSION_REV
						, STATE_VERSION_MAJ
						, STATE_VERSION_MIN
						, STATE_VERSION_PTH
						, ver.rev, ver.maj, ver.min, ver.pth);
					return 0;
				}
				break;
			case STATE_CMNT_TAG_ID:
#if defined DEBUG_MODE
				idx = tag_size;
				comment[100] = '\0';
				printf("\n====================\n");
				while (idx > (int)sizeof(comment) - 1) {
					if (fread(comment, sizeof(comment) - 1, 1, s) != 1)
						return 0;

					printf("%s", comment);
					idx -= (sizeof(comment) - 1);
				}
				comment[idx] = '\0';
				if (fread(comment, idx, 1, s) != 1)
					return 0;

				printf("%s", comment);
				printf("\n====================\n");
#endif
				break;

			// CPU state
			case STATE_CPU_TAG_ID: if (fread(&p, tag_size, 1, s) != 1) return 0; break;
			case STATE_APU_TAG_ID: if (fread(&apu, tag_size, 1, s) != 1) return 0; break;
			case STATE_RAM_TAG_ID: if (fread(&RAM, tag_size, 1, s) != 1) return 0; break;

			// PPU state
			case STATE_PPU_TAG_ID: if (fread(&ppu, tag_size, 1, s) != 1) return 0; break;
			case STATE_PALT_TAG_ID: if (fread(&palette, tag_size, 1, s) != 1) return 0; break;
			case STATE_NTA_TAG_ID: if (fread(&name_table, tag_size, 1, s) != 1) return 0; break;

			// Joypads state
			case STATE_JOY_TAG_ID: if (fread(&joypad, tag_size, 1, s) != 1) return 0; break;

			// Mapper state
			case STATE_MAPR_TAG_ID:
				tag_size = 0;
				break;				// Continue reading. This is a container tag.
			case STATE_PTRN_TAG_ID:
				if (fread(&pattern_table, tag_size, 1, s) != 1) return 0;
				break;
			case MAPPER_1_TAG_ID:
			case MAPPER_2_TAG_ID:
			case MAPPER_3_TAG_ID:
			case MAPPER_4_TAG_ID:
				if (Mapper_LoadState(s, &cr_tag) != 1) {
					DebugMessage("Could not load mapper state!");
					return 0;
				}
				break;

			// Emulator stete
			case STATE_EMU_TAG_ID:
				tag_size = 0;
				break;				// Continue reading. This is a container tag.

			case STATE_NESH_TAG_ID:
			case STATE_NUM_TAG_ID:
			case STATE_MROR_TAG_ID:
			case STATE_PATH_TAG_ID:
				fseek(s, tag_size, SEEK_CUR);
				break;				// Skip silently unused tags.

			default:
				DebugMessage("Unknown tag!     id: 0x%08x     name: '%c%c%c%c'", cr_tag.Sig.id
					, cr_tag.Sig.name[0]
					, cr_tag.Sig.name[1]
					, cr_tag.Sig.name[2]
					, cr_tag.Sig.name[3]);
				fseek(s, tag_size, SEEK_CUR);
				break;
		}
		file_size -= tag_size + sizeof(ChunkTag);
	}

	fclose(s);
	DebugMessage("State loaded from  <--  '%s'", path);
	return 1;
}

int State_Save(const char* path)
{
	FILE* s;
	char mapper_num;
	int state_size
		, emu_size, emu_offset
		, mapper_size, mapper_offset;
	ChunkTag tmp_path_tag;
	char* comment;
	ChunkTag tmp_comment_tag;

#if defined _WIN32 && !defined __GNUC__
	fopen_s(&s, path, "wb");
#else
	s = fopen(path, "wb");
#endif

	if (!s) {
		DebugMessage("Could not save state to '%s'!", path);
		return 0;
	}

	// Info tags
	state_size  = State_WriteChunkTag(s, &state_tag, 0);
	state_size += State_WriteChunkTag(s, &version_tag, &state_version);
	tmp_comment_tag = comment_tag;

	// Add your comments here.
	comment =
		"\tThis is a test comment.\n"
		"\tMy name is Vahid Heidari (DeltaCode).\n"
		"\tThis is my file format for saveing and loading state of NES games.\n"
		"\tThis is my multi-platform NES (Nintendo Entertainment System) emulator.\n"
		"\tYou can play it in Windows, Linux, or even STM32 ARM Cortex-M3 platfroms!\n"
		"\tFill free to enjoy playing with my emulator. :)\n"
		"\tYou can use comments for archaving your saved states, or adding every "
				"thing you want here.";

	tmp_comment_tag.len = strlen(comment);
	state_size += State_WriteChunkTag(s, &tmp_comment_tag, comment);

	// CPU tags
	state_size += State_WriteChunkTag(s, &cpu_tag, &p);
	state_size += State_WriteChunkTag(s, &apu_tag, &apu);
	state_size += State_WriteChunkTag(s, &ram_tag, &RAM);

	// PPU tags
	state_size += State_WriteChunkTag(s, &ppu_tag, &ppu);
	state_size += State_WriteChunkTag(s, &palette_tag, &palette);
	state_size += State_WriteChunkTag(s, &nta_tag, &name_table);

	// Joypads tag
	state_size += State_WriteChunkTag(s, &joy_tag, &joypad);

	// Mapper tag
	mapper_offset = state_size;				// Store mapper_tag offset.
	mapper_num = GET_MAPPER(&hdr);
	if (Mapper_IsSupported(mapper_num)) {
		state_size += State_WriteChunkTag(s, &mapper_tag, NULL);
		mapper_size = State_WriteChunkTag(s, &num_tag, &mapper_num);
		if (Mapper_HasChrRAM(mapper_num))
			mapper_size += State_WriteChunkTag(s, &pattern_tag, &pattern_table);
		mapper_size += Mapper_SaveState(s);
	} else {
		DebugMessage("Mapper not supported!");
		return 0;
	}
	state_size += mapper_size;

	// Emulator tags
	emu_offset = state_size;				// Store emu_tag offset.
	state_size += State_WriteChunkTag(s, &emu_tag, NULL);	// Add emu_tag size to total size
	tmp_path_tag = path_tag;				// Fill tmp path tag
	tmp_path_tag.len = strlen(image_path);
	emu_size = State_WriteChunkTag(s, &tmp_path_tag, image_path);
	emu_size += State_WriteChunkTag(s, &nes_hdr_tag, &hdr);
	emu_size += State_WriteChunkTag(s, &mirroring_tag, &mirroring);
	state_size += emu_size;

	fseek(s, 4, SEEK_SET); fwrite(&state_size, 4, 1, s);			// write state_tag size
	fseek(s, mapper_offset+4, SEEK_SET); fwrite(&mapper_size, 4, 1, s);// Write mapper_tag size
	fseek(s, emu_offset + 4, SEEK_SET); fwrite(&emu_size, 4, 1, s);	// Write emu_tag size

	fclose(s);
	DebugMessage("State saved to -> '%s'", path);
	return 1;
}

static int SaveSnapshot(const char* path)
{
#if defined _WIN32 || defined __linux__
	FILE* f;
	int x, y;
	BitmapHdr bmp;

#if _WIN32
	fopen_s(&f, path, "wb");
#else
	f = fopen(path, "wb");
#endif
	if (!f)
		return 0;

	// Initialize file header.
	memset(&bmp, 0, sizeof(BitmapHdr));
	bmp.sig = 0x4D42;
	bmp.file_size = sizeof(BitmapHdr) + SNAPSHOT_SIZE;
	bmp.data_offset = sizeof(BitmapHdr);
	// Initialize bitmap info header.
	bmp.info_size = 40;
	bmp.width = SNAPSHOT_WIDTH;
	bmp.height = SNAPSHOT_HEIGHT;
	bmp.planes = 1;
	bmp.bit_count = 24;
	// Write header.
	fwrite(&bmp, sizeof(BitmapHdr), 1, f);
	// Write pixels.
	for (y = SNAPSHOT_HEIGHT - 1; y >= 0; --y) {
		for (x = 0; x < SNAPSHOT_WIDTH; ++x) {
			int idx = SNAPSHOT_WIDTH * y + x;
			uint32_t color = ((uint32_t*)(surface->pixels))[idx];
			fwrite(&color, 1, 1, f);		// Write red component.
			color >>= 8;
			fwrite(&color, 1, 1, f);		// Write green component.
			color >>= 8;
			fwrite(&color, 1, 1, f);		// Write blue component.
		}
	}
	fclose(f);
	DebugMessage("Snapshot saved to -> '%s'", path);
#endif
	return 1;
}

int State_SaveSlot(int i)
{
	if (i > NUM_OF_SLOTS || i < 0) {
		DebugMessage("Could not save to slot #%d! Invalid rang.", i);
		return 0;
	}

	if (State_Save(&(slot[i][0])) != 1) {
		DebugMessage("Could not save state to slot #%d! Context save error.", i);
		return 0;
	}

	if (SaveSnapshot(&(bmps[i][0])) != 1) {
		DebugMessage("Could not save snapshot to slot #%d! Bitmap save error.", i);
		return 0;
	}

	return 1;
}

int State_LoadSlot(int i)
{
	if (i > NUM_OF_SLOTS || i < 0) {
		DebugMessage("Could not load from slot #%d!", i);
		return 0;
	}

	if (State_Load(&(slot[i][0])) != 1) {
		DebugMessage("Could not load state from slot #%d!", i);
		return 0;
	}

	return 1;
}

int State_WriteChunkTag(FILE* state, const ChunkTag* chunk, const void* data)
{
	size_t no_bytes;

	no_bytes  = fwrite(&(chunk->Sig.id), 4, 1, state) * 4;
	no_bytes += fwrite(&(chunk->len), 4, 1, state) * 4;

	if (data)
		no_bytes += fwrite(data, chunk->len, 1, state) * chunk->len;

	return no_bytes;
}

int State_ReadChunkTag(FILE* state, pChunkTag chunk)
{
	if (fread(chunk, sizeof(ChunkTag), 1, state) != 1)
		return 0;

	return chunk->len;
}
