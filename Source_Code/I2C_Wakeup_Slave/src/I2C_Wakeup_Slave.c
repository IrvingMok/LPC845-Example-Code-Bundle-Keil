/*
===============================================================================
 Name        : I2C_Wakeup_Slave.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


#include "LPC8xx.h"
#include "i2c.h"
#include "syscon.h"
#include "swm.h"
#include "utilities.h"
#include "iocon.h"
#include "chip_setup.h"

#define I2CBAUD 100000

#if (I2CNUM != 0)
  #define SCL_PIN       P1_12         // P0_10 for I2C0
  #define SCL_IOCON_REG PIO1_12
  #define SDA_PIN       P1_13         // P0_11 for I2C0
  #define SDA_IOCON_REG PIO1_13
#endif

#define slave_board_address 0x56

volatile enum {false, true} handshake;
volatile uint32_t slave_rx_data;

// ----------------------------------------------------------------------------
// I2C interrupt service routine
// ----------------------------------------------------------------------------
void I2CINSTIRQH(void) {

  LPC_SYSCON->SYSAHBCLKCTRL[0] |= GPIO0;                    // Turn GPIO clock back on for the LEDs

  if ((pI2CINST->STAT & SLAVE_STATE_MASK) == STAT_SLVADDR) {
    pI2CINST->SLVCTL = CTL_SLVCONTINUE;                     // ACK the address
    return;
  }

  if ((pI2CINST->STAT & SLAVE_STATE_MASK) == STAT_SLVRX) {
    slave_rx_data = pI2CINST->SLVDAT;                       // Read the data, store it
    pI2CINST->SLVCTL = CTL_SLVCONTINUE;                     // ACK the data
    handshake = true;                                       // Set flag for main
    return;
  }

  while(1);                                                 // Any other slave states to be handled, add the code here
}

// ----------------------------------------------------------------------------
// main routine
// ----------------------------------------------------------------------------
int main(void) {

  int k;
  uint32_t * addr = (uint32_t *)LPC_IOCON_BASE;

  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (I2CINST|SWM|GPIO0|GPIO1|IOCON);

  // Power down the various analog blocks that aren't needed
  LPC_SYSCON->PDSLEEPCFG |= (BOD_PD|WDTOSC_PD);
  LPC_SYSCON->PDRUNCFG |= (BOD_PD|ADC_PD|SYSOSC_PD|WDTOSC_PD|SYSPLL_PD|VREF2_PD|DAC0_PD|DAC1_PD|ACMP_PD);

  // Disable pull-ups and pull-downs in IOCON
  for (k = 0; k < NUM_IOCON_SLOTS; k++) {
    *(addr + k) &= ~(0x3<<3);
  }

  // Make all GPIO ports outputs driving '1'
  LPC_GPIO_PORT->DIR0 = 0xFFFFFFFF;
  LPC_GPIO_PORT->PIN0 = 0xFFFFFFFF;
  LPC_GPIO_PORT->DIR1 = 0xFFFFFFFF;
  LPC_GPIO_PORT->PIN1 = 0xFFFFFFFF;

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

  // SWM settings for I2C0 (slave):
  // P0.11 = I2C0_SDA
  // P0.10 = I2C0_SCL
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
  // Master enable = false
  // Slave enable = true
  // Monitor enable = false
  // Time-out enable = false
  // Monitor function clock stretching = false
  //
  pI2CINST->CFG = CFG_SLVENA;

  // Give the slave an address in one of its four address regs, and activate it
  pI2CINST->SLVADR3 = (slave_board_address<<1) | 0;

  // Enable the I2C slave pending interrupt
  pI2CINST->INTENSET = STAT_SLVPEND;
  NVIC_EnableIRQ(I2CINSTIRQn);

  // Enable I2C0 interrupt as wake-up interrupt in SYSCON
  //LPC_SYSCON->STARTERP1 = I2C0_INT_WAKEUP;
  LPC_SYSCON->STARTERP1 = I2CINSTINTW;

  Config_LED(LED_GREEN);

  while(1) {

    LED_On(LED_GREEN);

    // Clear handshake, will be set by ISR
    handshake = false;

    // Wait for handshake
    while(!handshake);

    LED_Off(LED_GREEN);

    switch (slave_rx_data) {
      case(0x30):
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
