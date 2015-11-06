#!/bin/bash

rm -rf test-rom.bin TEST.nes
dasm32 test-rom.asm -f3 -otest-rom.bin && cat Header test-rom.bin sp.bin bg.bin > TEST.nes
