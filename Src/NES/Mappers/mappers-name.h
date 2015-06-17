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

#ifndef MAPPERS_H_
#define MAPPERS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NROM						0		/// No mapper
#define NINTENDO_MMC1				1		/// Nintendo MMC1
#define UNROM						2		/// UNROM switch
#define CNROM						3		/// CNROM switch
#define NINTENDO_MMC3				4		/// Nintendo MMC3
#define NINTENDO_MMC5				5		/// Nintendo MMC5
#define FFE_F4XXX					6		/// FFE F4xxx
#define AOROM						7		/// AOROM switch
#define FFE_F3XXX					8		/// FFE F3xxx
#define NINTENDO_MMC2				9		/// Nintendo MMC2
#define NINTENDO_MMC4				10		/// Nintendo MMC4
#define COLOR_DREAMS_CHIP			11		/// ColorDreams chip
#define FFE_F6XXX					12		/// FFE F6xxx

#define _100_IN_1					15		/// 100-in-1 switch
#define BANDAI_CHIP					16		/// Bandai chip
#define FFE_F8XXX					17		/// FFE F8xxx
#define JALECO_SS8806_CHIP			18		/// Jaleco SS8806 chip
#define NAMCOT_106_CHIP				19		/// Namecot 106 chip
#define NINTENDO_DISK_SYSTEM		20		/// Nintendo DiskSystem
#define KONAMI_VRC4A				21		/// Konami VRC4a
#define KONAMI_VRC2A_1				22		/// Konami VRC2a
#define KONAMI_VRC2A_2				23		/// Konami VRC2a
#define KONAMI_VRC6					24		/// Konami VRC6
#define KONAMI_VRC4B				25		/// Konami VRC4b

#define IREM_G_101_CHIP				32		/// Irem G-101 chip
#define TAITO_TC0190_TC0350			33		/// Taito TC0190/TC0350
#define _32_KB_ROM					34		/// 32 KB ROM swtich

#define TENGEN_RAMBO_1_CHIP			64		/// Tengen RAMBO-1 chip
#define IREM_H_3001_CHIP			65		/// Irem H-3001 chip
#define GEROM						66		/// GNROM switch
#define SUN_SOFT3_CHIP				67		/// SunSoft3 chip
#define SUN_SOFT4_CHIP				68		/// SunSoft4 chip
#define SUN_SOFT5_FME_7_CHIP		69		/// SunSoft5 FME-7 chip

#define CAMERICA_CHIP				71		/// Camerica chip

#define IREM_74HC161_32_BASED		78		/// Irem 74HC161/32-based

#define PIRATE_HK_SF3_CHIP			91		/// Pirate HK-SF3 chip

#define MAPPER_TO_STRING(mpr)		# mpr

extern char* mappers_name[256];

char* name_of_mapper(uint8_t m);

#ifdef __cplusplus
}
#endif

#endif

