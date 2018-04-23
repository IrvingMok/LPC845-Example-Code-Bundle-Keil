/*===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================*/


#include <stdio.h>
#include <stdbool.h>
#include "LPC8xx.h"
#include "acomp.h"
#include "capt.h"
#include "utilities.h"
#include "syscon.h"
#include "swm.h"
#include "iocon.h"
#include "chip_setup.h"


void setup_debug_uart(void);
void Setup_Analog_Comparator(void);
void Compute_Notouch_Baseline(void);
void Setup_CTimer_PWM(void);

extern uint32_t to_cnt;
extern uint32_t ovr_cnt;
extern uint32_t x_data[NUM_SENSORS];
extern uint8_t largest;
extern uint32_t touch_threshold;
extern uint32_t slice;
extern uint32_t duty_cycle[NUM_SENSORS];
extern uint32_t last_touch_cnt[NUM_SENSORS];
extern bool dc_flag;

//
// main routine
//
int main(void) {
  uint32_t ctrl_reg_val, poll_tcnt_reg_val;
#if GET_SENSOR != 1
  uint32_t x;
#endif
  	
  // Configure the debug uart (see Serial.c).
  setup_debug_uart();
  printf("\033[2J"); // Clears the screen (Believe it or not!)
  printf("CapTouch_3Sensor_Slider example\n\r");
  
  // Configure and enable the Analog Comparator for use with the CAP Touch module (note: Comparator interrupt remains disabled)
  Setup_Analog_Comparator();
  
  // Setup CTimer for PWM and start it
  Setup_CTimer_PWM();

  // Enable clocks to relevant peripherals
  Enable_Periph_Clock(CLK_CAPT);
  Enable_Periph_Clock(CLK_SWM);
  Enable_Periph_Clock(CLK_IOCON);

  // Give the CAP Touch module a reset
  Do_Periph_Reset(RESET_CAPT);
  
  // Disable pullups and pulldowns on all the CAP Touch pins in IOCON
//LPC_IOCON->CAPTOUCH_X0_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);
//LPC_IOCON->CAPTOUCH_X1_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);
//LPC_IOCON->CAPTOUCH_X2_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);
//LPC_IOCON->CAPTOUCH_X3_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);
//LPC_IOCON->CAPTOUCH_X4_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);
//LPC_IOCON->CAPTOUCH_X5_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);
  LPC_IOCON->CAPTOUCH_X6_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);
  LPC_IOCON->CAPTOUCH_X7_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);
  LPC_IOCON->CAPTOUCH_X8_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);
  LPC_IOCON->CAPTOUCH_YL_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);
  LPC_IOCON->CAPTOUCH_YH_PORT &= (IOCON_MODE_MASK|MODE_INACTIVE);

  // Enable the CAP Touch functions on their pins in the SWM
  // The threshold is very sensitive to how many X pins are enabled in SWM/IOCON even if not activated in XPINSEL
  //EnableFixedPinFunc(CAPT_X0);
  //EnableFixedPinFunc(CAPT_X1);
  //EnableFixedPinFunc(CAPT_X2);
  //EnableFixedPinFunc(CAPT_X3);
  //EnableFixedPinFunc(CAPT_X4);
  //EnableFixedPinFunc(CAPT_X5);
  EnableFixedPinFunc(CAPT_X6);
  EnableFixedPinFunc(CAPT_X7);
  EnableFixedPinFunc(CAPT_X8);
  EnableFixedPinFunc(CAPT_YL);
  EnableFixedPinFunc(CAPT_YH);
  
  // Setup the FCLK for the CAP Touch block
  LPC_SYSCON->CAPTCLKSEL = CAPTCLKSEL_FRO_CLK;
  
  // Specify the divided FCLK freq. 
  SystemCoreClockUpdate();
  ctrl_reg_val = ((fro_clk/FREQ)-1)<<FDIV;
    
  // Choose how selected X pins are controlled when not active (Other X during state 1, charge balance).
  ctrl_reg_val |= XPINUSE_HIGHZ;    // High-Z
  //ctrl_reg_val |= XPINUSE_LOW;    // Driven low
  
  // Select what is being used as the trigger
  //ctrl_reg_val |= TYPE_TRIGGER_YH; // Use the YH pin as input
  ctrl_reg_val |= TYPE_TRIGGER_ACMP; // Use the Analog comarator ACMP_0 as input
  
  // Initialize the control register except for no XPINSEL yet, no POLLMODE yet
  LPC_CAPT->CTRL = ctrl_reg_val;
  
  // Set some count values
  // Time out = 1<<TOUT = 2^TOUT divided FCLKs. 4096 divided FCLKs ~= 1 ms @ FCLK = 4 MHz
  poll_tcnt_reg_val = 12<<TOUT;
  
  // Set the poll delay. Poll counter is a 12 bit counter that counts from 0 to 4095 then wraps. 
  // POLL is the number of wraps to wait between polling rounds.
  //poll_tcnt_reg_val |= ((FREQ)/(POLLS_PER_SEC*4096))<<POLL;
  poll_tcnt_reg_val |= 0<<POLL;
  
  // Choose a value for the TCHLOWER bit.
  // TCHLOWER = 1 means touch triggers at a lower count than no-touch.
  // TCHLOWER = 0 means touch triggers at a higher count than no-touch.
