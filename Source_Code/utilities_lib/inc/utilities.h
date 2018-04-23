/*
 * utilities.h
 *
 *  Created on: Feb 16, 2016
 *      Author:
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "swm.h"
#include "board.h"


void PutTerminalString(LPC_USART_TypeDef * ptr_LPC_USART, uint8_t * ptr_string);
uint32_t GetTerminalString(LPC_USART_TypeDef * ptr_LPC_USART, char * ptr_string);
void GetConsoleString(char * ptr_string);
uint32_t GetConsoleInput(uint32_t number_of_digits);
unsigned char GetConsoleCharacter(const char * pPromptString);
void WaitConsoleEnter(const char * pPromptString);
void DebugWaitEsc(void);
void Config_LED(uint32_t port);
void LED_Off(uint32_t port);
void LED_On(uint32_t port);
void Setup_LPC8xx_Low_Power(void);

#endif /* UTILITIES_H_ */
