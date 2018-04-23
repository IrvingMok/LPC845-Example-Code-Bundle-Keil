// Attention please!
// This is the header file for the LPC84x product family only.

/****************************************************************************
 *   $Id:: LPC8xx.h 6437 2012-10-31 11:06:06Z dep00694                     $
 *   Project: NXP LPC8xx software example  
 *
 *   Description:
 *     CMSIS Cortex-M0+ Core Peripheral Access Layer Header File for 
 *     NXP LPC800 Device Series 
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.

 * Permission to use, copy, modify, and distribute this software and its 
 * documentation is hereby granted, under NXP Semiconductors' 
 * relevant copyright in the software, without fee, provided that it 
 * is used in conjunction with NXP Semiconductors microcontrollers. This 
 * copyright, permission, and disclaimer notice must appear in all copies of 
 * this code.
****************************************************************************/
#ifndef __LPC8xx_H__
#define __LPC8xx_H__

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup LPC8xx_Definitions LPC8xx Definitions
  This file defines all structures and symbols for LPC8xx:
    - Registers and bitfields
    - peripheral base address
    - PIO definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup LPC8xx_CMSIS LPC8xx CMSIS Definitions
  Configuration of the Cortex-M0+ Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition ----------------------------------- 
 * ==========================================================================
 */
typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  Reset_IRQn                    = -15,    /*!< 1 Reset Vector, invoked on Power up and warm reset*/  
  NonMaskableInt_IRQn           = -14,    /*!< 2 Non Maskable Interrupt                           */
  HardFault_IRQn                = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                   */
  SVCall_IRQn                   = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                     */
  DebugMonitor_IRQn             = -4,     /*!<  12  Debug Monitor                                 */
  PendSV_IRQn                   = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                     */
  SysTick_IRQn                  = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                 */

/******  LPC84x Specific Interrupt Numbers ********************************************************/
  SPI0_IRQn                     = 0,        /*!< SPI0                                             */
  SPI1_IRQn                     = 1,        /*!< SPI1                                             */
  DAC0_IRQn                     = 2,        /*!< DAC0 Interrupt                                   */	
  UART0_IRQn                    = 3,        /*!< USART0                                           */
  UART1_IRQn                    = 4,        /*!< USART1                                           */
  UART2_IRQn                    = 5,        /*!< USART2                                           */
  I2C1_IRQn                     = 7,        /*!< I2C1                                             */
  I2C0_IRQn                     = 8,        /*!< I2C0                                             */
  SCT_IRQn                      = 9,        /*!< SCT                                              */
  MRT_IRQn                      = 10,       /*!< MRT                                              */ 
  CMP_IRQn                      = 11,       /*!< Analog Comparator shared slot with Cap Touch     */
  WDT_IRQn                      = 12,       /*!< WDT                                              */
  BOD_IRQn                      = 13,       /*!< BOD                                              */
  FLASH_IRQn                    = 14,       /*!< FLASH                                            */
  WKT_IRQn                      = 15,       /*!< WKT Interrupt                                    */
  ADC_SEQA_IRQn                 = 16,       /*!< ADC Seq. A                                       */    
  ADC_SEQB_IRQn                 = 17,       /*!< ADC Seq. B                                       */
  ADC_THCMP_IRQn                = 18,       /*!< ADC Thresh Comp                                  */    
  ADC_OVR_IRQn                  = 19,       /*!< ADC overrun                                      */
  DMA_IRQn                      = 20,       /*!< DMA                                              */    
  I2C2_IRQn                     = 21,       /*!< I2C2                                             */
  I2C3_IRQn                     = 22,       /*!< I2C3                                             */    
  CTIMER0_IRQn                  = 23,       /*!< Timer 0 Interrupt                                */
  PININT0_IRQn                  = 24,       /*!< External Interrupt 0                             */
  PININT1_IRQn                  = 25,       /*!< External Interrupt 1                             */  
  PININT2_IRQn                  = 26,       /*!< External Interrupt 2                             */
  PININT3_IRQn                  = 27,       /*!< External Interrupt 3                             */
  PININT4_IRQn                  = 28,       /*!< External Interrupt 4                             */
  PININT5_IRQn                  = 29,       /*!< External Interrupt 5 shared slot with DAC1       */
  PININT6_IRQn                  = 30,       /*!< External Interrupt 6 shared slot with UART3      */
  PININT7_IRQn                  = 31,       /*!< External Interrupt 7 shared slot with UART4      */
} IRQn_Type;

#define CAPT_IRQn   CMP_IRQn                // Analog Comparator shared slot with Cap Touch
#define DAC1_IRQn   PININT5_IRQn            // Pin int. 5 shared slot with DAC1
#define UART3_IRQn  PININT6_IRQn            // Pin int. 6 shared slot with UART3
#define UART4_IRQn  PININT7_IRQn            // Pin int. 7 shared slot with UART4

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0+ Processor and Core Peripherals */
#define __CM0PLUS_REV             0x0001
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */

/*@}*/ /* end of group LPC8xx_CMSIS */


#include "core_cm0plus.h"                  /* Cortex-M0+ processor and core peripherals          */
#include "system.h"                        /* System Header                                      */


#if defined ( __CC_ARM   )
#pragma anon_unions                        // This is needed by the Keil compiler 
#endif

/******************************************************************************/
/*                Device Specific Peripheral Registers structures             */
/******************************************************************************/

