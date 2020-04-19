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

#ifndef IO_H
#define IO_H

#include <stdint.h>

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IO_REG_SIZE				0x2020		/// ~8KB
#define IO_SIZE					0x6000		/// $2000-$8000

extern uint8_t io[IO_SIZE];

void IO_Write(uint16_t addr, uint8_t val);
uint8_t IO_Read(uint16_t addr);

#ifdef __cplusplus
}
#endif

#endif

