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

#ifndef PALETTE_H_
#define PALETTE_H_

#include <stdint.h>

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PALETTE_SIZE	64

#if defined _WIN32 || defined __linux__
extern const uint32_t colors[PALETTE_SIZE];
#elif defined NES_ARM_LIB
extern Point p;
extern const uint16_t colors[PALETTE_SIZE];
#endif

#ifdef __cplusplus
}
#endif

#endif

