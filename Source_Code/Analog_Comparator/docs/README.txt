${ProjName}:
Analog_Comparator


Intended purpose:
* To demonstrate a simple configuration and use of the Analog Comparator module.
* User input is via the debug UART.
* Connect a terminal emulator set to 9600/8/N/1 to the debug UART COM port.
* See chip_setup.h and Serial.c to setup the debug com port.


Functional description / configuration (please see the Comparator block diagram in the lpc8xx user manual):
 * Voltage ladder reference is Supply Vdd
 * Positive comparator input is voltage ladder out
 * Negative comparator input is internal 0.9V band-gap
 * No hysteresis.
 * Both rising and falling edges generate the comparator interrupt.
 * The available voltage values produced by the voltage ladder are roughly 0 - 3.3V in 0.1V increments:
   (SupplyVdd - Vss) / 32 = (3.3V / 32) ~= 0.1V






Program Flow:
* This example at 30 MHz. See chip_setup.h and SystemInit().

* main() routine
  1. Acomp is powered on, clocks are enabled, and the module is configured as described above. 

  2. In the main while(1) loop the user is prompted to enter a 2-digit hex number (only the 5 LSBs are used).

  3. The number entered by the user is written to the voltage ladder select field.
  
  4. Values from 0x0 to 0x8 produce voltages from 0 to 0.8V which are below the 0.9V band gap reference.
     In these cases the red LED is lit.
     
  5. Values from 0x9 to 0x1F produce voltages from 0.9V to 3.3V which are above the 0.9V band-gap reference.
     In these cases the green LED is lit.

  6. Whenever a number is entered which causes the comparator to change state, a falling or rising edge is detected
     which generates an interrupt. The ISR reads the COMPSTAT bit to determine the state of the comparator output, and
     lights the appropriate LED before returning.
  

  
This example runs from Flash.


To run this code: 
* Build, Debug, Run
or
* Download to flash, then reset.

Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.

