/*
 * lpc8xx_syscon.h
 *
 *  Created on: 
 *      Author: 
 */

#ifndef LPC8XX_SYSCON_H_
#define LPC8XX_SYSCON_H_


// SYSAHBCLKCTRL0 register bits
#define ROM       (1<<1)
#define RAM0_1    (1<<2)
#define FLASHREG  (1<<3)
#define FLASH     (1<<4)
#define I2C0      (1<<5)
#define GPIO      (1<<6)
#define GPIO0     (1<<6)
#define SWM       (1<<7)
#define SCT       (1<<8)
#define WKT       (1<<9)
#define MRT       (1<<10)
#define SPI0      (1<<11)
#define SPI1      (1<<12)
#define CRC       (1<<13)
#define UART0     (1<<14)
#define UART1     (1<<15)
#define UART2     (1<<16)
#define WWDT      (1<<17)
#define IOCON     (1<<18)
#define ACMP      (1<<19)
#define GPIO1     (1<<20)
#define I2C1      (1<<21)
#define I2C2      (1<<22)
#define I2C3      (1<<23)
#define ADC       (1<<24)
#define CTIMER0   (1<<25)
#define MTB       (1<<26)
#define DAC0      (1<<27)
#define GPIO_INT  (1<<28)
#define DMA       (1<<29)
#define UART3     (1<<30)
#define UART4     (1<<31)

// SYSAHBCLKCTRL1 register bits
#define CAPT      (1<<0)
#define DAC1      (1<<1)
#define FAIM      (1<<2)

// SYSAHBCLKCTRL[] register bits (alternate form)
typedef enum {
  CLK_ROM = 1   ,
  CLK_RAM0_1    ,
  CLK_FLASHREG  ,
  CLK_FLASH     ,
  CLK_I2C0      ,
  CLK_GPIO0     ,
  CLK_SWM       ,
  CLK_SCT       ,
  CLK_WKT       ,
  CLK_MRT       ,
  CLK_SPI0      ,
  CLK_SPI1      ,
  CLK_CRC       ,
  CLK_UART0     ,
  CLK_UART1     ,
  CLK_UART2     ,
  CLK_WWDT      ,
  CLK_IOCON     ,
  CLK_ACMP      ,
  CLK_GPIO1     ,
  CLK_I2C1      ,
  CLK_I2C2      ,
  CLK_I2C3      ,
  CLK_ADC       ,
  CLK_CTIMER0   ,
  CLK_MTB       ,
  CLK_DAC0      ,
  CLK_GPIO_INT  ,
  CLK_DMA       ,
  CLK_UART3     ,
  CLK_UART4     ,

  CLK_CAPT =  32,
  CLK_DAC1      ,
  CLK_FAIM      
} CHIP_SYSCON_CLOCK_CTRL_T;

// PRESETCTRL0 register bits
#define FLASH_RST_N   ~(1<<4)
#define I2C0_RST_N    ~(1<<5)
#define GPIO0_RST_N   ~(1<<6)
#define SWM_RST_N     ~(1<<7)
#define SCT_RST_N     ~(1<<8)
#define SCT0_RST_N    ~(1<<8)
#define WKT_RST_N     ~(1<<9)
#define MRT_RST_N     ~(1<<10)
#define SPI0_RST_N    ~(1<<11)
#define SPI1_RST_N    ~(1<<12)
#define CRC_RST_N     ~(1<<13)
#define UART0_RST_N   ~(1<<14)
#define UART1_RST_N   ~(1<<15)
#define UART2_RST_N   ~(1<<16)
#define IOCON_RST_N   ~(1<<18)
#define ACMP_RST_N    ~(1<<19)
#define GPIO1_RST_N   ~(1<<20)
#define I2C1_RST_N    ~(1<<21)
#define I2C2_RST_N    ~(1<<22)
#define I2C3_RST_N    ~(1<<23)
#define ADC_RST_N     ~(1<<24)
#define CTIMER0_RST_N ~(1<<25)
#define DAC0_RST_N    ~(1<<27)
#define GPIOINT_RST_N ~(1<<28)
#define DMA_RST_N     ~(1<<29)
#define UART3_RST_N   ~(1<<30)
#define UART4_RST_N   ~(1<<31)

// PRESETCTRL1 register bits
#define CAPT_RST_N    ~(1<<0)
#define DAC1_RST_N    ~(1<<1)
#define FAIM_RST_N    ~(1<<2)
#define FRG0_RST_N    ~(1<<3)
#define FRG1_RST_N    ~(1<<4)

// PRESETCTRL[] register bits (alternate form)
typedef enum {
  RESET_FLASH = 4 ,
  RESET_I2C0      ,
  RESET_GPIO0     ,
  RESET_SWM       ,
  RESET_SCT       ,
  RESET_WKT       ,
  RESET_MRT       ,
  RESET_SPI0      ,
  RESET_SPI1      ,
  RESET_CRC       ,
  RESET_UART0     ,
  RESET_UART1     ,
  RESET_UART2     ,
  RESET_WWDT      ,
  RESET_IOCON     ,
  RESET_ACMP      ,
  RESET_GPIO1     ,
  RESET_I2C1      ,
  RESET_I2C2      ,
  RESET_I2C3      ,
  RESET_ADC       ,
  RESET_CTIMER0   ,
  RESET_MTB       ,
  RESET_DAC0      ,
  RESET_GPIO_INT  ,
  RESET_DMA       ,
  RESET_UART3     ,
  RESET_UART4     ,

  RESET_CAPT = 32 ,
  RESET_DAC1      ,
  RESET_FAIM      ,
  RESET_FRG0      ,
  RESET_FRG1   
} CHIP_SYSCON_PERIPH_RESET_T;


