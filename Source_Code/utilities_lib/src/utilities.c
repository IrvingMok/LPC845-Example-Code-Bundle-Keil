/*
 * utilities.c
 *
 *  Created on: Feb 15, 2016
 *
 */
#include "LPC8xx.h"
#include "syscon.h"
#include "uart.h"
#include "board.h"
#include <stdio.h>




/*****************************************************************************
** Function name:		PutTerminalString
**
** Description:			This function writes a string, one character at a time, to
**                  a USART. The string MUST be terminated by a terminating
**                  NUL ("\0") character. Both the USART instance and the string
**                  are passed into the function by reference using pointers.
**
** parameters:			ptr_LPC_USART: A pointer to any USART on the target.
**                  ptr_string: A pointer to an array of char terminated by NUL.
**
** Returned value:	void
**
*****************************************************************************/
void PutTerminalString(LPC_USART_TypeDef * ptr_LPC_USART, uint8_t * ptr_string) {
  uint32_t i = 0;
  uint8_t temp;

  do {
    temp = ptr_string[i++];                   // Get the current character
	  while (!((ptr_LPC_USART->STAT) & TXRDY)); // Wait for TX Ready
	  ptr_LPC_USART->TXDAT  = temp;             // Write the character to the TX buffer
  } while (temp != 0);                        // NUL character terminates

  return;
}



/*****************************************************************************
** Function name:		GetTerminalString
**
** Description:			This function accepts a string, one character at a time,
**                  from a USART. Both the USART instance and the array in
**                  which to store the string are passed into the function
**                  by reference using pointers.
**
** parameters:			ptr_LPC_USART: A pointer to any USART on the target.
**                  ptr_string: A pointer to an array of char.
**
** Returned value:	length of the string not including the NUL terminator
**
*****************************************************************************/
uint32_t GetTerminalString(LPC_USART_TypeDef * ptr_LPC_USART, char * ptr_string) {
  uint32_t i = 0;
  uint8_t temp = 0;

  while (1) {
    while (!((ptr_LPC_USART->STAT) & RXRDY));   // Wait for RX Ready
    temp = ptr_LPC_USART->RXDAT ;               // Read the current character from RXDAT 
    ptr_string[i++] = temp;                     // Append the current character to the rx buffer
    while (!((ptr_LPC_USART->STAT) & TXRDY));   // Wait for TXREADY
    ptr_LPC_USART->TXDAT  = temp;               // Echo it back to the terminal

    if (temp == 0x0D) {                         // CR (carriage return) is current character or buffer full. End of string.
      ptr_string[i++] = 0x0A;                   // Append a new line character to rx_buffer.
      ptr_string[i++] = 0x00;                   // Append a NUL terminator character to rx_buffer to complete the string.
      while (!((ptr_LPC_USART->STAT) & TXRDY)); // Wait for TXREADY
      ptr_LPC_USART->TXDAT  = 0x0A;             // Echo a NL (new line) character to the terminal.
      return(i);
    }
  }
}



/*****************************************************************************
** Function name:		GetConsoleString 
**
** Description:			Accepts non-whitespace characters from the console,
**                  terminated by the [Enter] key, and stores them in an array
**                  as a string. The NUL string terminator is added to the end
**                  of the array by the [Enter] key.
**                  Please avoid using whitespace characters!
**                  The number of characters is currently limited to 32 (31 plus
**                  NUL terminator).
**
** parameters:			None
**
** Returned value:  void
**
*****************************************************************************/
void GetConsoleString(char * ptr_string) {
  printf("Enter a string (with no white-space characters) terminated by [Enter]: \n\r");
  scanf("%s", ptr_string );
}



/*****************************************************************************
** Function name:		GetConsoleCharacter
**
** Description:			Returns one non-whitespace character from the console,
**                  terminated by the [Enter] key.
**                  Please avoid using whitespace characters!
**
** parameters:			pPromptString: A pointer to a NUL-terminated prompt string.
**
** Returned value:	unsigned char: The character typed before [Enter]
**
*****************************************************************************/
unsigned char GetConsoleCharacter(const char * pPromptString) {
  uint8_t j[10];
  printf(pPromptString);  // Print the prompt to the console
  scanf("%1c", &j[0]);    // Store the last character before CR in j[0]
  return(j[0]);
}



/*****************************************************************************
** Function name:   WaitConsoleEnter
**
** Description:     Waits for the user to type any key, discards it, then returns.
**
** parameters:      pPromptString: A pointer to a NUL-terminated prompt string.
**
** Returned value:  void
**
*****************************************************************************/
void WaitConsoleEnter(const char * pPromptString) {
  unsigned char j[10];
  printf(pPromptString);
  scanf((const char *) &j[0]); // Waits for any key
  return;
}



