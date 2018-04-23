/*
===============================================================================
 Name        : USART1M_USART2S.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include <stdio.h>
#include "LPC8xx.h"
#include "swm.h"
#include "syscon.h"
#include "uart.h"
#include "utilities.h"

extern void setup_debug_uart(void);

#define USARTBAUD 115200

volatile enum {false, true} handshake;
volatile unsigned char RX_Buffer[32];
char thestring[32];
char * tx_ptr;
static uint32_t interrupt_counter = 0;


//
// UART2 ISR
void UART2_IRQHandler() {
  unsigned char temp;

  temp = LPC_USART2->RXDAT ;
  RX_Buffer[interrupt_counter] = temp;

  if (temp == 0) { // NULL string terminator is current character
    handshake = true;
    interrupt_counter = 0;
  }
  else {
    interrupt_counter++;
  }

  return;
}


//
// main routine
int main(void) {
  uint32_t temp;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();

  // Configure the function clocks to USARTs 1 and 2
  // Select the clock source for FRG1
  LPC_SYSCON->FRG1CLKSEL = 1;                   // '1' selects main_clk as input to FRG1

  // Select the function clock source for USART1
  LPC_SYSCON->FCLKSEL[1] = FCLKSEL_FRG1CLK;     // Selects frg1clk as fclk to this USART

  // Select the function clock source for USART2
  LPC_SYSCON->FCLKSEL[2] = FCLKSEL_FRG1CLK;     // Selects frg1clk as fclk to this USART

  // Enable bus clocks to USARTs 1, 2, SWM
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (UART1 | UART2 | SWM);

  // Configure the SWM (see peripherals_lib and swm.h)
  ConfigSWM(U1_SCLK, P0_24);
  ConfigSWM(U1_TXD, P0_13);
  ConfigSWM(U1_RXD, P0_14);

  ConfigSWM(U2_SCLK, P0_25);
  ConfigSWM(U2_RXD, P0_13);
  ConfigSWM(U2_TXD, P0_14);

  // Configure FRG1 (divide-by-1)
  LPC_SYSCON->FRG1MULT = 0;
  LPC_SYSCON->FRG1DIV = 255;

  // Setup USART1 ...
  // Give USART1 a reset
  LPC_SYSCON->PRESETCTRL0 &= (UART1_RST_N);
  LPC_SYSCON->PRESETCTRL0 |= ~(UART1_RST_N);

  // Get the main_clk frequency
  SystemCoreClockUpdate();
  
  // Configure the USART1 baud rate generator (value written to BRG divides by value+1)
  LPC_USART1->BRG = (main_clk/USARTBAUD) - 1;
  
  // Configure the USART1 CFG register:
  // 8 data bits, no parity, one stop bit, no flow control, synchronous mode, sample on falling edges, master mode, no loop-back
  LPC_USART1->CFG = DATA_LENG_8|PARITY_NONE|STOP_BIT_1|SYNC_EN|SYNC_MS;

  // Configure the USART1 CTL register (nothing to be done here)
  // No continuous break, no address detect, no Tx disable, no CC, no CLRCC
  LPC_USART1->CTL = 0;

  // Clear any pending flags
  LPC_USART1->STAT = 0xFFFF;

  // Enable USART1
  LPC_USART1->CFG |= UART_EN;



  // Setup USART2 ...
  // Give USART2 a reset
  LPC_SYSCON->PRESETCTRL0 &= (UART2_RST_N);
  LPC_SYSCON->PRESETCTRL0 |= ~(UART2_RST_N);

  // Configure the baud rate generator (value written to BRG divides by value+1)
  LPC_USART1->BRG = (main_clk/USARTBAUD) - 1;

  // Configure the USART2 CFG register:
  // 8 data bits, no parity, one stop bit, no flow control, synchronous mode, sample on falling edges, slave mode, no loop-back
  LPC_USART2->CFG = DATA_LENG_8|PARITY_NONE|STOP_BIT_1|SYNC_EN;

  // Configure the USART2 CTL register (nothing to be done here)
  // No continuous break, no address detect, no Tx disable, no CC, no CLRCC
  LPC_USART2->CTL = 0;

  // Clear any pending flags (Just to be safe, isn't necessary after the peripheral reset)
  LPC_USART2->STAT = 0xFFFF;

  // Enable the UART2 RX Ready Interrupt
  LPC_USART2->INTENSET = RXRDY;
  NVIC_EnableIRQ(UART2_IRQn);

  // Enable USART2
  LPC_USART2->CFG |= UART_EN;


  while (1) {

    GetConsoleString(thestring);  // See utilities_lib
    printf("Characters to be transmitted are: %s\n\r", thestring);

    tx_ptr = &thestring[0];
    handshake = false;

    do {
      temp = *tx_ptr++;
      // Wait for TXRDY on USART1
      while (!(LPC_USART1->STAT & TXRDY));
      // Write the current character to the USART1 TXDAT register
      LPC_USART1->TXDAT  = temp;
    } while (temp != 0);


    // Wait for handshake from ISR
    while(handshake == false);

    // Echo the string received by USART2 back to the console
    printf("Characters received were: %s\n\r", RX_Buffer);

  } // end of while 1

} // end of main
