/* Example_GPIO_PatternMatch_ISR.c */

#include <stdio.h>
#include "LPC8xx.h"
#include "gpio.h"
#include "utilities.h"



/*****************************************************************************
** Function name:		PatternMatch_ISR
**
** Descriptions:		General call made by the PININT0 - PININT7 IRQs.
**
** parameters:			IRQ number(0~7)
**
** Returned value:	None
**
*****************************************************************************/
void PatternMatch_ISR ( uint32_t irq_num )
{
	uint32_t condition;

	LED_Off(LED_RED);
	LED_Off(LED_GREEN);
	LED_Off(LED_BLUE);

	condition = ((LPC_PIN_INT->PMCTRL)>>24) & 0xFF;   // Read the PMAT bits into low byte
	if (condition == ((1<<1) | (1<<4))) {             // 0x12 =  both PININT4 and PININT1
		LED_On(LED_GREEN);
		LPC_PIN_INT->PMCTRL = 0;                      // Erase pattern match edge detect history
		LPC_PIN_INT->PMCTRL = 1;                      // Reenable pattern match
		LPC_PIN_INT->FALL = 0x0E;                     // Clear falling edge detects on PINTSEL3-1 for NVIC so we don't reenter ISR
		LPC_PIN_INT->RISE = 0x03;                     // Clear rising edge detects on PINTSEL1,0 for NVIC so we don't reenter ISR
		NVIC->ICPR[0] = 0x12000000;                   // Clear PININT4_IRQ and PININT1_IRQ in NVIC so we don't reenter ISR
    return;
	}

	if (condition == (1<<1)) {                      // 0x01 = PININT1 only
		LED_On(LED_RED);
	  LPC_PIN_INT->PMCTRL = 0;                      // Erase pattern match edge detect history
	  LPC_PIN_INT->PMCTRL = 1;                      // Reenable pattern match
	  LPC_PIN_INT->RISE = 0x03;
	  return;
	}

  if (condition == (1<<4)) {                      // 0x10 = PININT4 only
    LED_On(LED_BLUE);
    LPC_PIN_INT->PMCTRL = 0;                      // Erase pattern match edge detect history
    LPC_PIN_INT->PMCTRL = 1;                      // Reenable pattern match
	  LPC_PIN_INT->FALL = 0x0E;
	  return;
	}

	if (condition == 0) {
	  printf("ISPR = 0x%x", NVIC->ISPR);
	  while(1);
	}

	// Any other condition is unexpected, stop here
	while(1);
	//return;
}



/*****************************************************************************
** Function name:		PININT0_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void PININT0_IRQHandler(void)
{
  PatternMatch_ISR(0);
  return;
}

/*****************************************************************************
** Function name:		PININT1_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void PININT1_IRQHandler(void)
{
  PatternMatch_ISR(1);
  return;
}

/*****************************************************************************
** Function name:		PININT2_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void PININT2_IRQHandler(void)
{
  PatternMatch_ISR(2);
  return;
}

/*****************************************************************************
** Function name:		PININT3_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void PIN3_IRQHandler(void)
{
  PatternMatch_ISR(3);
  return;
}

/*****************************************************************************
** Function name:		PININT4_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void PININT4_IRQHandler(void)
{
  PatternMatch_ISR(4);
  return;
}

/*****************************************************************************
** Function name:		PININT5_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void PININT5_IRQHandler(void)
{
  PatternMatch_ISR(5);
  return;
}

/*****************************************************************************
** Function name:		PININT6_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void PININT6_IRQHandler(void)
{
  PatternMatch_ISR(6);
  return;
}

/*****************************************************************************
** Function name:		PININT7_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void PININT7_IRQHandler(void)
{
  PatternMatch_ISR(7);
  return;
}


