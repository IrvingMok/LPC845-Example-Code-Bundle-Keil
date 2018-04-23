/*
===============================================================================
 Name        : PMU_Modes.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "LPC8xx.h"
#include "PMU_Modes.h"
#include "swm.h"
#include "syscon.h"
#include "utilities.h"
#include "uart.h"
#include "pmu.h"
#include "chip_setup.h"

const unsigned char promptstring[] = "\rChoose a low power mode:\n\r0 for Sleep\n\r1 for Deep-sleep\n\r2 for Power-down\n\r3 for Deep power-down\n\r";
const unsigned char sleepstring[] = "Entering Sleep ... Use pin interrupt (P0.4) to wake up\n\n\r";
const unsigned char dsleepstring[] = "Entering Deep-sleep ... Use pin interrupt (P0.4) to wake up\n\n\r";
const unsigned char pdstring[] = "Entering Power-down ... Use pin interrupt (P0.4) to wake up\n\n\r";
const unsigned char dpdstring[] = "Entering Deep power-down ... Use wakeup pin (P0.4) to wake up\n\n\r";

extern unsigned char rx_buffer[RX_BUFFER_SIZE];
volatile enum {false, true} handshake;


int main(void) {

  int k;
  uint32_t * addr = (uint32_t *)LPC_IOCON_BASE;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();
  
  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (GPIO0|GPIO1|GPIO_INT|IOCON|SWM);

  // Power down the various analog blocks that aren't needed
  LPC_SYSCON->PDSLEEPCFG |= (BOD_PD|WDTOSC_PD);
  LPC_SYSCON->BODCTRL &= ~(1<<BODRSTENA);                  // Disable BOD reset before de-powering BOD
  LPC_SYSCON->PDRUNCFG |= (BOD_PD|ADC_PD|SYSOSC_PD|WDTOSC_PD|SYSPLL_PD|VREF2_PD|DAC0_PD|DAC1_PD|ACMP_PD);

  // Don't clock clock select muxes and dividers that aren't needed
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
    //if  (k == 4) {             // Optionally, leave P0.4 (k = 4) pull-up on
    //  continue;
    //}
    //else {
      *(addr + k) &= ~(0x3<<3);
    //}
  }
  
  // Make all ports outputs driving '1', except P0.4 is input
  LPC_GPIO_PORT->DIR[0] = 0xFFFFFFeF;
  LPC_GPIO_PORT->PIN[0] = 0xFFFFFFFF;
  LPC_GPIO_PORT->DIR[1] = 0xFFFFFFFF;
  LPC_GPIO_PORT->PIN[1] = 0xFFFFFFFF;

  // Configure P0.4 as PININT7, falling edge sensitive
  LPC_PIN_INT->IST = 0xFF;     // Clear any pending edge-detect interrupt flags
  LPC_SYSCON->PINTSEL[7] = 4;  // P0.4 assigned to PININT7
  LPC_PIN_INT->ISEL = 0x0;     // Edge-sensitive
  LPC_PIN_INT->SIENF = 1<<7;   // Falling-edge interrupt enabled on PININT7

  // Configure PININT7 as wake up interrupt in SysCon block
  LPC_SYSCON->STARTERP0 = 1<<7;

  // Enable pin interrupt 7 in the NVIC
  NVIC_EnableIRQ(PININT7_IRQn);

  // Enable USART0 interrupt
  pDBGU->INTENSET = RXRDY;
  NVIC_EnableIRQ(DBGUIRQ);

  Config_LED(LED_GREEN);

  while(1) {

    LED_On(LED_GREEN);

    PutTerminalString(pDBGU, (uint8_t *)promptstring);

    handshake = false;                                      // Clear handshake flag, will be set by ISR at end of user input
    while (handshake == false);                             // Wait here for handshake from ISR

    LED_Off(LED_GREEN);

    switch(rx_buffer[0]) {
      default:
      case(0x30):
        PutTerminalString(pDBGU, (uint8_t *)sleepstring);
        // Wait for Tx buffer empty
        while (!((pDBGU->STAT) & TXRDY));
        // Prepare for low-power mode
        Setup_Low_Power();
        // Clear the SleepDeep bit
        SCB->SCR &= ~(1<<2);
        // PCON = 0 selects sleep mode
        LPC_PMU->PCON = 0x0;
        // Wait here for pin interrupt. Startup time begins on falling edge of P0.4
        __WFI();

        break;

      case(0x31):
        PutTerminalString(pDBGU, (uint8_t *)dsleepstring);
        // Wait for Tx buffer empty
        while (!((pDBGU->STAT) & TXRDY));
        // Prepare for low-power mode
        Setup_Low_Power();
        // Set the SleepDeep bit
        SCB->SCR |= (1<<2);
        // PCON = 1 selects Deep-sleep mode
        LPC_PMU->PCON = 0x1;
        // Wait here for pin interrupt. Startup time begins on falling edge of P0.4
        __WFI();

        break;

      case(0x32):
        PutTerminalString(pDBGU, (uint8_t *)pdstring);
        // Wait for Tx buffer empty
        while (!((pDBGU->STAT) & TXRDY));
        // Prepare for low-power mode
        Setup_Low_Power();
        // Set the SleepDeep bit
        SCB->SCR |= (1<<2);
        // PCON = 2 selects Power-down mode mode
        LPC_PMU->PCON = 0x2;
        // Wait here for pin interrupt. Startup time begins on falling edge of P0.4
        __WFI();

        break;

      case(0x33):
        PutTerminalString(pDBGU, (uint8_t *)dpdstring);
        // Wait for Tx buffer empty
        while (!((pDBGU->STAT) & TXRDY));
        // Prepare for low-power mode
        Setup_Low_Power();
        // Set the SleepDeep bit
        SCB->SCR |= (1<<2);
        // PCON = 3 selects Deep power-down mode
        LPC_PMU->PCON = 0x3;
        // Wait here for pin interrupt. Startup time begins on falling edge of P0.4
        __WFI();

        break;

    } // end of switch

  } // end of while(1)

} // end of main

