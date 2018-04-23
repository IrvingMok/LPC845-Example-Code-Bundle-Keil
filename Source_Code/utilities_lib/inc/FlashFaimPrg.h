/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 2003 - 2007                         */
/***********************************************************************/
/*                                                                     */
/*  FlashOS.H:  Data structures and entries                            */
/*              for Flash Programming Functions                        */
/*                                                                     */
/***********************************************************************/
#include <stdint.h>             // standard types definitions

// Specify Flash type here
#define FLASH_TYPE    FLASH_64

#if FLASH_TYPE == FLASH_64
#define SECTOR_SIZE     (0x400)	// 1K sector size.
#define END_SECTOR      (63)
#define MAX_FLASH_SIZE  (0x10000)
#define PAGE_SIZE       (0x40)
#define MIRROR_OFFSET   (0x04000000)
#define SECTOR_NUM      (64)    // Max Number of Sector Items
#endif

#define VERS            (1)     // Interface Version 1.01

#define UNKNOWN         (0)     // Unknown
#define ONCHIP          (1)     // On-chip Flash Memory


struct FlashSectors  {
  uint32_t        szSector;     // Sector Size in Bytes
  uint32_t      AddrSector;     // Address of Sector
};

#define SECTOR_END 0xFFFFFFFF, 0xFFFFFFFF

struct FlashDevice  {
   uint16_t           Vers;     // Version Number and Architecture
   char       DevName[128];     // Device Name and Description
   uint16_t        DevType;     // Device Type: ONCHIP, EXT8BIT, EXT16BIT, ...
   uint32_t         DevAdr;     // Default Device Start Address
   uint32_t          szDev;     // Total Size of Device
   uint32_t         szPage;     // Programming Page Size
   uint32_t            Res;     // Reserved for future Extension
   uint8_t        valEmpty;     // Content of Erased Memory

   uint32_t         toProg;     // Time Out of Program Page Function
   uint32_t        toErase;     // Time Out of Erase Sector Function

   struct FlashSectors sectors[SECTOR_NUM];
};

#define FLASH_DRV_VERS (0x0100+VERS)   // Driver Version, do not modify!

// Specify FAIM parameters here
//#define FAIM_PAGE_SIZE	(8)          // A bad name, since a FAIM page (or row) contains 32 bits
#define NUM_FAIM_WORDS  8              // A better name
#define FAIM_ADDRESS    (0x50010000)


int EraseChip(void);
int EraseSector(uint32_t adr);
int ErasePage(uint32_t adr);
int ErasePages(uint32_t adr0, uint32_t adr1);
int EraseAllPages(void);
int BlankCheckChip(void);
int BlankCheckSector(uint32_t adr);
int Program(uint32_t adr, uint32_t sz, uint8_t *buf);
int Compare(uint32_t adr0, uint32_t adr1, uint32_t sz);
int FAIMRead(uint32_t adr0, uint32_t adr1);
int FAIMWrite(uint32_t adr0, uint32_t adr1);