//------------- System Control (SYSCON) --------------------------------------
typedef struct
{
  __IO uint32_t SYSMEMREMAP         ; ///< (0x000) System memory remap
  __IO uint32_t RESERVED0[1]        ; ///< (0x004)
  __IO uint32_t SYSPLLCTRL          ; ///< (0x008) System PLL control
  __I  uint32_t SYSPLLSTAT          ; ///< (0x00C) System PLL status
  __IO uint32_t RESERVED1[4]        ; ///< (0x010 - 0x01C)
  __IO uint32_t SYSOSCCTRL          ; ///< (0x020) System oscillator control
  __IO uint32_t WDTOSCCTRL          ; ///< (0x024) Watchdog oscillator control
  __IO uint32_t FROOSCCTRL          ; ///< (0x028) FRO oscillator control 
  __IO uint32_t RESERVED2[1]        ; ///< (0x02C)
  __IO uint32_t FRODIRECTCLKUEN     ; ///< (0x030) FRO direct clock source update
  __IO uint32_t RESERVED3[1]        ; ///< (0x034)
  __IO uint32_t SYSRSTSTAT          ; ///< (0x038) System reset status 0
  __IO uint32_t RESERVED3a[1]       ; ///< (0x03C) 
  __IO uint32_t SYSPLLCLKSEL        ; ///< (0x040) System PLL clock source select 0
  __IO uint32_t SYSPLLCLKUEN        ; ///< (0x044) System PLL clock source update
  __IO uint32_t MAINCLKPLLSEL       ; ///< (0x048) Main clock pll source select 0
  __IO uint32_t MAINCLKPLLUEN       ; ///< (0x04C) Main clock pll source update enable
  __IO uint32_t MAINCLKSEL          ; ///< (0x050) Main clock source select
  __IO uint32_t MAINCLKUEN          ; ///< (0x054) Main clock source update enable
  __IO uint32_t SYSAHBCLKDIV        ; ///< (0x058) System clock divider
  __IO uint32_t RESERVED4[1]        ; ///< (0x05C)
  __IO uint32_t CAPTCLKSEL          ; ///< (0x060) CAPT clock source select
  __IO uint32_t ADCCLKSEL           ; ///< (0x064) ADC clock source select
  __IO uint32_t ADCCLKDIV           ; ///< (0x068) ADC clock divider
  __IO uint32_t SCTCLKSEL           ; ///< (0x06C) SCT clock source select
  __IO uint32_t SCTCLKDIV           ; ///< (0x070) SCT clock divider
  __IO uint32_t EXTCLKSEL           ; ///< (0x074) External clock source select
  __IO uint32_t RESERVED5[2]        ; ///< (0x078 - 0x07C)
  union {
    __IO uint32_t SYSAHBCLKCTRL[2]  ;
    struct {
    __IO uint32_t SYSAHBCLKCTRL0    ; ///< (0x080) System clock group 0 control
    __IO uint32_t SYSAHBCLKCTRL1    ; ///< (0x084) System clock group 1 control
    };
  };
  union {
    __IO uint32_t PRESETCTRL[2]     ;
    struct {
    __IO uint32_t PRESETCTRL0       ; ///< (0x088) Peripheral reset group 0 control
    __IO uint32_t PRESETCTRL1       ; ///< (0x08C) Peripheral reset group 1 control
    };
  };
  union {
    __IO uint32_t FCLKSEL[11]       ;
    struct {
      __IO uint32_t UART0CLKSEL     ; ///< (0x090) FCLK0 clock source select
      __IO uint32_t UART1CLKSEL     ; ///< (0x094) FCLK1 clock source select
      __IO uint32_t UART2CLKSEL     ; ///< (0x098) FCLK2 clock source select
      __IO uint32_t UART3CLKSEL     ; ///< (0x09C) FCLK3 clock source select
      __IO uint32_t UART4CLKSEL     ; ///< (0x0A0) FCLK4 clock source select
      __IO uint32_t I2C0CLKSEL      ; ///< (0x0A4) FCLK5 clock source select
      __IO uint32_t I2C1CLKSEL      ; ///< (0x0A8) FCLK6 clock source select
      __IO uint32_t I2C2CLKSEL      ; ///< (0x0AC) FCLK7 clock source select
      __IO uint32_t I2C3CLKSEL      ; ///< (0x0B0) FCLK8 clock source select
      __IO uint32_t SPI0CLKSEL      ; ///< (0x0B4) FCLK9 clock source select
      __IO uint32_t SPI1CLKSEL      ; ///< (0x0B8) FCLK10 clock source select
    };
  }; 
  __IO uint32_t RESERVED6[5]        ; ///< (0x0BC - 0x0CC)
  __IO uint32_t FRG0DIV             ; ///< (0x0D0) Fractional generator divider value
  __IO uint32_t FRG0MULT            ; ///< (0x0D4) Fractional generator multiplier value
  __IO uint32_t FRG0CLKSEL          ; ///< (0x0D8) FRG0 clock source select
  __IO uint32_t RESERVED8[1]        ; ///< (0x0DC)
  __IO uint32_t FRG1DIV             ; ///< (0x0E0) Fractional generator divider value
  __IO uint32_t FRG1MULT            ; ///< (0x0E4) Fractional generator multiplier value
  __IO uint32_t FRG1CLKSEL          ; ///< (0x0E8) FRG1 clock source select
  __IO uint32_t RESERVED9[1]        ; ///< (0x0EC)
  __IO uint32_t CLKOUTSEL           ; ///< (0x0F0) CLKOUT clock source select
  __IO uint32_t CLKOUTDIV           ; ///< (0x0F4) CLKOUT clock divider
  __IO uint32_t RESERVED10[1]       ; ///< (0x0F8)
  __IO uint32_t EXTTRACECMD         ; ///< (0x0FC) External trace buffer command
  __I  uint32_t PIOPORCAP0          ; ///< (0x100) POR captured PIO0 status 0
  __I  uint32_t PIOPORCAP1          ; ///< (0x104) POR captured PIO1 status 0
  __IO uint32_t RESERVED11[11]      ; ///< (0x108 - 0x130)
  __IO uint32_t IOCONCLKDIV6        ; ///< (0x134) Peripheral clock 6 to the IOCON block for programmable glitch filter
  __IO uint32_t IOCONCLKDIV5        ; ///< (0x138) Peripheral clock 5 to the IOCON block for programmable glitch filter
  __IO uint32_t IOCONCLKDIV4        ; ///< (0x13C) Peripheral clock 4 to the IOCON block for programmable glitch filter
  __IO uint32_t IOCONCLKDIV3        ; ///< (0x140) Peripheral clock 3 to the IOCON block for programmable glitch filter
  __IO uint32_t IOCONCLKDIV2        ; ///< (0x144) Peripheral clock 2 to the IOCON block for programmable glitch filter
  __IO uint32_t IOCONCLKDIV1        ; ///< (0x148) Peripheral clock 1 to the IOCON block for programmable glitch filter
  __IO uint32_t IOCONCLKDIV0        ; ///< (0x14C) Peripheral clock 0 to the IOCON block for programmable glitch filter
  __IO uint32_t BODCTRL             ; ///< (0x150) Brown-Out Detect
  __IO uint32_t SYSTCKCAL           ; ///< (0x154) System tick counter calibration
  __IO uint32_t RESERVED12[6]       ; ///< (0x158 - 0x16C)
  __IO uint32_t IRQLATENCY          ; ///< (0x170) IRQ delay. Allows trade-off between interrupt latency and determinism.
  __IO uint32_t NMISRC              ; ///< (0x174) NMI Source Control
  union {
    __IO uint32_t PINTSEL[8]        ;
    struct {
    __IO uint32_t PINTSEL0          ; ///< (0x178) GPIO Pin Interrupt Select 0
    __IO uint32_t PINTSEL1          ; ///< (0x17C) GPIO Pin Interrupt Select 1
    __IO uint32_t PINTSEL2          ; ///< (0x180) GPIO Pin Interrupt Select 2
    __IO uint32_t PINTSEL3          ; ///< (0x184) GPIO Pin Interrupt Select 3
    __IO uint32_t PINTSEL4          ; ///< (0x188) GPIO Pin Interrupt Select 4
    __IO uint32_t PINTSEL5          ; ///< (0x18C) GPIO Pin Interrupt Select 5
    __IO uint32_t PINTSEL6          ; ///< (0x190) GPIO Pin Interrupt Select 6
    __IO uint32_t PINTSEL7          ; ///< (0x194) GPIO Pin Interrupt Select 7
    };
  };
  __IO uint32_t RESERVED13[27]      ; ///< (0x198 - 0x200)
  __IO uint32_t STARTERP0           ; ///< (0x204) Start logic 0 pin wake-up enable
  __IO uint32_t RESERVED14[3]       ; ///< (0x208 - 0x210)
  __IO uint32_t STARTERP1           ; ///< (0x214) Start logic 1 interrupt wake-up enable
  __IO uint32_t RESERVED15[6]       ; ///< (0x218 - 0x22C)
  __IO uint32_t PDSLEEPCFG          ; ///< (0x230) Power-down states in deep-sleep mode
  __IO uint32_t PDAWAKECFG          ; ///< (0x234) Power-down states for wake-up from deep-sleep
  __IO uint32_t PDRUNCFG            ; ///< (0x238) Power configuration
  __IO uint32_t RESERVED16[111]     ; ///< (0x23C - 0x3F4)
  __I  uint32_t DEVICE_ID           ; ///< (0x3F8) Device ID
} LPC_SYSCON_TypeDef;




