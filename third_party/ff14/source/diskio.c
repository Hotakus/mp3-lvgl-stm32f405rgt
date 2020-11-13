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
#include "usart.h"

#include "ff.h"
#include "diskio.h"		/* Declarations of disk functions */


/* Definitions of physical drive number for each drive */
#define DEV_SPIF        0	/* Example: Map SPI FLASH to physical drive 0 */
#define DEV_SD_SDIO	    1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_SD_SPI	    2	/* Example: Map MMC/SD card to physical drive2 */
#define DEV_USB		    3	/* Example: Map USB MSD to physical drive 3 */

#if USER_USE_SD_NUM == 1
static SD_CardInfo card_info = {
    .CardType = 0xff,
};
#elif USER_USE_SD_NUM == 2
SD_CardInfo card_info;
#endif

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	switch (pdrv) {
	case DEV_SPIF :
		return !STA_NOINIT;
	case DEV_SD_SDIO :
		return !STA_NOINIT;
	case DEV_USB :
		// translate the reslut code here
		return !STA_NOINIT;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    
    SD_Error sd_err;
    u8 retry = 5;
    
	switch (pdrv) {
	case DEV_SPIF :
		w25qxx_init();
        return !STA_NOINIT;
	case DEV_SD_SDIO :
        /* initialize card */
        do {
            sd_err = sd_sdio_init();
            if ( sd_err != SD_OK ) {
                printf( "SD init retry %d. (%d)\n", retry, sd_err );
            }
        } while ( --retry && (sd_err != SD_OK) );
        if ( sd_err != SD_OK )
            return STA_NOINIT;
        printf( "SD init successfully.\n" );
        /* Get Card info */
        if ( card_info.CardType == 0xff ) {
            do {
                sd_err = SD_GetCardInfo( &card_info );
                if ( sd_err != SD_OK ) {
                    printf( "SD get info retry %d. (%d)\n", retry, sd_err );
                }
            } while ( --retry && (sd_err != SD_OK) );
            if ( sd_err != SD_OK ) 
                return STA_NOINIT;
            printf( "SD get info successfully.\n" );
        }
        SD_HighSpeed();
        return !STA_NOINIT;
	case DEV_USB :
		return STA_NOINIT;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	int res;
    u8 retry = 5;
    SD_Error sd_err;

	switch (pdrv) {
	case DEV_SPIF :
		res = w25qxx_read_sector( buff, sector, count );
        if ( res ) return RES_PARERR;
		return RES_OK;

	case DEV_SD_SDIO :
    
        if ( count == 1 )
            sd_err = SD_ReadBlock( buff, (sector<<9), card_info.CardBlockSize );
        else {
            do {
                sd_err = SD_ReadMultiBlocks(
                    buff, \
                    (sector<<9), \
                    card_info.CardBlockSize, \
                    count \
                );
                sd_err = SD_WaitReadOperation();
            } while( --retry && sd_err != SD_OK );
            while(SD_GetStatus() != SD_TRANSFER_OK);
            if ( sd_err != SD_OK ) {
                printf( "SD read error (%d)\n\r", sd_err );
                return RES_PARERR;
            }
        }
        return RES_OK;
	case DEV_USB :
		// translate the reslut code here
		return RES_PARERR;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	int res;
    u8 retry = 5;
    SD_Error sd_err;
    
	switch (pdrv) {
	case DEV_SPIF :
        res = w25qxx_writ_sector( (u8*)buff, sector, count );
        if ( res ) 
            return RES_PARERR;
		return RES_OK;
	case DEV_SD_SDIO :
        if ( count == 1 )
            sd_err = SD_ReadBlock( (u8*)buff, (sector<<9), card_info.CardBlockSize );
        else {
            sd_err = SD_WriteMultiBlocks(
                (u8*)buff, \
                (sector<<9), \
                card_info.CardBlockSize, \
                count \
            );
            SD_WaitWriteOperation();
        }
        while(SD_GetStatus() != SD_TRANSFER_OK);
        if ( sd_err != SD_OK ) {
            printf( "SD writ retry %d. (%d)\n\r", retry, sd_err );
            return RES_PARERR;
        }
        return RES_OK;
	case DEV_USB :
		return RES_PARERR;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

extern w25qxx_feature w25qxx;

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	switch (pdrv) {
	case DEV_SPIF :
        switch( cmd ) {
            case CTRL_SYNC:
                return RES_OK;
            case GET_SECTOR_COUNT:
                *(DWORD*)buff = w25qxx.sect_cnt;
                return RES_OK;
            case GET_SECTOR_SIZE:
                *(DWORD*)buff = w25qxx.sect_size;
                return RES_OK;
            case GET_BLOCK_SIZE:
                *(DWORD*)buff = 1;
                return RES_OK;
            case CTRL_TRIM: 
                return RES_OK;
        }

	case DEV_SD_SDIO :
        switch( cmd ) {
            case CTRL_SYNC:
                return RES_OK;
            case GET_SECTOR_COUNT:
                *(DWORD*)buff = card_info.CardCapacity>>9;
                return RES_OK;
            case GET_SECTOR_SIZE:
                *(DWORD*)buff = card_info.CardBlockSize;
                return RES_OK;
            case GET_BLOCK_SIZE:
                *(DWORD*)buff = 1;
                return RES_OK;
            case CTRL_TRIM:
                return RES_OK;
        }
	case DEV_USB :

		return RES_OK;
	}

	return RES_PARERR;
}

