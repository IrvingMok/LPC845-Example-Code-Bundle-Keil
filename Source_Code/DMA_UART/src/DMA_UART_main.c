/*
===============================================================================
 Name        : DMA_UART_main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : read the readme
===============================================================================
*/


#include "lpc_types.h"
#include "LPC8xx.h"
#include "dma.h"
#include "syscon.h"
#include "utilities.h"


void setup_debug_uart(void);


const uint8_t prompt_string[] = "\n\rType a string followed by [Enter], for replay by the Channel Descriptor (A), which links to Reload Descriptor B\n\r";
const char seqb_string[] = " was the string that you typed (this is Descriptor B talking)\n\r";
char seqa_string[32];
volatile bool seqa_handshake, seqb_handshake;



// Instantiate the channel descriptor table, which must be 512-byte aligned (see dma.h)
ALIGN(512) DMA_CHDESC_T Chan_Desc_Table[NUM_DMA_CHANNELS];

// Instantiate one reload descriptor. All descriptors must be 16-byte aligned (see dma.h)
ALIGN(16) DMA_RELOADDESC_T Reload_Descriptor_B;




// ============================================================================
// DMA Interrupt Service Routine
// Clears the A or B interrupt flag.
// Then sets either the seqA or seqB handshake flag and returns.
// ============================================================================
void DMA_IRQHandler(void) {
  uint32_t temp = LPC_DMA->INTA0; // Get the interrupt A flags
  if (temp &= 1<<1) {             // If CH1 interrupt flag is set
    LPC_DMA->INTA0 = temp;        // Clear it
    seqa_handshake = true;        // Set the handshake flag for main
  }
  temp = LPC_DMA->INTB0;          // Get the interrupt B flags
  if (temp &= 1<<1) {             // If CH1 interrupt flag is set
    LPC_DMA->INTB0 = temp;        // Clear it
    seqb_handshake = true;        // Set the handshake flag for main
  }
  return;                         // Return from interrupt
}




