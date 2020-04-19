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

cmake_minimum_required(VERSION 2.8)
project(ines-info)
set(CMAKE_USE_RELATIVE_PATHS TRUE)
add_definitions(-g -std=c99 -Wall -Wextra)
include_directories(../../Src ../../Src/NES ../../Src/NES/Mappers)
add_executable(ines-info main.c ../../Src/NES/nes-hdr.c ../../Src/NES/Mappers/mappers-name.c)
