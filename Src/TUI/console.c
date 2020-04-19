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

#include "console.h"

#include <stdio.h>

#if defined _WIN32
HANDLE console_handel = NULL;
#endif

void InitConsole()
{
#if defined _WIN32
	console_handel = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

/// Moves cursor to (x, y).
void GoToXY(int x, int y)
{
#ifdef _WIN32
	COORD c;

	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(console_handel, c);
#elif defined __linux__
	printf("%c[%d;%df", 0x1B, y, x);
#elif defined NES_ARM_LIB
#endif
}

void SetBlackOnWhite()
{
#if defined _WIN32
	SetConsoleTextAttribute(console_handel, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
#else
	printf("\x1b[0;7m");
#endif
}

void SetWhiteOnBlack()
{
#if defined _WIN32
	SetConsoleTextAttribute(console_handel, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
	printf("\x1b[7;0m");
#endif
}

void ClearCmdLine()
{
#if defined _WIN32 || defined __linux__
	GoToXY(CMD_X, CMD_Y);
	printf("                                   ");
	GoToXY(CMD_X, CMD_Y);
#endif
}

void ClearScreen()
{
#if defined _WIN32 || defined __linux__
	int i;
	GoToXY(0, 0);
	for (i = 0; i < CONSOLE_HEIGHT; ++i)
		printf("                                                            ");
	GoToXY(0, 0);
#elif defined NES_ARM_LIB
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
#endif
}

