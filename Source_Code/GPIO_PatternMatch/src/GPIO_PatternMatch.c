/*
===============================================================================
 Name        : GPIO_PatternMatch.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include <stdio.h>
#include "LPC8xx.h"
#include "gpio.h"
#include "syscon.h"
#include "utilities.h"


extern void setup_debug_uart(void);



int main(void) {

  // Local variables
  uint32_t number, temp;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();
	
  // Peripheral reset to the GPIO and pin interrupt modules. '0' asserts, '1' deasserts reset.
  LPC_SYSCON->PRESETCTRL0 &=  (GPIO0_RST_N & GPIO1_RST_N & GPIOINT_RST_N);
  LPC_SYSCON->PRESETCTRL0 |= ~(GPIO0_RST_N & GPIO1_RST_N & GPIOINT_RST_N);
  
  // Enable clocks.
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (GPIO0 | GPIO1 | GPIO_INT | SWM);
  
  // Disable fixed-pin functions on P0.8 and P0.9
  LPC_SWM->PINENABLE0 |= (XTALIN | XTALOUT);

  // Config. ports (red LED), (blue LED), (green LED) as outputs,
  // with LEDs off ('1' = off).
  Config_LED(LED_RED);
  Config_LED(LED_BLUE);
  Config_LED(LED_GREEN);


//   --------------------------------------------------------------------------------
//   PINTSEL:          IN0     IN1     IN2     IN3     IN4     IN5     IN6     IN7
//   GPIO port bit:    P0.08   P0.09   P0.10   P0.11   unused  unused  unused  unused

//   Boolean minterm:  |-Term0-|       |---- Term1 ----|       |---- Term2 ----|
//   Bit-slice:        0       1       2       3       4       5       6       7
//   Input:            IN0     IN1     IN1     IN2     IN3     unused  unused  unused
//   Condition:        re      re      fe      fe      fe      -       -       -

//   Pin interrupt:    0       1       2       3       4       5       6       7
//   Associated term:  unused  Term1   unused  unused  Term2   unused  unused  unused
//   --------------------------------------------------------------------------------


  // Configure p0.8 - p0.11 as pattern match inputs 0 - 3 by writing to PINTSEL0-3 in SYSCON
  LPC_SYSCON->PINTSEL[0] =  P0_8; // IN0 is P0.8
  LPC_SYSCON->PINTSEL[1] =  P0_9; // IN1 is P0.9
  LPC_SYSCON->PINTSEL[2] = P0_10; // IN2 is P0.10
  LPC_SYSCON->PINTSEL[3] = P0_11; // IN3 is P0.11


  // Setup the PMSRC register
  //                    B.S.0      B.S.1       B.S.2       B.S.3       B.S.4       B.S.5       B.S.6       B.S.7
  //                     IN0        IN1         IN1         IN2         IN3        unused      unused      unused
  LPC_PIN_INT->PMSRC = (0 << 8) | (1 << 11) | (1 << 14) | (2 << 17) | (3 << 20) | (0 << 23) | (0 << 26) | (0 << 29);


  // Setup the PMCFG register to configure the bit slice conditions and the endpoints.
  // Users can easily create macros for the conditions and the shifters.
  LPC_PIN_INT->PMCFG = (1 << 1)  |     // Slice 1 is endpoint of Term0
                       (1 << 4)  |     // Slice 4 is endpoint of Term1
                       (1 << 8)  |     // Slice 0 rising edge
                       (1 << 11) |     // Slice 1 rising edge
                       (2 << 14) |     // Slice 2 falling edge
                       (2 << 17) |     // Slice 3 falling edge
                       (2 << 20) |     // Slice 4 falling edge
                       (6 << 23) |     // Slice 5 unused
                       (6 << 26) |     // Slice 6 unused
                       (6UL << 29);    // Slice 7 unused


  // The pattern match inputs can be activated in two ways:
  //
  // 1. By configuring the port bits as inputs, and driving the package pins high and low externally.
  //
  // 2. By configuring the port bits as outputs, and writing to their port bit registers via software.
  //    The enabled GPIO pin interrupt and pattern match logic will always monitor the state of the pins,
  //    even when configured as outputs.
  //
  //
  // We will use approach #2.

  // Make ports outputs driving '0'.
  LPC_GPIO_PORT->CLR0 = 0xF00;                   // Clear P0.11 - P0.8 to '0000'
  LPC_GPIO_PORT->DIR0 |= 0xF00;                  // P0.11 - P0.8 to output
  
  // Clear any pending or left-over edge-detect logic
  LPC_PIN_INT->IST = 0xFF;
  LPC_PIN_INT->RISE = 0xFF;
  LPC_PIN_INT->FALL = 0xFF;

  // Enable pin interrupts 1, 4 in the NVIC (see core_cm0plus.h)
  NVIC_EnableIRQ((IRQn_Type) (PININT1_IRQn));
  NVIC_EnableIRQ((IRQn_Type) (PININT4_IRQn));

  // Enable the pattern match interrupts
  LPC_PIN_INT->PMCTRL = 1;


  while (2) {

    number = GetConsoleInput(1);                 // 
    temp = (~0xF00) & (LPC_GPIO_PORT->PIN0);     // Read the pin register, preserve all except bits 11-8
    LPC_GPIO_PORT->PIN0 = temp | (number << 8);  // Write back the new data into bits 11-8

  }

} // end of main
