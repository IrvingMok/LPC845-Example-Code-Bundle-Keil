Project_Name
============
- FAIM_and_Clocks



IMPORTANT DISCLAIMER, PLEASE READ CAREFULLY AND UNDERSTAND EXPLICITLY!
======================================================================
- Reprogramming of the FAIM memory with hastily chosen values can limit the  
  ability to communicate with your device via a debugger, or force it into a
  state of perpetual reset.
- The FAIM memory is only rated for 200 write cycles.



Intended purpose
================
- To demonstrate initial clock setup of the device using the chip_setup.h file,
  while monitoring internal clock frequencies and clock out. 
- To demonstrate write and read of the FAIM memory using ROM API calls.



Hardware requirements
=====================
- Micro USB cable
- LPC8xx Xpresso Max board
- Personal Computer
- Oscilloscope (optional)
- External clock source (optional)



Board settings / external connections
=====================================
- Attach an oscilloscope to the CLKOUT_PIN as defined in the main file.
- Provide an external clock on the CLKIN pin if desired (be sure settings in chip_setup.h are in accordance).



Functional description
======================
- User input is via the debug UART.
- Connect a terminal emulator (9600/8/N/1) to the debug UART COM port.
- See chip_setup.h and Serial.c to setup the debug UART.

- In the main file, there is a global array variable 'faim_write_data' which is declared and initialized.
  -- The initial values reflect the FAIM contents that should be programmed in a new part at the factory.
  -- Reprogramming any FAIM row using these values is safe.
  -- If you wish to change these values, be very careful as there can be severe consequences.
  -- Bit[1] of FAIM word 0 is the low power start bit. If programmed to '1', fro_clk will be fro_osc_out
     divided by 16 (instead of fro_osc_out divided by two).
  -- Only after a reset, will a reprogrammed low power start bit take effect.
  -- The debug UART baud rate may need to be manually adjusted, if low power start = 1, depending on other settings.

- The ROM API routines are not accessed directly (although they could be), but
  are instead accessed via the drivers provided in utilities_lib. Please see
  the files iap.h, FlashFaimPrg.h, and FlashFaimPrg.c in utilities_lib.
- Detailed descriptions of all ROM APIs can be found in the User Manual.



Program Flow
============
- The chip clocking is configured according to macros defined in chip_setup.h (see function SystemInit()).
- The debug UART is configured and peripheral clocks are enabled as appropriate.
- The clock out function is assigned to a package pin using the switch matrix, and given an initial divider value.
- Main while (1) loop
  - Display an internal clock node frequency (and route it to clock out where applicable), or type 's' to skip.
  - Read a FAIM row and display its value, or type 's' to skip.
  - Reprogram a FAIM row then display its value, or type 's' to skip.
  - Loop again.

- Initially, this example runs at system_ahb_clk = 9 MHz. (main_clk = 18 MHz. FRO dividers output).
  See SystemInit() and chip_setup.h.
- Users are encouraged to experiment with different clock setups by modifying chip_setup.h, 
  recompiling, reprogramming, then executing a reset.
- This example runs from Flash. 



To run this code
================
- Build, Debug, Run.
- or
- Download to flash, then reset.

- Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
      


