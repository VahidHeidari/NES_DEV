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

#ifndef JOYPAD_H
#define JOYPAD_H

#ifdef __cplusplus
extern "C" {
#endif

#define RIGHT_BOTTUN		(0x01 << 7)
#define LEFT_BOTTUN			(0x01 << 6)
#define DOWN_BOTTUN			(0x01 << 5)
#define UP_BOTTUN			(0x01 << 4)
#define START_BOTTUN		(0x01 << 3)
#define SELECT_BOTTUN		(0x01 << 2)
#define B_BOTTUN			(0x01 << 1)
#define A_BOTTUN			(0x01 << 0)

#define JOYPAD_RESET(joypad) do {			\
	(joypad)->buttons1.read_index = 1;		\
	(joypad)->buttons2.read_index = 1;		\
	(joypad)->strobe = 0;					\
} while (0)

typedef struct JoypadButtons
{
	int buttons;
	int read_index;
} JoypadButtons, *pJoypadButtons;

typedef struct Joypad
{
	JoypadButtons buttons1;								/// $40 16
	JoypadButtons buttons2;								/// $40 17

	int strobe;
} Joypad, *pJoypad;

int joypad_init(pJoypad joypad);
void joypad_close(pJoypad joypad);

#ifdef __cplusplus
}
#endif

#endif

