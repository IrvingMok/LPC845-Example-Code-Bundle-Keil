//--------------------
// Ctimer_PWM_funcs.c
//--------------------

#include "LPC8xx.h"
#include "syscon.h"
#include "ctimer.h"
#include "pmu.h"
#include "gpio.h"
#include "wkt.h"
#include "Ctimer_pwm.h"






// Function name: WKT_Config
// Description:   Initialize a GPIO output pin and the WKT, then start it.
// Parameters:    None
// Returns:       Void
void WKT_Config() {
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (WKT|GPIO0);// Enable some clocks
  LPC_PMU->DPDCTRL |= 1<<LPOSCEN;           // Power-up the LPOSC
  LPC_SYSCON->PRESETCTRL0 &= (WKT_RST_N);   // Reset the WKT
  LPC_SYSCON->PRESETCTRL0 |= ~(WKT_RST_N);
  NVIC_EnableIRQ(WKT_IRQn);                 // Enable the WKT interrupt in the NVIC
  GPIOSetDir(OUT_PORT, OUT_BIT, OUTPUT);    // Port pin configured as output
  GPIOSetBitValue(OUT_PORT, OUT_BIT, 1);    // Initially driving '1'
  LPC_WKT->CTRL = LOW_POWER_OSC<<WKT_CLKSEL;// Select LPOSC as WKT clock source (approx. 10 KHz)
  LPC_WKT->COUNT = 1;                       // Start the WKT, counts down for 1 clock then interrupts
}






// Function name: WKT_IRQHandler (interrupt service routine)
// Description:   WKT interrupt service routine.
//                Toggles P0.4, restarts the WKT.
// Parameters:    None
// Returns:       Void
void WKT_IRQHandler(void) {
  LPC_WKT->CTRL |= 1<<WKT_ALARMFLAG;        // Clear the interrupt flag
  LPC_WKT->COUNT = 1;                       // Restart the WKT
  LPC_GPIO_PORT->NOT[OUT_PORT] = 1<<OUT_BIT;// Toggle the port pin, each edge will clock the capture input
  return;
}






// Function name: CTimer0_IRQHandler (interrupt service routine)
// Description:   Updates the duty cycles in the PWM0, 1, and 2 match registers
// Parameters:    None
// Returns:       Void
void CTIMER0_IRQHandler(void) {
  static uint32_t red_duty_cycle = 0;
  static uint32_t grn_duty_cycle = 67;
  static uint32_t blu_duty_cycle = 67;
  static uint8_t red_dir = 1;
  static uint8_t grn_dir = 1;
  static uint8_t blu_dir = 0;

  LPC_CTIMER0->IR = 1<<MR3INT;              // Clear the interrupt flag

  // Update duty-cycle for PWM MR0
  if (red_dir) {                            // If red LED is getting brighter
    LPC_CTIMER0->MR[0] = red_duty_cycle++;
    if (red_duty_cycle == 101)
      red_dir = 0;
  }
  else {                                    // Else red LED is getting dimmer
    LPC_CTIMER0->MR[0] = --red_duty_cycle;
    if (red_duty_cycle == 0)
      red_dir = 1;
  }
  
  // Update duty-cycle for PWM MR1
  if (grn_dir) {                            // If green LED is getting brighter
    LPC_CTIMER0->MR[1] = grn_duty_cycle++;
    if (grn_duty_cycle == 101)
      grn_dir = 0;
  }
  else {                                    // Else green LED is getting dimmer
    LPC_CTIMER0->MR[1] = --grn_duty_cycle;
    if (grn_duty_cycle == 0)
      grn_dir = 1;
  }

  // Update duty-cycle for PWM MR2
  if (blu_dir) {                            // If blue LED is getting brighter
    LPC_CTIMER0->MR[2] = blu_duty_cycle++;
    if (blu_duty_cycle == 101)
      blu_dir = 0;
  }
  else {                                    // Else blue LED is getting dimmer
    LPC_CTIMER0->MR[2] = --blu_duty_cycle;
    if (blu_duty_cycle == 0)
      blu_dir = 1;
  }
} // end of Ctimer0 ISR




