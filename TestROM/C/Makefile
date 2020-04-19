#
# NES_DEV is a cross-platform, portable, and hand-held NES emulator.
#
# Copyright (C) 2015-2020 Vahid Heidari (DeltaCode)
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

ASM=cc65/win/bin/ca65.exe
CC=cc65/win/bin/cl65.exe
LD=cc65/win/bin/ld65.exe
C_FLAGS=-t nes -IHardware -Icc65/include --add-source -Oi

all: main.o
	$(LD) -C cc65-test.cfg nes.o ppu.o main.o runtime.lib -o cc65-test.nes

main.o : main.c Hardware/ppu-memory.h ../Asm/ppu.asm
	$(ASM) --cpu 6502 nes.asm -o nes.o
	$(ASM) --cpu 6502 ../Asm/ppu.asm -o ppu.o
	$(CC) $(C_FLAGS) -c main.c -o main.o

clean:
	rm -rf *.o *.nes *~ *.deb
