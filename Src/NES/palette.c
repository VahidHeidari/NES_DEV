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
const uint32_t colors[PALETTE_SIZE] =
{
	//R   G   B
	(117 << 16) | (117 << 8) | 117, ( 39 << 16) | ( 27 << 8) | 143,
	(  0 << 16) | (  0 << 8) | 171, ( 71 << 16) | (  0 << 8) | 159,
	(143 << 16) | (  0 << 8) | 119, (171 << 16) | (  0 << 8) |  19,
	(167 << 16) | (  0 << 8) |   0, (127 << 16) | ( 11 << 8) |   0,
	( 67 << 16) | ( 47 << 8) |   0, (  0 << 16) | ( 71 << 8) |   0,
	(  0 << 16) | ( 81 << 8) |   0, (  0 << 16) | ( 63 << 8) |  23,
	( 27 << 16) | ( 63 << 8) |  95, (  0 << 16) | (  0 << 8) |   0,
	(  0 << 16) | (  0 << 8) |   0, (  0 << 16) | (  0 << 8) |   0,
	(188 << 16) | (188 << 8) | 188, (  0 << 16) | (115 << 8) | 239,
	( 35 << 16) | ( 59 << 8) | 239, (131 << 16) | (  0 << 8) | 243,
	(191 << 16) | (  0 << 8) | 191, (231 << 16) | (  0 << 8) |  91,
	(219 << 16) | ( 43 << 8) |   0, (203 << 16) | ( 79 << 8) |  15,
	(139 << 16) | (115 << 8) |   0, (  0 << 16) | (151 << 8) |   0,
	(  0 << 16) | (171 << 8) |   0, (  0 << 16) | (147 << 8) |  59,
	(  0 << 16) | (131 << 8) | 139, (  0 << 16) | (  0 << 8) |   0,
	(  0 << 16) | (  0 << 8) |   0, (  0 << 16) | (  0 << 8) |   0,
	(255 << 16) | (255 << 8) | 255, ( 63 << 16) | (191 << 8) | 255,
	( 95 << 16) | (151 << 8) | 255, (167 << 16) | (139 << 8) | 253,
	(247 << 16) | (123 << 8) | 255, (255 << 16) | (119 << 8) | 183,
	(255 << 16) | (119 << 8) |  99, (255 << 16) | (155 << 8) |  59,
	(243 << 16) | (191 << 8) |  63, (131 << 16) | (211 << 8) |  19,
	( 79 << 16) | (223 << 8) |  75, ( 88 << 16) | (248 << 8) | 152,
	(  0 << 16) | (235 << 8) | 219, (  0 << 16) | (  0 << 8) |   0,
	(  0 << 16) | (  0 << 8) |   0, (  0 << 16) | (  0 << 8) |   0,
	(255 << 16) | (255 << 8) | 255, (171 << 16) | (231 << 8) | 255,
	(199 << 16) | (215 << 8) | 255, (215 << 16) | (203 << 8) | 255,
	(255 << 16) | (199 << 8) | 255, (255 << 16) | (199 << 8) | 219,
	(255 << 16) | (191 << 8) | 179, (255 << 16) | (219 << 8) | 171,
	(255 << 16) | (231 << 8) | 163, (227 << 16) | (255 << 8) | 163,
	(171 << 16) | (243 << 8) | 191, (179 << 16) | (255 << 8) | 207,
	(159 << 16) | (255 << 8) | 243, (  0 << 16) | (  0 << 8) |   0,
	(  0 << 16) | (  0 << 8) |   0, (  0 << 16) | (  0 << 8) |   0
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

