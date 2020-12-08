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

#include "ff.h"
#include "diskio.h"		/* Declarations of disk functions */


/* Definitions of physical drive number for each drive */
#define DEV_SPIF        0	/* Example: Map SPI FLASH to physical drive 0 */
#define DEV_SD_SDIO	    1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_SD_SPI	    2	/* Example: Map MMC/SD card to physical drive2 */
#define DEV_USB		    3	/* Example: Map USB MSD to physical drive 3 */

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
        // translate the reslut code here
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

    HAL_StatusTypeDef sd_state = HAL_ERROR;
    uint16_t retry = 5;

    switch (pdrv) {
    case DEV_SPIF:
        w25qxx_init();
        return !STA_NOINIT;
    case DEV_SD_SDIO:
        /* initialize card */
        do {
            sd_state = sd_sdio_init();
            if (sd_state != HAL_OK) {
                printf("SD init retry %d. (%d)\n", retry, sd_state);
            }
        } while (--retry && (sd_state != HAL_OK));
        if (sd_state != HAL_OK)
            return STA_NOINIT;

        /* Get Card info */
        if (card_info.CardType == 0xFF) {
            do {
                sd_state = HAL_SD_GetCardInfo(&hsd, &card_info);
                if (sd_state != HAL_OK) {
                    printf("SD get info retry %d. (%d)\n", retry, sd_state);
                }
            } while (--retry && (sd_state != HAL_OK));
            if (sd_state != HAL_OK)
                return STA_NOINIT;
        }
        return !STA_NOINIT;
    case DEV_USB:
        return STA_NOINIT;
    }
    return STA_NOINIT;
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
    HAL_StatusTypeDef sd_state;
    w25qxx_stat w25qxx_err;

    switch (pdrv) {
    case DEV_SPIF:
        w25qxx_err = w25qxx_read_sector(buff, sector, count);
        if (w25qxx_err != W25QXX_STAT_OK)
            return RES_ERROR;
        return RES_OK;
    case DEV_SD_SDIO:
        // do {
        //     sd_state = HAL_SD_ReadBlocks(&hsd, buff, sector, count, 0xFFFFFFFF);
        // } while (--retry && sd_state != HAL_OK);
        // if (sd_state != HAL_OK) {
        //     DEBUG_PRINT("SD read error (%d)\n", sd_state);
        //     return RES_ERROR;
        // }

        while (count--) {
            do {
                // sd_state = HAL_SD_ReadBlocks(&hsd, buff, sector, 1, 0xFFFFFFFF);
                sd_state = HAL_SD_ReadBlocks_IT(&hsd, buff, sector, 1);
            } while (--retry && sd_state != HAL_OK);
            if (sd_state != HAL_OK) {
                DEBUG_PRINT("SD read error (%d)\n", sd_state);
                return RES_ERROR;
            } else {
                while( HAL_SD_GetState(&hsd) != HAL_SD_STATE_READY );
                sector++;
            }
        }

        return RES_OK;
    case DEV_USB:
        // translate the reslut code here
        return RES_PARERR;
    }

    return RES_PARERR;
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
    HAL_StatusTypeDef sd_state;
    w25qxx_stat w25qxx_err;

    switch (pdrv) {
    case DEV_SPIF:
        w25qxx_err = w25qxx_writ_sector((uint8_t*)buff, sector, count);
        if (w25qxx_err != W25QXX_STAT_OK)
            return RES_ERROR;
        return RES_OK;
    case DEV_SD_SDIO:

        // do {
        //     sd_state = HAL_SD_WriteBlocks(&hsd, (uint8_t*)buff, sector, count, 0xFFFFFFFF);
        // } while (--retry && sd_state != HAL_OK);
        // if (sd_state != HAL_OK) {
        //     DEBUG_PRINT("SD write error (%d)\n", sd_state);
        //     return RES_ERROR;
        // }

        while (count--) {
            do {
                // sd_state = HAL_SD_WriteBlocks(&hsd, (uint8_t*)buff, sector, 1, 0xFFFFFFFF);
                sd_state = HAL_SD_WriteBlocks_IT(&hsd, (uint8_t*)buff, sector, 1);
            } while (--retry && sd_state != HAL_OK);
            if (sd_state != HAL_OK) {
                DEBUG_PRINT("SD write error (%d)\n", sd_state);
                return RES_ERROR;
            } else {
                while( HAL_SD_GetState(&hsd) != HAL_SD_STATE_READY );
                sector++;
            }

        }

        return RES_OK;
    case DEV_USB:
        return RES_PARERR;
    }

    return RES_PARERR;
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

