/*
  initial: 31.1.2015
  implement fetch execute cycle, interrupts

*/

some ideas for instruktions below


		       (A 6502 microprocessor simulator)

		copyright 1996 by Daniel Boris (dan.boris@coat.com)

    6502sim is a 6502 microprocessor simulator that I wrote for people
who want to learn to program the 6502. 

The current version of 6502sim can be found at:

	http://www.geocities.com/SiliconValley/9461

---------------------------------------------------------------------------
The Emulator:

	This simulator emulates a imaginary 6502 system. The current system
uses the following address:

$0000 - $3fff   RAM
$4000 - $7fff   Reserved for future I/O.
$8000 - $FFFF   ROM

The simulator commands listed below are free to write to both the RAM and
the ROM, but the 6502 code that is being run cannot write to the ROM.

---------------------------------------------------------------------------
Display:

The display is broken up into a series of boxes:

DISASM - This box shows the disassembly of memory. Each line shows the
	 address, the bytes that make up the instruction, and the
	 disassembly of the instruction. This box will follow the
	 program counter and can be set using the DIS command. A line
	 that is highlighted green is the current position of the program
	 counter, and a line highlighted red is a break point.

STATUS - This box shows the bits in the processor status register.

STACK  - This box shows the contents of the stack. The first part of each
	 line is the stack position (note that this is the position as
	 represented by the stack pointer, the actual RAM addresses of the
	 stack are $100 - $1ff). The second part of each line is the
	 value stored on the stack. The green highlighted line is the
	 current stack pointer position.

REG    - This box shows the processor registers. The registers are as follows:

	 CLK = This is a representation of the system clock. As each 6502
	       instruction is executed this value is incremented by the
	       number of clock cycles that instruction would take.
	 PC  = Program counter
	 A   = Accumulator
	 X   = X register
	 Y   = Y register
	 SP  = Stack pointer
	 S   = Processor status register

MEMORY  - This box shows a portion of the simulators memory. The memory 
	  area that is displayed can be set with the MD command.

FUTURE EXPANSION - This section of the screen is reserved for future 
	  features. I hope to add some simulator I/O chips and peripherals
	  in the future.

INPUT -   The box at the bottom of the screen is where you type commands.

---------------------------------------------------------------------------
	
Conventions: 
	n - a hex number (1 byte)
	a - a hex address (2 bytes)
	[file] - a DOS file name

Commands:
	MD a    - Set the position of the memory display window to address a.

	DIS a   - Set the position of the disassemble window to address a.

	QUIT    - Quit the simulator.

	STEP    - Single step next instruction.

	RUN n   - Execute n instructions. Execution will stop after n
		  instructions have been executed, or a break point is 
		  reached.

	BREAK a - Set a breakpoint at address a. Breakpoints will only
		  work if they are set on the address of the first byte
		  of an instruction. If a breakpoint is set on the address
		  of the 2nd or 3rd byte of an instruction it will not be
		  recognized.

	DELAY n - Sets an n milli-second delay between each instruction that
		  is executed while the emulator is running. This slow down
		  will allow you to watch registers and memory change as the
		  program is running.

	LOAD [file] - Loads a file into the emulators memory. The load command
		  support 2 file formats each designated by their extensions:

		  .BIN - A raw binary file. Binary files will be loaded into
		  memory so that the last byte in the file will end up at
		  address $FFFF. So if you load a file that is $1000 bytes
		  long, it will load into memory from $F000 to $FFFF.

		  .HEX - Intel Hex format. These file have all the address
		  information included in them, so the data will be loaded
		  to the appropriate addresses.

	BLOAD [file] start len - This command loads a raw binary file to
		  a specific address in memory. The emulator will load
		  len byte from the file and put them into memory starting
		  at the address start.
	RESET   - This is similar to doing a hardware reset of the processor.
		  
	IRQ     - This simulates the proccessor reciving and IRQ signal.

	NMI     - This simulates the processor recieving an NMI signal.

	AC n    - Load accumulator with the value n.

	XR n    - Load the X register with the value n.

	YR n    - Load the Y register with the value n.

	MM a n1 n2 ... nX - This command allows you to modify the contents
		of the emulators memory. 'a' is that address at which you
		want to change bytes, and 'n1 n2 ... nX' are the byte you
		want to put into memory starting at that location. You can
		specify as many bytes to put into memory as you want. For
		example:

		MM 1000 01 02 04

		This will put $01 at $1000, $02 at $1001, and $04 at $1002

	FILL start end data - This command fills memory with the value 'data'
		starting at address 'start' and ending at address 'end'. For
		example:

		FILL 1000 2000 00

		This is fill locations $1000 through $2000 with $00.


Revision History:

v1.00: 3/1/97 First Public Release
V1.01: 11/12/97
        - Fixed bug in .HEX file load routine that caused crashes
        - Input box is now redraw after help command
        - Put title and version number at top of help screen

