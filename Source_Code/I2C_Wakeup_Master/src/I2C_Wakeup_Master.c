/*
===============================================================================
 Name        : I2C_Wakeup_Master.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include <stdio.h>
#include "LPC8xx.h"
#include "i2c.h"
#include "syscon.h"
#include "swm.h"
#include "utilities.h"
#include "iocon.h"
#include "chip_setup.h"

extern void setup_debug_uart(void);


#define I2CBAUD 100000

#if (I2CNUM != 0)
  #define SCL_PIN       P1_12         // P0_10 for I2C0
  #define SCL_IOCON_REG PIO1_12
  #define SDA_PIN       P1_13         // P0_11 for I2C0
  #define SDA_IOCON_REG PIO1_13
#endif

#define slave_board_address 0x56

const char promptstring[] = "Choose a low power mode for the slave:\n\r0 for Sleep\n\r1 for Deep-sleep\n\r2 for Power-down\n\r";


// Program flow
// 1. Configure I2C as master (requires external connections with I2C on the slave board, don't forget the external pull-ups.
// 2. Prompt the user to type a character ('0', '1', or '2') then [Return]
// 3. Transmit the character to the slave board's I2C address to put the slave to sleep.
// 4. Prompt the user to type [Esc] to proceed with waking up the slave.
// 5. Transmit the slave board's address to wake up the slave.


int main(void) {

  unsigned char temp;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();

  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (I2CINST|SWM|IOCON);

  // SWM settings for I2C0 (master):
  // P0.10 = I2C0_SCL
  // P0.11 = I2C0_SDA
  //
  // SWM settings for other I2C (master):
  // SCL_PIN = I2C1_SCL
  // SDA_PIN = I2C1_SDA
  //
  // Configure the SWM 
#if (I2CNUM == 0)
  LPC_SWM->PINENABLE0 &= ~(I2C0_SCL|I2C0_SDA);
#else
  ConfigSWM(I2CINSTSCL, SCL_PIN);
  ConfigSWM(I2CINSTSDA, SDA_PIN);
#endif
  
  // If not using I2C0, put the appropriate standard I/O pins into quasi-open-drain mode
#if (I2CNUM != 0)
  LPC_IOCON->SCL_IOCON_REG |= 1<<IOCON_OD;
  LPC_IOCON->SDA_IOCON_REG |= 1<<IOCON_OD;
#endif

  // Give I2C a reset
  LPC_SYSCON->PRESETCTRL0 &=  (I2CINSTRSTN);
  LPC_SYSCON->PRESETCTRL0 |= ~(I2CINSTRSTN);

  // Enable main_clk as function clock to I2C
  LPC_SYSCON->I2CINSTCLKSEL = FCLKSEL_MAIN_CLK;
  
  // Get main_clk frequency
  SystemCoreClockUpdate();
  
  // Configure the I2C clock divider
  // Use default clock high and clock low times (= 2 clocks each)
  // Thus, 4 main_clk periods for each I2C bit time.
  // So the I2C clock divider must produce main_clk/4*I2CBAUD clocks per second.
  // Remember, value written to DIV divides by value+1
  pI2CINST->DIV = (main_clk/(4*I2CBAUD)) - 1;

  // Configure the I2C CFG register:
  // Master enable = true
  // Slave enable = false
  // Monitor enable = false
  // Time-out enable = false
  // Monitor function clock stretching = false
  //
  pI2CINST->CFG = CFG_MSTENA;

  Config_LED(LED_GREEN);

  while(1) {

    LED_On(LED_GREEN);

    // Prompt user to select a low-power mode for the slave (see utilities_lib)
    temp = GetConsoleCharacter((const char *)&promptstring);

    // Wait until master pending and idle
    WaitI2CMasterState(pI2CINST, I2C_STAT_MSTST_IDLE);

    pI2CINST->MSTDAT = (slave_board_address<<1) | 0; // Send the address with 0 for RWn bit (WRITE)
    pI2CINST->MSTCTL = CTL_MSTSTART;                 // Start the transaction.

    // Wait for the address to be ACK'd (stuck waiting for master pending here it appears)
    WaitI2CMasterState(pI2CINST, I2C_STAT_MSTST_TX);

    // Accept '0' '1' or '2' only
    if (temp >= 0x30 && temp <= 0x32) {
      pI2CINST->MSTDAT = temp;                       // '0', '1', or '2' character to put the slave board to sleep
    }
    else {
      pI2CINST->MSTDAT = 0x30;                       // Default is Sleep mode if garbage was entered
    }

    // Send the data
    pI2CINST->MSTCTL = CTL_MSTCONTINUE;

    // Wait for the data to be ACK'd
    WaitI2CMasterState(pI2CINST, I2C_STAT_MSTST_TX);

    // Send a stop to end the transaction
    pI2CINST->MSTCTL = CTL_MSTSTOP;

    LED_Off(LED_GREEN);

    // Prompt user, wait for 'Esc' before proceeding
    DebugWaitEsc();
		
    // Wait until master pending and idle
    WaitI2CMasterState(pI2CINST, I2C_STAT_MSTST_IDLE);

    // Send a wakeup transaction (slave_board's address with a dummy write command, then stop)
    pI2CINST->MSTDAT = (slave_board_address<<1) | 0; // Send the address with 0 for RWn bit (WRITE)
    pI2CINST->MSTCTL = CTL_MSTSTART;                 // Start the transaction

    // Wait for the address to be ACK'd
    WaitI2CMasterState(pI2CINST, I2C_STAT_MSTST_TX);

    // Send a stop to end the transaction
    pI2CINST->MSTCTL = CTL_MSTSTOP;

  } // end of while(1)

}  // end of main
