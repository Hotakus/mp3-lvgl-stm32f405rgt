/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "pro_conf.h"
#include "w25qxx.h"
#include "uart.h"
#include "sdio.h"
#include "ff.h"
#include "diskio.h"		/* Declarations of disk functions */
#include "ff_devices.h"


/* Definitions of physical drive number for each drive */
#define DEV_SPIF        0	/* Example: Map SPI FLASH to physical drive 0 */
#define DEV_SD_SDIO	    1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_SD_SPI	    2	/* Example: Map MMC/SD card to physical drive2 */
#define DEV_USB		      3	/* Example: Map USB MSD to physical drive 3 */

HAL_SD_CardInfoTypeDef card_info = {
  .CardType = 0xFF,
};

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
  BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
  switch (pdrv) {
  case DEV_SPIF:
    return !STA_NOINIT;
  case DEV_SD_SDIO:
    return !STA_NOINIT;
  case DEV_USB:
    return !STA_NOINIT;
  }
  return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
  BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{

  HAL_StatusTypeDef state = HAL_ERROR;
  uint16_t retry = 5;

  state = fs_dev[pdrv]->init();
  if (state != HAL_OK)
    return state;

  switch (pdrv) {
  case DEV_SPIF:
    break;
  case DEV_SD_SDIO:
  /* Get Card info */
    if (card_info.CardType == 0xFF) {
      do {
        state = HAL_SD_GetCardInfo(&hsd, &card_info);
        if (state != HAL_OK) {
          printf("SD get info retry %d. (%d)\n", retry, state);
        }
      } while (--retry && (state != HAL_OK));
    }
    break;
  case DEV_USB:
    return STA_NOINIT;
  }

  return state;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
  BYTE pdrv,		/* Physical drive nmuber to identify the drive */
  BYTE* buff,		/* Data buffer to store read data */
  LBA_t sector,	/* Start sector in LBA */
  UINT count		/* Number of sectors to read */
)
{
  uint16_t retry = 0xFFFF;
  HAL_StatusTypeDef state;

  state = fs_dev[pdrv]->read(buff, sector, count);
  if (state != HAL_OK) {
    DEBUG_PRINT("dev : %d read error (%d)\n", pdrv, state);
    return RES_ERROR;
  }

  return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
  BYTE pdrv,			/* Physical drive nmuber to identify the drive */
  const BYTE* buff,	/* Data to be written */
  LBA_t sector,		/* Start sector in LBA */
  UINT count			/* Number of sectors to write */
)
{
  uint16_t retry = 0xFFFF;
  HAL_StatusTypeDef state;

  state = fs_dev[pdrv]->write((uint8_t*)buff, sector, count);
  if (state != HAL_OK) {
    DEBUG_PRINT("dev : %d write error (%d)\n", pdrv, state);
    return RES_ERROR;
  }

  return RES_OK;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
extern w25qxx_feature_s w25qxx;

DRESULT disk_ioctl(
  BYTE pdrv,		/* Physical drive nmuber (0..) */
  BYTE cmd,		/* Control code */
  void* buff		/* Buffer to send/receive control data */
)
{
  switch (pdrv) {
  case DEV_SPIF:

    switch (cmd) {
    case CTRL_SYNC:
      return RES_OK;
    case GET_SECTOR_COUNT:
      *(DWORD*)buff = w25qxx.sect_cnt;
      return RES_OK;
    case GET_SECTOR_SIZE:
      *(DWORD*)buff = 4096;
      return RES_OK;
    case GET_BLOCK_SIZE:
      *(DWORD*)buff = 1;
      return RES_OK;
    case CTRL_TRIM:
      return RES_OK;
    }

  case DEV_SD_SDIO:
    switch (cmd) {
    case CTRL_SYNC:
      return RES_OK;
    case GET_SECTOR_COUNT:
      *(DWORD*)buff = card_info.BlockNbr;
      return RES_OK;
    case GET_SECTOR_SIZE:
      *(DWORD*)buff = card_info.BlockSize;
      return RES_OK;
    case GET_BLOCK_SIZE:
      *(DWORD*)buff = 1;
      return RES_OK;
    case CTRL_TRIM:
      return RES_OK;
    }
  case DEV_USB:

    return RES_OK;
  }

  return RES_PARERR;
}

