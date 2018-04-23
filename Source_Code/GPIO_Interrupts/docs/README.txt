${ProjName}:
GPIO_Interrupts

Intended purpose:
* To demonstrate the configuration and use of the pin interrupts function.
* User input is via the debug UART.
* Connect a terminal emulator, set to 9600/8/N/1, to the debug UART COM port.
* See Serial.c and chip_setup.h to setup the debug com port.



	// The pin interrupts can be activated in two ways:
	//
	// 1. By configuring the port bits as inputs, and driving the package pins high and low externally.
	//
	// 2. By configuring the port bits as outputs, and writing to their port bit registers via software.
	//    The enabled GPIO pin interrupt logic will always monitor the state of the pins,
	//    even when configured as outputs.
	//
	// We will use approach #2.


Program Flow:
* This example runs with:
    system_ahb_clk = 30 MHz. (main_clk = 30 MHz. FRO direct output)
    See chip_setup.h and SystemInit()

* main() routine
  1. Clock to the GPIO module is enabled and the module is configured for GPIO interrupts.
  
  2. P0.15 and P0.14 are configured as outputs driving '0'.
  
  3. P0.15 and P0.14 are configured as PINTSEL1 and PINTSEL0
  
  4. Both PINTSELs are made rising and falling edge sensitive.
  
  5. Both PIN interrupts are enabled, and code enters a while(1) loop.
  
  6. Execution pauses while awaiting user input of one hex digit (with no prefix) via the terminal.
  
  7. Bits 1 and 0 of the hex digit are written to P0.15 and P0.14, and the loop repeats.
  
  8. A rising edge interrupt on PININT 0 turns on the red LED, and a falling edge turns it off.
     A rising edge interrupt on PININT 1 turns on the green LED, and a falling edge turns it off.
     
     * Note: On boards with a tri-color LED, when both the red and green LEDs are lit, one of the LEDs may be barely visible.

* For example, the sequence '0' '1' '0' or 'C' 'D' 'C' will turn on, then off, the red LED.
  The sequence '2' '0' '2' or '6' '4' 'A' will turn on, then off, the green LED.


To run this code: 
* Build, Debug, Run
or
* Download to flash, then reset


Note: With IAR it may be necessary to power-cycle the board in order to regain control of the reset button after programming.
                                