// ---------------- IOCON ----------------
typedef struct {                            
  __IO uint32_t PIO0_17;                    // 0x00
  __IO uint32_t PIO0_13;                    // 0x04
  __IO uint32_t PIO0_12;                    // 0x08
  __IO uint32_t PIO0_5;                     // 0x0C
  __IO uint32_t PIO0_4;                     // 0x10
  __IO uint32_t PIO0_3;                     // 0x14
  __IO uint32_t PIO0_2;                     // 0x18
  __IO uint32_t PIO0_11;                    // 0x1C
  __IO uint32_t PIO0_10;                    // 0x20
  __IO uint32_t PIO0_16;                    // 0x24
  __IO uint32_t PIO0_15;                    // 0x28
  __IO uint32_t PIO0_1;                     // 0x2C
  __IO uint32_t Reserved0;                  // 0x30
  __IO uint32_t PIO0_9;                     // 0x34
  __IO uint32_t PIO0_8;                     // 0x38
  __IO uint32_t PIO0_7;                     // 0x3C
  __IO uint32_t PIO0_6;                     // 0x40
  __IO uint32_t PIO0_0;                     // 0x44
  __IO uint32_t PIO0_14;                    // 0x48
  __IO uint32_t Reserved1;                  // 0x4C
  __IO uint32_t PIO0_28;                    // 0x50
  __IO uint32_t PIO0_27;                    // 0x54
  __IO uint32_t PIO0_26;                    // 0x58
  __IO uint32_t PIO0_25;                    // 0x5C
  __IO uint32_t PIO0_24;                    // 0x60
  __IO uint32_t PIO0_23;                    // 0x64
  __IO uint32_t PIO0_22;                    // 0x68
  __IO uint32_t PIO0_21;                    // 0x6C
  __IO uint32_t PIO0_20;                    // 0x70
  __IO uint32_t PIO0_19;                    // 0x74
  __IO uint32_t PIO0_18;                    // 0x78
  __IO uint32_t PIO1_8;                     // 0x7C
  __IO uint32_t PIO1_9;                     // 0x80
  __IO uint32_t PIO1_12;                    // 0x84
  __IO uint32_t PIO1_13;                    // 0x88
  __IO uint32_t PIO0_31;                    // 0x8C
  __IO uint32_t PIO1_0;                     // 0x90
  __IO uint32_t PIO1_1;                     // 0x94
  __IO uint32_t PIO1_2;                     // 0x98
  __IO uint32_t PIO1_14;                    // 0x9C
  __IO uint32_t PIO1_15;                    // 0xA0
  __IO uint32_t PIO1_3;                     // 0xA4
  __IO uint32_t PIO1_4;                     // 0xA8
  __IO uint32_t PIO1_5;                     // 0xAC
  __IO uint32_t PIO1_16;                    // 0xB0
  __IO uint32_t PIO1_17;                    // 0xB4
  __IO uint32_t PIO1_6;                     // 0xB8
  __IO uint32_t PIO1_18;                    // 0xBC
  __IO uint32_t PIO1_19;                    // 0xC0
  __IO uint32_t PIO1_7;                     // 0xC4
  __IO uint32_t PIO0_29;                    // 0xC8
  __IO uint32_t PIO0_30;                    // 0xCC
  __IO uint32_t PIO1_20;                    // 0xD0
  __IO uint32_t PIO1_21;                    // 0xD4
  __IO uint32_t PIO1_11;                    // 0xD8
  __IO uint32_t PIO1_10;                    // 0xDC
} LPC_IOCON_TypeDef;




// ================================================================================
// ================                    FLASHCTRL                   ================
// ================================================================================
typedef struct {                         /*!< (@ 0x40040000) FLASHCTRL Structure */
  __I  uint32_t  RESERVED0[4];
  __IO uint32_t  FLASHCFG;               /*!< (@ 0x40040010) Flash configuration register */
  __I  uint32_t  RESERVED1[3];
  __IO uint32_t  FMSSTART;               /*!< (@ 0x40040020) Signature start address register */
  __IO uint32_t  FMSSTOP;                /*!< (@ 0x40040024) Signature stop-address register  */
  __I  uint32_t  RESERVED2;
  __I  uint32_t  FMSW0;                  /*!< (@ 0x4004002C) Signature Word */
} LPC_FLASHCTRL_TypeDef;




//------------- Power Management Unit (PMU) --------------------------
typedef struct
{
  __IO uint32_t PCON;                   /*!< Offset: 0x000 Power control Register (R/W) */
  __IO uint32_t GPREG0;                 /*!< Offset: 0x004 General purpose Register 0 (R/W) */
  __IO uint32_t GPREG1;                 /*!< Offset: 0x008 General purpose Register 1 (R/W) */
  __IO uint32_t GPREG2;                 /*!< Offset: 0x00C General purpose Register 2 (R/W) */
  __IO uint32_t GPREG3;                 /*!< Offset: 0x010 General purpose Register 3 (R/W) */
  __IO uint32_t DPDCTRL;                /*!< Offset: 0x014 Deep power-down control register (R/W) */
} LPC_PMU_TypeDef;




