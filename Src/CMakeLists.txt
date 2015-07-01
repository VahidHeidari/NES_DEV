#
# NES_DEV is a cross-platform, portable, and hand-held NES emulator.
#
# Copyright (C) 2015  Vahid Heidari (DeltaCode)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.8)

project(NES)

set(CMAKE_USE_RELATIVE_PATHS true)

include_directories(../TestROM NES/Mappers NES .)

add_executable(NES_DEV
	main.c
	NES/apu.c
	NES/cycles.c
	NES/debuger.c
	NES/emulator.c
	NES/io.c
	NES/joypad.c
	NES/nes-hdr.c
	NES/p6502.c
	NES/palette.c
	NES/ppu.c
	NES/state.c
	NES/Mappers/bank-switch.c
	NES/Mappers/mapper.c
	NES/Mappers/mappers-name.c
	../TestROM/pattern.c
	../TestROM/test-rom.c
	../TestROM/vect.c)

if(MSVC)
	set_property(TARGET NES_DEV APPEND_STRING PROPERTY COMPILE_FLAGS /W3)
elseif(CMAKE_COMPILER_IS_GNUCC)
	add_definitions(-Wall -Wextra -pedantic -std=c99)
	target_link_libraries(NES_DEV SDL2 SDL2main)

	if (CMAKE_SYSTEM_NAME STREQUAL Windows)
		add_definitions(-mwindows)
		target_link_libraries(NES_DEV mingw32)
	endif()
	
	if(CMAKE_BUILD_TYPE STREQUAL Release)
		add_definitions(-O3)
	endif()
endif(MSVC)

