
//===============================================================================
// Name        : ADC_Seqa_Seqb_main.c
// Author      : $(author)
// Version     :
// Copyright   : $(copyright)
// Description : Read the readme
//===============================================================================
//

#include <stdio.h>
#include "LPC8xx.h"
#include "adc.h"
#include "swm.h"
#include "syscon.h"
#include "utilities.h"




void setup_debug_uart(void);

const char promptstring[] = "Choose a sequence to convert:\n\ra for sequence a\n\rb for sequence b\n\r";

volatile uint32_t seqa_buffer[12];
volatile uint32_t seqb_buffer[12];
volatile enum {false, true} seqa_handshake, seqb_handshake;
uint32_t current_seqa_ctrl, current_seqb_ctrl;

#define OUTPORT_B P0_24
#define INPORT_B  P0_28
#define OUTPORT_A P0_25
#define INPORT_A  P0_16

// To use software triggering, define this to '1', otherwise define it to '0' to use hardware triggering.
#define SOFTWARE_TRIGGER 1




/*****************************************************************************
*****************************************************************************/
int main(void) {
  unsigned char temp;
  uint32_t current_clkdiv, val, temp_data, n;

  // Configure the debug uart (see Serial.c)
  setup_debug_uart();

  // Step 1. Power up and reset the ADC, and enable clocks to peripherals
  LPC_SYSCON->PDRUNCFG &= ~(ADC_PD);
  LPC_SYSCON->PRESETCTRL0 &= (ADC_RST_N);
  LPC_SYSCON->PRESETCTRL0 |= ~(ADC_RST_N);
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (ADC | GPIO0 | GPIO_INT | SWM);
  LPC_SYSCON->ADCCLKDIV = 1;                 // Enable clock, and divide-by-1 at this clock divider
  LPC_SYSCON->ADCCLKSEL = ADCCLKSEL_FRO_CLK; // Use fro_clk as source for ADC async clock

  // Step 2. Perform a self-calibration
  // Choose a CLKDIV divider value that yields about 500 KHz.
  SystemCoreClockUpdate();                   // Get the fro_clk frequency
  current_clkdiv = (fro_clk / 500000) - 1;

  // Start the self-calibration
  // Calibration mode = true, low power mode = false, CLKDIV = appropriate for 500,000 Hz
  LPC_ADC->CTRL = ( (1<<ADC_CALMODE) | (0<<ADC_LPWRMODE) | (current_clkdiv<<ADC_CLKDIV) );

  // Poll the calibration mode bit until it is cleared
  while (LPC_ADC->CTRL & (1<<ADC_CALMODE));

  // Step 3. Configure the external pins we will use
  // Disable any fixed-pin functions on pins we are using as digital outs and ins (P0.11 and P0.10 I2C functions)
  //LPC_SWM->PINENABLE0 |= (I2C0_SDA | I2C0_SCL); // P0.11 and P0.10 disable fixed pin functions if using them
  
  // ADC1 - ADC6, ADC7, ADC9 - ADC11 pins to be ADC analog functions
  // We won't use ADC0 or ADC8 (no affirmative action in this institution)
  LPC_SWM->PINENABLE0 &= ~(ADC_1|ADC_2|ADC_3|ADC_4|ADC_5|ADC_6|ADC_7|ADC_9|ADC_10|ADC_11); // Fixed pin analog functions enabled
  LPC_SWM->PINENABLE0 |= (ADC_0|ADC_8);                                                    // Movable digital functions enabled

  // Make the ouput ports GPIO outputs driving '1', connect to the input ports externally.
  // Make the input ports GPIO inputs.
  LPC_GPIO_PORT->SET0  = (1<<OUTPORT_B)|(1<<OUTPORT_A);  // Output GPIOs initially driving '1'
  LPC_GPIO_PORT->DIR0 |= (1<<OUTPORT_B)|(1<<OUTPORT_A);  // to output mode
  LPC_GPIO_PORT->DIR0 &= ~((1<<INPORT_B)|(1<<INPORT_A)); // Input GPIOs to input mode

  // Assign the ADC pin trigger functions to the input port pins
  // For LPC82x and LPC83x : ADC pin triggers are movable digital functions assignable through the SWM
  // For LPC84x (thank you, side Birns) :  ADC and DAC pin triggers driven by GPIO_INT
  // Configure pinint0_irq and pinint1_irq for high level sensitive, then assign them in seq_ctrl[14:12]
  // Important Note! We must configure the pin interrupts to generate the ADC pin triggers, but we don't use the pin interrupts as interrupts.
  
  // Assign the input ports to be pin interrupts 1, 0 by writing to the PINTSELs in SYSCON
  LPC_SYSCON->PINTSEL[1] = INPORT_B;
  LPC_SYSCON->PINTSEL[0] = INPORT_A;

  // Configure the pin interrupt mode register (a.k.a ISEL) for level-sensitive on PINTSEL1,0 ('1' = level-sensitive)
  LPC_PIN_INT->ISEL |= 0x3; // level-sensitive
  
  // Configure the active level for PINTSEL1,0 to active-high ('1' = active-high)
  LPC_PIN_INT->IENF |= 0x3;  // active high
  
  // Enable interrupt generation for PINTSEL1,0
  LPC_PIN_INT->SIENR = 0x3;
  
  // Step 4. Configure the ADC for the appropriate analog supply voltage using the TRM register
  // For a sampling rate higher than 1 Msamples/s, VDDA must be higher than 2.7 V (on the Max board it is 3.3 V)
  LPC_ADC->TRM &= ~(1<<ADC_VRANGE); // '0' for high voltage

  // Step 4. Choose a CLKDIV divider value
  // A fully accurate conversion requires 25 ADC clocks.
  // A 30 MHz system clock with CLKDIV = 0 results in 1.2 MSPS.
  // We can use the SystemCoreClock variable to calculate for a desired sample rate in Hz.
  #define desired_sample_rate 1200000
  current_clkdiv = (fro_clk / (25 * desired_sample_rate)) - 1;

  // Calibration mode = false, low power mode = false, CLKDIV = appropriate for desired sample rate.
  LPC_ADC->CTRL = ( (0<<ADC_CALMODE) | (0<<ADC_LPWRMODE) | (current_clkdiv<<ADC_CLKDIV) );

  // Step 5. Assign some ADC channels to each sequence
  // Let sequence A = channels 1, 2, 3, 4, 5, 6
  // Let sequence B = channels 7, 9, 10, 11
  current_seqa_ctrl = ((1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6))<<ADC_CHANNELS;
  current_seqb_ctrl = ((1<<7)|(1<<9)|(1<<10)|(1<<11))<<ADC_CHANNELS;

  // Step 6. Select a trigger source for each of the sequences
  // Let sequence A trigger = PININT0_IRQ. Connected to an external pin using PINTSELs above.
  // Let sequence B trigger = PININT1_IRQ. Connected to an external pin using PINTSELs above.
#if SOFTWARE_TRIGGER
  current_seqa_ctrl |= NO_TRIGGER<<ADC_TRIGGER;    // Use for software-only triggering
  current_seqb_ctrl |= NO_TRIGGER<<ADC_TRIGGER;    // Use for software-only triggering
#else
  current_seqa_ctrl |= PININT0_IRQ<<ADC_TRIGGER;   // Use for hardware triggering
  current_seqb_ctrl |= PININT1_IRQ<<ADC_TRIGGER;   // Use for hardware triggering
#endif

  // Step 7. Select positive (1) or negative (0) edge for the hardware trigger
  // Let sequence A be negative edge triggered
  // Let sequence B be negative edge triggered
  current_seqa_ctrl |= 0<<ADC_TRIGPOL;
  current_seqb_ctrl |= 0<<ADC_TRIGPOL;

  // Step 8. Choose (1) to bypass, or (0) not to bypass the hardware trigger synchronization
  // Since our trigger sources are on-chip, system clock based, we may bypass.
  current_seqa_ctrl |= 1<<ADC_SYNCBYPASS;
  current_seqb_ctrl |= 1<<ADC_SYNCBYPASS;

  // Step 9. Choose burst mode (1) or no burst mode (0) for each of the sequences
  // Let sequences A and B use no burst mode
  current_seqa_ctrl |= 0<<ADC_BURST;
  current_seqb_ctrl |= 0<<ADC_BURST;

  // Step 10. Choose single step (1), or not (0), for each sequence.
  // Let sequences A and B use no single step
  current_seqa_ctrl |= 0<<ADC_SINGLESTEP;
  current_seqb_ctrl |= 0<<ADC_SINGLESTEP;

  // Step 11. Choose A/B sequence priority
  // Use default
  current_seqa_ctrl |= 0<<ADC_LOWPRIO;

  // Step 12. Choose end-of-sequence mode (1), or end-of-conversion mode (0), for each sequence
  // Let sequences A and B use end-of-sequence mode
  current_seqa_ctrl |= 1<<ADC_MODE;
  current_seqb_ctrl |= 1<<ADC_MODE;

  // Step 13. Set the low and high thresholds for both sets of threshold compare registers
  // Note: This step is included for completeness, threshold crossings are not currently used in this example.
  // Let both low thresholds be (1/3)*0xfff
  // Let both high thresholds be (2/3)*0xfff
  #define low_thresh (0xFFF / 3)
  #define high_thresh ((2 * 0xFFF) / 3)
  LPC_ADC->THR0_LOW = low_thresh;
  LPC_ADC->THR1_LOW = low_thresh;
  LPC_ADC->THR0_HIGH = high_thresh;
  LPC_ADC->THR1_HIGH = high_thresh;

  // Step 14. For each channel, choose which pair of threshold registers (0 or 1) conversion results should be compared against.
  // Note: This step is included for completeness, threshold crossings are not currently used in this example.
  // For sequence A (channels 1 - 6), compare against threshold register set 0
  // For sequence B (channels 7, 9, 10, 11), compare against threshold register set 1
  LPC_ADC->CHAN_THRSEL = (0<<1)|(0<<2)|(0<<3)|(0<<4)|(0<<5)|(0<<6)|(1<<7)|(1<<9)|(1<<10)|(1<<11);

  // Step 15. Choose which interrupt conditions will contribute to the ADC interrupt/DMA triggers
  // Only the sequence A and sequence B interrupts are enabled for this example
  // SEQA_INTEN = true
  // SEQB_INTEN = true
  // OVR_INTEN = false
  // ADCMPINTEN0 to ADCMPEN11 = false
  LPC_ADC->INTEN = (1<<SEQA_INTEN)  |
                   (1<<SEQB_INTEN)  |
                   (0<<OVR_INTEN)   |
                   (0<<ADCMPINTEN0) |
                   (0<<ADCMPINTEN1) |
                   (0<<ADCMPINTEN2) |
                   (0<<ADCMPINTEN3) |
                   (0<<ADCMPINTEN4) |
                   (0<<ADCMPINTEN5) |
                   (0<<ADCMPINTEN6) |
                   (0<<ADCMPINTEN7) |
                   (0<<ADCMPINTEN8) |
                   (0<<ADCMPINTEN9) |
                   (0<<ADCMPINTEN10)|
                   (0<<ADCMPINTEN11);


  // Write the sequence control word with enable bit set for both sequences
  current_seqa_ctrl |= 1U<<ADC_SEQ_ENA;
  LPC_ADC->SEQA_CTRL = current_seqa_ctrl;

  current_seqb_ctrl |= 1U<<ADC_SEQ_ENA;
  LPC_ADC->SEQB_CTRL = current_seqb_ctrl;

  // Enable ADC interrupts in the NVIC
  NVIC_EnableIRQ(ADC_SEQA_IRQn);
  NVIC_EnableIRQ(ADC_SEQB_IRQn);


  // The main loop
  while(1) {

    // Prompt user to select a sequence to convert (see utilities_lib)
    temp = GetConsoleCharacter((const char *)&promptstring);

    // Accept 'a' only, anything else is sequence B
    if (temp == 'a') {
      seqa_handshake = false;

#if SOFTWARE_TRIGGER
      // Launch sequence A with software
      current_seqa_ctrl |= 1<<ADC_START;
      LPC_ADC->SEQA_CTRL = current_seqa_ctrl;
#else
      // Launch sequence A with hardware
      LPC_GPIO_PORT->CLR0 = 1<<OUTPORT_A;      // Clear port pin to '0', creates a falling edge on PININT0_IRQ 
#endif
      // Wait for return from ISR
      while (!seqa_handshake);

      // Get the data from SRAM and print it to the terminal
      for (n = 0; n < 12; n++) {
        val = current_seqa_ctrl;
        val = (val>>n) & 0x1;    // Shift the current channel of interest into bit 0, see if it's set
        if (val) {               // If set, get this channel's data from the SRAM array
          temp_data = (seqa_buffer[n]>>4) & 0x00000FFF;
          printf("\rADC channel %d data was %d\n\r", n, temp_data);
        }
      }
    }
    else {
      seqb_handshake = false;

#if SOFTWARE_TRIGGER
      // Launch sequence B with software
      current_seqb_ctrl |= 1<<ADC_START;
      LPC_ADC->SEQB_CTRL = current_seqb_ctrl;
#else
      // Launch sequence B with hardware
      LPC_GPIO_PORT->CLR0 = 1<<OUTPORT_B;      // Clear port pin to '0', creates a falling edge on PININT1_IRQ
#endif
      // Wait for return from ISR
      while (!seqb_handshake);

      // Get the data from SRAM and print it to the terminal
      for (n = 0; n < 12; n++) {
        val = current_seqb_ctrl;
        val = (val>>n) & 0x1;    // Shift the current channel of interest into bit 0, see if it's set
        if (val) {               // If set, get this channel's data from the SRAM array
          temp_data = (seqb_buffer[n]>>4) & 0x00000FFF;
          printf("\rADC channel %d data was %d\n\r", n, temp_data);
        }
      }
    }


    // Put the external pins back to '1'
    LPC_GPIO_PORT->SET0 = (1<<OUTPORT_B)|(1<<OUTPORT_A);      // Set port pins to '1'


  } // end of while 1

} // end of main

