Project_Name
============
- ROM_Divide



Intended purpose
================
- To demonstrate the usage of the ROM-based integer divide routine APIs.
- Two methods are used: Direct ROM API function calls, and overloaded "/" and "%" 
  EABI function calls.



Hardware requirements
=====================
- Micro USB cable
- LPC8xx Xpresso Max board
- Personal Computer



Board settings / external connections
=====================================
- None



Functional description
======================
- User input is via the debug UART.
- Connect a terminal emulator (9600/8/N/1) to the debug UART COM port.
- A board modification may be required to use the debug COM port. Please refer
  to the board schematic.
- See chip_setup.h and Serial.c to setup the debug UART.


- Divide routines are called with user defined divisors and dividends, using both methods.
- The results are displayed to the terminal.



- This example runs at system_ahb_clk = 30 MHz. (main_clk = 30 MHz. FRO direct output). 
  See SystemInit() and chip_setup.h.
- This example runs from Flash. 



To run this code
================
- Build, Debug, Run.
- or
- Download to flash, then reset.

- Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
      


