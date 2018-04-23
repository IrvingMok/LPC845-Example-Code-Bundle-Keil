Project_Name
============
- CapTouch_3Sensor_Slider



Intended purpose
================
Example software for the Capacitive Touch module



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
- Output is via the debug UART (9600/8/n/1) and via the FreeMaster GUI.
- This example runs at system_ahb_clk = 24 MHz. See SystemInit() and chip_setup.h.
- This example runs from Flash. 



To run this code
================
- Build, download to flash, then reset.
- It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
- An active debug session and a FreeMaster session cannot run simultaneously, but FreeMaster and the terminal emulator can.
- Make sure "Plug-in Module: FreeMaster CMSIS DAP Communication Plug-in" is selected under project/options/Comm/Communication
- Make sure FreeMaster is pointing to the correct executable for the toolset you are using (go to project/options/map file)
  For Keil:        CapTouch_*/Objects/CapTouch_*.axf
  For IAR:         CapTouch_*/Flash/Exe/CapTouch_*.out
  For MCUExpresso: CapTouch_*/flash/Captouch_*.axf
- More details can be found in the app. notes associated with these examples.


      


