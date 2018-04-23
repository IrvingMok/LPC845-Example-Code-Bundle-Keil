/*
===============================================================================
 Name        : SCT_Event_Counter.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include <stdio.h>
#include "LPC8xx.h"
#include "syscon.h"
#include "sct.h"
#include "swm.h"
#include "inmux_trigmux.h"
#include "utilities.h"
#include "SCT_Event_Counter.h"


extern volatile uint8_t handshake;


int main(void) {
  uint32_t pulse_count;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();
	
  // Configure and start the WKT, also configures an output pin, which toggles approximately at 10 KHz
  WKT_Config();

  // Configure and start the MRT for once-per-second interrupt
  MRT_Config();

  // Enable clocks to peripherals
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (SWM|SCT);

  // Configure the SWM
  ConfigSWM(SCT_PIN0, IN_PORT); // Assign the input pin to function SCT_PIN0

  // Write to SCT0_INMUX[0] to select SCT_PIN0 function (which was connected to IN_PORT in the switch matrix) as SCT input SCT_IN0
  LPC_INMUX_TRIGMUX->SCT0_INMUX0 = SCT_INMUX_SCT_PIN0;

  // Configure the SCT ...
  // Give the module a reset
  LPC_SYSCON->PRESETCTRL0 &= (SCT0_RST_N);
  LPC_SYSCON->PRESETCTRL0 |= ~(SCT0_RST_N);

  // Configure the CONFIG register
  // Two 16-bit counters, CLKMODE=input, CKSEL=in0re, NORELOADL/H=unused(default), INSYNC=unused(default), AUTOLIMIT=false
  LPC_SCT->CONFIG |= (0<<UNIFY) |
                     (Input<<CLKMODE) |
                     (In0re<<CKSEL);


  // Configure the CTRL register
  // Don't run yet, clear the counter, BIDIR=0(default,unidirectional up-count), PRE=0(default,div-by-1)
  LPC_SCT->CTRL_L |= (0<<Stop_L) |       // Stay in halt mode until SCT setup is complete
                     (1<<Halt_L) |       // Stay in halt mode until SCT setup is complete
                     (1<<CLRCTR_L) |     // Clear the counter
                     (0<<BIDIR_L) |      // Unidirectional mode (Up-count)
                     (0<<PRE_L);         // Prescaler = div-by-1

  // Now run it. Clearing bit 2 of the CTRL register takes it out of HALT.
  LPC_SCT->CTRL_L &= ~(1<<Halt_L);

  while(1) {

    // Wait here for the once-per-second handshake flag
    while(!handshake);
    handshake = 0;

    // Read the L counter, halt it, reset it, restart it
    pulse_count = LPC_SCT->COUNT_L;      // Read
    LPC_SCT->CTRL_L |= (1<<Halt_L);      // Halt
    LPC_SCT->CTRL_L |= (1<<CLRCTR_L);    // Reset
    LPC_SCT->CTRL_L &= ~(1<<Halt_L);     // Restart

    printf("Number of pulses in the last second = %d\n\n\r", pulse_count);

  } // end of while(1)
	
}  // end of main

