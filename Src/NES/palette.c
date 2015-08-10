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

#include "palette.h"

#if defined _WIN32 || defined __linux__
const uint32_t colors[PALETTE_SIZE * 3] =
{
	//R   G   B
	117, 117, 117,
	 39,  27, 143,
	  0,   0, 171,
	 71,   0, 159,
	143,   0, 119,
	171,   0,  19,
	167,   0,   0,
	127,  11,   0,
	 67,  47,   0,
	  0,  71,   0,
	  0,  81,   0,
	  0,  63,  23,
	 27,  63,  95,
	  0,   0,   0,
	  0,   0,   0,
	  0,   0,   0,
	188, 188, 188,
	  0, 115, 239,
	 35,  59, 239,
	131,   0, 243,
	191,   0, 191,
	231,   0,  91,
	219,  43,   0,
	203,  79,  15,
	139, 115,   0,
	  0, 151,   0,
	  0, 171,   0,
	  0, 147,  59,
	  0, 131, 139,
	  0,   0,   0,
	  0,   0,   0,
	  0,   0,   0,
	255, 255, 255,
	 63, 191, 255,
	 95, 151, 255,
	167, 139, 253,
	247, 123, 255,
	255, 119, 183,
	255, 119,  99,
	255, 155,  59,
	243, 191,  63,
	131, 211,  19,
	 79, 223,  75,
	 88, 248, 152,
	  0, 235, 219,
	  0,   0,   0,
	  0,   0,   0,
	  0,   0,   0,
	255, 255, 255,
	171, 231, 255,
	199, 215, 255,
	215, 203, 255,
	255, 199, 255,
	255, 199, 219,
	255, 191, 179,
	255, 219, 171,
	255, 231, 163,
	227, 255, 163,
	171, 243, 191,
	179, 255, 207,
	159, 255, 243,
	  0,   0,   0,
	  0,   0,   0,
	  0,   0,   0,
};
#elif defined NES_ARM_LIB
Point p;
const uint16_t colors[PALETTE_SIZE] =
{
	0x73ae, 0x20d1, 0x0015, 0x4013, 0x880e, 0xa802, 0xa000, 0x7840,
	0x4160, 0x0220, 0x0280, 0x01e2, 0x19eb, 0x0000, 0x0000, 0x0000,
	0xbdf7, 0x039d, 0x21dd, 0x801e, 0xb817, 0xe00b, 0xd940, 0xca61,
	0x8b80, 0x04a0, 0x0540, 0x0487, 0x0411, 0x0000, 0x0000, 0x0000,
	0xffff, 0x3dff, 0x5cbf, 0xa45f, 0xf3df, 0xfbb6, 0xfbac, 0xfcc7,
	0xf5e7, 0x8682, 0x4ee9, 0x5fd3, 0x075b, 0x0000, 0x0000, 0x0000,
	0xffff, 0xaf3f, 0xc6bf, 0xd65f, 0xfe3f, 0xfe3b, 0xfdf6, 0xfed5,
	0xff34, 0xe7f4, 0xaf97, 0xb7f9, 0x9ffe, 0x0000, 0x0000, 0x0000,
};
#endif

