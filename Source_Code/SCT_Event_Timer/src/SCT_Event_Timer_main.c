/*
 * SCT_Event_Timer_main.c
 *
 *  Created on: Apr 18, 2016
 *      Author:
 */


#include <stdio.h>
#include "LPC8xx.h"
#include "syscon.h"
#include "sct.h"
#include "swm.h"
#include "inmux_trigmux.h"
#include "utilities.h"
#include "uart.h"

extern void setup_debug_uart(void);

#define IN_PORT P0_4

volatile uint8_t ev0_handshake, ev1_handshake, interrupt_counter;

int main(void) {
  uint32_t clock_count;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();

  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (SWM|SCT);

  // Configure the SWM (see peripherals_lib and swm.h)
  ConfigSWM(SCT_PIN0, IN_PORT);   // SCT input 0, for input transition events (default internal pull-up remains on)

  // Write to SCT0_INMUX[0] to select SCT_PIN0 function (which was connected to IN_PORT in the switch matrix) as SCT input SCT_IN0
  LPC_INMUX_TRIGMUX->SCT0_INMUX0 = SCT_INMUX_SCT_PIN0;

  // Configure the SCT ...
  // Give the module a reset
  LPC_SYSCON->PRESETCTRL[0] &= (SCT0_RST_N);
  LPC_SYSCON->PRESETCTRL[0] |= ~(SCT0_RST_N);

  // Configure the CONFIG register
  // UNIFY counter, CLKMODE=busclock, CKSEL=unused(default), NORELOADL/H=unused(default), INSYNC=unused(default), AUTOLIMIT=false
  LPC_SCT->CONFIG |= (1<<UNIFY) |
                     (Bus_clock<<CLKMODE) |
                     (0<<AUTOLIMIT_L);

  // Configure the CTRL register
  // Don't run yet, clear the counter, BIDIR=0(default,unidirectional up-count), PRE=0(default,div-by-1)
  LPC_SCT->CTRL   |= (0<<Stop_L) |       // Stay in halt mode until SCT setup is complete
                     (1<<Halt_L) |       // Stay in halt mode until SCT setup is complete
                     (1<<CLRCTR_L) |     // Clear the counter (good practice)
                     (0<<BIDIR_L) |      // Unidirectional mode (Up-count)
                     (0<<PRE_L);         // Prescaler = div-by-1

  // Setup the LIMIT register
  // Events 0 and 1 serve as counter limits. The count automatically gets cleared to '0' by these events.
  LPC_SCT->LIMIT_L = 1<<event0 | 1<<event1;

  // Setup the HALT register
  // Events 0 and 1 will set the HALT_L bit in the CTRL register, so no further events can occur until halt is removed
  LPC_SCT->HALT_L = 1<<event0 | 1<<event1;

  // Setup the STOP register
  // No events will set the STOP_L bit in the CTRL reg.
  LPC_SCT->STOP_L = 0;

  // Setup the START register
  // No events will clear the STOP_L bit in the CTRL reg.
  LPC_SCT->START_L = 0;

  // Initialize the SCT unified COUNT register
  // Start counting at '0' (we already cleared it, but for purposes of demonstration, now we write '0' to it)
  LPC_SCT->COUNT = 0;

  // Initialize the STATE register
  // Start in state 0 (and STAY there!)
  LPC_SCT->STATE_L = 0;

  // Setup the REGMODE register
  // Match/Capture registers 0 and 1 act as capture registers, others are unused remain at default.
  LPC_SCT->REGMODE_L = 0x3;

  // Configure the OUTPUT register
  // Nothing to be done here since this example uses no outputs
  LPC_SCT->OUTPUT = 0;

  // Configure the OUTPUTDIRCTRL register
  // The counting direction has no impact on the meaning of set and clear for all outputs
  LPC_SCT->OUTPUTDIRCTRL = 0;

  // Configure the RES register
  // Simultaneous set and clear (which would be a programming error) has no effect for all outputs
  LPC_SCT->RES = 0;

  // Configure the EVEN register 
  // Enable Event flag interrupts for Events 0 and 1
  LPC_SCT->EVEN = 1<<event0 | 1<<event1;

  // Clear any pending event flags by writing '1's to the EVFLAG register
  LPC_SCT->EVFLAG = 0x3F;

  // Configure the CONEN register
  // Don't enable any 'no-change conflict' event flags to interrupt.
  LPC_SCT->CONEN = 0;

  // Clear any pending 'no-change conflict' event flags, and BUSSERR flags, by writing '1's to the CONLAG register
  LPC_SCT->CONFLAG = 0xFFFFFFFF;

  // Configure the Capture Control registers for the capture events we are defining (others are default)
  LPC_SCT->CAPCTRL[0].U = 1<<event0;
  LPC_SCT->CAPCTRL[1].U = 1<<event1;

  // Configure the EVENT_STATE and EVENT_CTRL registers for all events
  //
  // Event EVENT_STATE                        EVENT_CTRL
  // ----- ---------------------------------  -------------------------------------------------------------------------------------------
  // EV0   Enabled in State 0.                A falling edge on CTIN_0 is associated with this event, no effect on state.
  // EV1   Enabled in State 0.                A rising edge on CTIN_0 is associated with this event, no effect on state.
  // EV2   Disabled in all states.
  // EV3   Disabled in all states.
  // EV4   Disabled in all states.
  // EV5   Disabled in all states.
  LPC_SCT->EVENT[0].STATE = 1<<state0;           // Event 0 is enabled in State 0
  LPC_SCT->EVENT[1].STATE = 1<<state0;           // Event 1 is enabled in state 0
  LPC_SCT->EVENT[2].STATE = 0;                   // Event 2 is disabled (enabled in no states)
  LPC_SCT->EVENT[3].STATE = 0;                   // Event 3 is disabled (enabled in no states)
  LPC_SCT->EVENT[4].STATE = 0;                   // Event 4 is disabled (enabled in no states)
  LPC_SCT->EVENT[5].STATE = 0;                   // Event 5 is disabled (enabled in no states)
  LPC_SCT->EVENT[0].CTRL  = (0<<OUTSEL) |        // Select the input related to IOSEL below
                            (0<<IOSEL)  |        // Selects SCT PIN_0
                            (Fall<<IOCOND) |     // Falling edge
                            (IO_Only<<COMBMODE)| // No match component for this event
                            (0<<STATELD) |       // This event adds STATEV below to current state
                            (0<<STATEV);         // Adding 0=STATEV to current state, causes no change to state
  LPC_SCT->EVENT[1].CTRL  = (0<<OUTSEL) |        // Select the input related to IOSEL below
                            (0<<IOSEL)  |        // Selects SCT PIN_0
                            (Rise<<IOCOND) |     // Rising edge
                            (IO_Only<<COMBMODE)| // No match component for this event
                            (0<<STATELD) |       // This event adds STATEV below to current state
                            (0<<STATEV);         // Adding 0=STATEV to current state, causes no change to state
  LPC_SCT->EVENT[2].CTRL  = 0;                   // Event 2 is disabled, so value here is don't care
  LPC_SCT->EVENT[2].CTRL  = 0;                   // Event 3 is disabled, so value here is don't care
  LPC_SCT->EVENT[4].CTRL  = 0;                   // Event 4 is disabled, so value here is don't care
  LPC_SCT->EVENT[5].CTRL  = 0;                   // Event 5 is disabled, so value here is don't care

  // Configure the OUT registers for the 4 SCT outputs (there are no outputs in this example)
  LPC_SCT->OUT[0].SET = 0; // Unused output, set by no event (default)
  LPC_SCT->OUT[0].CLR = 0; // Unused output, cleared by no event (default)
  LPC_SCT->OUT[1].SET = 0; // Unused output, set by no event (default)
  LPC_SCT->OUT[1].CLR = 0; // Unused output, cleared by no event (default)
  LPC_SCT->OUT[2].SET = 0; // Unused output, set by no event (default)
  LPC_SCT->OUT[2].CLR = 0; // Unused output, cleared by no event (default)
  LPC_SCT->OUT[3].SET = 0; // Unused output, set by no event (default)
  LPC_SCT->OUT[3].CLR = 0; // Unused output, cleared by no event (default)

  // Initialize to '0' the handshake flags, will be set by ISR
  ev0_handshake = 0;
  ev1_handshake = 0;
  interrupt_counter = 0;

  // Enable the SCT interrupt in the NVIC
  NVIC_EnableIRQ(SCT_IRQn);

  // FINALLY ... now let's run it. Clearing bit 2 of the CTRL register takes it out of HALT.
  LPC_SCT->CTRL_L &= ~(1<<Halt_L);

  while(1) {

    // Wait here for SCT interrupt
    while (!interrupt_counter);
    interrupt_counter = 0;

    // Based on which event occurred, output the elapsed time in clock counts
    if (ev0_handshake) {
      clock_count = LPC_SCT->CAP[0].U;
      ev0_handshake = 0;
      printf("Number of clocks from rising-to-falling was: %.10d\n\r", clock_count);
    }
    else {
      clock_count = LPC_SCT->CAP[1].U;
      ev1_handshake = 0;
      printf("Number of clocks from falling-to-rising was: %.10d\n\r", clock_count);
    }

    // Restart the timer
    LPC_SCT->CTRL &= ~(1<<Halt_L);

  } // end of while(1)
}  // end of main

