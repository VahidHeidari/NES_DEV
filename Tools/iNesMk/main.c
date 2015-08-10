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

#include <string.h>
#include <stdio.h>

#include "config.h"
#include "console.h"
#include "nes-hdr.h"

#define NUMBER_OF_ITEMS		8
#define ROM_INDEX			0
#define VROM_INDEX			1
#define MAPPER_INDEX		2
#define MIRRORING_INDEX		3
#define BATTERY_INDEX		4
#define MIRRORING_OVR_INDEX	5
#define TRAINER_INDEX		6
#define FILE_NAME_INDEX		7

//#define SIMPLE_ITEM			1

#define FILE_NAME_BUFF_SIZE	512

typedef enum
{
	INTEGER,
	STRING,
	YES_NO,
} ItemType;

typedef struct Item
{
	char* title;

#ifdef SIMPLE_ITEM
	int value;
#else
	ItemType type;

	union {
		int integer;
		char* string;
		int boolean;
	} value;
#endif
} Item, *pItem;

int selected_item = 0;

char file_name_buff[FILE_NAME_BUFF_SIZE];

Item items[NUMBER_OF_ITEMS] =
{
	{" Number of 16KB ROM(s)      : ", INTEGER, {1} },
	{" Number of 8KB VROM(s)      : ", INTEGER, {1} },
	{" Mapper number              : ", INTEGER, {0} },
	{" Mirroring                  : ", STRING, { .string = "Vertical" } },
	{" Battery packed             : ", YES_NO, {0} },
	{" Mirroring overriden        : ", YES_NO, {0} },
	{" Trainer                    : ", YES_NO, {0} },
	{" File name                  : ", STRING, { .string = "Header.bin" } }
};

void print_items()
{
	int i;

	clear_screen();

	gotoxy(0, 0);

	for (i = 0; i < NUMBER_OF_ITEMS; ++i)
	{
		if (i == selected_item)
			set_black_on_white();

		printf("%s", items[i].title);
#if defined SIMPLE_ITEM
		printf("%d\n", items[i].value);
#else
		switch (items[i].type) {
			case INTEGER:
				printf("%d", items[i].value.integer);
				break;

			case STRING:
				printf("%s", items[i].value.string);
				break;

			case YES_NO:
				printf("%s", (char*)(items[i].value.boolean ? "YES" : "NO"));
				break;

			default:
				break;
		}
		putchar('\n');
#endif

		if (i == selected_item)
			set_white_on_black();
	}
}

void fill_nes_hder(const Item* items, NesHeader* hdr)
{
	// Filling hdr with configured values from user.
	// Magic number
	hdr->nes_str[0] = 'N';
	hdr->nes_str[1] = 'E';
	hdr->nes_str[2] = 'S';
	hdr->magic_number = MAGIC_NUMBER;

	// PRG ROM and CHR ROM numbers
	hdr->num_of_rom_banks = items[ROM_INDEX].value.integer;
	hdr->num_of_vrom_banks = items[VROM_INDEX].value.integer;

	// Mapper number
	hdr->rom_control_1 = items[MAPPER_INDEX].value.integer << 4;
	hdr->rom_control_2 = (items[MAPPER_INDEX].value.integer >> 4) & 0x0F;

	// Mirroring
	hdr->rom_control_1 |= (strcmp(items[MIRRORING_INDEX].value.string , "Vertical") == 0) ? MIRRORING : 0;

	// Battery packed
	hdr->rom_control_1 |= items[BATTERY_INDEX].value.boolean ? BATTERY_BACKED_RAM : 0;

	// Trainer
	hdr->rom_control_1 |= items[TRAINER_INDEX].value.boolean ? MEMORY_TRAINER : 0;

	// Mirroring overriden
	hdr->rom_control_1 |= items[MIRRORING_OVR_INDEX].value.boolean ? MIRRORING_OVERRIDE : 0;
}

int main()
{
	NesHeader hdr;
	int key;
	int file_name_idx;
	int finished = 0;
	FILE* output = NULL;

	memset(&hdr, 0, sizeof(hdr));
	strncpy(file_name_buff, items[FILE_NAME_INDEX].value.string, FILE_NAME_BUFF_SIZE);
	file_name_idx = strlen(items[FILE_NAME_INDEX].value.string);
	items[FILE_NAME_INDEX].value.string = file_name_buff;
	
	print_items();

	// Read input and configure settings.
	while (!finished) {
		key = getch();

		switch (key) {

			case 'q':
			case 'Q':
				finished = 1;
				break;

			case ESCAPE_KEY: {
					if (feof(stdin)) {
						finished = 1;
						break;
					}

					if ((key = getch()) != '[') {
						finished = 1;
						break;
					}

					key = getch();

					switch (key) {

						case ENTER_KEY:
						case UP_KEY:
							--selected_item;
							selected_item = (selected_item < 0) ? NUMBER_OF_ITEMS - 1 : selected_item;
							break;

						case DOWN_KEY:
							++selected_item;
							selected_item %= NUMBER_OF_ITEMS;
							break;

						case RIGHT_KEY:
							if (selected_item == MIRRORING_INDEX) {
								if (strcmp(items[selected_item].value.string, "Vertical") == 0)
									items[selected_item].value.string = "Horizontal";
								else
									items[selected_item].value.string = "Vertical";
								break;
							}

							switch (items[selected_item].type) {
								case INTEGER: ++items[selected_item].value.integer; break;
								case YES_NO: items[selected_item].value.boolean ^= 1; break;
								default: break;
							}
							break;

						case LEFT_KEY:
							if (selected_item == MIRRORING_INDEX) {
								if (strcmp(items[selected_item].value.string, "Vertical") == 0)
									items[selected_item].value.string = "Horizontal";
								else
									items[selected_item].value.string = "Vertical";
								break;
							}

							switch (items[selected_item].type) {
								case INTEGER: --items[selected_item].value.integer; break;
								case YES_NO: items[selected_item].value.boolean ^= 1; break;
								default: break;
							}
							break;
					}

					break;
				}

			default:
				if (selected_item == FILE_NAME_INDEX)
				{
					while (!feof(stdin))
					{
						if (key == BACK_SPACE_KEY || key == DELETE_KEY) {
							if (file_name_idx < 0)
								file_name_buff[--file_name_idx] = 0;
						} else {
							if (key != ENTER_KEY && file_name_idx < FILE_NAME_BUFF_SIZE - 2)
								file_name_buff[file_name_idx++] = key;
						}

						if ((key = getch()) == ENTER_KEY)
							break;
					}

					file_name_buff[file_name_idx] = 0;
					items[FILE_NAME_INDEX].value.string = file_name_buff;
				}
				break;
		}

		print_items();
	}

#if defined _WIN32 && !defined __GNUC__
	fopen_s(&output, items[FILE_NAME_INDEX].value.string, "wb");
#else
	output = fopen(items[FILE_NAME_INDEX].value.string, "wb");
#endif

	// Dump it down to file.
	if (output == NULL)
	{
		fprintf(stderr, "Could not create file!\n");
		return 1;
	}

	fill_nes_hder(items, &hdr);

	if (fwrite(&hdr, sizeof(NesHeader), 1, output) != 1)
	{
		fprintf(stderr, "Could not write to output file!\n");
		return 1;
	}

	fclose(output);

	return 0;
}

