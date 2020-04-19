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

#include "bank-switch.h"

#include <string.h>

#include "nes-hdr.h"
#include "emulator.h"

int BankSwitch1K(int bank, uint8_t* rom, FILE* image)
{
	fseek(image, sizeof(NesHeader) + (bank * BANK_SIZE_1K) + (IS_TRAINER(&hdr) ? 512 : 0) , SEEK_SET);
	return fread(rom, BANK_SIZE_1K, 1, image) == 1;
}

int BankSwitch2K(int bank, uint8_t* rom, FILE* image)
{
	fseek(image, sizeof(NesHeader) + (bank * BANK_SIZE_2K) + (IS_TRAINER(&hdr) ? 512 : 0) , SEEK_SET);
	return fread(rom, BANK_SIZE_2K, 1, image) == 1;
}

int BankSwitch4K(int bank, uint8_t* rom, FILE* image)
{
	fseek(image, sizeof(NesHeader) + (bank * BANK_SIZE_4K) + (IS_TRAINER(&hdr) ? 512 : 0) , SEEK_SET);
	return fread(rom, BANK_SIZE_4K, 1, image) == 1;
}

int BankSwitch8K(int bank, uint8_t* rom, FILE* image)
{
	fseek(image, sizeof(NesHeader) + (bank * BANK_SIZE_8K) + (IS_TRAINER(&hdr) ? 512 : 0) , SEEK_SET);
	return fread(rom, BANK_SIZE_8K, 1, image) == 1;
}

int BankSwitch16K(int bank, uint8_t* rom, FILE* image)
{
	fseek(image, sizeof(NesHeader) + (bank * BANK_SIZE_16K) + (IS_TRAINER(&hdr) ? 512 : 0) , SEEK_SET);
	return fread(rom, BANK_SIZE_16K, 1, image) == 1;
}

int BankSwitch32K(int bank, uint8_t* rom, FILE* image)
{
	fseek(image, sizeof(NesHeader) + (bank * BANK_SIZE_32K) + (IS_TRAINER(&hdr) ? 512 : 0) , SEEK_SET);
	return fread(rom, BANK_SIZE_32K, 1, image) == 1;
}