#if TOUCH_TRIGGERS_LOWER == 1
  poll_tcnt_reg_val |= 1U<<TCHLOWER;
#else
  poll_tcnt_reg_val |= 0<<TCHLOWER;
#endif

  // Choose an RDELAY. How many divided FCLKs to hold in step 0 (reset state or draining capacitance)
  // RDELAY has a marked effect on the threshold
  poll_tcnt_reg_val |= 3<<RDELAY;
  
  // Choose an MDELAY. How many divided FCLKs to wait in measurement mode before reading the YH pin or the comparator output.
  poll_tcnt_reg_val |= 1<<MDELAY;
  
  // Initialize the Poll and Measurement Counter register, except for the threshold count which will be calculated next.
  LPC_CAPT->POLL_TCNT = poll_tcnt_reg_val;
  
  // Calculate the no-touch baseline counts and initial threshold. Please don't touch sensors while this runs.
  Compute_Notouch_Baseline();
  
  poll_tcnt_reg_val |= touch_threshold<<TCNT;
  LPC_CAPT->POLL_TCNT = poll_tcnt_reg_val;
 
  // Select any or all available X pins to be used
  //ctrl_reg_val |= (X0_ACTV)<<XPINSEL;
  //ctrl_reg_val |= (X1_ACTV)<<XPINSEL; 
  //ctrl_reg_val |= (X2_ACTV)<<XPINSEL;
  //ctrl_reg_val |= (X3_ACTV)<<XPINSEL;
  //ctrl_reg_val |= (X4_ACTV)<<XPINSEL; 
  //ctrl_reg_val |= (X5_ACTV)<<XPINSEL;
  ctrl_reg_val |= (X6_ACTV)<<XPINSEL;
  ctrl_reg_val |= (X7_ACTV)<<XPINSEL; 
  ctrl_reg_val |= (X8_ACTV)<<XPINSEL;

  // Select the poll mode 
  //ctrl_reg_val |= POLLMODE_NOW;     // One-time-only integration cycle with all selected X pins active simultaneously
  ctrl_reg_val |= POLLMODE_CONTINUOUS;// Cycle through the selected X pins continuously (with timing and delays as specified above)
  
  // Enable some CAP Touch interrupts
  LPC_CAPT->INTENSET = YESTOUCH|NOTOUCH|TIMEOUT|OVERRUN;
  
  // Enable the CAP Touch IRQ in the NVIC
  NVIC_EnableIRQ(CAPT_IRQn);

  // Start the action
  LPC_CAPT->CTRL = ctrl_reg_val;

  printf("Touch the slider\r\n\n");
  printf(" S6    S7    S8   Timeouts    Overruns    TCNT    Slice\n\r");
  printf("----  ----  ----  --------    --------    ----    -----\n\r");

  while(1) {

      for (x=0; x<NUM_SENSORS-1; x++) {
        printf("%4d  ", x_data[x]&0xFFF);
        //printf("%4d  ", duty_cycle[x]&0xFFF);
        //printf("%4d  ", last_touch_cnt[x]&0xFFF);
      }
      printf("%4d  %8d    %8d    %4d    %5d\r", x_data[NUM_SENSORS-1]&0xFFF, to_cnt, ovr_cnt, (LPC_CAPT->POLL_TCNT>>TCNT)&0xFFF, slice);
      //printf("%4d  %8d    %8d    %5d\r", duty_cycle[NUM_SENSORS-1]&0xFFF, to_cnt, ovr_cnt, slice);
      //printf("%4d  %8d    %8d    %5d\r", last_touch_cnt[NUM_SENSORS-1]&0xFFF, to_cnt, ovr_cnt, slice);

  } // end of while 1
} // end of main