//------------- Switch Matrix (SWM) --------------------------
typedef struct
{
  union {
    __IO uint32_t PINASSIGN[15];
    struct {
      __IO uint32_t PINASSIGN0;   // 0x000
      __IO uint32_t PINASSIGN1;   // 0x004
      __IO uint32_t PINASSIGN2;   // 0x008
      __IO uint32_t PINASSIGN3;   // 0x00C
      __IO uint32_t PINASSIGN4;   // 0x010
      __IO uint32_t PINASSIGN5;   // 0x014
      __IO uint32_t PINASSIGN6;   // 0x018
      __IO uint32_t PINASSIGN7;   // 0x01C
      __IO uint32_t PINASSIGN8;   // 0x020
      __IO uint32_t PINASSIGN9;   // 0x024
      __IO uint32_t PINASSIGN10;  // 0x028
      __IO uint32_t PINASSIGN11;  // 0x02C
      __IO uint32_t PINASSIGN12;  // 0x030
      __IO uint32_t PINASSIGN13;  // 0x034
      __IO uint32_t PINASSIGN14;  // 0x038

    };
  };
  __I  uint32_t  Reserved0[97];   // 0x03C - 0x1BC
  __IO uint32_t  PINENABLE0;      // 0x1C0
  __IO uint32_t  PINENABLE1;      // 0x1C4
} LPC_SWM_TypeDef;




// ------------------------------------------------------------------------------------------------
// -----                      General Purpose I/O (GPIO)                                      -----
// ------------------------------------------------------------------------------------------------
typedef struct {                            
  __IO uint8_t B0[32];            // 0x00 - 0x1F Byte pin registers P0.0 - P0.31
  __IO uint8_t B1[32];            // 0x20 - 0x3F Byte pin registers P1.0 - P1.31
  __I  uint8_t Reserved0[4032];   // 0x40 - 0xFFF
  __IO uint32_t W0[32];           // 0x1000 - 0x107C Word pin registers P0.0 - P0.31
  __IO uint32_t W1[32];           // 0x1080 - 0x10FC Word pin registers P1.0 - P1.31
  __I  uint32_t Reserved1[960];   // 0x1100 - 0x1FFC (960d = 0x3c0)

  union {
    __IO uint32_t DIR[2];         // 0x2000 - 0x2004
    struct {
      __IO uint32_t DIR0;         // 0x2000
      __IO uint32_t DIR1;         // 0x2004
    };
  };

  __I  uint32_t Reserved2[30];    // 0x2008 - 0x207C

  union {
    __IO uint32_t MASK[2];        // 0x2080 - 0x2084
    struct {
      __IO uint32_t MASK0;        // 0x2080
      __IO uint32_t MASK1;        // 0x2084
    };
  };

  __I  uint32_t Reserved3[30];    // 0x2088 - 0x20FC

  union {
  __IO uint32_t PIN[2];           // 0x2100 - 0x2104
    struct {
      __IO uint32_t PIN0;         // 0x2100 
      __IO uint32_t PIN1;         // 0x2104
    };
  };

  __I  uint32_t Reserved4[30];    // 0x2108 - 0x217C

  union {
    __IO uint32_t MPIN[2];        // 0x22180 - 0x2184
    struct {
      __IO uint32_t MPIN0;        // 0x2180
      __IO uint32_t MPIN1;        // 0x2184
    };
  };

  __I  uint32_t Reserved5[30];    // 0x2188 - 0x21FC

  union {
    __IO uint32_t SET[2];         // 0x2200 -0x2204
    struct {
      __IO uint32_t SET0;         // 0x2200
      __IO uint32_t SET1;         // 0x2204
    };
  };

  __I  uint32_t Reserved6[30];    // 0x2208 - 0x227C

  union {
    __O uint32_t CLR[2];          // 0x2280 - 0x2284
    struct {
      __O  uint32_t CLR0;         // 0x2280
      __O  uint32_t CLR1;         // 0x2284
    };
  };

  __I  uint32_t Reserved7[30];    // 0x2288 - 0x22FC

  union {
    __O  uint32_t NOT[2];          // 0x2300 - 0x2304
    struct {
      __O  uint32_t NOT0;          // 0x2300
      __O  uint32_t NOT1;          // 0x2304
    };
  };

  __I  uint32_t Reserved8[30];    // 0x2308 - 0x237C

  union {
    __O uint32_t DIRSET[2];       // 0x2380 - 0x2384
    struct {
      __O  uint32_t DIRSET0;      // 0x2380
      __O  uint32_t DIRSET1;      // 0x2384
    };
  };

  __I  uint32_t Reserved9[30];    // 0x2388 - 0x23FC

  union {
    __O uint32_t DIRCLR[2];       // 0x2400 - 0x2404
    struct {
      __O  uint32_t DIRCLR0;      // 0x2400
      __O  uint32_t DIRCLR1;      // 0x2404
    };
  };

  __I  uint32_t Reserved10[30];   // 0x2408 - 0x247C

  union {
    __O uint32_t DIRNOT[2];       // 0x2480 - 0x2484
    struct {
      __O  uint32_t DIRNOT0;      // 0x2480
      __O  uint32_t DIRNOT1;      // 0x2484
    };
  };

} LPC_GPIO_PORT_TypeDef;




// ------------------------------------------------------------------------------------------------
// ----- Pin Interrupts and Pattern Match  (PIN_INT)                                          -----
// ------------------------------------------------------------------------------------------------
typedef struct {                            /*!< (@ 0xA0004000) PIN_INT Structure */
  __IO uint32_t ISEL;                       /*!< (@ 0xA0004000) Pin Interrupt Mode register */
  __IO uint32_t IENR;                       /*!< (@ 0xA0004004) Pin Interrupt Enable (Rising) register */
  __IO uint32_t SIENR;                      /*!< (@ 0xA0004008) Set Pin Interrupt Enable (Rising) register */
  __IO uint32_t CIENR;                      /*!< (@ 0xA000400C) Clear Pin Interrupt Enable (Rising) register */
  __IO uint32_t IENF;                       /*!< (@ 0xA0004010) Pin Interrupt Enable Falling Edge / Active Level register */
  __IO uint32_t SIENF;                      /*!< (@ 0xA0004014) Set Pin Interrupt Enable Falling Edge / Active Level register */
  __IO uint32_t CIENF;                      /*!< (@ 0xA0004018) Clear Pin Interrupt Enable Falling Edge / Active Level address */
  __IO uint32_t RISE;                       /*!< (@ 0xA000401C) Pin Interrupt Rising Edge register */
  __IO uint32_t FALL;                       /*!< (@ 0xA0004020) Pin Interrupt Falling Edge register */
  __IO uint32_t IST;                        /*!< (@ 0xA0004024) Pin Interrupt Status register */
  __IO uint32_t PMCTRL;                     /*!< (@ 0xA0004028) GPIO pattern match interrupt control register          */
  __IO uint32_t PMSRC;                      /*!< (@ 0xA000402C) GPIO pattern match interrupt bit-slice source register */
  __IO uint32_t PMCFG;                      /*!< (@ 0xA0004030) GPIO pattern match interrupt bit slice configuration register */
} LPC_PIN_INT_TypeDef;




//------------- CRC Engine (CRC) -----------------------------------------
typedef struct
{
  __IO uint32_t MODE;
  __IO uint32_t SEED;
  union {
  __I  uint32_t SUM;
  __O  uint32_t WR_DATA;
  };
} LPC_CRC_TypeDef;




