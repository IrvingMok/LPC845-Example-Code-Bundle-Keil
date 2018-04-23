/*
===============================================================================
 Name        : Analog_Comparator.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


// Configuration
// * Voltage ladder reference is Supply Vdd
// * Positive comparator input is voltage ladder out
// * Negative comparator input is internal 0.9V band-gap
// * No hysteresis.
// * Both rising and falling edges generate the comparator interrupt.
// * In the ISR read COMPSTAT to determine if rising or falling.
// * If rising, turn on the green LED, if falling, turn on the red LED, and clear the interrupt.
//
// The available voltage values are roughly 0 - 3.3V in 0.1V increments
// (SupplyVdd - Vss) / 32 = (3.3V / 32) ~= 0.1V
//
// Program flow
// Configure the module as described above.
// In the main while(1) loop the user is prompted to enter a 2-digit hex number (only the 5 LSBs are used).
// The number entered by the user is written to the voltage ladder select field.
// Values from 0x0 to 0x8 produce voltages from 0 to 0.8V which are below the 0.9V band gap reference.
// In these cases the red LED is lit.
// Values from 0x9 to 0x1F produce voltages from 0.9V to 3.3V which are above the 0.9V band-gap reference.
// In these cases the green LED is lit.
// Whenever a number is entered which causes the comparator to change state, a falling or rising edge is detected
// which generates an interrupt. The ISR reads the COMPSTAT bit to determine the state of the comparator output, and
// lights the appropriate LED before returning.


#include <stdio.h>
#include "LPC8xx.h"
#include "acomp.h"
#include "utilities.h"
#include "syscon.h"


extern void setup_debug_uart(void);


//
// Comparator Interrupt Service Routine
//
void CMP_IRQHandler(void) {

  uint32_t temp = LPC_CMP->CTRL;

  LED_Off(LED_RED);
  LED_Off(LED_GREEN);

  // If comparator out is a '1' light the green LED, otherwise light the red LED
  if (temp & (1<<COMPSTAT)) {
    LED_On(LED_GREEN);
  }
  else {
    LED_On(LED_RED);
  }

  // Clear the edge-detects interrupt flag
  LPC_CMP->CTRL |= 1<<EDGECLR;
  LPC_CMP->CTRL &= ~(1<<EDGECLR);

  return;
}


//
// main routine
//
int main(void) {
  uint32_t number, temp;
  	
  // Configure the debug uart (see Serial.c)
  setup_debug_uart();

  // Enable power to the Acomp
  LPC_SYSCON->PDRUNCFG &= ~(ACMP_PD);

  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (ACMP|GPIO);

  // Give the Analog comparator module a reset
  LPC_SYSCON->PRESETCTRL0 &= (ACMP_RST_N);
  LPC_SYSCON->PRESETCTRL0 |= ~(ACMP_RST_N);

  // CTRL register setup
  // Hysteresis=none, INTENA=interrupt_enabled, Vm=bandgap, Vp=Vladder, EdgeDetect=both
  LPC_CMP->CTRL = (NONE<<HYS) | (1<<INTENA) | (V_BANDGAP<<COMP_VM_SEL) | (V_LADDER_OUT<<COMP_VP_SEL) | (BOTH<<EDGESEL);

  // Voltage ladder setup
  // Reference = Vdd, select = 0x00, ladder enabled
  LPC_CMP->LAD = (SUPPLY_VDD<<LADREF) | (0x00<<LADSEL) | (1<<LADEN);

  // Clear any edge-detects
  LPC_CMP->CTRL |= 1<<EDGECLR;
  LPC_CMP->CTRL &= ~(1<<EDGECLR);

  // Enable the Acomp interrupt
  NVIC_EnableIRQ(CMP_IRQn);

  Config_LED(LED_RED);
  Config_LED(LED_GREEN);
  LED_On(LED_RED);

  while(1) {

    // Get a 2-digit unsigned hex number from the user
    number = GetConsoleInput(2);
    //printf("\n\r%#x\n\r", number);

    // Truncate to 5 bits
    number &= 0x1F;

    // Read the current contents of the LAD register
    temp = LPC_CMP->LAD;

    // Clear out the old value for the select field
    temp &= ~(0x1F<<LADSEL);

    // Replace it with the new number
    temp |= number<<1;
    LPC_CMP->LAD = temp;

  } // end of while(1)
  
} // end of main
