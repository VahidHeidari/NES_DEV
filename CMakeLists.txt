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

project(NES_DEV)

add_subdirectory(Src)
add_subdirectory(UnitTest/6502)
add_subdirectory(Tools/iNesInfo)
add_subdirectory(Tools/iNesMk)
add_subdirectory(Tools/iNesDPT)