//------------- Comparator (CMP) --------------------------------------------------
typedef struct {                            /*!< (@ 0x40024000) CMP Structure          */
  __IO uint32_t  CTRL;                      /*!< (@ 0x40024000) Comparator control register */
  __IO uint32_t  LAD;                       /*!< (@ 0x40024004) Voltage ladder register */
} LPC_CMP_TypeDef;






//------------- Self Wakeup Timer (WKT) --------------------------------------------------
typedef struct {                            /*!< (@ 0x40028000) WKT Structure          */
  __IO uint32_t  CTRL;                      /*!< (@ 0x40028000) Alarm/Wakeup Timer Control register */
       uint32_t  Reserved[2];
  __IO uint32_t  COUNT;                     /*!< (@ 0x4002800C) Alarm/Wakeup TImer counter register */
} LPC_WKT_TypeDef;






//------------- Multi-Rate Timer(MRT) --------------------------------------------------
typedef struct {
__IO uint32_t INTVAL;        							
__IO uint32_t TIMER;         							
__IO uint32_t CTRL;          							
__IO uint32_t STAT;          							
} MRT_Channel_cfg_Type;

typedef struct {
  MRT_Channel_cfg_Type Channel[4]; 		
  uint32_t Reserved0[45];          // Address offsets = 0x40 - 0xF0
  __IO uint32_t IDLE_CH; 			
  __IO uint32_t IRQ_FLAG; 						
} LPC_MRT_TypeDef;






//------------- Universal Asynchronous Receiver Transmitter (USART) -----------
typedef struct
{
  __IO uint32_t  CFG;
  __IO uint32_t  CTL;
  __IO uint32_t  STAT;
  __IO uint32_t  INTENSET;
  __O  uint32_t  INTENCLR;
  __I  uint32_t  RXDAT;
  __I  uint32_t  RXDATSTAT;
  __IO uint32_t  TXDAT;
  __IO uint32_t  BRG;
  __I  uint32_t  INTSTAT;
  __IO uint32_t  OSR;
  __IO uint32_t  ADDR;
} LPC_USART_TypeDef;







//------------- SPI -----------------------
typedef struct
{
  __IO uint32_t  CFG;			    /* 0x00 */
  __IO uint32_t  DLY;
  __IO uint32_t  STAT;
  __IO uint32_t  INTENSET;
  __O  uint32_t  INTENCLR;		/* 0x10 */
  __I  uint32_t  RXDAT;
  __IO uint32_t  TXDATCTL;
  __IO uint32_t  TXDAT;
  __IO uint32_t  TXCTL; 		  /* 0x20 */
  __IO uint32_t  DIV;
  __I  uint32_t  INTSTAT;
} LPC_SPI_TypeDef;






//------------- Inter-Integrated Circuit (I2C) -------------------------------
typedef struct
{
  __IO uint32_t  CFG;			  /* 0x00 */
  __IO uint32_t  STAT;
  __IO uint32_t  INTENSET;
  __O  uint32_t  INTENCLR;
  __IO uint32_t  TIMEOUT;		/* 0x10 */
  union {
    __IO uint32_t  CLKDIV;
    __IO uint32_t  DIV;
  };
  __IO uint32_t  INTSTAT;
       uint32_t  Reserved0[1];  
  __IO uint32_t  MSTCTL;			  /* 0x20 */
  __IO uint32_t  MSTTIME;
  __IO uint32_t  MSTDAT;
       uint32_t  Reserved1[5];
  __IO uint32_t  SLVCTL;			  /* 0x40 */
  __IO uint32_t  SLVDAT;
  __IO uint32_t  SLVADR0;
  __IO uint32_t  SLVADR1;
  __IO uint32_t  SLVADR2;			  /* 0x50 */
  __IO uint32_t  SLVADR3;
  __IO uint32_t  SLVQUAL0;
       uint32_t  Reserved2[9];
  __I  uint32_t  MONRXDAT;			/* 0x80 */		
} LPC_I2C_TypeDef;






// ================================================================================
// == SCT 
// ================================================================================

#define CONFIG_SCT_nEV   (8)      // Number of events 
#define CONFIG_SCT_nRG   (8)      // Number of match/compare registers 
#define CONFIG_SCT_nOU   (7)      // Number of outputs 

