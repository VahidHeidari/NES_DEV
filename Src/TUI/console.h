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

#ifndef CONSOLE_H_
#define CONSOLE_H_

#ifdef _WIN32
#include <conio.h>
#include <Windows.h>
#endif

/// Command line screen coordination
#define CMD_X 0
#define CMD_Y 21

/// Processor context screen coordination
#define REGS_X 0
#define REGS_Y 14

#define CONSOLE_HEIGHT 40

#ifdef __cplusplus
extern "C" {
#endif

#define ESCAPE_KEY		27
#define DELETE_KEY		127
#define UP_KEY			'A'
#define DOWN_KEY		'B'
#define RIGHT_KEY		'C'
#define LEFT_KEY		'D'
#define ENTER_KEY		'\n'	
#define BACK_SPACE_KEY	'\b'

#if defined _WIN32
extern HANDLE console_handel;
#endif

void init_console(void);
void gotoxy(int x, int y);
void set_black_on_white(void);
void set_white_on_black(void);
void clear_cmd_line(void);
void clear_screen(void);

#ifdef __cplusplus
}
#endif

#endif

