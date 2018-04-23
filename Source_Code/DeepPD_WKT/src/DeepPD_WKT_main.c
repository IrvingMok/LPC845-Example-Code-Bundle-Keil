/*
===============================================================================
 Name        : DeepPD_WKT_main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "LPC8xx.h"
#include "swm.h"
#include "syscon.h"
#include "utilities.h"
#include "uart.h"
#include "pmu.h"
#include "wkt.h"
#include "chip_setup.h"

const unsigned char promptstring[] = "\rChoose 3 to enter Deep power-down\n\r";
const unsigned char dpdstring[] = "Entering Deep power-down ... WKT will wake up the chip in 5 seconds\n\n\r";

volatile enum {false, true} handshake;

void setup_debug_uart(void);

int main(void) {

  int k;
  uint32_t * addr = (uint32_t *)LPC_IOCON_BASE;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();
  
  // Enable bus clocks to peripherals
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (GPIO0|GPIO1|IOCON|SWM|WKT);

  // Configure the WKT and the LPOSC
  LPC_PMU->DPDCTRL |= (1<<LPOSCEN)|          // LPOSC powered
                      (1<<LPOSCDPDEN)|       // LPOSC stays powered in deep pd mode
                      (1<<WAKEPAD_DISABLE)|  // Disable wakeup pin function
                      (1<<WAKE2PAD_DISABLE); // Disable wakeup2 pin function
  LPC_SYSCON->PRESETCTRL0 &= (WKT_RST_N);    // Reset the WKT
  LPC_SYSCON->PRESETCTRL0 |= ~(WKT_RST_N);
  LPC_SYSCON->STARTERP1 = WKT_INT_WAKEUP;    // Enable the WKT as the wakeup source
  NVIC_EnableIRQ(WKT_IRQn);                  // Enable the WKT interrupt in the NVIC
  LPC_WKT->CTRL = LOW_POWER_OSC<<WKT_CLKSEL; // Select LPOSC as WKT clock source
  
  // Power down the various analog blocks that aren't needed
  LPC_SYSCON->PDSLEEPCFG |= (BOD_PD|WDTOSC_PD);
  LPC_SYSCON->PDRUNCFG |= (BOD_PD|ADC_PD|SYSOSC_PD|WDTOSC_PD|SYSPLL_PD|VREF2_PD|DAC0_PD|DAC1_PD|ACMP_PD);

  // Don't clock the clock select muxes and function clock dividers that aren't needed
  LPC_SYSCON->CLKOUTSEL = CLKOUTSEL_OFF;
  LPC_SYSCON->SCTCLKSEL = SCTCLKSEL_OFF;
  LPC_SYSCON->FRG0CLKSEL = FRGCLKSEL_OFF;
  LPC_SYSCON->FRG1CLKSEL = FRGCLKSEL_OFF;
  LPC_SYSCON->ADCCLKSEL = ADCCLKSEL_OFF;
  for (k = 1; k <= 10; k++) {
    LPC_SYSCON->FCLKSEL[k] = FCLKSEL_OFF;
  }

  // Disable pull-ups and pull-downs.
  // We may leave P0.4 pull-up on, if P0.4 is floated,
  // BUT WE CAN'T FLOAT IT FOR DPD mode, it MUST be pulled high externally!
  //
  // Note: On some LPC8xx parts, there are more slots in the iocon register map than there are GPIO port bits.
  // This is because there is at least one reserved address sitting in the middle of the
  // register address map for some reason. This is why the macro NUM_IOCON_SLOTS,
  // which is defined in board.h, may be greater than the number of GPIO port bits on the chip.
  for (k = 0; k < NUM_IOCON_SLOTS; k++) {
    *(addr + k) &= ~(0x3<<3);
  }

  // Make all ports outputs driving '1'
  LPC_GPIO_PORT->DIR[0] = 0xFFFFFFFF;
  LPC_GPIO_PORT->PIN[0] = 0xFFFFFFFF;
  LPC_GPIO_PORT->DIR[1] = 0xFFFFFFFF;
  LPC_GPIO_PORT->PIN[1] = 0xFFFFFFFF;

  // Enable USART0 interrupt
  pDBGU->INTENSET = RXRDY;
  NVIC_EnableIRQ(DBGUIRQ);

  Config_LED(LED_GREEN);

  while(1) {

    LED_On(LED_GREEN);
    PutTerminalString(pDBGU, (uint8_t *)promptstring);            // Print the prompt to the terminal
    handshake = false;                                            // Clear handshake flag, will be set by ISR at end of user input
    while (handshake == false);                                   // Wait here for handshake from ISR

    LED_Off(LED_GREEN);
    PutTerminalString(pDBGU, (uint8_t *)dpdstring);               // Print the prompt to the terminal

    // Prepare for low-power mode
    LPC_SYSCON->PDAWAKECFG = LPC_SYSCON->PDRUNCFG;                // Copy current PDRUNCFG to PDAWAKECFG
    LPC_SYSCON->SYSAHBCLKCTRL[0] &= ~(UART0|IOCON|SWM|GPIO1|WKT); // Turn off peripheral bus clocks
    LPC_WKT->COUNT = 50000;                                       // Start the WKT, counts down for 5 sec. @10,000 Hz then interrupts

    // Enter Deep PD 
    SCB->SCR |= (1<<2);                                           // Set the SleepDeep bit
    LPC_PMU->PCON = 0x3;                                          // PCON = 3 selects Deep power-down mode
        
    __WFI();                                                      // Wait here for WKT interrupt.

    NVIC_SystemReset();

  } // end of while(1)

} // end of main

