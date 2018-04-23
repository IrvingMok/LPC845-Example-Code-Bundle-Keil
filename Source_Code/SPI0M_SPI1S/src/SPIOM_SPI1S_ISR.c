/*
 * SPIOM_SPI1S_ISR.c
 *
 *  Created on: Mar 7, 2016
 *      Author: 
 */

#include "LPC8xx.h"
#include "spi.h"

extern volatile unsigned char master_rx_buffer[32];
extern volatile unsigned char slave_rx_buffer[32];
extern volatile enum {false, true} master_handshake, slave_handshake;
static uint32_t master_interrupt_counter = 0;
static uint32_t slave_interrupt_counter = 0;

/*****************************************************************************
** Function name:	SPI0_IRQHandler
** Description: SPI0 interrupt service routine (handles master RX data)
** parameters: None
** Returned value: None
*****************************************************************************/
void SPI0_IRQHandler() {
  unsigned char temp;

  temp = LPC_SPI0->RXDAT;
  master_rx_buffer[master_interrupt_counter] = temp;

  if (temp == 0) { // NULL string terminator is current character
    master_handshake = true;
    master_interrupt_counter = 0;
  }
  else {
    master_interrupt_counter++;
  }
  return;
}


/*****************************************************************************
** Function name: SPI1_IRQHandler
** Description: SPI1 interrupt service routine (handles slave RX data)
** parameters: None
** Returned value: None
*****************************************************************************/
void SPI1_IRQHandler() {
  unsigned char temp_data, temp_stat;

  temp_stat = LPC_SPI1->STAT;          // Get the status register contents

  // If RXRDY interrupt flag is set, store the received character and increment the counter
  if (temp_stat & SPIRXRDY) {
    temp_data = LPC_SPI1->RXDAT;
    slave_rx_buffer[slave_interrupt_counter++] = temp_data;
  }

  // If SSD interrupt flag is set, transmission is over.
  if (temp_stat & SPISSD) {            // SSD interrupt flag is set, means EOT
    LPC_SPI1->STAT = SPISSD;           // Clear the interrupt flag
    slave_handshake = true;            // Set handshake flag for main
    slave_interrupt_counter = 0;       // Reset counter
  }
  return;
}
