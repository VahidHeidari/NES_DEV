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
project(cc65-test)

# Set development environment, to use cc65 compiler
set(CMAKE_USE_RELATIVE_PATHES TRUE)
set(CMAKE_C_COMPILER "cc65/win/bin/cl65.exe")
set(NES_LIB cc65/win/lib/nes.lib)
include_directories(cc65/include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -t nes")

include_directories(Hardware)

add_executable(cc65-test.nes main.c)
target_link_libraries(cc65-test.nes ${NES_LIB})
