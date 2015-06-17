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

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

#include "config.h"

#if defined JOYSTIC_AVR_LIB
#define JOYSTICK_PORT			PORTC
#define JOYSTICK_DDR			DDRC
#define JOYSTICK_PIN			PINC

#define JOYSTICK_PORT_LEFT		JOYSTICK_PORT.0
#define JOYSTICK_DDR_LEFT		JOYSTICK_DDR.0
#define JOYSTICK_PIN_LEFT		JOYSTICK_PIN.0

#define JOYSTICK_PORT_RIGHT		JOYSTICK_PORT.1
#define JOYSTICK_DDR_RIGHT		JOYSTICK_DDR.1
#define JOYSTICK_PIN_RIGHT		JOYSTICK_PIN.1

#pragma used+
#else

// BTN 3
#define JOYSTICK_LEFT			(1 << 5)

// BTN 2
#define JOYSTICK_RIGHT			(1 << 4)

// BTN 1
#define JOYSTICK_DOWN			(1 << 3)

// BTN 4
#define JOYSTICK_UP				(1 << 6)

#define JOYSTICK_IS_RIGHT(KEY_STATE)	((KEY_STATE & JOYSTICK_RIGHT) == 0)
#define JOYSTICK_IS_LEFT(KEY_STATE)		((KEY_STATE & JOYSTICK_LEFT) == JOYSTICK_LEFT)
#define JOYSTICK_IS_UP(KEY_STATE)		((KEY_STATE & JOYSTICK_UP) == 0)
#define JOYSTICK_IS_DOWN(KEY_STATE)		((KEY_STATE & JOYSTICK_DOWN) == 0)
// TODO: Implement.
#define JOYSTICK_IS_A(KEY_STATE)			(0)
#define JOYSTICK_IS_B(KEY_STATE)			(0)
#define JOYSTICK_IS_SELECT(KEY_STATE)		(0)
#define JOYSTICK_IS_START(KEY_STATE)		(0)
#endif

void joystick_init(void);
uint16_t joystick_get_key_state(void);

#if defined JOYSTIC_AVR_LIB
#pragma used-
#endif

#endif

