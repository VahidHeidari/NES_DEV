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

#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "p6502.h"
#include "debuger.h"
#include "emulator.h"
#include "console.h"

/// Update processor context
void UpdateRegs()
{
	GoToXY(REGS_X, REGS_Y);
	PrintRegs(&p);
	Dissassemble(&p);
	printf("%s                  \n", asm_m);
}

#if defined _WIN32 || defined __linux__
int main(int argc, char** argv)
{
	int i;
	unsigned int cmd;
	unsigned int val;
	unsigned int addr;
	char* file_path;

	ClearScreen();

#ifdef DEBUG_MODE
	for (i = 0; i < argc; ++i)
		DebugMessage("argv[%2d] : '%s'", i, argv[i]);
#endif
	printf("This is NES emulator.\n");

	if (argc >= 2) {
		char* arg = argv[1];
		int arg_len = strlen(arg);
		if (arg_len == 2) {
			if (arg[0] == '-' && (arg[1] == 't' || arg[1] == 'T'))			// TEST ROM
				InitTestROM(&hdr, ROM, pattern_table);
			else if (arg[0] == '-' && (arg[1] == 'd' || arg[1] == 'D')) {	// Dumped image
				FILE* rom_image;
				FILE* chr_image;

				if (argc < 3)
					return 1;

#if defined _WIN32 && defined _MSC_VER
				fopen_s(&rom_image, argv[2], "rb");
				fopen_s(&chr_image, argv[3], "rb");
#elif defined __linux__ || defined __GNUC__
				rom_image = fopen(argv[2], "rb");
				chr_image = fopen(argv[3], "rb");
#endif
				if (!rom_image || !chr_image) {
					DebugMessage("Could not open dumped ROM image!");
					return 1;
				}

				if (fread(ROM, ROM_SIZE, 1, rom_image) != 1)
					return 1;

				if (fread(pattern_table, PPU_PATTERN_TABLE_SIZE, 1, chr_image) != 1)
					return 1;

				fclose(rom_image);
				fclose(chr_image);
			} else if (arg[0] == '-' && (arg[1] == 'r' || arg[1] == 'R')) {
				if (argc < 3) {
					DebugMessage("File name required.");
					return 1;
				}

				if (DumpTestROM(argv[2]) != 1) {
					DebugMessage("Could not dump test ROM!");
					return 1;
				}

				DebugMessage("Test ROM dumped!");
				return 0;
			} else {
				DebugMessage("Invalid command line arguments!");
				return 1;
			}
		} else {
			file_path = argv[1];
			if (ReadROMImage(file_path) != 1) {
				printf("Reading ROM image failed!\n");
				return 1;
			}
		}
	} else if (argc < 2) {
		printf("Emulation failed! No ROM image supplied!\n");
		return 1;
	}

	if (Emulator_Init() != 1) {
		printf("Emulator initialization failed!\n");
		return 1;
	}

	Run();		// Comment this line for debuging.

	//clear_screen();
	GoToXY(REGS_X, REGS_Y);
	UpdateRegs();
	GoToXY(CMD_X, CMD_Y);

	while (!finished_emulation) {
		ClearCmdLine();
		cmd = GET_CHAR();
 		putchar(cmd);

		switch (cmd) {
			case 's':
			case 'S':
				Step();
				UpdateRegs();
				break;

			// Finish emulation
			case 'q':
			case 'Q':
				finished_emulation = 1;
				break;

			case 'w':
			case 'W':
				if (scanf_s("%4x %2x", &addr, &val))
					Write(addr, val);
				break;

			case 'r':
			case 'R':
				if (scanf_s("%4x", &addr)) {
					val = Read(addr);
					printf("    $%04x : %02X", addr, val);
				}
				break;

			case 'd':
			case 'D':
				if (scanf_s("%4x %u", &addr, &val))
					for (i = 0; i < (int)val; ++i)
						printf("\n    %04X : %02X\n", addr + i, Read(addr + i));
				break;

			case 'h':
			case 'H':
				printf("\n"
					"    S          : Step\n"
					"    Q          : Quit\n"
					"    W Addr Val : Write memory\n"
					"    R Addr     : Read memory\n"
					"    D Addr Sz  : Dump memory\n"
					"    C Val      : Run for Val cycles\n"
					"    H          : Help\n"
					"    E          : Run to PC equals to Val\n"
					);
				break;

			case 'a':
			case 'A':
				if (scanf_s("%2x", &val))
					WriteA(&p, val);
				break;

			case 'x':
			case 'X':
				if (scanf_s("%2x", &val))
					WriteX(&p, val);
				break;

			case 'y':
			case 'Y':
				if (scanf_s("%2x", &val))
					WriteY(&p, val);
				break;

			case 'p':
			case 'P':
				if (scanf_s("%2x", &val))
					WriteP(&p, val | FLAG_1);
				break;

			case 'c':
			case 'C':
				if (scanf_s("%u", &val)) {
					RunCycles(val);
					UpdateRegs();
				}
				break;

			case 'e':
			case 'E':
				if (scanf_s("%4x", &val)) {
					while (p.pc.w != val)
						Step();
					UpdateRegs();
				}
				break;

			default:
				break;
		}
	}

	Emulator_Close();
	return 0;
}
#elif defined NES_ARM_LIB

