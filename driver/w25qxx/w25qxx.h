#ifndef W25QXX_H
#define W25QXX_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include "systick.h"
#include "usart.h"

/* definations */
#define W25QXX_USE_SPI      3
#if W25QXX_USE_SPI == 1
#define W25QXX_SPI     SPI1
#define W25QXX_SPI_CS         GPIO_Pin_4        // PA
#define W25QXX_SPI_SCLK       GPIO_Pin_5        // PA
#define W25QXX_SPI_MISO       GPIO_Pin_6        // PA
#define W25QXX_SPI_MOSI       GPIO_Pin_7        // PA


#elif W25QXX_USE_SPI == 2
#define W25QXX_SPI     SPI2
#define W25QXX_SPI_CS         GPIO_Pin_8        // PA
#define W25QXX_SPI_SCLK       GPIO_Pin_13       // PB
#define W25QXX_SPI_MISO       GPIO_Pin_14       // PB
#define W25QXX_SPI_MOSI       GPIO_Pin_15       // PB

#elif W25QXX_USE_SPI == 3
#define W25QXX_SPI     SPI1
#define W25QXX_SPI_CS         GPIO_Pin_15        // PA
#define W25QXX_SPI_SCLK       GPIO_Pin_3       // PB
#define W25QXX_SPI_MISO       GPIO_Pin_4       // PB
#define W25QXX_SPI_MOSI       GPIO_Pin_5       // PB
#endif

#define W25QXX_CS_CLR     GPIOA->BSRRH |= W25QXX_SPI_CS
#define W25QXX_CS_SET     GPIOA->BSRRL |= W25QXX_SPI_CS

/* w25qxx instructions */
enum w25qxx_instructions {

    /* 数据操作相关 */
    W25QXX_CMD_WRIT_DISABLE     = 0x04,
    W25QXX_CMD_READ_STATUS1           ,
    W25QXX_CMD_WRIT_ENABLE            ,
    
    W25QXX_CMD_PAGE_PROG        = 0x02,
    W25QXX_CMD_READ_DATA        = 0x03,
    W25QXX_CMD_FAST_READ        = 0x0B,
    
    W25QXX_CMD_ERASE_SECT       = 0x20,
    W25QXX_CMD_ERASE_CHIP       = 0xC7,
    
    W25QXX_CMD_POWERUP          = 0xAB,
    W25QXX_CMD_JEDECID          = 0x9F,
    W25QXX_DUMMY_BYTE           = 0xFF,
};


/* w25qxx opt stat */
enum w25qxx_opt_stat {
    W25QXX_OPT_OK       = 0,
    W25QXX_OPT_FAILED      ,
    W25QXX_TIMEOUT         ,
};

#define W25Q16      0x4015
#define W25Q32      0x4016
#define W25Q64      0x4017
#define W25Q128     0x4018
#define W25Q256     0x4019
typedef struct w25qxx_feature {
    u16     pageSize;       // bytes
    u32     pageCnt;        // pages
    u16     sect_size;       // bytes
    u32     sect_cnt;        // sectors
    u32     capacity;      // bytes
    u32     JEDECID;    
    u16     mType;
} w25qxx_feature;

/* functions */
void w25qxx_gpio( void );
void w25qxx_init( void );
void w25qxx_powerUp( void );
void w25qxx_JEDECID( void );

u8 w25qxx_wait_busy( void );

u8 w25qxx_erase_sector( u32 ssect, u32 sectLen );
u8 w25qxx_erase_chip( void );

u8 w25qxx_read_page( u8* pRecBuf , u32 page, u32 cnt );
u8 w25qxx_writ_page( u8* pSendBuf, u32 page, u32 cnt );

u8 w25qxx_read_sector( u8* pBuf, u32 sector, u32 cnt );
u8 w25qxx_writ_sector( u8* pBuf, u32 sector, u32 cnt );

#ifdef __cplusplus
}
#endif
		
#endif

