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

#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/// STM32F103 NES emulator library
//#define NES_ARM_LIB

/// Debuging level
#define DEBUG_MODE				1

/// Logging level
//#define DEBUG_LOGGING				1

/// PPU debugging level
//#define DEBUG_PPU_NAMETABLE		1
//#define DEBUG_PPU_PATTERNTABLE	1

/// Processor options
//#define UN_OFFICIALS

/// Joystick options
#define READ_SDL_JOYPAD 1

#ifdef __cplusplus
}
#endif

#endif

