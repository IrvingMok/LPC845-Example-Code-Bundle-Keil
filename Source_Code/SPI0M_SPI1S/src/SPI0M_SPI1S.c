/*
===============================================================================
 Name        : SPI0M_SPI1S.c
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
#include "spi.h"
#include "utilities.h"

extern void setup_debug_uart(void);

#define SPIBAUD 15000000

volatile enum {false=0, true=1} master_handshake, slave_handshake;
volatile unsigned char master_rx_buffer[32];
volatile unsigned char slave_rx_buffer[32];
char thestring[32];


int main(void) {
  uint32_t temp, n;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();

  // Shut off clocks to both FGRs since unused.
  LPC_SYSCON->FRG0CLKSEL = FRGCLKSEL_OFF;
  LPC_SYSCON->FRG1CLKSEL = FRGCLKSEL_OFF; 

  // Select the function clock source for the master, SPI0 (slave will be clocked by the master's SCK)
  LPC_SYSCON->SPI0CLKSEL = FCLKSEL_MAIN_CLK;
//LPC_SYSCON->SPI1CLKSEL = FCLKSEL_MAIN_CLK;
  
  // Enable bus clocks to SPI0/1, SWM 
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (SPI0 | SPI1 | SWM);

  // Configure the SWM (see peripherals_lib and swm.h)
  ConfigSWM(SPI0_SCK,   P0_20); 
  ConfigSWM(SPI0_MISO,  P0_21);
  ConfigSWM(SPI0_MOSI,  P0_25); 
  ConfigSWM(SPI0_SSEL0, P0_24); 

  ConfigSWM(SPI1_SCK,   P0_6); 
  ConfigSWM(SPI1_MISO,  P0_7); 
  ConfigSWM(SPI1_MOSI,  P1_19); 
  ConfigSWM(SPI1_SSEL0, P1_18); 

  // Configure SPI0 as master, SPI1 as slave
  // Bit rate = 15  m.b.p.s with 30 MHz main_clk
  // SPI mode 0 (CPOL = 0 CPHA = 0)
  // 8 data bits
  //

  // Setup the SPIs ...
  // Give both SPIs a reset  (see syscon.h)
  LPC_SYSCON->PRESETCTRL0 &= (SPI0_RST_N) & (SPI1_RST_N);
  LPC_SYSCON->PRESETCTRL0 |= ~((SPI0_RST_N) & (SPI1_RST_N));

  // Configure the SPI master's clock divider, slave's value meaningless. (value written to DIV divides by value+1)
  SystemCoreClockUpdate();                // Get main_clk frequency
  LPC_SPI0->DIV = (main_clk/SPIBAUD) - 1; //(2-1);

  // Configure the CFG registers:
  // Enable=true, master/slave, no LSB first, CPHA=0, CPOL=0, no loop-back, SSEL active low
  LPC_SPI0->CFG = CFG_ENABLE | CFG_MASTER;
  LPC_SPI1->CFG = CFG_ENABLE | CFG_SLAVE;

  // Configure the master SPI delay register (DLY), slave's value meaningless.
  // Pre-delay = 0 clocks, post-delay = 0 clocks, frame-delay = 0 clocks, transfer-delay = 0 clocks
  LPC_SPI0->DLY = 0x0000;

  // Configure the SPI control registers
  // Master: End-of-frame true, LEN = 8 bits. Slave: LEN = 8 bits
  LPC_SPI0->TXCTL = CTL_EOF | CTL_LEN(8);
  LPC_SPI1->TXCTL = CTL_LEN(8);

  // Enable interrupts
  LPC_SPI0->INTENSET = RXRDYEN;                    // Master interrupt only on received data
  NVIC_EnableIRQ(SPI0_IRQn);
  LPC_SPI1->INTENSET = RXRDYEN | SSDEN;            // Slave interrupt on both received data and slave select deassert (end of transmission)
  NVIC_EnableIRQ(SPI1_IRQn);



  while (1) {

    GetConsoleString(thestring);                   // 
    printf("Characters to be transmitted (terminated by NUL) are: %s\n\r", thestring);

    LPC_SPI0->TXCTL &= ~(CTL_EOT);                 // Start a new transfer, clear the EOT bit
    master_handshake = false;                      // Clear handshake flags, set by ISRs
    slave_handshake = false;
    n = 0;

    do {
      temp = thestring[n++];                       // Get the current character
      while ((LPC_SPI1->STAT & STAT_TXRDY) == 0);  // Wait for slave TXRDY
      LPC_SPI1->TXDAT = temp;                      // Write the current character to the slave's TXDAT register
      while ((LPC_SPI0->STAT & STAT_TXRDY) == 0);  // Wait for master TXRDY

      if (temp == 0) {                             // If current char is NUL string terminator, Set EOT on this one
        LPC_SPI0->TXCTL |= CTL_EOT;
      }

      LPC_SPI0->TXDAT = temp;                      // Write the current character to the master's TXDAT register, start the frame

    } while ((temp & 0xFF) != 0);                  // Do the do loop until current character is NUL string terminator

    // Wait for handshakes from ISRs
    while(master_handshake == false);
    while(slave_handshake == false);

    // Echo the strings received by both master and slave SPIs back to the console/terminal
    printf("Characters received by slave (terminated by NUL) were: %s\n\r", slave_rx_buffer);
    printf("Characters received by master (terminated by NUL) were: %s\n\r", master_rx_buffer);

  } // end of while 1

} // end of main
