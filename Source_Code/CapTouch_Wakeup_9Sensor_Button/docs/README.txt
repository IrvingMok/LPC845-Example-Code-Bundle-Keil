Project_Name
============
- CapTouch_Wakeup_9Sensor_Button



Intended purpose
================
Example software for the Capacitive Touch module.



Hardware requirements
=====================
- Micro USB cable
- Debug probe
- Capacitive Touch Demo Boards (MP board, RWS and BUTT sensor boards)
- Personal Computer with terminal emulator and FreeMaster



Board settings / external connections
=====================================
- None.
  


Functional description
======================
- Output is via the debug UART (9600/8/n/1) only.
- This example runs at system_ahb_clk = 24 MHz. See SystemInit() and chip_setup.h.
- This example runs from Flash. 



To run this code
================
- Build, download to flash, then reset.
- It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
- This example is based on CapTouch_9Sensor_Button, but the MRT is configured to put
  the LPC into low power-mode after five seconds without a touch. A touch will wakeup the LPC.
- While the LPC is sleeping, reprogramming of the flash is not possible. You must first wake up the chip
  by either a touch or a reset.
- This example will neither run in a debug session nor with an active FreeMaster session.
- Only run with a terminal emulator connected to the debug UART COM port
- More details can be found in the app. notes associated with these examples.


      


