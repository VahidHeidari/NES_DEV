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

CC=cc65/win/bin/cl65.exe
C_FLAGS=-t nes -IHardware -Icc65/include --add-source -Oi
LINK_FLAGS=-Lcc65/win/lib

all: main.o
	$(CC) $(C_FLAGS) $(LINK_FLAGS) -C cc65-test.cfg main.o -o cc65-test.nes

main.o : main.c Hardware/ppu-memory.h
	$(CC) $(C_FLAGS) -c main.c -o main.o

clean:
	rm -rf *.o *.nes *~ *.deb
