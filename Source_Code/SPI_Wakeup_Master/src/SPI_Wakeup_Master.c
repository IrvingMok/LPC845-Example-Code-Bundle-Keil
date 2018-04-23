//--------------------
// SPI_Wakeup_Master.c
//--------------------

#include <stdio.h>
#include "LPC8xx.h"
#include "spi.h"
#include "syscon.h"
#include "swm.h"
#include "utilities.h"

extern void setup_debug_uart(void);

#define SPIBAUD 1000000
#define SCK_PIN P0_24
#define MOSI_PIN P0_25
#define SSEL_PIN P0_21

const char promptstring[] = "Choose a low power mode for the slave:\n\r0 for Sleep\n\r1 for Deep-sleep\n\r2 for Power-down\n\r";


// Program flow
// 1. Configure SPI0 as master (requires external connections with SPI1 on the slave board.
// 2. Prompt the user to type a character ('0', '1', or '2')
// 3. Transmit the character on SPI0 to put the slave board to sleep (via SPI1)
// 4. Prompt the user to type [Esc] to proceed with waking up the slave board.
// 5. Transmit one byte on SPI0 to wake up the slave board (via SPI1)

// SPI configuration
// Bit rate = 1  m.b.p.s
// SPI mode 0 (CPOL = 0 CPHA = 0)
// 8 data bits
//



int main(void) {

  unsigned char temp;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();
	
  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (SPI0|SWM);

  // Configure the SWM (see peripherals_lib and swm.h)
  ConfigSWM(SPI0_SCK, SCK_PIN);
  ConfigSWM(SPI0_MOSI, MOSI_PIN);
  ConfigSWM(SPI0_SSEL0, SSEL_PIN);

  // Give SPI0 a reset
  LPC_SYSCON->PRESETCTRL[0] &= (SPI0_RST_N);
  LPC_SYSCON->PRESETCTRL[0] |= ~(SPI0_RST_N);
  
  // Enable main_clk as function clock to SPI
  LPC_SYSCON->SPI0CLKSEL = FCLKSEL_MAIN_CLK;
  
  // Get main_clk frequency
  SystemCoreClockUpdate();
  
  // Configure the SPI master's clock divider (value written to DIV divides by value+1)
  LPC_SPI0->DIV = (main_clk/SPIBAUD) - 1;

  // Configure the CFG register:
  // Enable=true, master, no LSB first, CPHA=0, CPOL=0, no loop-back, SSEL active low
  LPC_SPI0->CFG = CFG_ENABLE | CFG_MASTER;

  // Configure the SPI delay register (DLY)
  // Pre-delay = 0 clocks, post-delay = 0 clocks, frame-delay = 0 clocks, transfer-delay = 0 clocks
  LPC_SPI0->DLY = 0x0000;

  // Configure the SPI control register
  // Master: End-of-frame true, End-of-transfer true, RXIGNORE true, LEN 8 bits.
  LPC_SPI0->TXCTL = CTL_EOF | CTL_EOT | CTL_RXIGNORE | CTL_LEN(8);

  Config_LED(LED_GREEN);

  while(1) {

    LED_On(LED_GREEN);

    // Prompt user to select a low-power mode for the slave
    temp = GetConsoleCharacter((const char *)&promptstring);

    // Wait for TXRDY on SPI0
    while (!(LPC_SPI0->STAT & STAT_TXRDY));

    // Accept '0' '1' or '2' only
    if (temp >= 0x30 && temp <= 0x32) {
      LPC_SPI0->TXDAT = temp;           // Transmit the '0', '1', or '2' character to put the slave board to sleep
    }
    else {
      LPC_SPI0->TXDAT = 0x30;           // Default is Sleep mode if garbage was entered
    }

    LED_Off(LED_GREEN);

    // Prompt user, wait for 'Esc' before proceeding
    DebugWaitEsc();

    // Wait for TXRDY on SPI0
    while (!(LPC_SPI0->STAT & STAT_TXRDY));

    // Send a wake-up byte (value is irrelevant)
    LPC_SPI0->TXDAT = 0x69;


  } // end of while(1)

}  // end of main
