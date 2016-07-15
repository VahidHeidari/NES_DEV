#!/bin/bash

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

cat 							\
Src/*.[c,h]						\
Src/ARM/*.[c,h]					\
Src/ARM/Graphics/*.[c,h]		\
Src/ARM/Joystick/*.[c,h]		\
Src/ARM/LCDDriver/*.[c,h]		\
Src/NES/*.[c,h]					\
Src/NES/Mappers/*.[c,h]			\
Src/TUI/*.[c,h]					\
Tools/*.[c,h]					\
Tools/iNesDPT/*.[c,h]			\
Tools/iNesInfo/*.[c,h]			\
Tools/iNesMk/*.[c,h]			\
UnitTest/6502/*.[c,h]			\
UnitTest/6502/M6502/*.[c,h]		\
UnitTest/6502/P6502/*.[c,h]		\
UnitTest/6502/X6502/*.[c,h]		\
TestROM/Asm/*.[c,h]				\
TestROM/Asm/*.asm				\
 | nl -ba | tail
