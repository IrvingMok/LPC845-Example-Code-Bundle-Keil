//--------------------
// DAC.c
//--------------------

#include <stdio.h>
#include "LPC8xx.h"
#include "syscon.h"
#include "iocon.h"
#include "SWM.h"
#include "DAC.h"
#include "gpio.h"
#include "utilities.h"
#include "chip_setup.h"

void setup_debug_uart(void);

const char promptstring[] = "Choose a note:\n\r1 for A4 (A440)\n\r2 for B4\n\r3 for C5 (Tenor C)\n\r4 for D5\n\r5 for E5\n\r6 for F5\n\r7 for G5\n\r8 for A5\n\r0 for silence (DC)\n\r";

#define SINE 1                             // '0' for trianglular, '1' for sinusoidal
#define DC_VALUE 0x7777
#define EN_AUDIO 1                         // '1' to enable, '0' to disable the audio amp.
#define DAC1_IRQHandler PININT5_IRQHandler // DAC1 shares NVIC slot with PININT5 for LPC845


#if (SINE == 0)
const uint32_t waveform[] = {0x0000, 0x1111, 0x2222, 0x3333, 
                             0x4444, 0x5555, 0x6666, 0x7777, 
                             0x8888, 0x9999, 0xAAAA, 0xBBBB,
                             0xCCCC, 0xDDDD, 0xEEEE, 0xFFFF,
                             0xFFFF, 0xEEEE, 0xDDDD, 0xCCCC,
                             0xBBBB, 0xAAAA, 0x9999, 0x8888,
                             0x7777, 0x6666, 0x5555, 0x4444,
                             0x3333, 0x2222, 0x1111, 0x0000};
#else
const uint32_t waveform[] = {32768, 39160, 45307, 50972,
                             55938, 60013, 63041, 64905,
                             65535, 64905, 63041, 60013,
                             55938, 50972, 45307, 39160,
                             32768, 26375, 20228, 14563,
                              9597,  5522,  2494,   630,
                                 0,   630,  2494,  5522,
                              9597, 14563, 20228, 26375};
#endif
#define SamplesPerCycle (sizeof(waveform)/4)


// DAC interrupt service routine
void DACn_IRQHandler(void) {
  static uint32_t windex = 0;
  LPC_DACn->CR = waveform[windex++];
  if (windex == SamplesPerCycle) {
    windex = 0;
  }
}


// main routine
int main(void) {
  uint32_t temp, Freq, Div;
  unsigned char temp_char;
  unsigned char note[2];

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();

  printf("DAC example\n\r");
  
  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= SWM|IOCON|GPIO1|GPIO_INT;
  Enable_Periph_Clock(CLK_DACn);
  
  // Do the following only for DAC1
#if (DACn == 1)
  // Clear the PININT5 interrupt flag that may be pending and shut off clock to GPIO interrupts
  LPC_PIN_INT->IST = 1<<5;
  LPC_SYSCON->SYSAHBCLKCTRL[0] &= ~(GPIO_INT); 
  // For the audio amp on Xpresso board, enable pin as output and drive it as requested
  GPIOSetDir(AUDIO_AMP_ENABLE_PORT, AUDIO_AMP_ENABLE_PIN, OUTPUT); 
  GPIOSetBitValue(AUDIO_AMP_ENABLE_PORT, AUDIO_AMP_ENABLE_PIN, EN_AUDIO);
#endif // (DACn == 1)

  // Enable DACOUT on its pin
  LPC_SWM->PINENABLE0 &= ~(DACOUTn);
  
  // Configure the DACOUT pin. Inactive mode (no pullups/pulldowns), DAC function enabled
  temp = (LPC_IOCON->DACOUTn_PIN) & (IOCON_MODE_MASK) & (IOCON_DACEN_MASK);
  temp |= (0<<IOCON_MODE)|(1<<IOCON_DAC_ENABLE);
  LPC_IOCON->DACOUTn_PIN = temp;
  
  // Configure the 16-bit DAC counter with an initial Freq. (Can you hear me now?)
  // SamplesPerCycle * Freq = samples/sec
  Freq = 440;
  Div = (system_ahb_clk)/(SamplesPerCycle * Freq);
  LPC_DACn->CNTVAL = Div - 1;
  
  // Power to the DAC!
  LPC_SYSCON->PDRUNCFG &= ~(DACn_PD);

  // Write to the CTRL register to start the action. Double buffering enabled, Count enabled.
  LPC_DACn->CTRL = (1<<DAC_DBLBUF_ENA) | (1<<DAC_CNT_ENA);
  
  // Enable the DAC interrupt
  NVIC_EnableIRQ(DACn_IRQn);
  
  while(1) {

    temp_char = GetConsoleCharacter((const char *)&promptstring);

    switch (temp_char) {
      case '1': Freq = 440; note[0] = 'A'; note[1] = '4'; break;
      case '2': Freq = 494; note[0] = 'B'; note[1] = '4'; break;
      case '3': Freq = 523; note[0] = 'C'; note[1] = '5'; break;
      case '4': Freq = 587; note[0] = 'D'; note[1] = '5'; break;
      case '5': Freq = 659; note[0] = 'E'; note[1] = '5'; break;
      case '6': Freq = 698; note[0] = 'F'; note[1] = '5'; break;
      case '7': Freq = 784; note[0] = 'G'; note[1] = '5'; break;
      case '8': Freq = 880; note[0] = 'A'; note[1] = '5'; break;
      default:
      case '0': LPC_DACn->CTRL = 0;
                LPC_DACn->CR = DC_VALUE;
                printf("Freq. = 0 Hz. Reset to start again.\n\n\r");
                while(1);
    } // end of switch
    
    printf("Freq. = %d Hz. note = %s\n\n\r", Freq, note);
    
    // Calculate the new DAC counter reload value, and load it
    LPC_DACn->CNTVAL = (system_ahb_clk)/(SamplesPerCycle * Freq) - 1;

  } // end of while 1
  
}  // end of main
