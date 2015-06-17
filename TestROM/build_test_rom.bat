REM @echo off

REM NES_DEV is a cross-platform, portable, and hand-held NES emulator.
REM 
REM Copyright (C) 2015  Vahid Heidari (DeltaCode)
REM 
REM This program is free software: you can redistribute it and/or modify
REM it under the terms of the GNU General Public License as published by
REM the Free Software Foundation, either version 3 of the License, or
REM (at your option) any later version.
REM 
REM This program is distributed in the hope that it will be useful,
REM but WITHOUT ANY WARRANTY; without even the implied warranty of
REM MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
REM GNU General Public License for more details.
REM 
REM You should have received a copy of the GNU General Public License
REM along with this program.  If not, see <http://www.gnu.org/licenses/>.


del /q test-rom.bin test-rom.c
del /q pattern.c sp.c bg.c
del /q vect.bin
..\dasm32\DASM.exe test-rom.asm -f3 -otest-rom.bin && xxd -g1 -i test-rom.bin > test-rom.c
..\dasm32\DASM.exe vect.asm -f3 -ovect.bin && xxd -g1 -i -s0x3FFA vect.bin > vect.c

xxd -g1 -l0x60 -i sp.bin > sp.c
xxd -g1 -l0x60 -i bg.bin > bg.c
copy sp.c + bg.c pattern.c

REM ..\dasm32\DASM.exe test.asm -f3 && copy /b Header + A.OUT + mario.bin TEST.nes
pause
