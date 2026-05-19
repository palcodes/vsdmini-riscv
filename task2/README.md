# Task 2 Process

This document details the implementation process of Task 2. 

## Setting up local development environment
1. Clone the repo using `git clone https://github.com/vsdsquadron-mini-riscv.git`
2. Run `make` to build the project. 
3. Ensure you have the board [VSDSquadron Mini] connected to your computer. Open a terminal to `ttyACM0` using 
> `screen -r ttyACM0 36500`.
4. Finally run `make flash` to flash the binary onto the board and the board will reset automatically, sending messages over UART to the host.

## Further reading
### Reading the Datasheet and Reference Manual
1. Day 1 began with exploring the scope of Task2, following the official VSDSquadron Mini guide and walked through understanding the hardware.  
2. Dove deep into the datasheet and Reference Manual to understand how the clocks, peripherals can be configured to get things working.
3. Used the provided blink example to compile and produce a binary that can be flashed the baord. 
4. Successfully flashed the board with the blink example, testing whether the Board button was properly resetting the program flashed onto the hardware. 

<img height="280" alt="image" src="./blinky.gif" />

### Setting up PlatformIO, Board bring-up
1. This involved setting up tools for cross-host compilation and flashing.
2. PlatformIO was helpful indeed and I was able to get the LED lighting up to my desired delay pretty quickly, but I also wanted to learn more about how startup, linking and Makefiles work.
3. So I set up a bare-metal project from scratch using the RISC-V toolchain and linker scripts to understand the build process and how linking memory space works.
4. Chose Pin 6 to program the LED on the board and test out my understanding of the toolchain, figure out any bugs or issues with the setup.
5. As guided the code was structured such that only `gpio.c` file will interact directly with the registers defined in the `gpio.h` file.
6. Main file only deals with the application logic as mentioned in the task description and only calls the GPIO setup and LED toggle functions.

### UART bare metal - No HAL
1. More details are shared in the `evidence.md` file about setting up a serial messaging with the host device.
2. This step involved reading the RM about USART1 and AFIO so they can configured to communicate with the Host device.
3. Understanding baud rate and the error correction also helped with dealing with improper and garbage values showing on the `tty/ACM0` serial port.