/*****************************************************************************
** Function name:   DebugWaitEsc
** Description:     Waits for 'Esc' char in the debug UART's RX data register
**                  then returns.
** parameters:      None
** Returned value:  void
*****************************************************************************/
void DebugWaitEsc(void) {
  printf("\n\rType 'Esc' to continue\n\r");
  while (1) {
    while (!((LPC_USART0->STAT) & RXRDY));   // Wait for RX Ready
    if (LPC_USART0->RXDAT  == 0x1B) {        // Read the current character from RXDAT 
      return;                                // If it's Esc, return, else loop again
    }
  }
}
    


/*****************************************************************************
** Function name:		GetConsoleInput
**
** Description:			Accepts characters from the console and concatenates them
** 						into an unsigned hex integer. Each number is terminated
** 						by the Enter key. The correct number of digits for the
** 						size of the number must be entered (one digit for 4-bits, two
** 						for 8-bits, etc.) including leading zeros if any.
** 						Only characters 0 - 9, a - f, A - F are processed.
** 						All others are treated as '0'.
**
** parameters:			number_of_digits (currently 1 and 2 are supported).
**
** Returned value:		The resulting unsigned integer
**
*****************************************************************************/
uint32_t GetConsoleInput(uint32_t number_of_digits) {
  uint32_t temp;
  uint32_t num[10];
  unsigned char j[10];
  uint32_t n;

  printf("\n\rEnter a %d-digit hex number (with no prefix): \n\r", number_of_digits);
  switch (number_of_digits) {
    default:
    case 1: scanf("%1c", &j[0]); break;   // NUM_DIGITS = 1
    case 2: scanf("%2c", &j[0]); break;   // NUM_DIGITS = 2
  //case 4: scanf("%4c", &j); break;      // NUM_DIGITS = 4
  //case 8: scanf("%8c", &j); break;      // NUM_DIGITS = 8
  }

  for (n = 0; n < number_of_digits; n++) {

    if ( ((j[n]<0x30) || (j[n]>0x66)) || ((j[n] > 0x39) && (j[n] < 0x41)) || ((j[n] > 0x46) && (j[n] < 0x61)) ) {
      printf("\n\rInvalid hex digit, ascii code 0x%x. Using '0' instead.", j[n]);
      j[n] = '0';
    }

    if (j[n] >= 0x61) {
      num[n] = j[n] - 0x57;
    }
    else {
      if (j[n] >= 0x41) {
  	  num[n] = j[n] - 0x37;
      }
      else {
  	  num[n] = j[n] - 0x30;
      }
    }

  } // end of for loop

  temp = 0;
  while(n--)
  {
    temp |= num[n]<<(4*((number_of_digits-1)-n)); // Reverse the order in which digits are received
  }

  return(temp);

} // End of function GetConsoleInput



//****************************************************************************
// Function name: Config_LED. Makes a port bit an output driving '1' (for LED off).
//****************************************************************************
void Config_LED(uint32_t port) {
  if (port <= 31) {
    LPC_SYSCON->SYSAHBCLKCTRL[0] |= GPIO0;   // Turn on clock to GPIO0
    LPC_GPIO_PORT->SET[0] = 1<<port;         // Make the port bit an output driving '1'
    LPC_GPIO_PORT->DIRSET[0]= 1<<port;
  }
  else if (port <= 63) {
    LPC_SYSCON->SYSAHBCLKCTRL[0] |= GPIO1;   // Turn on clock to GPIO1
    LPC_GPIO_PORT->SET[1] = 1<<(port-32);    // Make the port bit an output driving '1'
    LPC_GPIO_PORT->DIRSET[1]= 1<<(port-32);
  }
}

//****************************************************************************
// Function name: LED_Off. Makes a port bit drive '1' (for LED off).
//****************************************************************************
void LED_Off(uint32_t port) {
  if (port <= 31) {
    LPC_GPIO_PORT->SET[0] = 1<<port;
  }
  else if (port <= 63) {
    LPC_GPIO_PORT->SET[1] = 1<<(port-32);
  }
}

//****************************************************************************
// Function name: LED_On. Makes a port bit drive '0' (for LED on).
//****************************************************************************
void LED_On(uint32_t port) {
  if (port <= 31) {
    LPC_GPIO_PORT->CLR[0] = 1<<port;
  }
  else if (port <= 63) {
    LPC_GPIO_PORT->CLR[1] = 1<<(port-32);
  }
}




/*****************************************************************************
** Function name:  Setup_LPC8xx_Low_Power
**
** Description:    Sets PDAWAKECFG = PDRUNCFG and turns off some clocks.
**
** Parameters:     None
** Returned value: void
**
*****************************************************************************/
void Setup_LPC8xx_Low_Power() {

  // Configure PDAWAKECFG
  LPC_SYSCON->PDAWAKECFG = LPC_SYSCON->PDRUNCFG;

  // Turn clocks off to unused peripherals
  LPC_SYSCON->SYSAHBCLKCTRL0 &= ~(SWM | GPIO0 | GPIO1 | IOCON);

  return;
}





