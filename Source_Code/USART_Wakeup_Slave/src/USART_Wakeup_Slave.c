/*
===============================================================================
 Name        : USART_Wakeup_Slave.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "LPC8xx.h"
#include "utilities.h"
#include "swm.h"
#include "uart.h"
#include "syscon.h"

#define USARTBAUD 115200
#define SCLK_PIN P0_6
#define RXD_PIN P0_7

unsigned char rx_buffer;
volatile enum {false, true} handshake;


// Program flow
// 1. Configure USART1 as slave (requires external connections with USART1 on the master board)
// 2. Wait for a character from the master, and go into the low-power mode requested.
// 3. Wake-up from low-power mode on the next character.

// Configure USART1 as slave
// Bit rate = USARTBAUD b.p.s
// 8 data bits
// No parity
// 1 stop bit


//
// USART ISR. Reads one received char from the RXDAT register, appends it to the rx_buffer array.
void UART1_IRQHandler() {
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= GPIO0;    // Turn GPIO clock back on for the LEDs
  rx_buffer = LPC_USART1->RXDAT ;           // Get the current character, store it for main
  handshake = true;                         // Set handshake for main()
  return;
}

//
// Main routine
int main(void) {

  int k;
  uint32_t * addr = (uint32_t *)LPC_IOCON_BASE;

  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (GPIO0|GPIO1|IOCON|SWM|UART1);

  // Power down the various analog blocks that aren't needed
  LPC_SYSCON->PDSLEEPCFG |= (BOD_PD|WDTOSC_PD);
  LPC_SYSCON->PDRUNCFG |= (BOD_PD|ADC_PD|SYSOSC_PD|WDTOSC_PD|SYSPLL_PD|VREF2_PD|DAC0_PD|DAC1_PD|ACMP_PD);

  // Disable pull-ups and pull-downs
  for (k = 0; k < NUM_IOCON_SLOTS; k++) {
     *(addr + k) &= ~(0x3<<3);
  }

  // Make all GPIO ports outputs driving '1'
  LPC_GPIO_PORT->DIR[0] = 0xFFFFFFFF;
  LPC_GPIO_PORT->PIN[0] = 0xFFFFFFFF;
  LPC_GPIO_PORT->DIR[1] = 0xFFFFFFFF;
  LPC_GPIO_PORT->PIN[1] = 0xFFFFFFFF;

  // Configure the SWM
  ConfigSWM(U1_SCLK, SCLK_PIN);
  ConfigSWM(U1_RXD, RXD_PIN);

  // Give USART a reset
  LPC_SYSCON->PRESETCTRL[0] &= (UART1_RST_N);
  LPC_SYSCON->PRESETCTRL[0] |= ~(UART1_RST_N);

  // Kill the clocks to unused clock select muxes
  //LPC_SYSCON->CAPTCLKSEL = CAPTCLKSEL_OFF;
  LPC_SYSCON->CLKOUTSEL  = CLKOUTSEL_OFF;
  LPC_SYSCON->SCTCLKSEL  = SCTCLKSEL_OFF;
  LPC_SYSCON->ADCCLKSEL  = ADCCLKSEL_OFF;
  LPC_SYSCON->FRG0CLKSEL = FRGCLKSEL_OFF;
  LPC_SYSCON->FRG1CLKSEL = FRGCLKSEL_OFF;
  for (k = 0; k <= 10; k++) {
    LPC_SYSCON->FCLKSEL[k] = FCLKSEL_OFF;
  }

  // Select main_clk as the function clock source for USART1
  LPC_SYSCON->UART1CLKSEL = FCLKSEL_MAIN_CLK;

  // Get the main_clk frequency
  SystemCoreClockUpdate();
  
  // Configure the USART baud rate generator
  LPC_USART1->BRG = (main_clk/USARTBAUD) - 1;

  // Configure the USART CFG register:
  // 8 data bits, no parity, one stop bit, no flow control, synchronous mode, sample on falling edges, slave mode, no loop-back
  LPC_USART1->CFG = DATA_LENG_8|PARITY_NONE|STOP_BIT_1|SYNC_EN;

  // Configure the USART CTL register (nothing to be done here)
  // No continuous break, no address detect, no Tx disable, no CC, no CLRCC
  LPC_USART1->CTL = 0;

  // Enable the USART interrupt as wake-up interrupt in SYSCON
  LPC_SYSCON->STARTERP1 = USART1_INT_WAKEUP;

  // Enable the USART RXRDY interrupt
  LPC_USART1->INTENSET = RXRDY;
  NVIC_EnableIRQ(UART1_IRQn);

  // Enable USART
  LPC_USART1->CFG |= UART_EN;

  Config_LED(LED_GREEN);

  while(1) {

    LED_On(LED_GREEN);

    // Clear handshake, will be set by ISR
    handshake = false;

    // Wait for handshake
    while(handshake == false);

    switch (rx_buffer) {
      case(0x30):
        LED_Off(LED_GREEN);
        // Prepare for low-power mode
        Setup_LPC8xx_Low_Power();
        // Clear the SleepDeep bit
        SCB->SCR &= ~(1<<2);
        // PCON = 0 selects Sleep mode
        LPC_PMU->PCON = 0x0;
        // Wait here for wake-up interrupt.
        __WFI();
        break;

      case(0x31):
        LED_Off(LED_GREEN);
        // Prepare for low-power mode
        Setup_LPC8xx_Low_Power();
        // Set the SleepDeep bit
        SCB->SCR |= (1<<2);
        // PCON = 1 selects Deep-sleep mode
        LPC_PMU->PCON = 0x1;
        // Wait here for wake-up interrupt.
        __WFI();
        break;

      case(0x32):
        LED_Off(LED_GREEN);
        // Prepare for low-power mode
        Setup_LPC8xx_Low_Power();
        // Set the SleepDeep bit
        SCB->SCR |= (1<<2);
        // PCON = 2 selects Power-down mode
        LPC_PMU->PCON = 0x2;
        // Wait here for wake-up interrupt.
        __WFI();
        break;

      default:    // This code should never execute
        while(1);

    } // end of switch

  } // end of while(1)

} // end of main
