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
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include "nes-hdr.h"
#include "mappers-name.h"

#define ERROR_EXIT_FMT(STR, args...) do {	\
	fprintf(stderr, STR "\n", args);		\
	return 1;								\
} while (0)

#define ERROR_EXIT(STR) do {	\
	fprintf(stderr, STR "\n");	\
	return 1;					\
} while (0)

int main(int argc, char** argv)
{
	int i;
	uint8_t buffer[8 * 1024];		// Pattern table buffer
	NesHeader hdr;
	FILE* ines = NULL;
	FILE* output = NULL;
	size_t file_size = 0;
	size_t rom_size = 0;
	size_t chr_size = 0;

	if (argc < 3)
		ERROR_EXIT("File name required!\n"
				" Usage : ines-dpt input-file output-file\n");

	if (!(ines = fopen(argv[1], "rb")))
		ERROR_EXIT_FMT("Could not open '%s' File!", argv[1]);

	fseek(ines, 0, SEEK_END);
	file_size = ftell(ines);
    rewind(ines);

	if (file_size < sizeof(hdr))
		ERROR_EXIT("NES header is corrupted!");

	fread(&hdr, sizeof(hdr), 1, ines);
	if (!nes_hdr_is_valid(&hdr))
		ERROR_EXIT("NES file is not valid!");

    nes_hdr_info(&hdr);

	if (GET_MAPPER(&hdr) == 2)
		ERROR_EXIT_FMT("Mapper #2 '%s' does not have pattern table!"
				, name_of_mapper(GET_MAPPER(&hdr)));

	rom_size = GET_NUM_OF_ROM_BANKS(&hdr) * (16 * 1024);		// 16KB ROMs
	chr_size = GET_NUM_OF_VROM_BANKS(&hdr) * (8 * 1024);		// 8KB CHR ROMs

	if (file_size < sizeof(hdr) + rom_size)
		ERROR_EXIT("NES file, ROM section is corrupted!");
	if (file_size < sizeof(hdr) + rom_size + chr_size)
		ERROR_EXIT("NES file, CHR ROM section is corrupted!");

	if (!(output = fopen(argv[2], "wb")))
		ERROR_EXIT_FMT("Could not open '%s' file!", argv[2]);

	fseek(ines, rom_size, SEEK_CUR);		// Begining of CHR ROM section.

	for (i = 0; i < GET_NUM_OF_VROM_BANKS(&hdr); ++i) {
		if (fread(&buffer, sizeof(buffer), 1, ines) != 1)
			ERROR_EXIT("Could not read pattern table!");

		if (fwrite(&buffer, sizeof(buffer), 1, output) != 1)
			ERROR_EXIT("Coult not write to output file!");
	}

	printf("Writing pattern table to '%s' done!", argv[2]);

	return 1;
}

