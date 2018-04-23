/*
 * SCT_Event_Timer_ISR.c
 *
 *  Created on: Apr 18, 2016
 *      Author:
 */

#include <stdio.h>
#include "LPC8xx.h"
#include "sct.h"

extern volatile uint8_t ev0_handshake, ev1_handshake, interrupt_counter;

// Function name: SCT_IRQHandler
// Purpose:       SCT interrupt service routine
// Description:   1. Wait in a loop while the contacts stop bouncing.
//                2. Decide which event caused this interrupt, clear the
//                   relevant event interrupt flag, and set handshake flag for main.
// Parameters     none
// Return value:  void
//
void SCT_IRQHandler(void) {
  uint32_t temp;
  volatile static uint32_t debounce;

  // Wait around for approx. 100 ms while the bouncy contacts settle down
  for (debounce = 0; debounce != 4000; debounce++);

  // Increment the counter, semaphore with main
  interrupt_counter = 1;

  // Get the interrupt flags
  temp = LPC_SCT->EVFLAG;

  // Set the relevant event handshake flag, clear the relevant event interrupt flag, and return
  if (temp & (1<<event0)) {
    ev0_handshake = 1;
    LPC_SCT->EVFLAG |= 1<<event0;
    return;
  }

  if (temp & (1<<event1)) {
    ev1_handshake = 1;
    LPC_SCT->EVFLAG |= 1<<event1;
    return;
  }
}
