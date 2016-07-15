#!/bin/bash
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
 | nl -ba
