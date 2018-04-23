Project_Name
============
- DAC



Intended purpose
================
- To demonstrate configuration and use of the DAC to produce an arbitrary, user-
  defined waveform of selectable frequency.
- The output can drive the on-board audio amplifier and speaker, if present, and/or
  be observed with an oscilloscope. A solder-jumper modification may be needed.



Hardware requirements
=====================
- Micro USB cable
- LPC8xx Xpresso Max board
- Personal Computer
- Oscilloscope (optional)



Board settings / external connections
=====================================
- Attach an oscilloscope to the DACOUT1 pin as defined in the chip spec.
- The DACOUT1 pin, by default, is connected to the max board's audio amplifier. Change a
  solder-jumper to make it appear on an Arduino header. See the board schematic for details.
  


Functional description
======================
- User input is via the debug UART.
- Connect a terminal emulator (9600/8/N/1) to the debug UART COM port.
- See chip_setup.h and Serial.c to setup the debug UART.

- The user is prompted to choose a note, and the defined waveform is output by the DAC at that frequency.



Program Flow
============
- The debug UART is configured and peripheral clocks are enabled as appropriate.
- The DACOUT function is enabled on its pin, and the pin is configured in IOCON 
  (an unfortunate, but necessary extra step).
- The DAC counter is given an initial value, the DAC is started, and code enters the main while 1 loop.
- The user is prompted to enter a note, and the DAC counter is recalculated to cycle through the
  defined waveform at that frequency.
- Entering '0' stops the DAC counter, and outputs a user-defined DC value for measurement. Reset the board to restart.
- Otherwise, loop again.

- This example runs at system_ahb_clk = 30 MHz. See SystemInit() and chip_setup.h.
- This example runs from Flash. 



To run this code
================
- Build, Debug, Run.
- or
- Download to flash, then reset.

- Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
      


