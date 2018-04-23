Project_Name
============
- DeepPD_WKT



Intended purpose
================
- To demonstrate the configuration of the Wake up timer clocked by the Low power
  oscillator, to awaken the chip from Deep power-down mode, while allowing the user
  to measure current consumption if desired.



Hardware requirements
=====================
- Micro USB cable
- LPC8xx Xpresso Max board
- Personal Computer
- Digital muti-meter (optional)



Board settings / external connections
=====================================
- In order to measure the target's current consumption, the Max board
  must be modified as follows (please see the Max board schematic):
  1. Remove the resistor shunting JP2.
  2. Install a male header (HDR-1x02) across JP2 to accept the multimeter clip leads.
     When not using a meter across JP2, don't forget to insert a jumper there instead.
  3. Voltage drop may be measured across P1 as an alternative.
- Important note:
  If TARGET_RX on the MAX board is connected via solder-jumper to the LPC11U35, you can 
  connect the terminal emulator to the VCOM serial port instead of using a USB-to-UART
  cable, but the current you measure may be higher than given below, which was measured 
  at room temperature on one MAX board with the solder-jumper removed.
  
  Power mode        Typical VDD_main current running from 12 MHz FRO with LPOSC enabled
  ----------        -------------------------------------------------------------------
  while(1)          2.15  mA
  Deep power-down   1.22 uA



Functional description
======================
- This example uses the Self-wake-up timer (WKT), clocked by the Low Power 
  Oscillator, to wake up the chip from Deep power-down.
- The LPOSC is powered and enabled to stay on in Deep power-down
- The WKT is enabled as the wakeup source, and the LPOSC is selected as its
  clock source.
- Unused analog blocks are powered down, unused clocks are gated off.
- Upon user input via the debug UART, the WKT is started and Deep power-down is
  entered.
- The WKT will count down for five seconds then generate a wakeup interrupt.



Program Flow
============
- This example runs at system_ahb_clk = 12 MHz. See SystemInit() and chip_setup.h.
- This example runs from Flash. 



To run this code
================
- Build, download to flash, then reset.
- Because this code enters deep power-down mode, it is recommended not to run inside
  a debug session.

- Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.
      


