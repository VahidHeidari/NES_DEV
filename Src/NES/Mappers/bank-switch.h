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

#ifndef BANK_SWITCH_H_
#define BANK_SWITCH_H_

#include <stdio.h>
#include <stdint.h>

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BANK_SIZE_1K		(1024 *  1)
#define BANK_SIZE_2K		(1024 *  2)
#define BANK_SIZE_4K		(1024 *  4)
#define BANK_SIZE_8K		(1024 *  8)
#define BANK_SIZE_16K		(1024 * 16)
#define BANK_SIZE_32K		(1024 * 32)

int bank_switch_1k(int bank, uint8_t* rom, FILE* image);
int bank_switch_2k(int bank, uint8_t* rom, FILE* image);
int bank_switch_4k(int bank, uint8_t* rom, FILE* image);
int bank_switch_8k(int bank, uint8_t* rom, FILE* image);
int bank_switch_16k(int bank, uint8_t* rom, FILE* image);
int bank_switch_32k(int bank, uint8_t* rom, FILE* image);

#ifdef __cplusplus
}
#endif

#endif