// ============================================================================
// main routine
// ============================================================================
int main(void) {
  uint32_t xfercfg_a, xfercount_a, seqa_string_length;
  uint32_t xfercfg_b, xfercount_b, seqb_string_length;
  uint32_t ch1_cfg_val;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();

  // Get the user's first string to initialize the seqA buffer and length variable (see utilities_lib)
  PutTerminalString(LPC_USART0, (uint8_t *)prompt_string);
  seqa_string_length = GetTerminalString(LPC_USART0, seqa_string);

  // Initialize the seqB length variable
  seqb_string_length = sizeof(seqb_string);

  // Reset the DMA, and enable clocks to peripherals (see lpc8xx_syscon.h)
  LPC_SYSCON->PRESETCTRL0 &= (DMA_RST_N);
  LPC_SYSCON->PRESETCTRL0 |= ~(DMA_RST_N);
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (DMA | UART0);

  // Setup the channel's CFG register
  //
  // CFG1
  // PERIPREQEN   = 1  The peripheral DMA request is enabled
  // HWTRIGEN     = 0  Hardware triggering is disabled
  // TRIGPOL      = 0  Active low or falling edge hardware trigger (not used)
  // TRIGTYPE     = 0  Edge sensitive hardware trigger (not used)
  // TRIGBURST    = 0  Single transfer will be launched by each trigger
  // BURSTPOWER   = 0  Unused
  // SRCBURSTWRAP = 0  Unused
  // DSTBURSTWRAP = 0  Unused
  // CHPRIORITY   = 0  Highest priority
  ch1_cfg_val = 1<<DMA_CFG_PERIPHREQEN  |
                0<<DMA_CFG_HWTRIGEN     |
                0<<DMA_CFG_TRIGPOL      |
                0<<DMA_CFG_TRIGTYPE     |
                0<<DMA_CFG_TRIGBURST    |
                0<<DMA_CFG_BURSTPOWER   |
                0<<DMA_CFG_SRCBURSTWRAP |
                0<<DMA_CFG_DSTBURSTWRAP |
                0<<DMA_CFG_CHPRIORITY;
  LPC_DMA->CHANNEL[1].CFG = ch1_cfg_val;

  // Build the initial channel XFERCFG for the A sequence
  //
  // XFERCFG1
  // CFGVALID  = 1  Current channel descriptor is considered valid
  // RELOAD    = 1  Reload the control structure (the contents of this register) when the descriptor is exhausted
  // SWTRIG    = 0  Don't trigger now
  // CLRTRIG   = 0  The trigger is not cleared when this descriptor is exhausted
  // SETINTA   = 1  Set the INTA flag when this descriptor is exhausted
  // SETINTB   = 0  Don't use the INTB flag
  // WIDTH     = 0  Eight-bit transfers
  // SRCINC    = 1  Source address is incremented by 1 x WIDTH (source is buffer in memory)
  // DSTINC    = 0  Destination address is not incremented (destination is TXDAT register of UART)
  // XFERCOUNT =    Length of string (less the CR, less the LF, less the NUL terminator) minus 1
  xfercount_a = (seqa_string_length-3-1);
  xfercfg_a = 1<<DMA_XFERCFG_CFGVALID |
              1<<DMA_XFERCFG_RELOAD   |
              0<<DMA_XFERCFG_SWTRIG   |
              0<<DMA_XFERCFG_CLRTRIG  |
              1<<DMA_XFERCFG_SETINTA  |
              0<<DMA_XFERCFG_SETINTB  |
              0<<DMA_XFERCFG_WIDTH    |
              1<<DMA_XFERCFG_SRCINC   |
              0<<DMA_XFERCFG_DSTINC   |
              xfercount_a<<DMA_XFERCFG_XFERCOUNT;

  // Populate the initial Channel Descriptor for the A sequence
  LPC_DMA->CHANNEL[1].XFERCFG = xfercfg_a;                                            // Initial XFERCFG
  Chan_Desc_Table[1].source   = (uint32_t)((&seqa_string[0]) + xfercount_a);          // Initial source data end address (start address + (xfercount*srcinc))
  Chan_Desc_Table[1].dest     = (uint32_t)(&LPC_USART0->TXDAT);                       // Initial destination data end address
  Chan_Desc_Table[1].next     = (uint32_t)(&Reload_Descriptor_B);                     // Initial next points to Reload Descriptor B

  // Build the XFERCFG for the B sequence
  //
  // CFGVALID  = 1  Current channel descriptor is considered valid
  // RELOAD    = 0  Don't reload the control structure when the descriptor is exhausted
  // SWTRIG    = 0  Don't trigger now
  // CLRTRIG   = 1  The trigger is cleared when this descriptor is exhausted (and has passed out on the toilet)
  // SETINTA   = 0  Don't use the INTA flag
  // SETINTB   = 1  Set the INTB flag when this descriptor is exhausted
  // WIDTH     = 0  Eight-bit transfers
  // SRCINC    = 1  Source address is incremented by 1 x WIDTH (source is buffer in memory)
  // DSTINC    = 0  Destination address is not incremented (destination is TXDAT register of UART)
  // XFERCOUNT =    Length of string (less the NUL terminator) minus 1
  xfercount_b = (seqb_string_length-1-1);
  xfercfg_b = 1<<DMA_XFERCFG_CFGVALID |
              0<<DMA_XFERCFG_RELOAD   |
              0<<DMA_XFERCFG_SWTRIG   |
              1<<DMA_XFERCFG_CLRTRIG  |
              0<<DMA_XFERCFG_SETINTA  |
              1<<DMA_XFERCFG_SETINTB  |
              0<<DMA_XFERCFG_WIDTH    |
              1<<DMA_XFERCFG_SRCINC   |
              0<<DMA_XFERCFG_DSTINC   |
              xfercount_b<<DMA_XFERCFG_XFERCOUNT;

  // Populate the Reload Descriptor for the B sequence
  Reload_Descriptor_B.xfercfg = xfercfg_b;                                            // Buffer B transfer configuration
  Reload_Descriptor_B.source  = (uint32_t)((&seqb_string[0]) + xfercount_b);          // Buffer B source data end address + (xfercount*srcinc)
  Reload_Descriptor_B.dest    = (uint32_t)(&LPC_USART0->TXDAT);                       // Buffer B destination data end address
  Reload_Descriptor_B.next    = (uint32_t)(&Reload_Descriptor_B);                     // Buffer B next points to Reload descriptor A

  // Point the SRAMBASE register to the beginning of the channel descriptor SRAM table
  LPC_DMA->SRAMBASE = (uint32_t)(&Chan_Desc_Table);

  // Set the valid bit for channel 1 in the SETVALID register
  LPC_DMA->SETVALID0 = 1<<1;

  // Enable the channel 1 interrupt in the INTEN register
  LPC_DMA->INTENSET0 =  1<<1;

  // Enable DMA channel 1 in the ENABLE register
  LPC_DMA->ENABLESET0 = 1<<1;

  // Set the master DMA controller enable bit in the CTRL register
  LPC_DMA->CTRL = 1;

  // Enable the DMA interrupt in the NVIC
  NVIC_EnableIRQ(DMA_IRQn);


  while(1) {

    // Clear the sequence A and sequence B handshake flags
    seqa_handshake = false;
    seqb_handshake = false;

    // Cause a trigger on channel 1
    LPC_DMA->SETTRIG0 = 1<<1;

    // Wait for the A and B interrupts before proceeding (CPU can be paused, waiting for interrupt, if desired)
    __WFI();
    while (!seqa_handshake);
    while (!seqb_handshake);

    // Get the user's new string into the seqA buffer, and new string length variable (see utilities_lib)
    PutTerminalString(LPC_USART0, (uint8_t *)prompt_string);
    seqa_string_length = GetTerminalString(LPC_USART0, seqa_string);

    // Update the xfercount_a and xfercfg_a variables based on the new string length
    xfercount_a = (seqa_string_length-3-1);          // Recalculate xfercount_a based on new length
    xfercfg_a &= ~(0x3FF<<DMA_XFERCFG_XFERCOUNT);    // Clear out the old XFERCOUNT field from xfercfg_a
    xfercfg_a |= xfercount_a<<DMA_XFERCFG_XFERCOUNT; // Load new XFERCOUNT into xfercfg_a

    // Update the sequence A transfer configuration, and descriptor's source data end address, then loop again
    LPC_DMA->CHANNEL[1].XFERCFG = xfercfg_a;
    Chan_Desc_Table[1].source = (uint32_t)((&seqa_string[0]) + xfercount_a);

  } // end of while 1

} // end of main

