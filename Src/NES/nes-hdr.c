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

#include "nes-hdr.h"

#include <stdio.h>

#include "mappers-name.h"

#if defined _WIN32 || defined __linux__
#define EOL		"\n"
#else
#define EOL		"\r\n"
#endif

void nes_hdr_info(pNesHeader hdr)
{
#ifdef DEBUG_MODE
	printf("NES header infos:\n");
	printf("    ROMs                : %2d x 16K" EOL, GET_NUM_OF_ROM_BANKS(hdr));
	printf("    VROMs               : %2d x 8K" EOL, GET_NUM_OF_VROM_BANKS(hdr));
	printf("    Mapper              : %2d\t'%s'" EOL, GET_MAPPER(hdr), name_of_mapper(GET_MAPPER(hdr)));
	printf("    Mirroring           : %s" EOL, ((GET_MIRRORING_TYPE(hdr) == MIRRORING_VERTICAL)? "Vertical" : "Horizontal"));
	printf("    Battry backed       : %s" EOL, IS_BATTRY_BACKED(hdr) ? "Yes" : "No");
	printf("    Mirroring overriden : %s" EOL, IS_MIRRORING_OVERRIDE(hdr) ? "Yes" : "No");
	printf("    Trainer             : %s" EOL, IS_TRAINER(hdr) ? "Yes" : "No");
#endif
}

