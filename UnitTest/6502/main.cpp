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

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>

#if defined _WIN32
#include <Windows.h>
#include <conio.h>
#endif

#include "M6502.h"
#include "p6502.h"
#include "types.h"
#include "x6502struct.h"
#include "x6502.h"
#include "x6502abbrev.h"

using namespace std;

#if defined __linux__
#define _getch getchar
#endif

uint16_t tmp_pc;
uint8_t tmp_ins;
P6502 p;
M6502 m;
X6502 X;
bool test_failed = false;

extern "C" { extern int finished; }

void m6502_clock_cycle()
{
	Run6502(&m);
}

void cpu_clock_cycle()
{
	fetch(&p);
}

void print_stat()
{
	//cout << "MPC : $" << hex << setfill('0') << setw(4) << (int)m.PC.W << endl;
	cout << "XPC : $" << hex << setfill('0') << setw(4) << (int)_PC << endl;
	cout << " pc : $" << hex << setfill('0') << setw(4) << (int)p.pc.w << endl;
	cout << " tmp_pc : $" << setw(4) << (int)tmp_pc;
	cout << "    tmp_ins : $" << (int)tmp_ins << endl;
	cout << dec << " clock cycle :" << p.cycle_counter << "    ins cycle :" << p.ins_cycles;
	cout << endl << endl;
}

void test()
{
	if (   p.a != m.A
		|| p.x != m.X
		|| p.y != m.Y
		|| p.pc.w != m.PC.W
		|| p.sp != m.S
		|| p.p != m.P)
	{
		cout << "Context corrupted!" << endl;
		finished = 1;
		test_failed = true;
	}

	if (memcmp(RAM, MRAM, NOT_MIRRORED_RAM_SIZE) != 0)
	{
		cout << "RAM corrupted!" << endl;
		finished = 1;
		test_failed = true;
	}
}

void X6502_Test()
{
		if (   p.a != _A
		|| p.x != _X
		|| p.y != _Y
		|| p.pc.w != _PC
		|| p.sp != _S
		|| p.p != _P)
	{
		cout << "Context corrupted!" << endl;
		finished = 1;
		test_failed = true;
	}

	if (memcmp(RAM, XRAM, NOT_MIRRORED_RAM_SIZE) != 0)
	{
		cout << "RAM corrupted!" << endl;
		finished = 1;
		test_failed = true;
	}
}

bool read_nes(const char* path)
{
	FILE* image;
#if defined _WIN32 && !defined __GNUC__
	fopen_s(&image, path, "rb");
#else
	image = fopen(path, "rb");
#endif
	if (!image)
		return false;
	fseek(image, 16, SEEK_SET);			// jump from NES header.
	fread(ROM, ROM_SIZE, 1, image);
	fclose(image);
	return true;
}

bool read_bin(const char* path)
{
	FILE* image;
#if defined _WIN32 && !defined __GNUC__
	fopen_s(&image, path, "rb");
#else
	image = fopen(path, "rb");
#endif

	if (!image)
		return false;
	fread(ROM, ROM_SIZE, 1, image);
	fclose(image);
	return true;
}

bool read_rom(const char* path)
{
	size_t len = strlen(path);
	char ext[4];
	ext[0] = toupper(path[len - 3]);
	ext[1] = toupper(path[len - 2]);
	ext[2] = toupper(path[len - 1]);
	ext[3] = 0;

	if  (strcmp(ext, "BIN") == 0)
		return read_bin(path);
	else if (strcmp(ext, "NES") == 0)
		return read_nes(path);
	else
		return false;
}

int main(int argc, char** argv)
{
	bool res;

	// Test ROMs grabbed from : http://blargg.8bitalley.com/nes-tests/instr_test-v4.zip
	//res = read_rom("TEST\\TEST.bin");
#define PATH_REASM	(char*)"..\\..\\..\\UnitTest\\6502\\instr_test-v4\\"
	//res = read_rom(PATH_REASM "RE_01-basics.nes");		// PASSED
	//res = read_rom(PATH_REASM "RE_02-implied.nes");		// PASSED
	//res = read_rom(PATH_REASM "RE_03-immediate.nes");	// PASSED
	//res = read_rom(PATH_REASM "RE_04-zero_page.nes");	// PASSED
	//res = read_rom(PATH_REASM "RE_05-zp_xy.nes");		// PASSED
	//res = read_rom(PATH_REASM "RE_06-absolute.nes");	// PASSED
	//res = read_rom(PATH_REASM "RE_07-abs_xy.nes");		// PASSED
	//res = read_rom(PATH_REASM "RE_08-ind_x.nes");		// PASSED
	res = read_rom(PATH_REASM "RE_09-ind_y.nes");		// PASSED
	//res = read_rom(PATH_REASM "RE_10-branches.nes");	// PASSED
	//res = read_rom(PATH_REASM "RE_11-stack.nes");		// PASSED
	//res = read_rom(PATH_REASM "RE_12-jmp_jsr.nes");		// PASSED
	//res = read_rom(PATH_REASM "RE_13-rts.nes");			// PASSED
	//res = read_rom(PATH_REASM "RE_14-rti.nes");			// PASSED
	//res = read_rom(PATH_REASM "RE_15-brk.nes");			// PASSED
	//res = read_rom(PATH_REASM "RE_16-special.nes");		// PASSED

	if (!res)
	{
		cerr << "Could not open file!" << endl;
#if defined _WIN32
		system("PAUSE");
#endif
		return 1;
	}

	cout << "This is unit test of 6502 processor." << endl;
	//cout << "Reset M6502 . . ." << endl;
	//Reset6502(&m);
	//Print(&m);
	cout << "Reset P6502 . . ." << endl;
	reset(&p);
	//print(&p);
	cout << "Init X6502 . . ." << endl;
	X6502_Init();
	//X6502_Print();

	// RUN TEST
	while (!finished)
	{
		//if (p.cycle_counter >= 316546)
		//	printf("");

		tmp_pc = p.pc.w;
		tmp_ins = read(tmp_pc);
		cpu_clock_cycle();
#ifdef M6502_TEST
		m6502_clock_cycle();
		test();
#else
		X6502_Run(0);
		X6502_Test();
#endif
	}

	cout << endl << endl;
	if (test_failed)
	{
		cout << "--- TEST FAILED ---" << endl;
		print_stat();
	}
	else
		cout << "--- TEST PASSED ---" << endl;

	cout << "Processors context:" << endl;
	print(&p);
	//Print(&m);
	X6502_Print();

	cout << "Dump RAM ? [y/N] ";
	int cmd = _getch();
	cout << (char)cmd << endl;

	if (cmd == 'Y' || cmd == 'y')
	{
		for (int i = 0; i < NOT_MIRRORED_RAM_SIZE; i += 8)
		{
			cout << " " << hex << setfill('0') << setw(4) << i << " :   ";
			for (int j = 0; j < 8; ++j)
				cout << hex << setfill('0') << setw(2) << (int)RAM[i + j] << " ";
			cout << "         ";

			// M6502 RAM
			//for (int j = 0; j < 8; ++j)
			//	cout << hex << setfill('0') << setw(2) << (int)MRAM[i + j] << " ";

			// X6502 RAM
			for (int j = 0; j < 8; ++j)
				cout << hex << setfill('0') << setw(2) << (int)XRAM[i + j] << " ";

			cout << endl;
		}
	}

#if defined _WIN32
	system("PAUSE");
#endif

	return 0;
}
