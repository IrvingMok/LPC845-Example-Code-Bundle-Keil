/***********************************************************************/
/*  This file is part of the uVision/ARM development tools             */
/*  Copyright (c) 2003-2009 Keil Software. All rights reserved.        */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted for               */
/*               NXP LPC11xx/13xx/LPC17xx Flash using IAP              */
/*                                                                     */
/***********************************************************************/
#include "iap.h"
#include "FlashFaimPrg.h"                      // FlashOS Structures

#define SET_VALID_CODE		0           // Set Valid User Code Signature

static struct sIAP IAP;

/*
 * Get Sector Number
 *    Parameter:      adr:  Sector Address
 *    Return Value:   Sector Number
 */

uint32_t GetSecNum (uint32_t adr)
{
  uint32_t n;

  n = adr >> 10;                               // 1kB Sector (0x400 each)
  return (n);                                  // Sector Number
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  otherwise - Failed
 */

int EraseChip (void)
{

  IAP.cmd    = IAP_PREPARE;                    // Prepare Sector for Erase
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  IAP.cmd    = IAP_ERASE;                      // Erase Sector
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
//  IAP.par[2] = SystemCoreClock / 1000;         // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (uint32_t adr)
{
  uint32_t n;

  n = GetSecNum(adr);                          // Get Sector Number

  IAP.cmd    = IAP_PREPARE;                    // Prepare Sector for Erase
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  IAP.cmd    = IAP_ERASE;                      // Erase Sector
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
//  IAP.par[2] = SystemCoreClock / 1000;         // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}


/*
 *  Erase page in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int ErasePage (uint32_t adr)
{
  uint32_t n;

  n = GetSecNum(adr);                          // Get Sector Number

  IAP.cmd    = IAP_PREPARE;                    // Prepare Sector for Erase
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  n = adr >> 6;                                // Get Page Number, 64 bytes/page

  IAP.cmd    = IAP_ERASE_PAGE;                 // Erase Page
  IAP.par[0] = n;                              // Start page
  IAP.par[1] = n;                              // End page
//  IAP.par[2] = SystemCoreClock / 1000;         // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}


/*
 *  Erase pages in Flash Memory
 *    Parameter:      adr0, adr1:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int ErasePages (uint32_t adr0, uint32_t adr1)
{
  uint32_t n0, n1;

  n0 = GetSecNum(adr0);                        // Get Sector Number
  n1 = GetSecNum(adr1);                        // Get Sector Number

  IAP.cmd    = IAP_PREPARE;                    // Prepare Sector for Erase
  IAP.par[0] = n0;                             // Start Sector
  IAP.par[1] = n1;                             // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  n0 = adr0 >> 6;                              // Get Page Number, 64 bytes/page
  n1 = adr1 >> 6;                              // Get Page Number, 64 bytes/page

  IAP.cmd    = IAP_ERASE_PAGE;                 // Erase Page
  IAP.par[0] = n0;                             // Start page
  IAP.par[1] = n1;                             // End page
//  IAP.par[2] = SystemCoreClock / 1000;         // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}


/*
 *  Erase all pages in Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseAllPages(void)
{
  IAP.cmd    = IAP_PREPARE;                    // Prepare Sector for Erase
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed
  

  IAP.cmd    = IAP_ERASE_PAGE;                 // Erase Page
  IAP.par[0] = 0;                              // Start page
  IAP.par[1] = MAX_FLASH_SIZE/PAGE_SIZE-1;     // End page
//  IAP.par[2] = SystemCoreClock / 1000;         // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}


/*
 *  Blank Check Chip Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int BlankCheckChip (void)
{
  IAP.cmd    = IAP_BLANK_CHECK;                // Erase Sector
  IAP.par[0] = 0;                              // Start Sector
  IAP.par[1] = END_SECTOR;                     // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}


/*
 *  Blank Check Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int BlankCheckSector (uint32_t adr)
{
  uint32_t n;

  n = GetSecNum(adr);                          // Get Sector Number

  IAP.cmd    = IAP_BLANK_CHECK;                // Erase Sector
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}


/*
 *  Program Data in Flash Memory
 *    Parameter:      adr:  Destination Start Address
 *                    sz:   Data Size
 *                    buf:  Source Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int Program (uint32_t adr, uint32_t sz, uint8_t *buf)
{
  uint32_t n;

#if SET_VALID_CODE != 0                        // Set valid User Code Signature
  if (adr == 0) {                              // Check for Vector Table
    n = *((uint32_t *)(buf + 0x00)) +
        *((uint32_t *)(buf + 0x04)) +
        *((uint32_t *)(buf + 0x08)) +
        *((uint32_t *)(buf + 0x0C)) +
        *((uint32_t *)(buf + 0x10)) +
        *((uint32_t *)(buf + 0x14)) +
        *((uint32_t *)(buf + 0x18));
    *((uint32_t *)(buf + 0x1C)) = 0 - n;  // Signature at Reserved Vector
  }
#endif

  n = GetSecNum(adr);                          // Get Sector Number

  IAP.cmd    = IAP_PREPARE;                    // Prepare Sector for Write
  IAP.par[0] = n;                              // Start Sector
  IAP.par[1] = n;                              // End Sector
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  IAP.cmd    = IAP_COPY_RAM2FLASH;             // Copy RAM to Flash
  IAP.par[0] = adr;                            // Destination Flash Address
  IAP.par[1] = (uint32_t)buf;                  // Source RAM Address
  IAP.par[2] = sz;                             // Number of bytes
//  IAP.par[3] = SystemCoreClock / 1000;         // CCLK in kHz
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}


/*
 *  Compare Data in Flash Memory
 *    Parameter:      adr0:  Destination Address
 *                    adr1:  Source Address
 *                    sz:    Data Size
 *    Return Value:   0 - OK,  1 - Failed
 */

int Compare (uint32_t adr0, uint32_t adr1, uint32_t sz)
{
  IAP.cmd    = IAP_COMPARE;                    // Compare
  IAP.par[0] = adr0;                           // Destination Address
  IAP.par[1] = adr1;                           // Source Address
  IAP.par[2] = sz;                             // Number of bytes
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}

/*
 *  Read FAIM Memory
 *  Return Value:   0 - OK,  1 - Failed
 */

int FAIMRead (uint32_t adr0, uint32_t adr1)
{
  IAP.cmd    = IAP_READ_PAGE_FAIM;             // READ FAIM PAGE
  IAP.par[0] = adr0;                           // FAIM page number
  IAP.par[1] = adr1;                           // Destination RAM address to store the read value
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}

/*
 *  Write FAIM Memory
 *  Return Value:   0 - OK,  1 - Failed
 */

int FAIMWrite (uint32_t adr0, uint32_t adr1)
{
  IAP.cmd    = IAP_WRITE_PAGE_FAIM;            // WRITE FAIM PAGE
  IAP.par[0] = adr0;                           // FAIM page number
  IAP.par[1] = adr1;                           // Source RAM address to store the write value
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (IAP.stat);             // Command Failed

  return (0);                                  // Finished without Errors
}
