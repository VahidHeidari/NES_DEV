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

#include "tft-ili9325.h"

void TFTili9325Reset(void)
{
	TFT_ILI9325_RESET_0();
	DelayMs(50);
	DelayMs(50);
	TFT_ILI9325_RESET_1();
	DelayMs(50);
	DelayMs(50);
}

void TFTili9325WrCmd(uint8_t index, uint16_t val)
{
#if defined TFT_ILI9325_AVR_LIB
	TFT_ILI9325_RS_0;
	TFT_ILI9325_CS_0;

	TFT_ILI9325_WR_DATA = 0x00;
	TFT_ILI9325_WR_DATA_LOW = index;
	TFT_ILI9325_WR_0;
	TFT_ILI9325_WR_1;

	TFT_ILI9325_RS_1;

	TFT_ILI9325_WR_DATA = (unsigned char)(val >> 8);
	TFT_ILI9325_WR_DATA_LOW = (unsigned char)val;
	TFT_ILI9325_WR_0;
	TFT_ILI9325_WR_1;

	TFT_ILI9325_CS_1;
#else
	TFT_ILI9325_LCD->LCD_REG = index;
	TFT_ILI9325_LCD->LCD_RAM = val;
#endif
}

void TFTili9325Init(void)
{
#if defined TFT_ILI9325_AVR_LIB
	TFT_ILI9325_CONTROL_DDR = 0xFC;
	TFT_ILI9325_WR_DATA_DDR = 0xFF;
	TFT_ILI9325_WR_DATA_LOW_DDR = 0xff;
#else
	// TODO: GPIO initialization can done here.
#endif

	tft_ili9325_reset();

#if defined TFT_ILI9325_AVR_LIB
	TFT_ILI9325_CS_1;
	TFT_ILI9325_WR_1;
	TFT_ILI9325_RD_1;
#endif

	TFTili9325WrCmd(0xe3, 0x3008); // set internal timming
	TFTili9325WrCmd(0xe7, 0x0012);
	TFTili9325WrCmd(0xef, 0x1231);
	TFTili9325WrCmd(0x01, 0x0100); // set SS and SM bit
	TFTili9325WrCmd(0x02, 0x0700);
	TFTili9325WrCmd(0x03, 0x1030); // set GRAM write direction and BGR = 1.
	TFTili9325WrCmd(0x04, 0x0000); // Resize register
	TFTili9325WrCmd(0x08, 0x0202); // set the back proch and front proch
	TFTili9325WrCmd(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
	TFTili9325WrCmd(0x0a, 0x0000); // FMARK function
	TFTili9325WrCmd(0x0c, 0x0000); // RGB interface setting
	TFTili9325WrCmd(0x0d, 0x0000); // Frame marker Position
	TFTili9325WrCmd(0x0f, 0x0000); // RGB interface polarity
	TFTili9325WrCmd(0x2b, 0x000E); // Frame rate 112
	/***************power on  sequence********************/
	TFTili9325WrCmd(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	TFTili9325WrCmd(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	TFTili9325WrCmd(0x12, 0x0000); // VREG1OUT voltage
	TFTili9325WrCmd(0x13, 0x0000); // VDV[4:0] for VCOM amplitude

	DelayMs(50);			  // Dis-charge capacitor power voltage
	DelayMs(50);
	DelayMs(50);
	DelayMs(50);

	TFTili9325WrCmd(0x10, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
	TFTili9325WrCmd(0x11, 0x0227); // R11h=0x0221 at VCI=3.3v, DC0[2:0], VC[2:0]

	DelayMs(50);			  // Delay 50ms

	TFTili9325WrCmd(0x12, 0x001c); // External reference voltage=Vci;

	DelayMs(50);

	TFTili9325WrCmd(0x20, 0x0000); // GRAM horizontal address
	TFTili9325WrCmd(0x21, 0x0000); // GRAM vertical address
	/******************Adjust the Gamma curve*****************/
	TFTili9325WrCmd(0x30, 0x0007);
	TFTili9325WrCmd(0x31, 0x0302);
	TFTili9325WrCmd(0x32, 0x0105);
	TFTili9325WrCmd(0x35, 0x0206);
	TFTili9325WrCmd(0x36, 0x0808);
	TFTili9325WrCmd(0x37, 0x0206);
	TFTili9325WrCmd(0x38, 0x0504);
	TFTili9325WrCmd(0x39, 0x0007);
	TFTili9325WrCmd(0x3c, 0x0105);
	TFTili9325WrCmd(0x3d, 0x0808);
	/***************** Set GRAM area ***********************/
	TFTili9325WrCmd(0x50, 0x0000); // Horizontal GRAM start address
	TFTili9325WrCmd(0x51, 0x00ef); // Horizontal GRAM end address
	TFTili9325WrCmd(0x52, 0x0000); // Vertical GRAM start address
	TFTili9325WrCmd(0x53, 0x013f); // Vertical GRAM start address
	TFTili9325WrCmd(0x60, 0xA700); // Gate scan line
	TFTili9325WrCmd(0x61, 0x0001); // NDL, VLE, REV
	TFTili9325WrCmd(0x6a, 0x0000); // set scrolling line
	/***************** Partial display control ******************/
	TFTili9325WrCmd(0x80, 0x0000);
	TFTili9325WrCmd(0x81, 0x0000);
	TFTili9325WrCmd(0x82, 0x0000);
	TFTili9325WrCmd(0x83, 0x0000);
	TFTili9325WrCmd(0x84, 0x0000);
	TFTili9325WrCmd(0x85, 0x0000);
	/**************** Panel control **********************/
	TFTili9325WrCmd(0x90, 0x0010);
	TFTili9325WrCmd(0x92, 0x0000);
	TFTili9325WrCmd(0x93, 0x0003);
	TFTili9325WrCmd(0x95, 0x0110);
	TFTili9325WrCmd(0x97, 0x0000);
	TFTili9325WrCmd(0x98, 0x0000);
	TFTili9325WrCmd(0x07, 0x0133); // 262k color and display ON
}

void TFTili9325WrReg(uint8_t index)
{
#if defined TFT_ILI9325_AVR_LIB
	TFT_ILI9325_RS_0;
	TFT_ILI9325_CS_0;

	TFT_ILI9325_WR_DATA = 0x00;
	TFT_ILI9325_WR_DATA_LOW = index;
	TFT_ILI9325_WR_0;
	TFT_ILI9325_WR_1;

	TFT_ILI9325_CS_1;
	TFT_ILI9325_RS_1;
#else
	TFT_ILI9325_LCD->LCD_REG = index;
#endif
}

void TFTili9325WrData(uint16_t val)
{
#if defined TFT_ILI9325_AVR_LIB
	TFT_ILI9325_CS_0;

	TFT_ILI9325_WR_DATA = (unsigned char)(val >> 8);
	TFT_ILI9325_WR_DATA_LOW = (unsigned char)val;
	TFT_ILI9325_WR_0;
	TFT_ILI9325_WR_1;

	TFT_ILI9325_CS_1;
#else
	TFT_ILI9325_LCD->LCD_RAM = val;
#endif
}

unsigned int TFTili9325ReadData(void)