// STARTERP0 register bits
#define PINT0_WAKEUP      (1<<0)
#define PINT1_WAKEUP      (1<<1)
#define PINT2_WAKEUP      (1<<2)
#define PINT3_WAKEUP      (1<<3)
#define PINT4_WAKEUP      (1<<4)
#define PINT5_WAKEUP      (1<<5)
#define PINT6_WAKEUP      (1<<6)
#define PINT7_WAKEUP      (1<<7)

// STARTERP1 register bits
#define SPI0_INT_WAKEUP   (1<<0)
#define SPI1_INT_WAKEUP   (1<<1)
#define USART0_INT_WAKEUP (1<<3)
#define USART1_INT_WAKEUP (1<<4)
#define USART2_INT_WAKEUP (1<<5)
#define I2C1_INT_WAKEUP   (1<<7)
#define I2C0_INT_WAKEUP   (1<<8)
#define CAPT_INT_WAKEUP   (1<<11)
#define WWDT_INT_WAKE     (1<<12)
#define BOD_INT_WAKE      (1<<13)
#define WKT_INT_WAKEUP    (1<<15)
#define I2C2_INT_WAKEUP   (1<<21)
#define I2C3_INT_WAKEUP   (1<<22)
#define USART3_INT_WAKEUP (1<<30)
#define USART4_INT_WAKEUP (1<<31)

// PDAWAKECFG and PDRUNCFG register bits
#define FROOUT_PD         (1<<0)
#define FRO_PD            (1<<1)
#define FLASH_PD          (1<<2)
#define BOD_PD            (1<<3)
#define ADC_PD            (1<<4)
#define SYSOSC_PD         (1<<5)
#define WDTOSC_PD         (1<<6)
#define SYSPLL_PD         (1<<7)
#define VREF2_PD          (1<<10)
#define DAC0_PD           (1<<13)
#define DAC1_PD           (1<<14)
#define ACMP_PD           (1<<15)

// WDTOSCCTRL register bit field shifters
#define WDTOSC_DIVSEL  0
#define WDTOSC_FREQSEL 5

// SYSOSCCTRL register shifters
#define SYSOSC_BYPASS  0
#define SYSOSC_FREQSEL 1

// BODCTRL register bit field shifters
#define BODRSTLEV 0
#define BODINTVAL 2
#define BODRSTENA 4


// Below entries are for clock select mux functions
typedef enum {
  SYSPLL_AND_MAIN_CLKSEL_FRO_CLK,
  SYSPLL_AND_MAIN_CLKSEL_EXTERNAL_CLK,
  SYSPLL_AND_MAIN_CLKSEL_WDT_OSC_CLK,
  SYSPLL_AND_MAIN_CLKSEL_FRO_DIV_CLK
} SYSCON_SYSPLL_MAIN_CLKSEL_CLK_T;

typedef enum {
  FCLKSEL_FRO_CLK = 0,
  FCLKSEL_MAIN_CLK,
  FCLKSEL_FRG0CLK,
  FCLKSEL_FRG1CLK,
  FCLKSEL_FRO_DIV_CLK,
  FCLKSEL_OFF = 7
} SYSCON_FCLKSEL_CLK_T;

typedef enum {
  FCLK_TO_UART0 = 0,
  FCLK_TO_UART1,
  FCLK_TO_UART2,
  FCLK_TO_UART3,
  FCLK_TO_UART4,
  FCLK_TO_I2C0,
  FCLK_TO_I2C1,
  FCLK_TO_I2C2,
  FCLK_TO_I2C3,
  FCLK_TO_SPI0,
  FCLK_TO_SPI1
} SYSCON_FCLKSEL_T;

typedef enum {
  FRGCLKSEL_FRO_CLK = 0,
  FRGCLKSEL_MAIN_CLK,
  FRGCLKSEL_SYSPLL0_CLK,
  FRGCLKSEL_OFF
} SYSCON_FRGCLKSEL_T;

typedef enum {
  CLKOUTSEL_FRO_CLK = 0,
  CLKOUTSEL_MAIN_CLK,
  CLKOUTSEL_SYSPLL0_CLK,
  CLKOUTSEL_EXTERNAL_CLK,
  CLKOUTSEL_WDT_OSC_CLK,
  CLKOUTSEL_OFF = 7
} SYSCON_CLKOUTSEL_T;

typedef enum {
  CAPTCLKSEL_FRO_CLK = 0,
  CAPTCLKSEL_MAIN_CLK,
  CAPTCLKSEL_SYSPLL0_CLK,
  CAPTCLKSEL_FRO_DIV_CLK,
  CAPTCLKSEL_WDT_OSC_CLK,
  CAPTCLKSEL_OFF = 7
} SYSCON_CAPTCLKSEL_T;

typedef enum {
  SCTCLKSEL_FRO_CLK = 0,
  SCTCLKSEL_MAIN_CLK,
  SCTCLKSEL_SYSPLL0_CLK,
  SCTCLKSEL_OFF = 3
} SYSCON_SCTCLKSEL_T;

typedef enum {
  ADCCLKSEL_FRO_CLK = 0,
  ADCCLKSEL_SYSPLL0_CLK,
  ADCCLKSEL_OFF = 3
} SYSCON_ADCCLKSEL_T;











void Enable_Periph_Clock(unsigned int slot);
void Disable_Periph_Clock(unsigned int slot);
void Do_Periph_Reset(unsigned int slot);
void Config_Syspll(unsigned int which_clock, unsigned int pll_ctrl_value);
void Config_Fclksel_Mux(unsigned int to_which_periph, unsigned int which_clock);





#endif /* LPC8XX_SYSCON_H_ */
