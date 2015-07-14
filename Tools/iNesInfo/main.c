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

#include "nes-hdr.h"

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
	if (argc != 2)
		ERROR_EXIT("File name required!");

	FILE* image;
	if (!(image = fopen(argv[1], "rb")))
		ERROR_EXIT_FMT("Could not open file '%s'!", argv[1]);

	NesHeader hdr;
	fread(&hdr, sizeof(hdr), 1, image);
	if (!nes_hdr_is_valid(&hdr))
		ERROR_EXIT("Not valid NES Header!");
	nes_hdr_info(&hdr);
	return 0;
}

