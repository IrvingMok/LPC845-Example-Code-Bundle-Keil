/*
 * DeepPD_WKT_funcs.c
 *
 *  Created on: 
 *      Author:
 */

#include "LPC8xx.h"
#include "syscon.h"
#include "uart.h"
#include "utilities.h"
#include "swm.h"
#include "wkt.h"
#include "pmu.h"

unsigned char rx_buffer[1];
extern volatile enum {false, true} handshake;
const static uint32_t rx_char_counter = 0;

#define WaitForUART0txRdy  while(((LPC_USART0->STAT) & TXRDY) == 0)



// Function name: WKT_IRQHandler
// Description:   WKT interrupt service routine
// Parameters:    None
// Returns:       Void
void WKT_IRQHandler(void) {
  LED_On(LED_GREEN);
  LPC_WKT->CTRL |= 1<<WKT_ALARMFLAG;               // Clear the interrupt flag
  SCB->SCR &= ~(1<<2);                             // Clear the SleepDeep bit
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (UART0 | GPIO0); // Activate clocks to the needed peripherals for normal mode
  return;
}



/*****************************************************************************
** Function name:		UART0_IRQHandler
**
** Description:		    UART0 interrupt service routine.
**                      This ISR reads one received char from the UART0 RXDAT register,
**                      stores it in the rx_buffer array, and echos it back via the
**                      UART0 transmitter. If the char. is 0xD (carriage return),
**                      both a new line char (0xA) and a CR (0xD) are echoed.
**                      Only '0', '1', '2', '3', and 'CR' are handled, any other
**                      value is ignored.
**
** Parameters:			None
** Returned value:		void
**
*****************************************************************************/
void UART0_IRQHandler() {
  unsigned char temp;

  temp = LPC_USART0->RXDAT ;                // Get the current character

  if ((temp >= 0x30 && temp <= 0x33)) {     // Only integers 0-3 and CR are accepted
    rx_buffer[rx_char_counter] = temp;      // Append the current character to the rx_buffer
    WaitForUART0txRdy;                      // Wait for TXREADY
    LPC_USART0->TXDAT  = temp;              // Echo it back to the terminal
  }

  if (temp == 0x0D) {                       // CR (carriage return) is current character. End of entry.
    WaitForUART0txRdy;                      // Wait for TXREADY
    LPC_USART0->TXDAT  = 0x0A;              // Echo a NL (new line) character to the terminal.
    WaitForUART0txRdy;                      // Wait for TXREADY
    LPC_USART0->TXDAT  = 0x0D;              // Echo a CR character to the terminal.
    handshake = true;                       // Set handshake for main()
  }

  return;
}

