//--------------------
// FAIM_and_Clocks.c
//--------------------

#include <stdio.h>
#include "LPC8xx.h"
#include "syscon.h"
#include "swm.h"
#include "faim.h"
#include "utilities.h"
#include "iap.h"
#include "FlashFaimPrg.h"

void setup_debug_uart(void);

#define CLKOUT_DIVIDE 10
#define CLKOUT_PIN P0_16

const char promptstring_clock[] = "Choose a clock to display, or 's' to skip:\n\r0 for fro_clk\n\r1 for main_clk\n\r2 for sys_pll0_clk\n\r3 for external_clk\n\r4 for wdt_osc_clk\n\r5 for system_ahb_clk\n\r6 for fro_div_clk\n\r";
const char promptstring_faimw[] = "Enter a FAIM word to program ('0' - '7'), or 's' to skip\n\r";
const char promptstring_faimr[] = "Enter a FAIM word to read ('0' - '7'), or 's' to skip\n\r";
const char promptstring_lpstrt[]= "Set the low-power start bit? Type 'y' to set, 'n' to clear. Reset the board to make a new value take effect\n\r";

uint32_t faim_write_data[NUM_FAIM_WORDS] =
{
  0x28000000,  // Word 0 Bit[1]: '0' = normal start, '1' = LP start 
  0x00001918,  // Word 1
  0x00000000,  // Word 2
  0x00000000,  // Word 3
  0xAAA00000,  // Word 4
  0xAAAAAAAA,  // Word 5
  0xAAAAAAAA,  // Word 6
  0xAAAAAAAA   // Word 7
};





int main(void) {

  unsigned char temp;
  uint32_t read_data, n;
  
  // Configure the debug uart (see Serial.c)
  setup_debug_uart();
	
  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (SWM);

  // Enable the clkout clock divider, divide by the previously defined value
  LPC_SYSCON->CLKOUTDIV = CLKOUT_DIVIDE;

  // Configure the SWM for clock out
  ConfigSWM(CLKOUT, CLKOUT_PIN);
  
  // Configure either SYSOSC or CLK_IN as source for external_clk 
  
  // Update the global clock variables (this has already been called by setup_debug_uart) 
  SystemCoreClockUpdate();
  
  

  while(1) {
    
    // Display internal clock frequencies and connect them to clkout clock divider
    while (2) {
      temp = GetConsoleCharacter((const char *)&promptstring_clock);
      // Accept '0' '1' '2' '3', '4', or '5' only
      if (temp < 0x30 || temp > 0x36) {
        printf("\n\n\r");
        break;
      }
      switch (temp) {
        case '0': printf("fro_clk freq. = %d Hz.\n\n\r", fro_clk);
                  LPC_SYSCON->CLKOUTSEL = CLKOUTSEL_FRO_CLK;
                  break;
        case '1': printf("main_clk freq. = %d Hz.\n\n\r", main_clk);
                  LPC_SYSCON->CLKOUTSEL = CLKOUTSEL_MAIN_CLK;
                  break;
        case '2': printf("sys_pll0_clk freq. = %d Hz.\n\n\r", sys_pll0_clk);
                  LPC_SYSCON->CLKOUTSEL = CLKOUTSEL_SYSPLL0_CLK;
                  break;
        case '3': printf("external_clk freq. is a mystery. See clock out pin.\n\n\r");
                  LPC_SYSCON->CLKOUTSEL = CLKOUTSEL_EXTERNAL_CLK;
                  break;
        case '4': printf("wdt_osc_clk freq. = %d Hz.\n\n\r", wdt_osc_clk);
                  LPC_SYSCON->CLKOUTSEL = CLKOUTSEL_WDT_OSC_CLK;
                  break;
        case '5': printf("system_ahb_clk freq. = %d Hz. Not available on clock out pin.\n\n\r", system_ahb_clk);
                  LPC_SYSCON->CLKOUTSEL = CLKOUTSEL_OFF;
                  break;
        case '6': printf("fro_div_clk freq. = %d Hz. Not available on clock out pin.\n\n\r", fro_div_clk);
                  LPC_SYSCON->CLKOUTSEL = CLKOUTSEL_OFF;
                  break;
      } // end of switch
    } // end of while 2
    
    // Read FAIM
    while (3) {
      temp = GetConsoleCharacter((const char *)&promptstring_faimr);
      // Accept '0' - '7' only
      if (temp < 0x30 || temp > 0x37) {
        printf("\n\n\r");
        break;
      }
      n = temp - 0x30;                             // n = the FAIM word number 
      FAIMRead(n, (uint32_t)&read_data);           // Call the read API to make the current data visible, and load it into read_data
      printf("The content of FAIM word %d is 0x%x\n\n\r", n, read_data);
    } // end of while 3

    // Write FAIM (Use with great caution! Know the repercussions before proceeding!)
    while (4) {
      temp = GetConsoleCharacter((const char *)&promptstring_faimw);
      // Accept '0' - '7' only
      if (temp < 0x30 || temp > 0x37) {
        printf("\n\n\r");
        break;
      }
      n = temp - 0x30;                             // n = the FAIM word number
      if (n == 0) {                                // If programming FAIM word 0
        temp = GetConsoleCharacter((const char *)&promptstring_lpstrt);
        if (temp == 'y') {
          faim_write_data[0] |= FAIM_LP_BOOT;   // Set the LP start bit
        }
        else {
          faim_write_data[0] &= ~(FAIM_LP_BOOT);// Clear the LP start bit
        }
      }
      FAIMWrite(n, (uint32_t)&faim_write_data[n]); // Call the write API to program the new data word from the faim_write_data array
      FAIMRead(n, (uint32_t)&read_data);           // Call the read API to make the new data visible, and load it into read_data variable
      printf("The new value of FAIM word %d is 0x%x\n\n\r", n, read_data);
    } // end of while 4

  } // end of while(1)

}  // end of main
