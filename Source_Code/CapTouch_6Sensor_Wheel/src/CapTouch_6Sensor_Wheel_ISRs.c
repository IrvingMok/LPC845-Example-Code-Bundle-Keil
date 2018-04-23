///////////////////////////////////////////////////////////////////////////////
// ISRs.c
///////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include "LPC8xx.h"
#include "acomp.h"
#include "capt.h"
#include "utilities.h"
#include "syscon.h"
#include "swm.h"
#include "iocon.h"
#include "chip_setup.h"


uint32_t temp_status;
uint32_t ovr_cnt;
uint32_t to_cnt;
uint32_t debounce_cnt = 0;
bool debounce_done = false;
uint32_t x_data[NUM_SENSORS];
uint32_t last_touch_cnt[NUM_SENSORS];
uint8_t largest;
bool touching;
bool dc_flag;
uint8_t latest_largests[CHAIN_LENGTH];
int32_t slice;
uint32_t duty_cycle[NUM_SENSORS]; 
uint32_t raw_data;
uint32_t filter[NUM_SENSORS][NUM_SAMPLES];
uint8_t current_x;
uint32_t decim_counter[NUM_SENSORS] = {0};
extern uint32_t touch_threshold;


// Filtering function declarations
uint32_t FIR_average(uint32_t new_data);
void IIR_average(uint32_t new_data);
uint32_t Butterworth_lpf(uint32_t new_data);
uint32_t Decimator(uint32_t new_data);

// Other function declarations
uint8_t find_larger(uint32_t a, uint32_t b);
bool is_equal(uint8_t a, uint8_t b);
uint8_t find_smaller(uint32_t a, uint32_t b);
void Compute_Notouch_Baseline(void);
void Setup_Analog_Comparator(void);
void Clear_Duty_Cycles(void);
void Update_SCT_PWM_Regs(void);
void Setup_SCT_PWM(void);