typedef struct {

  __IO  uint32_t CONFIG;           // 0x0

  union {
    __IO uint32_t CTRL;            // 0x4
    struct {
      __IO uint16_t CTRL_L;
      __IO uint16_t CTRL_H;
    };
  };

  union {
    __IO uint32_t LIMIT;           // 0x8
    struct {
      __IO uint16_t LIMIT_L;
      __IO uint16_t LIMIT_H;
    };
  };

  union {
    __IO uint32_t HALT;            // 0xc
    struct {
      __IO uint16_t HALT_L;
      __IO uint16_t HALT_H;
    };
  };

  union {
    __IO uint32_t STOP;            // 0x10
    struct {
      __IO uint16_t STOP_L;
      __IO uint16_t STOP_H;
    };
  };

  union {
    __IO uint32_t START;           // 0x14
    struct {
      __IO uint16_t START_L;
      __IO uint16_t START_H;
    };
  };

  uint32_t RESERVED1[10];

  union {
    __IO uint32_t COUNT;           // 0x40
    struct {
      __IO uint16_t COUNT_L;
      __IO uint16_t COUNT_H;
    };
  };

  union {
    __IO uint32_t STATE;           // 0x44
    struct {
      __IO uint16_t STATE_L;
      __IO uint16_t STATE_H;
    };
  };

  __I  uint32_t INPUT;             // 0x48

  union {
    __IO uint32_t REGMODE;         // 0x4c
    struct {
      __IO uint16_t REGMODE_L;
      __IO uint16_t REGMODE_H;
    };
  };

  __IO uint32_t OUTPUT;            // 0x50
  __IO uint32_t OUTPUTDIRCTRL;     // 0x54
  __IO uint32_t RES;               // 0x58
  __IO uint32_t DMAREQ0;           // 0x5c
  __IO uint32_t DMAREQ1;           // 0x60
  uint32_t RESERVED2[35];          // 0x64 - 0xec
  __IO uint32_t EVEN;              // 0xf0
  __IO uint32_t EVFLAG;            // 0xf4
  __IO uint32_t CONEN;             // 0xf8
  __IO uint32_t CONFLAG;           // 0xfc

  union {                          // Match / Capture 0x100 - 0x13c
    __IO union {                          
      uint32_t U;                  // MATCH[i].U  Unified 32-bit register
      struct {
        uint16_t L;                // MATCH[i].L  Access to L value
        uint16_t H;                // MATCH[i].H  Access to H value
      };
    } MATCH[CONFIG_SCT_nRG];

    __I union {
      uint32_t U;                  // CAP[i].U  Unified 32-bit register 
      struct {
        uint16_t L;                // CAP[i].L  Access to L value 
        uint16_t H;                // CAP[i].H  Access to H value 
      };
    } CAP[CONFIG_SCT_nRG];
  };

  uint32_t RESERVED3[32 - CONFIG_SCT_nRG];  // ...-0x17C reserved

  union {
    __IO uint16_t MATCH_L[CONFIG_SCT_nRG];  // 0x180-... Match Value L counter 
    __I  uint16_t CAP_L[CONFIG_SCT_nRG];    // 0x180-... Capture Value L counter
  };

  uint16_t RESERVED4[32 - CONFIG_SCT_nRG];  // ...-0x1BE reserved

  union {
    __IO uint16_t MATCH_H[CONFIG_SCT_nRG];  // 0x1C0-... Match Value H counter
    __I  uint16_t CAP_H[CONFIG_SCT_nRG];    // 0x1C0-... Capture Value H counter
  };

  uint16_t RESERVED5[32 - CONFIG_SCT_nRG];  // ...-0x1FE reserved
  
  union {
    __IO union {                            // 0x200-... Match Reload / Capture Control value
      uint32_t U;                           // MATCHREL[i].U  Unified 32-bit register
      struct {
        uint16_t L;                         // MATCHREL[i].L  Access to L value 
        uint16_t H;                         // MATCHREL[i].H  Access to H value 
      };
    } MATCHREL[CONFIG_SCT_nRG];

    __IO union {
      uint32_t U;                           // CAPCTRL[i].U  Unified 32-bit register
      struct {
        uint16_t L;                         // CAPCTRL[i].L  Access to L value 
        uint16_t H;                         // SCTCAPCTRL[i].H  Access to H value
      };
    } CAPCTRL[CONFIG_SCT_nRG];
  };

  uint32_t RESERVED6[32 - CONFIG_SCT_nRG];   // ...-0x27C reserved 

  union {
    __IO uint16_t MATCHREL_L[CONFIG_SCT_nRG]; // 0x280-... Match Reload value L counter
    __IO uint16_t CAPCTRL_L[CONFIG_SCT_nRG];  // 0x280-... Capture Control value L counter
  };

  uint16_t RESERVED7[32 - CONFIG_SCT_nRG];    // ...-0x2BE reserved 

  union {
    __IO uint16_t MATCHREL_H[CONFIG_SCT_nRG]; // 0x2C0-... Match Reload value H counter 
    __IO uint16_t CAPCTRL_H[CONFIG_SCT_nRG];  // 0x2C0-... Capture Control value H counter 
  };

  uint16_t RESERVED8[32 - CONFIG_SCT_nRG];    // ...-0x2FE reserved

  __IO struct {                               // 0x300-0x3FC  EVENT[i].STATE / EVENT[i].CTRL
    uint32_t STATE;
    uint32_t CTRL;
  } EVENT[CONFIG_SCT_nEV];

  uint32_t RESERVED9[128 - (2 * CONFIG_SCT_nEV)]; // ...-0x4FC reserved 

  __IO struct {                               // 0x500-0x57C  OUT[n].SET / OUT[n].CLR 
    uint32_t SET;                             // Output n Set Register 
    uint32_t CLR;                             // Output n Clear Register
  } OUT[CONFIG_SCT_nOU];

  uint32_t RESERVED10[((0x300 / 4) - 1) - (2 * CONFIG_SCT_nOU)]; // ...-0x7F8 reserved 
  
  __I  uint32_t MODULECONTENT;                // 0x7FC Module Content 

} LPC_SCT_TypeDef;








//------------------- Standard Counter/Timer (CTIMER) ---------------------
typedef struct {
  __IO uint32_t IR;               // 0x00
  __IO uint32_t TCR;              // 0x04
  __IO uint32_t TC;               // 0x08
  __IO uint32_t PR;               // 0x0C
  __IO uint32_t PC;               // 0x10
  __IO uint32_t MCR;              // 0x14
  __IO uint32_t MR[4];            // 0x18 - 0x24
  __IO uint32_t CCR;              // 0x28
  __IO uint32_t CR[4];            // 0x2C - 0x38
  __IO uint32_t EMR;              // 0x3C
  __I  uint32_t RESERVED0[12];    // 0x40 - 0x6C 
  __IO uint32_t CTCR;             // 0x70
  __IO uint32_t PWMC;             // 0x74
} LPC_TIMER_TypeDef;






//------------- Widowed Watchdog Timer (WWDT) -----------------------------------------
typedef struct
{
  __IO uint32_t MOD;                    /*!< Offset: 0x000 Watchdog mode register (R/W) */
  __IO uint32_t TC;                     /*!< Offset: 0x004 Watchdog timer constant register (R/W) */
  __O  uint32_t FEED;                   /*!< Offset: 0x008 Watchdog feed sequence register (W) */
  __I  uint32_t TV;                     /*!< Offset: 0x00C Watchdog timer value register (R) */
       uint32_t RESERVED;               /*!< Offset: 0x010 RESERVED                          */
  __IO uint32_t WARNINT;                /*!< Offset: 0x014 Watchdog timer warning int. register (R/W) */
  __IO uint32_t WINDOW;                 /*!< Offset: 0x018 Watchdog timer window value register (R/W) */
} LPC_WWDT_TypeDef;









//-------------------------------------------------------------------------
// Input multiplexing and DMA trigger multiplexing (INPUT MUX, DMA TRIGMUX)
//-------------------------------------------------------------------------
typedef struct {
  __IO uint32_t DMA_INMUX_INMUX0;       // 0x0
  __IO uint32_t DMA_INMUX_INMUX1;       // 0x4
  __I  uint32_t Preserved[6];           // 0x8 - 0x1C
  __IO uint32_t SCT0_INMUX0;            // 0x20
  __IO uint32_t SCT0_INMUX1;            // 0x24
  __IO uint32_t SCT0_INMUX2;            // 0x28
  __IO uint32_t SCT0_INMUX3;            // 0x2C
  __I  uint32_t Severed[4];             // 0x30 - 0x3C
  __IO uint32_t DMA_ITRIG_INMUX0;       // 0x40
  __IO uint32_t DMA_ITRIG_INMUX1;       // 0x44
  __IO uint32_t DMA_ITRIG_INMUX2;       // 0x48
  __IO uint32_t DMA_ITRIG_INMUX3;       // 0x4C
  __IO uint32_t DMA_ITRIG_INMUX4;       // 0x50
  __IO uint32_t DMA_ITRIG_INMUX5;       // 0x54
  __IO uint32_t DMA_ITRIG_INMUX6;       // 0x58
  __IO uint32_t DMA_ITRIG_INMUX7;       // 0x5C
  __IO uint32_t DMA_ITRIG_INMUX8;       // 0x60
  __IO uint32_t DMA_ITRIG_INMUX9;       // 0x64
  __IO uint32_t DMA_ITRIG_INMUX10;      // 0x68
  __IO uint32_t DMA_ITRIG_INMUX11;      // 0x6C
  __IO uint32_t DMA_ITRIG_INMUX12;      // 0x70
  __IO uint32_t DMA_ITRIG_INMUX13;      // 0x74
  __IO uint32_t DMA_ITRIG_INMUX14;      // 0x78
  __IO uint32_t DMA_ITRIG_INMUX15;      // 0x7C
  __IO uint32_t DMA_ITRIG_INMUX16;      // 0x80
  __IO uint32_t DMA_ITRIG_INMUX17;      // 0x84
  __IO uint32_t DMA_ITRIG_INMUX18;      // 0x88
  __IO uint32_t DMA_ITRIG_INMUX19;      // 0x8C
  __IO uint32_t DMA_ITRIG_INMUX20;      // 0x90
  __IO uint32_t DMA_ITRIG_INMUX21;      // 0x94
  __IO uint32_t DMA_ITRIG_INMUX22;      // 0x98
  __IO uint32_t DMA_ITRIG_INMUX23;      // 0x9C
  __IO uint32_t DMA_ITRIG_INMUX24;      // 0xA0
} LPC_INMUX_TRIGMUX_TypeDef;






