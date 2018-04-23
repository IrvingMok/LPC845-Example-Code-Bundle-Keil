/*
 * lpc8xx_swm.h
 *
 *  Created on: Feb 16, 2016
 *
 */

#ifndef LPC8XX_SWM_H_
#define LPC8XX_SWM_H_

#include "LPC8xx.h"

// Port pin number equates
#define P0_0      0
#define P0_1      1
#define P0_2      2
#define P0_3      3
#define P0_4      4
#define P0_5      5
#define P0_6      6
#define P0_7      7
#define P0_8      8
#define P0_9      9
#define P0_10    10
#define P0_11    11
#define P0_12    12
#define P0_13    13
#define P0_14    14
#define P0_15    15
#define P0_16    16
#define P0_17    17
#define P0_18    18
#define P0_19    19
#define P0_20    20
#define P0_21    21
#define P0_22    22
#define P0_23    23
#define P0_24    24
#define P0_25    25
#define P0_26    26
#define P0_27    27
#define P0_28    28
#define P0_29    29
#define P0_30    30
#define P0_31    31
#define P1_0     32
#define P1_1     33
#define P1_2     34
#define P1_3     35
#define P1_4     36
#define P1_5     37
#define P1_6     38
#define P1_7     39
#define P1_8     40
#define P1_9     41
#define P1_10    42
#define P1_11    43
#define P1_12    44
#define P1_13    45
#define P1_14    46
#define P1_15    47
#define P1_16    48
#define P1_17    49
#define P1_18    50
#define P1_19    51
#define P1_20    52
#define P1_21    53
#define P1_22    54
#define P1_23    55
#define P1_24    56
#define P1_25    57
#define P1_26    58
#define P1_27    59
#define P1_28    60
#define P1_29    61
#define P1_30    62
#define P1_31    63



// Function name equates (for normal people)
#define U0_TXD        0
#define U0_RXD        1
#define U0_RTS        2
#define U0_CTS        3
#define U0_SCLK       4

#define U1_TXD        5
#define U1_RXD        6
#define U1_RTS        7
#define U1_CTS        8
#define U1_SCLK       9

#define U2_TXD        10
#define U2_RXD        11
#define U2_RTS        12
#define U2_CTS        13
#define U2_SCLK       14

#define SPI0_SCK      15
#define SPI0_MOSI     16
#define SPI0_MISO     17
#define SPI0_SSEL0    18
#define SPI0_SSEL1    19
#define SPI0_SSEL2    20
#define SPI0_SSEL3    21

#define SPI1_SCK      22
#define SPI1_MOSI     23
#define SPI1_MISO     24
#define SPI1_SSEL0    25
#define SPI1_SSEL1    26

#define SCT_PIN0      27
#define SCT_PIN1      28
#define SCT_PIN2      29
#define SCT_PIN3      30
#define SCT_OUT0      31
#define SCT_OUT1      32
#define SCT_OUT2      33
#define SCT_OUT3      34
#define SCT_OUT4      35
#define SCT_OUT5      36
#define SCT_OUT6      37

#define I2C1_SDA      38
#define I2C1_SCL      39
#define I2C2_SDA      40
#define I2C2_SCL      41
#define I2C3_SDA      42
#define I2C3_SCL      43

#define COMP0_OUT     44
#define ACOMP         44
#define CLKOUT        45
#define GPIO_INT_BMAT 46

#define U3_TXD        47
#define U3_RXD        48
#define U3_SCLK       49
#define U4_TXD        50
#define U4_RXD        51
#define U4_SCLK       52
#define T0_MAT0       53
#define T0_MAT1       54
#define T0_MAT2       55
#define T0_MAT3       56
#define T0_CAP0       57
#define T0_CAP1       58
#define T0_CAP2       59

#define num_funcs     60 



// PINENABLE0 defines
#define ACMP_I1   (1<<0)
#define ACMP_I2   (1<<1)
#define ACMP_I3   (1<<2)
#define ACMP_I4   (1<<3)
#define ACMP_I5   (1<<4)
#define SWCLK     (1<<5)
#define SWDIO     (1<<6)
#define XTALIN    (1<<7)
#define XTALOUT   (1<<8)
#define RESETN    (1<<9)
#define CLKIN     (1<<10)
#define VDDCMP    (1<<11)
#define I2C0_SDA  (1<<12)
#define I2C0_SCL  (1<<13)
#define ADC_0     (1<<14)
#define ADC_1     (1<<15)
#define ADC_2     (1<<16)
#define ADC_3     (1<<17)
#define ADC_4     (1<<18)
#define ADC_5     (1<<19)
#define ADC_6     (1<<20)
#define ADC_7     (1<<21)
#define ADC_8     (1<<22)
#define ADC_9     (1<<23)
#define ADC_10    (1<<24)
#define ADC_11    (1<<25)
#define DACOUT0   (1<<26)
#define DACOUT1   (1<<27)
#define CAPT_X0   (1<<28)
#define CAPT_X1   (1<<29)
#define CAPT_X2   (1<<30)
#define CAPT_X3   (1U<<31)

// PINENABLE1 defines
#define CAPT_X4   (1<<0) | 0x80000000
#define CAPT_X5   (1<<1) | 0x80000000
#define CAPT_X6   (1<<2) | 0x80000000
#define CAPT_X7   (1<<3) | 0x80000000
#define CAPT_X8   (1<<4) | 0x80000000
#define CAPT_YL   (1<<5) | 0x80000000
#define CAPT_YH   (1<<6) | 0x80000000


// Function prototypes
void ConfigSWM(uint32_t func, uint32_t port_pin);
void EnableFixedPinFunc(uint32_t func);
void DisableFixedPinFunc(uint32_t func);


#endif /* LPC8XX_SWM_H_ */
