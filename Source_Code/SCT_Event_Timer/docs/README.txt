${ProjName}:
SCT_Event_Timer


Intended purpose:
 * To demostrate the configuration and use of the SCT to capture the elapsed time between events.
 * Output from this code is sent to the debug UART.
 * Use a terminal emulator set to 9600/8/N/1 connected to the debug UART COM port.
 * See Serial.c to setup the debug com port.



Functional description (also see accompanying JPEG):
 * Configure the SCT for unified, unidirectional mode, with state 0 only.
 * Falling edge on IN_PORT is Event 0.
 * Rising edge on IN_PORT is Event 1.
 * Event 0 causes the value of the timer to be captured in capture register 0, halts, and resets the timer.
 * Event 1 causes the value of the timer to be captured in capture register 1, halts, and resets the timer.
 * Both events cause an interrupt in which the ISR waits in a debounce loop, then sets a flag for main
   that a new captured value is available for the appropriate event.
 * Main outputs the elapsed time (in clocks) to the debug UART, restarts the SCT counter, and the process repeats.



Pins used in this example:
 * IN_PORT (defined in main.c) [Input]  - (falling edge = Event 0, rising edge = Event 1). Note: Internal pull-up is on.
 * IN_PORT can be floated or pulled-up externally, and then grounded to cause events 1 and 0. 
 * Grounding IN_PORT causes Event 0, pulling IN_PORT high causes Event 1.
 * The default IN_PORT may already be conected to a user push-button, with pullup, on the board. 
 * See accompanying JPEG in ../../Source_Code/${ProjName}/docs




Detailed Program Flow:
 * This example runs at 30 MHz. 
   See funtion SystemInit() and chip_setup.h.

 * main() routine
   1. Configure the switch matrix
      A. Assign IN_PORT to function SCT_PIN0
   2. Configure SCT0_INMUX0
      A. Select SCT_PIN0 function (which was connected to IN_PORT in the switch matrix) as SCT input SCT_IN0
   3. Configure the SCT
      A. Operate as one unified 32-bit counter in unidirectional mode
      B. Events 0 and 1 act as both limit events and halt events.
      C. Do not use states, always remain in State 0.
      D. Match/Capture registers 0 and 1 act as capture registers.
      E. Events 0 and 1 are enabled to cause interrupts.
      F. Event 0 causes capture[0] to be loaded, Event 1 causes capture[1] to be loaded.
      G. Event 0 is defined as a falling edge on input 0, with no match component.
      H. Event 1 is defined as a rising edge on input 0, with no match component.
      I. Interrupt is enabled and the SCT is started.
   4. Enter the main while(1) loop
      A. Wait for the handshake flag from the ISR.
      B. Based on which event occurred, read the appropriate CAP register.
      C. Output the count to the console, restart the SCT, and repeat.

 
This example runs from Flash. To run this code: 

* Build, Debug, Run or Go
  or
* Download to flash, Reset

Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.

                                




