Project_Name
============
- Flash_IAP



Intended purpose
================
- To demonstrate Flash programming ROM API calls.



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
- See chip_setup.h and Serial.c to setup the debug UART.

- This example demonstrates usage of the PageErase, Copy Ram to Flash, and 
  Compare IAP commands to copy 64 (configurable) bytes from RAM to a page 
  (configurable) in Flash memory. 
- The ROM API routines are not accessed directly (although they could be), but
  are instead accessed via the drivers provided in utilities_lib. Please see
  the files iap.h, FlashFaimPrg.h, and FlashFaimPrg.c in utilities_lib.
- Detailed descriptions of all ROM APIs can be found in the User Manual.



Program Flow
============
- An array in memory is first initialized, then a user-supplied string is embedded in the array.
- The IAP Erase Page command, then the IAP Copy RAM to Flash commands are invoked via the 
  supplied drivers. The drivers themselves handle the IAP Prepare Sector command.
- The IAP compare command is then invoked, and if the flash contents match the RAM contents,
  a message is printed and the code enters a while 1.
- This example runs from Flash. 



To run this code
================
- Build, Debug, Run.
- or
- Download to flash, then reset.

- Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
      


