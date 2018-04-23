/*
 * lpc8xx_dma.h
 *
 *  Created on: This day, August 23, 2016
 *      Author: The Creator
 */

#ifndef LPC8XX_DMA_H_
#define LPC8XX_DMA_H_


// DMA channel descriptor (must be 16 byte aligned)
typedef struct {
  uint32_t  notused;
  uint32_t  source;   // DMA transfer source end address 
  uint32_t  dest;     // DMA transfer destination end address
  uint32_t  next;     // Link to next DMA descriptor 
} DMA_CHDESC_T;

// DMA reload descriptor (must be 16 byte aligned) 
typedef struct {
  uint32_t  xfercfg;  // DMA transfer configuration 
  uint32_t  source;   // DMA transfer source end address 
  uint32_t  dest;     // DMA transfer destination end address
  uint32_t  next;     // Link to next DMA descriptor 
} DMA_RELOADDESC_T;

// CTRL register
#define DMA_CTRL_ENABLE 0

// INSTAT register
#define DMA_INTSTAT_ACTIVEINT    1
#define DMA_INTSTAT_ACTIVEERRINT 2

// SRAMBASE register
#define DMA_SRAMBASE_OFFSET 9

// CFGn registers
#define DMA_CFG_PERIPHREQEN   0 
#define DMA_CFG_HWTRIGEN      1 
#define DMA_CFG_TRIGPOL       4
#define DMA_CFG_TRIGTYPE      5
#define DMA_CFG_TRIGBURST     6
#define DMA_CFG_BURSTPOWER    8
#define DMA_CFG_SRCBURSTWRAP 14
#define DMA_CFG_DSTBURSTWRAP 15
#define DMA_CFG_CHPRIORITY   16

// CTLSTATn registers
#define DMA_CTLSTAT_VALIDPENDING 0
#define DMA_CTLSTAT_TRIG         2

// XFERCFGn registers
#define DMA_XFERCFG_CFGVALID  0
#define DMA_XFERCFG_RELOAD    1
#define DMA_XFERCFG_SWTRIG    2
#define DMA_XFERCFG_CLRTRIG   3
#define DMA_XFERCFG_SETINTA   4
#define DMA_XFERCFG_SETINTB   5
#define DMA_XFERCFG_WIDTH     8
#define DMA_XFERCFG_SRCINC    12
#define DMA_XFERCFG_DSTINC    14
#define DMA_XFERCFG_XFERCOUNT 16



#endif /* LPC8XX_DMA_H_ */

