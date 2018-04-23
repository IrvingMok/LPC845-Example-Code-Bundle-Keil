//--------------------
// Flash_IAP_main.c
//--------------------

#include <stdio.h>
#include "LPC8xx.h"
#include "syscon.h"
#include "swm.h"
#include "faim.h"
#include "utilities.h"
#include "iap.h"
#include "FlashFaimPrg.h"
#include "chip_setup.h"

void setup_debug_uart(void);

#define WHICH_SECTOR END_SECTOR // 63
#define WHICH_PAGE   15
#define ADDR ((WHICH_SECTOR * SECTOR_SIZE) + (WHICH_PAGE * PAGE_SIZE))
#define NUM_BYTES    64

char the_data[NUM_BYTES];



int main(void) {
  uint32_t k;
  uint32_t ret_code;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();
  
  // Update the global clock variables
  SystemCoreClockUpdate();
  
  // Initialize the data array
  for (k=0; k<NUM_BYTES; k++)
    the_data[k] = 0x55;

  // Print the prompt
  printf("Enter something to be programmed to Flash memory:\n\r");

  // Load the data array with the user's input
  GetTerminalString(pDBGU, the_data);

  // Disable all interrupts during IAP calls
  __disable_irq();

  // IAP Prepare Sectors for Write command can be skipped since we are using 
  // the drivers in FlashFaimPrg.c which do the Prepare.

  // IAP Erase Page command (pass the start address of the last page in the last sector; sector 63, page 15)
  // The ending address, or any other address in the page will work also. The driver handles the math.
  if ((ret_code = ErasePage(ADDR))) {
    printf ("ErasePage failed. Return code was 0x%X\n\r", ret_code);
    while(1);
  }

  // IAP Copy RAM to Flash command
  if ((ret_code = Program(ADDR, NUM_BYTES, (uint8_t *)the_data))) {
    printf ("Copy RAM to Flash failed. Return code was 0x%X\n\r", ret_code);
    while(1);
  }

  // IAP Compare command
  if ((ret_code = Compare(ADDR, (uint32_t)the_data, NUM_BYTES))) {
    printf ("Compare failed. Return code was 0x%X\n\r", ret_code);
    while(1);
  }

  // Reenable interrupts
  __enable_irq();

  printf("Programming succeeded.\n\n\r");

  while (1) {
  } // end of while(1)

}  // end of main
