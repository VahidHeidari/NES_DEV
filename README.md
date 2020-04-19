 Introduction:
===============

NES_DEV is an open source NES emulator for educational and experimental purposes.

I want to port this emulator on a STM32F103ZET6 micro controller, and making
a portable and hand held game console, which can run on a low cost ARM device.
There is may NES emulators in the world, but none of them runs on low cost micro
controllers, most of them runs on PCs. I want to make a simple and easy to build
system, which anyone can make and enjoy it.

This emulator only depends on SDL library. You can get it from www.libsdl.org.

NES_DEV running on linux with enabled debug options.

![Linux](Images/linux.jpg?raw=true "NES_DEV on linux")

My STM32F103ZET board. I bought it from [ECA](http://eshop.eca.ir).

![STM32 board](Images/NESonSTM32.jpg?raw=true "NES on STM32")

Super Mario Bros. on ILI9325 TFT color LCD.

![Super Mario Bros.](Images/SuperMarioBros.jpg?raw=true "Super Mario Bros.")


 Known issues:
===============
* Sprite 0 hit flag not working properly in some conditions. I think that it is
because poor implementation of transparent color of palette.
* Mapper 4 not working perfectly.
* When PPU rendering is disabled, emulator doesn't render any thing (It must
render color 0 from palette).
* Clipping background and sprites not implemented yet.
* Generated .sln file for visual studio by cmake, doesn't understand start up
project. After opening project in visual studio, you must right click on NES_DEV
project and select it as start up project manually.

 Future works:
===============
* Better PPU and graphic implementation.
* Optimized and fast rendering for ARM devices.
* APU and sound implementation.
* Adding more mappers.
* Adding SD card file system for loading ROM images for ARM devices.
* Making a small game for tests.
* Adding a better build script for making TEST ROM.
* Make debug viewers flexible and user friendly, it is hardcoded, and needs to
be enabled/disabled from source.
* Designing low const, felexible, and extensible PCB for hand-held console.

 How to make project:
======================
* First get and install cmake build system, and get SDL library and install it.
* Make bin directory in project root directory with "mkdir bin" command.
* Go to bin directory with "cd bin" command.
* Run cmake with "cmake .." command.
* Build project with "make" command in linux environments or click on .sln file
to open visual studio.

 Key map:
==========
* Esc -> Quit emulator
* Arrow keys -> Directional buttons
* Key pad 1 or S key -> A button
* Key pad 2 or A key -> B button
* Key pad 7 or Enter key -> Start button
* Key pad 8 or Right Ctrl key -> Select button
* P key -> Toggle sprites
* B key -> Toggle background
* F1 to F4 keys -> Save game state into slot 1 to slot 4
* F5 to F8 keys -> Load game state from slot 1 to slot 4

 Tools and utilities:
======================
* iNesDPT : ines pattern table dumper.
* iNesInfo : Prints ines file information.
* iNesMk : ines header file maker.

 Unit test:
============
I used X6502 from FCEUX project, and M6502 from Marat Fayzullin project source
codes for my p6502 module testing. My test runs all modules and checks all CPU
contexts and RAMs with each other. If there is inconsistency, it fails. I found
that there is some bugs in M6502 module.
