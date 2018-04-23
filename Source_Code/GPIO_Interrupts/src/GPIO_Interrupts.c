/*
 ===============================================================================
 Name        : GPIO_Interrupts.c
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

//
// Pin Interrupt 0 ISR
//
void PININT0_IRQHandler(void)
{
  if (LPC_PIN_INT->RISE & (1<<0)) {
    LED_On(LED_RED);                // Rising edge on PIN INT0, LED on
    LPC_PIN_INT->RISE = 1<<0;       // Clear the interrupt flag
    return;
  }
  if (LPC_PIN_INT->FALL & (1<<0)) {
    LED_Off(LED_RED);               // Falling edge on PIN INT0, LED off
    LPC_PIN_INT->FALL = 1<<0;       // Clear the interrupt flag
    return;
  }
}

//
// Pin Interrupt 1 ISR
//
void PININT1_IRQHandler(void)
{
  if (LPC_PIN_INT->RISE & (1<<1)) {
    LED_On(LED_GREEN);              // Rising edge on PIN INT1, LED on
    LPC_PIN_INT->RISE = 1<<1;       // Clear the interrupt flag
    return;
  }
  if (LPC_PIN_INT->FALL & (1<<1)) {
    LED_Off(LED_GREEN);             // Falling edge on PIN INT1, LED off
    LPC_PIN_INT->FALL = 1<<1;       // Clear the interrupt flag
  return;
  }
}


//
// main routine
//
int main(void) {

  // Local variables
  uint32_t number, temp;
  
  // Configure the debug uart (see Serial.c)
  setup_debug_uart();
	
  // Peripheral reset to the GPIO0 and pin interrupt modules. '0' asserts, '1' deasserts reset.
  LPC_SYSCON->PRESETCTRL0 &=  (GPIO0_RST_N & GPIOINT_RST_N);
  LPC_SYSCON->PRESETCTRL0 |= ~(GPIO0_RST_N & GPIOINT_RST_N);
  
  // Enable clock to GPIO0 and pin interrupt modules.
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (GPIO0 | GPIO_INT);

  // Config. (red LED), (blue LED), (green LED) as outputs,
  // with LEDs off ('1' = off).
  Config_LED(LED_RED);
  Config_LED(LED_BLUE);
  Config_LED(LED_GREEN);
  
  // Configure P0.15, P0.14 as pin interrupts 1, 0
  //
  // The interrupts can be activated in two ways:
  //
  // 1. By configuring the port bits as inputs, and driving the package pins high and low externally.
  //
  // 2. By configuring the port bits as outputs, and writing to their port bit registers via software.
  //    The enabled GPIO pin interrupt and pattern match logic will always monitor the state of the pins,
  //    even when configured as outputs.
  //
  // We will use approach #2.

  // Make PORT0.15, PORT0.14 outputs driving '0'.
  LPC_GPIO_PORT->CLR0 = 0xC000;           // Clear P0.15, P0.14 to '00'
  LPC_GPIO_PORT->DIR0 |= 0xC000;          // P0.15, P0.14 to output

  // Configure P0.15 - P0.14 as pin interrupts 1 - 0 by writing to the PINTSELs in SYSCON
  LPC_SYSCON->PINTSEL[0] = 14;  // PINTSEL0 is P0.14
  LPC_SYSCON->PINTSEL[1] = 15;  // PINTSEL1 is P0.15

  // Configure the Pin interrupt mode register (a.k.a ISEL) for edge-sensitive on PINTSEL1,0
  LPC_PIN_INT->ISEL = 0x0;

  // Configure the IENR (pin interrupt enable rising) for rising edges on PINTSEL0,1
  LPC_PIN_INT->IENR = 0x3;

  // Configure the IENF (pin interrupt enable falling) for falling edges on PINTSEL0,1
  LPC_PIN_INT->IENF = 0x3;

  // Clear any pending or left-over interrupt flags
  LPC_PIN_INT->IST = 0xFF;

  // Enable pin interrupts 0 - 1 in the NVIC (see core_cm0plus.h)
  NVIC_EnableIRQ(PININT0_IRQn);
  NVIC_EnableIRQ(PININT1_IRQn);

  while (1) {

    number = GetConsoleInput(1);                 // 
    //printf("%#x\n\r", number);                 // Echo it if desired
    temp = (~0xC000) & (LPC_GPIO_PORT->PIN0);    // Read the pin register, preserve all except bits 15, 14
    LPC_GPIO_PORT->PIN0 = temp | (number << 14); // Write back the new data into bits 15, 14

  } // end of while(1)

} // end of main