//
// CAP Touch interrupt service routine
//
void CAPT_IRQHandler(void) {
  uint8_t n;
  bool false_notouch;
  static uint8_t index = 0;
  
  temp_status = LPC_CAPT->STATUS;                        // Read the status flags from the STATUS register
  raw_data = LPC_CAPT->TOUCH;                            // Read the data from the TOUCH register.
  
  // Find the sensor Xn which caused this interrupt.
  for (n=0; n!=NUM_SENSORS; n++) {
    if (((raw_data & TOUCH_XVAL)>>12) == (uint32_t)n) {  
      current_x = n;
      x_data[n] = raw_data;
      break;
    }
  }
  
  // On every YESTOUCH or NOTOUCH interrupt 
  if (temp_status & (YESTOUCH|NOTOUCH)) {

    // Define 'false_notouch' as a NOTOUCH interrupt for any given sensor, while ANY OTHER 
    // sensor is inside of the threshold.
    // This can happen if the physically touched sensor's count actually goes outside of  
    // threshold and triggers a NOTOUCH interrupt instead of a TOUCH.
    false_notouch = 0;
    if (temp_status & (NOTOUCH)) {
      for (n=0; n<NUM_SENSORS; n++) {
        if ((x_data[n]&0xFFF) < touch_threshold) {
          false_notouch = 1;
          break;
        }
      }
    }

    if ((temp_status & (YESTOUCH)) | false_notouch) {

#if DEBOUNCE == 1    
      if (!debounce_done) {
        if (++debounce_cnt < DB_VAL) {              // This executes when any sensor is below threshold
          LPC_CAPT->STATUS = YESTOUCH;              // Clear the interrupt flag by writing '1' to it
          return;
        }
        debounce_cnt = 0;
        debounce_done = true;                       // Only clear this when all sensors are above threshold
      }
#endif

      // Apply your filter of choice to the new sample from this sensor Xn
#if FIR == 1
      // Call FIR L-point average filter
      last_touch_cnt[current_x] = FIR_average(raw_data & 0xFFF);
#elif IIR == 1
      // Call IIR L-point average filter
      IIR_average(raw_data & 0xFFF);
      last_touch_cnt[current_x] = filter[current_x][0];
#elif BUTT == 1
      // Call digital Butterworth low-pass filter 
      last_touch_cnt[current_x] = Butterworth_lpf(raw_data & 0xFFF);
#elif DECIM == 1
      // Call the decimating filter (envelope follower)
      n = Decimator(raw_data & 0xFFF);
      if (decim_counter[current_x] == DECIMATE_FACTOR) {
        decim_counter[current_x] = 0;
        last_touch_cnt[current_x] = n;
      }
#else
      // Update the last_touch_cnt variable for this sensor with unfiltered data
      last_touch_cnt[current_x] = raw_data & 0xFFF;
#endif

#if DECIM == 1
  if (decim_counter[current_x] == 0) {
#endif

      // find the sensor with the largest count of the last_touch_cnts
      largest = 0;
      for (n=0; n!=NUM_SENSORS-1; n++) {
        if (find_larger(last_touch_cnt[n+1], last_touch_cnt[largest]))
          largest = n+1;
      }
 
      // Replace the oldest of the latest_largests 
      latest_largests[index++] = largest;
      if (index == CHAIN_LENGTH)
        index = 0;
      
      // If all elements of latest_largests[] match, report this as DC, else return from interrupt
      for (n=0; n!=CHAIN_LENGTH-1; n++) {
        if (is_equal(latest_largests[n], latest_largests[n+1])) {
          dc_flag = 1;
        }
        else {
          dc_flag = 0;
          break;
        }
      }
    
      if (dc_flag) {
        // Consider only the sensor indicated by largest, and the sensors to each side
        switch (largest) {
          case 0:
            if (last_touch_cnt[5] >= last_touch_cnt[1]) {
              slice = (last_touch_cnt[0] - last_touch_cnt[5]);
              duty_cycle[0] = LED_GF*(slice - 0);
            }
            else {
              slice = 30 - (last_touch_cnt[0] - last_touch_cnt[1]);
              duty_cycle[0] = LED_GF*(30 - slice);
            }
            break;
            
          case 1:
            if (last_touch_cnt[0] >= last_touch_cnt[2]) {
              slice = (last_touch_cnt[1] - last_touch_cnt[0]) + 30;
              duty_cycle[1] = LED_GF*(slice - 30);
            }
            else {
              slice = 60 - (last_touch_cnt[1] - last_touch_cnt[2]);
              duty_cycle[1] = LED_GF*(60 - slice);
            }
            break;
            
          case 2:
            if (last_touch_cnt[1] >= last_touch_cnt[3]) {
              slice = (last_touch_cnt[2] - last_touch_cnt[1]) + 60;
              duty_cycle[2] = LED_GF*(slice - 60);
            }
            else {
              slice = 90 - (last_touch_cnt[2] - last_touch_cnt[3]);
              duty_cycle[2] = LED_GF*(90 - slice);
            }
            break;
            
          case 3:
            if (last_touch_cnt[2] >= last_touch_cnt[4]) {
              slice = (last_touch_cnt[3] - last_touch_cnt[2]) + 90;
              duty_cycle[3] = LED_GF*(slice - 90);
            }
            else {
              slice = 120 - (last_touch_cnt[3] - last_touch_cnt[4]);
              duty_cycle[3] = LED_GF*(120 - slice);
            }
            break;
            
          case 4:
            if (last_touch_cnt[3] >= last_touch_cnt[5]) {
              slice = (last_touch_cnt[4] - last_touch_cnt[3]) + 120;
              duty_cycle[4] = LED_GF*(slice - 120);
            }
            else {
              slice = 150 - (last_touch_cnt[4] - last_touch_cnt[5]);
              duty_cycle[4] = LED_GF*(150 - slice);
            }
            break;
            
          case 5:
            if (last_touch_cnt[4] >= last_touch_cnt[0]) {
              slice = (last_touch_cnt[5] - last_touch_cnt[4]) + 150;
              duty_cycle[5] = LED_GF*(slice - 150);
            }
            else {
              slice = 180 - (last_touch_cnt[5] - last_touch_cnt[0]);
              duty_cycle[5] = LED_GF*(180 - slice);
            }
            break;
        } // end switch     
      }
      else {
        Clear_Duty_Cycles();
      } // end if (dc_flag)

      Update_SCT_PWM_Regs();

#if DECIM == 1
  }
#endif

      LPC_CAPT->STATUS = YESTOUCH|NOTOUCH;      // Clear the interrupt flag by writing '1' to it
      return;                                   // Return from interrupt  
    } // end if (temp_status & YESTOUCH | false_notouch)
    else {
      // This is a NOTOUCH interrupt for this sensor, and ALL other sensors are outside of threshold, revert to NOTOUCH idle
#if DEBOUNCE == 1
      debounce_done = false;
      debounce_cnt = 0;
#endif
      for (n=0; n!=CHAIN_LENGTH; n++) {         // Invalidate the delay chain
        latest_largests[n] = n;
      }
      dc_flag = 0;
      Clear_Duty_Cycles();
      Update_SCT_PWM_Regs();     
      LPC_CAPT->STATUS = NOTOUCH;               // Clear the interrupt flag by writing '1' to it
      return;                                   // Return from interrupt  
    }// end if((temp_status & YESTOUCH) | false_notouch)
  }// end if ((temp_status & (YESTOUCH|NOTOUCH))

  // Other interrupts, increment their counters, clear flags, and return
  if (temp_status & (TIMEOUT)) {
    to_cnt++;
    LPC_CAPT->STATUS = TIMEOUT;
    return;
  }
  if (temp_status & (OVERRUN)) {
    ovr_cnt++;
    LPC_CAPT->STATUS = OVERRUN;
    return;
  }
  
} // end of ISR

