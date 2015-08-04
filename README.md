 Introduction:
===============

NES_DEV is an open source NES emulator for educational and experimental purposes.

I want to port this emulator on an STM32F103ZET6 micro controller, and making
a portable and hand held game console, witch can run on a low cost ARM device.

My STM32F103ZET board.

![STM32 board](NESonSTM32.jpg?raw=true "NES on STM32")

Super Mario Bros. on an ILI9325 TFT color LCD.

![Super Mario Bros.](SuperMarioBros.jpg?raw=true "Super Mario Bros.")


 Known issues:
===============
* Sprite 0 hit flag not working properly in some conditions.
* Mapper 4 not working perfectly.
* When PPU rendering is disabled, emulator doesn't rendering any thing (It must render color 0 from palette).
* Clipping background and sprites not implemented yet.
* Generated .sln file for visual studio by cmake, don't understand start up project. After opening project in visual studio, you must right click on NES_DEV project and select it as start up project manually.

 Future works:
===============
* Better PPU and graphic implementation.
* Optimized and fast rendering for ARM devices.
* APU and sound implementation.
* Adding more mappers.
* Adding SD card file system for loading ROM images for ARM devices.
* Making a small game for tests.
* Adding a better build script for making TEST ROM.

 How to make project:
======================
* Make bin directory in project root directory with "mkdir bin" command.
* Go to bin directory with "cd bin" command.
* Run cmake with "cmake .." command.
* Build project with "make" command in linux environments or click on .sln file to open visual studio.

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
* F1 to F4 keys -> Save game state to slot 1 to slot 4
* F5 to F8 keys -> Load game state from slot 1 to slot 4
