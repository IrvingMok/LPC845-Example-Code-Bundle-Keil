//--------------------
// ROM_Divide.c
//--------------------

#include <stdio.h>
#include "LPC8xx.h"
#include "rom_api.h"

void setup_debug_uart(void);


const int32_t I32Dividends[10] =  {-64052, -12002, -64498,  30883,  41538,  23513,  12972, 53852,  10253, 37127};
const int32_t I32Divisors[10] =   {-65535, -50973, -36407, -21845, - 7281,   7281,  21845, 36407,  50971, 65535};
const uint32_t U32Dividends[10] = { 37480,   4630,  59349,  20616,    727,  29784,  56556, 53166,  35237, 41271};
const uint32_t U32Divisors[10] =  {     1,   7283,  14563,  21845,  29127,  36407,  43691, 50973,  58253, 65535};


int main(void) {
  uint8_t k;

  // Declare variables of appropriate types to hold the return values
  int32_t direct_i32;             // For signed
  int32_t overload_i32;           // For signed
  //uint32_t direct_u32;          // For unsigned
  //uint32_t overload_u32;        // For unsigned
  //IDIV_RETURN_T direct_idivmod; // For signed
  UIDIV_RETURN_T direct_uidivmod; // For unsigned
  uint32_t div_val, mod_val;      // For "/" and "%"
  
  // Declare and initialize a pointer to the DIVIDE functions table
  ROM_DIV_API_T const *pROMDiv = LPC_ROM_API->divApiBase;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();
  
  // Using direct ROM API function call for integer divide
  printf("Direct API signed integer divide:\n\r");
  for (k=0; k!=10; k++) {
    direct_i32 = pROMDiv->sidiv(I32Dividends[k], I32Divisors[k]);
    printf("0x%X\n\r", direct_i32);
  }
  
  // Using overloaded "/" EABI function call for integer divide
  printf("\n\rOverloaded \"/\" operator signed integer divide:\n\r");
  for (k=0; k!=10; k++) {
    overload_i32 = (I32Dividends[k] / I32Divisors[k]);
    printf("0x%X\n\r", overload_i32);
  }
  
  // Using direct ROM API function call for unsigned integer divide with remainder
  printf("\n\rDirect API unsigned integer divide:\n\r");
  for (k=0; k!=10; k++) {
    direct_uidivmod = pROMDiv->uidivmod(U32Dividends[k], U32Divisors[k]);
    printf("Quotient = 0x%X, remainder = 0x%X\n\r", direct_uidivmod.quot, direct_uidivmod.rem);
  }
  
  // Using overloaded "/" and "%" EABI function calls for unsigned integer divide with remainder
  printf("\n\rOverloaded \"/\" and \"%%\" operators unsigned integer divide with remainder:\n\r");
  for (k=0; k!=10; k++) {
    div_val = (U32Dividends[k] / U32Divisors[k]);
    mod_val = (U32Dividends[k] % U32Divisors[k]);
    printf("Quotient = 0x%X, remainder = 0x%X\n\r", div_val, mod_val);
  }
  
  while(1);

}  // end of main
