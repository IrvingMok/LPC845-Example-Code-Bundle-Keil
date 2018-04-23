/*
 * SCT_Event_Counter_funcs.c
 *
 *  Created on: Apr 19, 2016
 *      Author:
 */


#include "LPC8xx.h"
#include "syscon.h"
#include "wkt.h"
#include "gpio.h"
#include "mrt.h"
#include "SCT_Event_Counter.h"


#define mrt_1sec 30000000

volatile uint8_t handshake;

// Function name: MRT_Config
// Description:   Setup MRT[0] to interrupt once-per-second at 30 MHz.
// Parameters:    None
// Returns:       Void
void MRT_Config() {
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (MRT);      // Enable some clocks
  LPC_SYSCON->PRESETCTRL0 &= (MRT_RST_N);   // Reset the MRT
  LPC_SYSCON->PRESETCTRL0 |= ~(MRT_RST_N);
  LPC_MRT->Channel[0].CTRL = (MRT_Repeat<<MRT_MODE) | (1<<MRT_INTEN);// Mode = repeat, interrupt = enable
  NVIC_EnableIRQ(MRT_IRQn);                 // Enable the MRT interrupt in the NVIC
  handshake = 0;                            // Initial value for the handshake flag
  LPC_MRT->Channel[0].INTVAL = mrt_1sec;    // Value for 1 MRT interrupt per second at 30 MHz, starts the MRT
}



// Function name: WKT_Config
// Description:   Initialize a GPIO output pin and the WKT, then start it.
// Parameters:    None
// Returns:       Void
void WKT_Config() {
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (WKT|GPIO); // Enable some clocks
  LPC_PMU->DPDCTRL |= 1<<2;                 // Power-up the LPOSC
  LPC_SYSCON->PRESETCTRL0 &= (WKT_RST_N);   // Reset the WKT
  LPC_SYSCON->PRESETCTRL0 |= ~(WKT_RST_N);
  NVIC_EnableIRQ(WKT_IRQn);                 // Enable the WKT interrupt in the NVIC
  GPIOSetDir(OUT_PORT, OUT_BIT, OUTPUT);    // Port pin configured as output
  GPIOSetBitValue(OUT_PORT, OUT_BIT, 1);    // Initially driving '1'
  LPC_WKT->CTRL = LOW_POWER_OSC<<WKT_CLKSEL;// Select LPOSC as WKT clock source (approx. 10 KHz)
  LPC_WKT->COUNT = 1;                       // Start the WKT, counts down for 1 clock then interrupts
}



// Function name: WKT_IRQHandler
// Description:   WKT interrupt service routine.
//                Toggles P0.4, restarts the WKT.
// Parameters:    None
// Returns:       Void
void WKT_IRQHandler(void) {
  LPC_WKT->CTRL |= 1<<WKT_ALARMFLAG;        // Clear the interrupt flag
  LPC_WKT->COUNT = 1;                       // Restart the WKT
  LPC_GPIO_PORT->NOT[OUT_PORT] = 1<<OUT_BIT;// Toggle the port pin, this is the clock for the SCT
  
  return;
}



// Function name: MRT_IRQHandler
// Description:   MRT interrupt service routine.
//                Sets a handshake flag and returns.
// Parameters:    None
// Returns:       Void
void MRT_IRQHandler(void) {
  LPC_MRT->Channel[0].STAT = 1<<MRT_INTFLAG; // Clear the interrupt flag
  handshake = 1;                             // Set the handshake flag
  return;
}