//------------- ADC -----------------------------------------
typedef struct {
  __IO uint32_t CTRL;           // 0x0
       uint32_t RESERVED0;      // 0x4
  __IO uint32_t SEQA_CTRL;      // 0x8
  __IO uint32_t SEQB_CTRL;      // 0xC
  __IO uint32_t SEQA_GDAT;      // 0x10
  __IO uint32_t SEQB_GDAT;      // 0x14
       uint32_t RESERVED1[2];   // 0x18 - 0x1C
  __IO uint32_t DAT[12];        // 0x20 - 0x4C
  __IO uint32_t THR0_LOW;       // 0x50
  __IO uint32_t THR1_LOW;       // 0x54
  __IO uint32_t THR0_HIGH;      // 0x58
  __IO uint32_t THR1_HIGH;      // 0x5C
  __IO uint32_t CHAN_THRSEL;    // 0x60
  __IO uint32_t INTEN;          // 0x64
  __IO uint32_t FLAGS;          // 0x68
  __IO uint32_t TRM;            // 0x6C
} LPC_ADC_TypeDef;






//------------- DMA -----------------------------------------
#define NUM_DMA_CHANNELS 25

typedef struct {
  __IO uint32_t  CFG;
  __I  uint32_t  CTLSTAT;
  __IO uint32_t  XFERCFG;
  __I  uint32_t  RESERVED;
} LPC_DMA_CHANNEL_T;

typedef struct {
  __IO uint32_t  CTRL;                          // 0x0
  __I  uint32_t  INTSTAT;                       // 0x4
  __IO uint32_t  SRAMBASE;                      // 0x8
  __I  uint32_t  Reserved0[5];                  // 0x10 - 0x1C
  __IO uint32_t  ENABLESET0;                    // 0x20
  __I  uint32_t  Reserved1;                     // 0x24
  __O  uint32_t  ENABLECLR0;                    // 0x28
  __I  uint32_t  Reserved2;                     // 0x2C
  __I  uint32_t  ACTIVE0;                       // 0x30
  __I  uint32_t  Reserved3;                     // 0x34
  __I  uint32_t  BUSY0;                         // 0x38
  __I  uint32_t  Reserved4;                     // 0x3C
  __IO uint32_t  ERRINT0;                       // 0x40
  __I  uint32_t  Reserved5;                     // 0x44
  __IO uint32_t  INTENSET0;                     // 0x48
  __I  uint32_t  Reserved6;                     // 0x4C
  __O  uint32_t  INTENCLR0;                     // 0x50
  __I  uint32_t  Reserved7;                     // 0x54
  __IO uint32_t  INTA0;                         // 0x58
  __I  uint32_t  Reserved8;                     // 0x5C
  __IO uint32_t  INTB0;                         // 0x60
  __I  uint32_t  Reserved9;                     // 0x64
  __O  uint32_t  SETVALID0;                     // 0x68
  __I  uint32_t  Reserved10;                    // 0x6C
  __O  uint32_t  SETTRIG0;                      // 0x70
  __I  uint32_t  Reserved11;                    // 0x74
  __O  uint32_t  ABORT0;                        // 0x78
  __I  uint32_t  Absolutely_Nothing[225];       // 0x7C - 0x3FC
  LPC_DMA_CHANNEL_T CHANNEL[NUM_DMA_CHANNELS];  // 0x400 - 0xThe_End
} LPC_DMA_TypeDef;






//------------- DAC ----------------
typedef struct {
  __IO uint32_t CR;             // 0x00
  __IO uint32_t CTRL;           // 0x04
  __IO uint32_t CNTVAL;         // 0x08
} LPC_DAC_TypeDef;






//------------- Capacitive Touch module (CAPT) ----------------
typedef struct {
  __IO uint32_t CTRL;        // 0x00
  __IO uint32_t STATUS;      // 0x04
  __IO uint32_t POLL_TCNT;   // 0x08
  __I  uint32_t reserved0;   // 0x0C
  __IO uint32_t INTENSET;    // 0x10
  __O  uint32_t INTENCLR;    // 0x14
  __I  uint32_t INTSTAT;     // 0x18
  __I  uint32_t reserved1;   // 0x1C
  __I  uint32_t TOUCH;       // 0x20
  __I  uint32_t A_hole[1014];// 0x24 - 0xFF8
  __I  uint32_t ID;          // 0xFFC
} LPC_CAPT_TypeDef;








#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
// Base addresses                                                             
#define LPC_FLASH_BASE         (0x00000000UL)
#define LPC_RAM_BASE           (0x10000000UL)
#define LPC_ROM_BASE           (0x1FFF0000UL)
#define LPC_APB0_BASE          (0x40000000UL)
#define LPC_AHB_BASE           (0x50000000UL)

