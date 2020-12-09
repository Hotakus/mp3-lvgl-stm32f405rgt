#ifndef W25QXX_H
#define W25QXX_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "sys_conf.h"
#include <stdio.h>
#include "uart.h"



/* definations */
#define W25QXX_SPI          SPI3
#define W25QXX_CS           GPIO_PIN_15
#define W25QXX_CS_PORT      GPIOA
#define W25QXX_CS_CLR       W25QXX_CS_PORT->BSRR = ((uint32_t)W25QXX_CS)<<16
#define W25QXX_CS_SET       W25QXX_CS_PORT->BSRR = ((uint32_t)W25QXX_CS)

/* w25qxx instructions */
typedef enum {

    /* 数据操作相关 */
    W25QXX_CMD_WRIT_DISABLE     = 0x04,     // 写失能
    W25QXX_CMD_READ_STATUS1           ,     // 读状态寄存器1
    W25QXX_CMD_WRIT_ENABLE            ,     // 写使能
    
    W25QXX_CMD_PAGE_PROG        = 0x02,     // 页编程
    W25QXX_CMD_READ_DATA        = 0x03,     // 普通读
    W25QXX_CMD_FAST_READ        = 0x0B,     // 快速读
    
    W25QXX_CMD_ERASE_SECT       = 0x20,     // 扇区擦除
    W25QXX_CMD_ERASE_CHIP       = 0xC7,     // 全片擦除
    
    W25QXX_CMD_POWERUP          = 0xAB,     // 上电
    W25QXX_CMD_JEDECID          = 0x9F,     // 读JEDECID
} w25qxx_instructions;


#define W25Q16      0xEF4015
#define W25Q32      0xEF4016
#define W25Q64      0xEF4017
#define W25Q128     0xEF4018
#define W25Q256     0xEF4019
typedef struct {
    uint32_t     page_cnt;        // pages
    uint32_t     sect_cnt;        // sectors
    uint32_t     capacity;        // bytes
    uint32_t     JEDECID;
} w25qxx_feature_s;

extern w25qxx_feature_s w25qxx;

/* functions */
HAL_StatusTypeDef w25qxx_init( void );
void w25qxx_powerUp( void );
void w25qxx_powerDown( void );
void w25qxx_get_JEDECID( void );

HAL_StatusTypeDef w25qxx_erase_sector( uint32_t ssect, uint32_t sect_num );
HAL_StatusTypeDef w25qxx_erase_chip( void );

HAL_StatusTypeDef w25qxx_read_page( uint8_t* rec_buf , uint32_t page, uint32_t cnt );
HAL_StatusTypeDef w25qxx_writ_page( uint8_t* send_buf, uint32_t page, uint32_t cnt );

HAL_StatusTypeDef w25qxx_read_sector( uint8_t* rec_buf, uint32_t sector, uint32_t cnt );
HAL_StatusTypeDef w25qxx_writ_sector( uint8_t* send_buf, uint32_t sector, uint32_t cnt );

#ifdef __cplusplus
}
#endif
		
#endif