#define USART_FLAG_TXE		((uint16_t)0x0080)
#define USART_FLAG_RXNE		((uint16_t)0x0020)

int SendChar (int ch)  {
	while (!(USART1->SR & USART_FLAG_TXE));
	USART1->DR = (ch & 0x1FF);

	return (ch);
}

int GetKey (void)
{
	while (!(USART1->SR & USART_FLAG_RXNE))
		continue;

	return ((int)(USART1->DR & 0x1FF));
}

__inline static void InitClock()
{
	// Initialize clock source and PLL
	RCC->CFGR = RCC_CFGR_PLLMULL9		// Mulipliction factor
		| RCC_CFGR_PLLSRC_HSE			// PLL soruce clock is external high speed crystal
		| RCC_CFGR_SW_PLL;				// Switch to PLL

	RCC->CR = RCC_CR_PLLON				// Turn on PLL
		| RCC_CR_HSEON;					// Turn on HSE

	while ((RCC->CR & RCC_CR_HSERDY) == 0)		// Wait for high speed external crystal.
		continue;
	while ((RCC->CR & RCC_CR_PLLRDY) == 0)		// Wait for PLL.
		continue;
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL)		// Wait for switching
		continue;
}

__inline static void InitPort()
{
	// Initialize PORTA
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPEEN
		| RCC_APB2ENR_IOPFEN | RCC_APB2ENR_IOPGEN;

	// Clear previouse config.
	GPIOB->CRL &= ~0x0000000F;
	GPIOE->CRL &= ~0x0FFFF000;
	GPIOF->CRL &= ~0x0F000000;
	GPIOF->CRH &= ~0x0000F000;
	GPIOG->CRL &= ~0xF0000000;

	GPIOA->CRL = 0x33333333;				// Control Register Low
	GPIOB->CRL |= 0x00000003;
	GPIOE->CRL |= 0x04444000;
	GPIOF->CRL |= 0x03000000;
	GPIOF->CRH |= 0x00003000;
	GPIOG->CRL |= 0x30000000;
}

__inline static void InitLCD()
{
	// Initial control lines alternate function.
	// AHB clock enable
	RCC->AHBENR |= RCC_AHBENR_FSMCEN;
	// AHB2 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN		// AFIO may be enabled already!
		| RCC_APB2ENR_IOPDEN
		| RCC_APB2ENR_IOPEEN
		| RCC_APB2ENR_IOPFEN
		| RCC_APB2ENR_IOPGEN;

	GPIOD->CRL = 0x44BB44BB;
	GPIOD->CRH = 0xBB444BBB;
	GPIOE->CRL = 0xB4444444;
	GPIOE->CRH = 0xBBBBBBBB;
	GPIOG->CRL = 0x4444444B;
	GPIOG->CRH = 0x444B4444;

	FSMC_Bank1->BTCR[7] = 0x10000501;
	FSMC_Bank1->BTCR[6] = 0x00001011;

	// Initialize LCD.
	tft_ili9325_init();
}

void SystemInit()
{
	FLASH->ACR = 0x00000012;
	InitClock();
	InitLCD();
	InitPort();
	InitUSART();
}

int main()
{
	printf("This is NES emulator on STM32F103\r\n");
	//init_test_rom(&hdr, ROM, pattern_table);
	InitTestROM(&hdr, ROM, pattern_table);
	ClearScreen(WHITE);
	//ClearScreen(BLACK);

	// Set LCD window for fast drawing
	// Direction
	TFTili9325WrCmd(0x03, 0x1028); // set GRAM write direction and BGR = 1.
	// Window
	TFTili9325WrCmd(0x50, 0);
	TFTili9325WrCmd(0x51, 239);
	TFTili9325WrCmd(0x52, 0);
	TFTili9325WrCmd(0x53, 255);
	// VRAM start
	TFTili9325WrCmd(0x20, 0);
	TFTili9325WrCmd(0x21, 0);
	// GRAM register ready to write
	TFTili9325WrReg(0x22);

	EmulatoInit();
	//UpdateRegs();
	//PPU_PrintStatus(&ppu);
	while (1) {
		Step();
		//UpdateRegs();
		//Delay(9999);
	}
	return 0;
}
#endif

