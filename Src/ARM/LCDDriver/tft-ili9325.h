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

#ifndef TFT_ILI9325_H
#define TFT_ILI9325_H

#include <stdint.h>

#include "config.h"

#if defined TFT_ILI9325_AVR_LIB
#include <delay.h>
#else
#include "delay.h"
#endif

#if defined TFT_ILI9325_AVR_LIB
#define TFT_ILI9325_CONTROL_PORT		PORTD
#define TFT_ILI9325_CONTROL_DDR			DDRD
#define TFT_ILI9325_CONTROL_PIN			PIND

#define TFT_ILI9325_WR_DATA				PORTB
#define TFT_ILI9325_WR_DATA_DDR			DDRB
#define TFT_ILI9325_WR_DATA_PIN			PINB

#define TFT_ILI9325_WR_DATA_LOW			PORTA
#define TFT_ILI9325_WR_DATA_LOW_DDR		DDRA
#define TFT_ILI9325_WR_DATA_LOW_PIN		PINA

#define TFT_ILI9325_RESET				TFT_ILI9325_CONTROL_PORT.2
#define TFT_ILI9325_CS					TFT_ILI9325_CONTROL_PORT.3
#define TFT_ILI9325_RS					TFT_ILI9325_CONTROL_PORT.4
#define TFT_ILI9325_WR					TFT_ILI9325_CONTROL_PORT.5
#define TFT_ILI9325_RD					TFT_ILI9325_CONTROL_PORT.6

#define TFT_ILI9325_RESET_1()			TFT_ILI9325_RESET = 1
#define TFT_ILI9325_RESET_0()			TFT_ILI9325_RESET = 0
#define TFT_ILI9325_CS_1				TFT_ILI9325_CS = 1
#define TFT_ILI9325_CS_0				TFT_ILI9325_CS = 0
#define TFT_ILI9325_RS_1				TFT_ILI9325_RS = 1
#define TFT_ILI9325_RS_0				TFT_ILI9325_RS = 0
#define TFT_ILI9325_WR_1				TFT_ILI9325_WR = 1
#define TFT_ILI9325_WR_0				TFT_ILI9325_WR = 0
#define TFT_ILI9325_RD_1				TFT_ILI9325_RD = 1
#define TFT_ILI9325_RD_0				TFT_ILI9325_RD = 0

#pragma used+
#else

//#define TFT_ILI9325_RESET_0()			(GPIOG->BRR = GPIO_BRR_BR0)
//#define TFT_ILI9325_RESET_1()			(GPIOG->BSRR = GPIO_BSRR_BS0)
#define TFT_ILI9325_RESET_0()
#define TFT_ILI9325_RESET_1()

#define TFT_ILI9325_BANK1				((uint32_t)0x00000000)
#define TFT_ILI9325_BANK2				((uint32_t)0x04000000)
#define TFT_ILI9325_BANK3				((uint32_t)0x08000000)
#define TFT_ILI9325_BANK4				((uint32_t)0x0C000000)

typedef struct
{
	uint16_t LCD_REG;
	uint16_t LCD_RAM;
} LCD_TypeDef;

#define FSMC_NORSRAM_START				((uint32_t)0x60000000)
#define FSMC_A10_LINE_START				((uint32_t)0x000007FE)
#define TFT_ILI9325_BASE				(FSMC_NORSRAM_START | TFT_ILI9325_BANK4 | FSMC_A10_LINE_START)
#define TFT_ILI9325_LCD					((LCD_TypeDef*)TFT_ILI9325_BASE)
#endif

void tft_ili9325_init(void);
void tft_ili9325_reset(void);
void tft_ili9325_wr_cmd(uint8_t index, uint16_t val);
void tft_ili9325_wr_reg(uint8_t index);
void tft_ili9325_wr_data(uint16_t val);
unsigned int tft_ili9325_read_data(void);

#if defined TFT_ILI9325_AVR_LIB
#pragma used-
#endif

#endif

