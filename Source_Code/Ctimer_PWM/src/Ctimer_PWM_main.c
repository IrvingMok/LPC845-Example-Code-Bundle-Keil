//--------------------
// Ctimer_PWM_main.c
//--------------------

#include <stdio.h>
#include "LPC8xx.h"
#include "syscon.h"
#include "SWM.h"
#include "ctimer.h"
#include "utilities.h"
#include "Ctimer_PWM.h"


//
// main routine
//
int main(void) {
  uint32_t count_freq;
  
  // Configure the debug uart (see Serial.c)
  setup_debug_uart();

  printf("Ctimer_PWM example\n\r");
  
  // Configure and start the WKT, also configures an output pin, which toggles approximately at 10 KHz
  WKT_Config();

  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= SWM|CTIMER0;
  
  // Configure the Count Control register
  // Counter mode dual edge (TC is incremented on both edges of the capture input selected)
  // Select capture input 0 for clocking
  LPC_CTIMER0->CTCR = COUNTER_MODE_BOTH<<CTMODE | 0<<CINSEL;
  
  // Note: If Counter mode is selected for a particular CAPn input in the CTCR, the 3 bits for
  // that input in the Capture Control Register (CCR) must be programmed as 000.
  LPC_CTIMER0->CCR &= ~(1<<CAP0RE|1<<CAP0FE|1<<CAP0I);
  
  // Connect the Ctimer Match/PWM outputs, and the Capture 0 function, to port pins
  ConfigSWM(T0_MAT0, LED_RED);
  ConfigSWM(T0_MAT1, LED_GREEN);
  ConfigSWM(T0_MAT2, LED_BLUE);
  ConfigSWM(T0_CAP0, IN_PORT);
  
  // Set the PWM period in counts/cycle of the TC
  LPC_CTIMER0->MR[3] = PWM_RES;

  // Configure the prescaler (want to count from 0 to PWM_RES every 1/PWM_FREQ seconds)
  // PRE+1 = clock_freq /(pwm_freq)*(pwm_res) ... Remember, the prescaler divides the bus clock by PRE+1
  count_freq = PWM_FREQ * PWM_RES;             // in counts/second
  LPC_CTIMER0->PR = (WKT_FREQ/count_freq) - 1; // in WKT clocks/count
  
  // Configure the Match Control register to interrupt and reset on match in MR3
  LPC_CTIMER0->MCR = (1<<MR3I)|(1<<MR3R);
  
  // Put the Match 0 - 2 outputs into PWM mode
  LPC_CTIMER0->PWMC = (1<<PWMEN0)|(1<<PWMEN1)|(1<<PWMEN2);
  
  // Enable the Ctimer0 interrupt
  NVIC_EnableIRQ(CTIMER0_IRQn);
  
  // Start the action
  LPC_CTIMER0->TCR = 1<<CEN;
  
  while(1) { // Have a tequila
  } // end of while 1

} // end of main
