${ProjName}:
SCT_Event_Counter


Intended purpose:
* A simple program to demostrate the configuration and use of the SCT as an external event counter.
* Output from this code is sent to the debug UART.
* Use a terminal emulator set to 9600/8/N/1 connected to the debug UART COM port.  
* See Serial.c to setup the debug com port.



Functional description:
 * This example uses the SCT Low counter (UNIFY=0, CLKMODE=2, CKSEL=0) in external clock mode to count external pulses.
 * The WKT (clocked by the Low Power Oscillator) is used to generate a slow interrupt (~10 KHz.).
 * Each WKT interrupt toggles a pin, which generates a roughly 5 KHz pulse train, which is connected
   externally to the SCT input, and clocks the SCT. The SCT count will increment on every incoming clock pulse.
 * The MRT is used to generate one interrupt per second which sets a handshake flag for main.
 * In the main while(1) loop, each time the once-per-second flag becomes set, the accumulated count
   during the past second is printed out, and the process repeats.
 * Because the Low Power Oscillator is accurate only within 40% of 10 KHz, the pulse counts will be somewhere in
   the 3000 - 7000 per second range.



Pins used in this example, defined in ${ProjName}.h:
* OUT_PORT[OUT_BIT]: This is the output toggled by the WKT ISR. Connects with IN_PORT externally.
* IN_PORT: This is configured as the the SCT_PIN0 input which will clock the SCT.
External connections:
* OUT_PORT[OUT_BIT] connected to IN_PORT. 



Detailed Program Flow:
* This example runs at 30 MHz.
  See funtion SystemInit() and chip_setup.h.

* main() routine
   1. Configure the WKT
      A. Run WKT from LPOSC
      B. Enable the WKT interrupt
      C. Start it
   2. Configure the MRT channel 0
      A. Repetitive interrupt mode
      B. Interrupt enabled.
      C. Reload value = once-per-second, and start it.
   3. Configure the switch matrix
      A. Assign Port0.0 to function SCT_PIN0
   4. Configure SCT0_INMUX0
      A. Select SCT_PIN0 function (which was connected to P0.0 in the switch matrix) as SCT input SCT_IN0
   5. Configure the SCT
      A. Operate as two individual 16-bit counters, the L counter only is used
      B. External clock mode, clocked on rising edges of SCTIN_0
   6. Enter the main while(1) loop
      A. Wait for the MRT once-per-second interrupt.
      B. Read the current SCT COUNT_L value, halt, reset, and restart COUNT_L.
      C. Output the count to the console, and repeat.
 
This example runs from Flash. 


To run this code: 
* Build, Debug, Run
or
* Download to flash, reset 

Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.






