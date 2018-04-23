${ProjName}:
SCT_PWM_2State


Intended purpose:
 * To demostrate the configuration and use of the SCT to generate two PWMs with different duty cycles.


The following will make more sense while refering to ${ProjName}.jpeg in Source_Code/${ProjName}/docs.


Functional description:
 * One external pin (IN_PORT defined in main.c) is configured as SCT_PIN0, and selects between states.
 * External pins P0.x (green LED) and P0.y (red LED) are configured as SCT_OUT0 and SCT_OUT1 respectively
   (see board.h for actual ports).
 * Falling edge on SCT_IN0 is defined as Event 2, which is enabled only in State 0, and causes State to go from 0 to 1.
   In State 1 the red LED flashes with a small on-time duty cycle (red LED is on when SCT_OUT1 is low).
 * Rising edge on SCT_IN0 is defined as Event 5, which is enabled only in State 1, and causes State to go from 1 to 0. 
   In State 0 the green LED flashes with a large on-time duty cycle (green LED is on when SCT_OUT0 is low).
 * SCT counter is configured for unified, unidirectional mode, with autolimit. I.e., when COUNT = MATCH0, count resets.
   This determines the period of the PWM waveform.
 * COUNT = MATCH[1] is defined as Event 0, which is enabled only in State 0, and clears SCT_OUT0 to '0'.
 * COUNT = MATCH[2] is defined as Event 1, which is enabled only in State 0, and sets SCT_OUT0 to '1'.
 * COUNT = MATCH[3] is defined as Event 3, which is enabled only in State 1, and clears SCT_OUT1 to '0'.
 * COUNT = MATCH[4] is defined as Event 4, which is enabled only in State 1, and sets SCT_OUT1 to '1'.
 
 

Pins used in this example:
 * IN_PORT [Input]         - SCT_IN0  (falling edge = Event 2, rising edge = Event 5). Note: Internal pull-up is on.
 * green led port [Output] - SCT_OUT0
 * red led port   [Output] - SCT_OUT1
 * IN_PORT can be floated or pulled-up externally, and then grounded to cause events 2 and 5. 
 * Grounding IN_PORT causes Event 2, pulling IN_PORT high causes Event 5.
 * The default IN_PORT may already be connected to a user push-button, with pullup, on the board.




Detailed Program Flow:
* This example runs at 30 MHz. See chip_setup.h and funtion SystemInit().

* main() routine
   1. Configure the SWM as described above.
   2. Configure the SCT
      A. Operate as one unified 32-bit counter, clocked by bus clock, with autolimit, in unidirectional mode.
      B. No events will limit, halt, stop, or start the counter.
      C. Initial COUNT = 0, initial State = 0.
      D. All Match/Capture registers act as match registers.
      E. Initialize SCT_OUT0 and SCT_OUT1 to '1'.
      F. Initialize the Match registers for the PWM period, and the duty cycles.
      G. EV0 is enabled in State 0. A match in Match1 is associated with this event, no effect on state.
      H. EV1 is enabled in State 0. A match in Match2 is associated with this event, no effect on state.
      I. EV2 is enabled in State 0. A match in Match0 AND SCT_IN0=low is associated with this event, changes State from 0 to 1.
      J. EV3 is enabled in State 1. A match in Match3 is associated with this event, no effect on state.
      K. EV4 is enabled in State 1. A match in Match4 is associated with this event, no effect on state.
      L. EV5 is enabled in State 1. A match in Match0 AND SCT_IN0=high is associated with this event, changes state from 1 to 0.
      M. Event 0 clears output SCT_OUT0 to '0' (green LED on), Event 1 sets output SCT_OUT0 to '1' (green LED off).
      N. Event 3 clears output SCT_OUT1 to '0' (red LED on), Event 4 sets output SCT_OUT1 to '1' (red LED off).
      O. Run the counter (clear the 'Halt' bit), and put the CPU into Sleep mode, since it is no longer needed.
      

 
This example runs from Flash. 


To run this code: 1. Build
                  2. Debug
                  3. Run or Go
              or:
                  1. Program to flash
                  2. Reset the target

Note: It may be necessary to power-cycle the board in order to regain control of the reset button after programming.

                                



 
