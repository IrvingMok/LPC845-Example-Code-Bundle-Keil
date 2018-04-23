${ProjName}:
PMU_Modes


Important note: It is recommended to NOT run this example from within a debug
                session, as the software enters low-power modes in which the 
                debugger may lose connection.


Intended purpose:
* To demonstrate the entry into, and wakeup from, the four low-power modes, 
  while allowing the user to measure current consumption and wakeup time.


Power mode        Typical VDD_main current running from 12 MHz FRO
----------        ------------------------------------------------
while(1)          2.15  mA
Sleep             1.20  mA
Deep sleep        0.209 mA
Power down        1.71  uA
Deep power down   0.340 uA


Important note:
* If TARGET_RX on the MAX board is connected via solder-jumper to the LPC11U35, you can 
  connect the terminal emulator to the VCOM serial port instead of using a USB-to-UART
  cable, but the currents you measure may be higher than given above, which were measured 
  at room temperature on one MAX board with the solder-jumper removed.


Functional description:
* Input/output is accomplished by UART0 communicating with a
  terminal emulator running on the PC, via a USB-to-RS232 breakout cable.
  (Setup the terminal for 9600/8/N/1 and connect the breakout cable as described below
  in 'External connections')
  
  




* In order to measure the target's current consumption, the Max board
  must be modified as follows (please see the Max board schematic):
  1. Remove the resistor shunting JP2.
  2. Install a male header (HDR-1x02) across JP2 to accept the multimeter clip leads.
     When not using a meter across JP2, don't forget to insert a jumper there instead.
  3. Voltage drop may be measured across P1 as an alternative.

* Port pin P0.4 (a.k.a. the wakeup pin) is used to wakeup the target from the low power modes.
  SW1 on the Max board is connected to this pin and is externally pulled high, grounded when depressed.

* A falling edge on P0.4 is used to trigger the wakeup interrupt from Sleep, Deep-sleep,
  and Power-down modes.

* Grounding an EXTERNALLY pulled up P0.4 will wakeup the target from Deep power-down mode.
  P0.4 MUST be pulled high externally to enter Deep power-down mode.
  On the Max board this pullup may already be in place.

* The user is prompted to enter '0', '1', '2', or '3' to enter
  Sleep, Deep-sleep, Power-down, or Deep power-down modes, respectively.

* The target enters the low power mode selected, and is awakened by grounding P0.4.


External connections:
* RS232 breakout cable RXD and TXD, plus a ground connection.

* // Connect UART0 TXD, RXD signals to port pins 
* P0.25 (selectable in chip_setup.h) = U0_TXD = breakout cable RXD
* P0.24 (selectable in chip_setup.h) = U0_RXD = breakout cable TXD
* Board GND = breakout cable GND




Program Flow:
* This example runs from the default 12MHz FRO.

* main() routine
  1. Clocks are enabled. 
  
  2. The BOD is powered down.

  3. Internal pull-ups (and pull-downs) are disabled in IOCON.
  
  4. GPIO ports are configured.
  
  5. P0.4 is configured as PININT7, falling edge sensitive, made the wakeup source, and interrupt enabled.
  
  6. The SWM is configured for UART0 TxD and RxD on the appropriate pins.
  
  7. UART0 and its clocking are configured for 9600, 8 bits, no parity, one stop bit.

  8. USART0 Rx Ready interrupt is enabled, some clocks are disabled,
     and code execution enters the main while(1) loop.
 
  9. The user is prompted to enter '0', '1', '2', or '3' followed by [Enter], to enter one of the low power modes
     (Sleep, Deep-sleep, Power-down, or Deep power-down respectively).

  10. A logic low on P0.4 will wake up the target and the process repeats.
  

  
  
 
This example runs from Flash. 


To run this code: 
  1. Build
  2. Program the image to flash 
  3. Launch your terminal emulator and connect to the appropriate USB COM port (use 9600/8/N/1)
  4. Reset and follow the prompts.
  
Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.


                                



