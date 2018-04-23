/*
===============================================================================
 Name        : USART_Wakeup_Master.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include <stdio.h>
#include "LPC8xx.h"
#include "utilities.h"
#include "swm.h"
#include "uart.h"
#include "syscon.h"

extern void setup_debug_uart(void);

#define USARTBAUD 115200
#define SCLK_PIN P0_6
#define TXD_PIN P0_7
#define WaitForUSART1txRdy while (((LPC_USART1->STAT) & (1<<2)) == 0)

const char promptstring[] = "Choose a low power mode for the slave:\n\r0 for Sleep\n\r1 for Deep-sleep\n\r2 for Power-down\n\r";


// Program flow
// 1. Configure USART1 as master (requires external connections with USART1 on the slave board)
// 2. Prompt the user to type a character ('0', '1', or '2') then [Return]
// 3. Transmit the character on USART1 to put the slave board to sleep.
// 4. Prompt the user to type [Enter] to proceed with waking up the slave board.
// 5. Transmit one byte on USART1 to wake up the slave board.


int main(void) {

  unsigned char temp;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();

  // Enable clocks to USART1, SWM, GPIO
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (UART1 | SWM | GPIO);

  // Configure the SWM
  ConfigSWM(U1_SCLK, SCLK_PIN);
  ConfigSWM(U1_TXD, TXD_PIN);

  // Kill the clock to FRG1 since we aren't using it
  LPC_SYSCON->FRG1CLKSEL = FRGCLKSEL_OFF;

  // Select main_clk as the function clock source for USART1
  LPC_SYSCON->UART1CLKSEL = FCLKSEL_MAIN_CLK;

  // Give USART1 a reset
  LPC_SYSCON->PRESETCTRL[0] &= (UART1_RST_N);
  LPC_SYSCON->PRESETCTRL[0] |= ~(UART1_RST_N);

  // Configure the USART1 baud rate generator
  LPC_USART1->BRG = (main_clk/USARTBAUD) - 1;

  // Configure the USART1 CFG register:
  // 8 data bits, no parity, one stop bit, no flow control, synchronous mode, sample on falling edges, master mode, no loop-back
  LPC_USART1->CFG = DATA_LENG_8|PARITY_NONE|STOP_BIT_1|SYNC_EN|SYNC_MS;

  // Configure the USART1 CTL register (nothing to be done here)
  // No continuous break, no address detect, no Tx disable, no CC, no CLRCC
  LPC_USART1->CTL = 0;

  // Enable USART1
  LPC_USART1->CFG |= UART_EN;

  Config_LED(LED_GREEN);

  while(1) {

    LED_On(LED_GREEN);

    // Prompt user to select a low-power mode for the slave
    temp = GetConsoleCharacter((const char *)&promptstring);

    // Wait for TXRDY on USART1
    WaitForUSART1txRdy;

    // Accept '0' '1' or '2' only
    if (temp >= 0x30 && temp <= 0x32) {
      LPC_USART1->TXDAT  = temp; // Transmit the '0', '1', or '2' character to put the slave board to sleep
    }
    else {
      LPC_USART1->TXDAT  = 0x30; // Default is Sleep mode if garbage was entered
    }

    LED_Off(LED_GREEN);

    // Prompt user, wait for 'Esc' before proceeding
		DebugWaitEsc();

    // Wait for TXRDY on USART1
    WaitForUSART1txRdy;

    // Send a wake-up byte (value is irrelevant)
    LPC_USART1->TXDAT  = 0x55;


  } // end of while(1)

} // end of main


