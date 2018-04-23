#include "LPC8xx.h"
#include "chip_setup.h"

extern uint32_t filter[NUM_SENSORS][NUM_SAMPLES];
extern uint8_t current_x;
extern uint32_t decim_counter[NUM_SENSORS];
extern uint32_t old_data[NUM_SENSORS][NUM_SAMPLES+1];
//
// L-point FIR moving-average filter ... y[n] = (1/L)*(SUM k=0 to L-1) x[n-k]
//
uint32_t FIR_average(uint32_t new_data) {
  static uint8_t index = 0;
  uint32_t temp = 0;
  uint8_t n;
  
  filter[current_x][index++] = new_data; // Replace oldest data with the new, and update index for next time
  if (index == NUM_SAMPLES)
    index = 0;
  for (n=0; n<NUM_SAMPLES; n++) {        // Add 'em up
    temp += filter[current_x][n];
  }
  return(temp>>FILTER_GAIN);             // Divide by L = NUM_SAMPLES
}



//
// Recursive implementation of M-point moving average filter y[n] = y[n-1] + (x[n-1] – x[n-M-1])*(1/M)
//
void IIR_average(uint32_t new_data) { // Bad name!
  uint8_t b;
  // Note! Break it into two pieces to avoid negative numbers
  filter[current_x][0] = filter[current_x][1] + (new_data)/NUM_SAMPLES;
  filter[current_x][0] -= (old_data[current_x][NUM_SAMPLES])/NUM_SAMPLES;
  filter[current_x][1] = filter[current_x][0];
  for (b=NUM_SAMPLES; b!=0; b--) {
    old_data[current_x][b] = old_data[current_x][b-1];
  }
  old_data[current_x][0] = new_data;
  
}



//
// Digital Butterworth low-pass filter ... y[n] = x[n] + x[n-1] + Ay[n-1] 
// A = 0.8125 = 13/16 = 1 - 1/4 + 1/16
// A = 0.8750 = 7/8 = 1 - 1/8
// A = 0.9375 = 15/16 = 1 - 1/16
// A = 0.9688 = 31/32 = 1 - 1/32
uint32_t Butterworth_lpf(uint32_t new_data) {
  static uint32_t x[NUM_SENSORS][2] = {0};
  uint32_t Ay1;

  x[current_x][0] = new_data;
  Ay1 = filter[current_x][1] - (filter[current_x][1]>>2) + (filter[current_x][1]>>4); // Compute Ay[n-1] (for A = 0.8125)
//Ay1 = filter[current_x][1] - (filter[current_x][1]>>3);                             // Compute Ay[n-1] (for A = 0.8750)
  filter[current_x][0] = x[current_x][0] + x[current_x][1] + Ay1;                     // Compute y[n]
  filter[current_x][1] = filter[current_x][0];                                        // y[n] becomes y[n-1] for next 
  x[current_x][1] = x[current_x][0];                                                  // x[n] becomes x[n-1] for next
  return(filter[current_x][0]>>FILTER_GAIN);                                          // return y[n] / 2^gain
}



//
// Decimating, or slew-rate limiting, filter
//
uint32_t Decimator(uint32_t new_data) {
  if (filter[current_x][0] == 0) {       // Intialize
    filter[current_x][0] = new_data;
  }
  else {
    if (new_data > filter[current_x][0])
      filter[current_x][0]++;
    else
      filter[current_x][0]--;
  }
  decim_counter[current_x]++;
  return (filter[current_x][0]);
}
