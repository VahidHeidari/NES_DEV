cmake_minimum_required(VERSION 2.8)

project(unit-test-6502)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_USE_RELATIVE_PATHS true)

add_executable(unit-test-6502
	M6502/Debug.c
	M6502/M6502.c
	main.cpp
	P6502/cycles.c
	P6502/p6502.c
	write-read.c
	X6502/ops.inc
	X6502/x6502.cpp)

include_directories(M6502 P6502 X6502 .)
