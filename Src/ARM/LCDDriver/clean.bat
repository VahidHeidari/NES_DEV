@echo off

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

del /f /q ".\*.c!"
del /f /q ".\*.c~"
del /f /q ".\*.c@"
del /f /q ".\*.c_cbf"
del /f /q ".\*.fct"
del /f /q ".\*.map"
del /f /q ".\*.i"
del /f /q ".\*.h~"
del /f /q ".\*.prj~"
del /f /q ".\*.hdr"
del /f /q ".\*.h!"
del /f /q ".\*.h_cbf"
rmdir /s /q "Linker"
rmdir /s /q "List"
rmdir /s /q "Obj"

echo "Done."
rem pause