// APB0 peripherals 
#define LPC_WWDT_BASE          (LPC_APB0_BASE + 0x00000)
#define LPC_MRT_BASE           (LPC_APB0_BASE + 0x04000)
#define LPC_WKT_BASE           (LPC_APB0_BASE + 0x08000)
#define LPC_SWM_BASE           (LPC_APB0_BASE + 0x0C000)
#define LPC_DAC0_BASE          (LPC_APB0_BASE + 0x14000)
#define LPC_DAC1_BASE          (LPC_APB0_BASE + 0x18000)
#define LPC_ADC_BASE           (LPC_APB0_BASE + 0x1C000)
#define LPC_PMU_BASE           (LPC_APB0_BASE + 0x20000)
#define LPC_CMP_BASE           (LPC_APB0_BASE + 0x24000)
//                             (LPC_APB0_BASE + 0x28000)
#define LPC_INMUX_TRIGMUX_BASE (LPC_APB0_BASE + 0x2C000)
#define LPC_I2C2_BASE          (LPC_APB0_BASE + 0x30000)
#define LPC_I2C3_BASE          (LPC_APB0_BASE + 0x34000)
#define LPC_CTIMER0_BASE       (LPC_APB0_BASE + 0x38000)
//                             (LPC_APB0_BASE + 0x38000)
#define LPC_FLASHCTRL_BASE     (LPC_APB0_BASE + 0x40000)
#define LPC_IOCON_BASE         (LPC_APB0_BASE + 0x44000)
#define LPC_SYSCON_BASE        (LPC_APB0_BASE + 0x48000)
#define LPC_I2C0_BASE          (LPC_APB0_BASE + 0x50000)
#define LPC_I2C1_BASE          (LPC_APB0_BASE + 0x54000)
#define LPC_SPI0_BASE          (LPC_APB0_BASE + 0x58000)
#define LPC_SPI1_BASE          (LPC_APB0_BASE + 0x5C000)
#define LPC_CAPT_BASE          (LPC_APB0_BASE + 0x60000)
#define LPC_USART0_BASE        (LPC_APB0_BASE + 0x64000)
#define LPC_USART1_BASE        (LPC_APB0_BASE + 0x68000)
#define LPC_USART2_BASE        (LPC_APB0_BASE + 0x6C000)
#define LPC_USART3_BASE        (LPC_APB0_BASE + 0x70000)
#define LPC_USART4_BASE        (LPC_APB0_BASE + 0x74000)

// AHB peripherals
#define LPC_CRC_BASE           (LPC_AHB_BASE + 0x00000)
#define LPC_SCT_BASE           (LPC_AHB_BASE + 0x04000)
#define LPC_DMA_BASE           (LPC_AHB_BASE + 0x08000)
#define LPC_MTB_SFR_BASE       (LPC_AHB_BASE + 0x0C000)
#define LPC_FAIM_BASE          (LPC_AHB_BASE + 0x10000)
#define LPC_GPIO_PORT_BASE     (0xA0000000)
#define LPC_PIN_INT_BASE       (LPC_GPIO_PORT_BASE + 0x4000)

/******************************************************************************/
/*                         Peripheral declarations                            */
/******************************************************************************/

#define LPC_WWDT              ((LPC_WWDT_TypeDef   *) LPC_WWDT_BASE  )
#define LPC_MRT               ((LPC_MRT_TypeDef    *) LPC_MRT_BASE   )
#define LPC_WKT               ((LPC_WKT_TypeDef    *) LPC_WKT_BASE   )
#define LPC_SWM               ((LPC_SWM_TypeDef    *) LPC_SWM_BASE   )
#define LPC_DAC0              ((LPC_DAC_TypeDef    *) LPC_DAC0_BASE  )
#define LPC_DAC1              ((LPC_DAC_TypeDef    *) LPC_DAC1_BASE  )
#define LPC_ADC               ((LPC_ADC_TypeDef    *) LPC_ADC_BASE   )
#define LPC_PMU               ((LPC_PMU_TypeDef    *) LPC_PMU_BASE   )
#define LPC_CMP               ((LPC_CMP_TypeDef    *) LPC_CMP_BASE   )
#define LPC_INMUX_TRIGMUX     ((LPC_INMUX_TRIGMUX_TypeDef *) LPC_INMUX_TRIGMUX_BASE)
#define LPC_I2C2              ((LPC_I2C_TypeDef    *) LPC_I2C2_BASE  )
#define LPC_I2C3              ((LPC_I2C_TypeDef    *) LPC_I2C3_BASE  )
#define LPC_CTIMER0           ((LPC_TIMER_TypeDef  *) LPC_CTIMER0_BASE  )
#define LPC_FLASHCTRL         ((LPC_FLASHCTRL_TypeDef *) LPC_FLASHCTRL_BASE )
#define LPC_IOCON             ((LPC_IOCON_TypeDef  *) LPC_IOCON_BASE )
#define LPC_SYSCON            ((LPC_SYSCON_TypeDef *) LPC_SYSCON_BASE)
#define LPC_I2C0              ((LPC_I2C_TypeDef    *) LPC_I2C0_BASE  )
#define LPC_I2C1              ((LPC_I2C_TypeDef    *) LPC_I2C1_BASE  )
#define LPC_SPI0              ((LPC_SPI_TypeDef    *) LPC_SPI0_BASE  )
#define LPC_SPI1              ((LPC_SPI_TypeDef    *) LPC_SPI1_BASE  )
#define LPC_CAPT              ((LPC_CAPT_TypeDef   *) LPC_CAPT_BASE  )
#define LPC_USART0            ((LPC_USART_TypeDef   *) LPC_USART0_BASE )
#define LPC_USART1            ((LPC_USART_TypeDef   *) LPC_USART1_BASE )
#define LPC_USART2            ((LPC_USART_TypeDef   *) LPC_USART2_BASE )
#define LPC_USART3            ((LPC_USART_TypeDef   *) LPC_USART3_BASE )
#define LPC_USART4            ((LPC_USART_TypeDef   *) LPC_USART4_BASE )

#define LPC_CRC               ((LPC_CRC_TypeDef    *) LPC_CRC_BASE   )
#define LPC_SCT               ((LPC_SCT_TypeDef    *) LPC_SCT_BASE   )
#define LPC_SCT0              ((LPC_SCT_TypeDef    *) LPC_SCT_BASE   )
#define LPC_DMA               ((LPC_DMA_TypeDef    *) LPC_DMA_BASE   )
#define LPC_GPIO_PORT         ((LPC_GPIO_PORT_TypeDef  *) LPC_GPIO_PORT_BASE  )
#define LPC_PIN_INT           ((LPC_PIN_INT_TypeDef   *) LPC_PIN_INT_BASE  )





///////////////////////////////////////////////////////////////////////////////
// Other chip-specific macro definitions (a.k.a. the chip.h section)
///////////////////////////////////////////////////////////////////////////////

// CAP Touch pins to IOCON mapping
#define CAPTOUCH_X0_PORT PIO0_31
#define CAPTOUCH_X1_PORT PIO1_0
#define CAPTOUCH_X2_PORT PIO1_1
#define CAPTOUCH_X3_PORT PIO1_2
#define CAPTOUCH_X4_PORT PIO1_3
#define CAPTOUCH_X5_PORT PIO1_4
#define CAPTOUCH_X6_PORT PIO1_5
#define CAPTOUCH_X7_PORT PIO1_6
#define CAPTOUCH_X8_PORT PIO1_7
#define CAPTOUCH_YL_PORT PIO1_8
#define CAPTOUCH_YH_PORT PIO1_9

// ACMP_I-to-IOCON mapping
#define ACMP_I1_PORT PIO0_0
#define ACMP_I2_PORT PIO0_1
#define ACMP_I3_PORT PIO0_14
#define ACMP_I4_PORT PIO0_23
#define ACMP_I5_PORT PIO0_30


#ifdef __cplusplus
}
#endif

#endif  /* __LPC8xx_H__ */
