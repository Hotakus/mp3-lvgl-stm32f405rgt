#ifndef SPI_CONF_H
#define SPI_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>

/* definations */
#define SPI_DUMMY_BYTE  0xFF

typedef enum { 
    SPI_MODE_1  = 0,        // Mode 0: CPOL=0, CPHA=0 
    SPI_MODE_2     ,        // Mode 1: CPOL=0, CPHA=1
    SPI_MODE_3     ,        // Mode 2: CPOL=1, CPHA=0
    SPI_MODE_4     ,        // Mode 3: CPOL=1, CPHA=1
} SPI_MODE;

#define SPI1_SCLK    GPIO_Pin_5      // PA
#define SPI1_MISO    GPIO_Pin_6      // PA
#define SPI1_MOSI    GPIO_Pin_7      // PA

#define SPI2_SCLK    GPIO_Pin_13      // PB
#define SPI2_MISO    GPIO_Pin_14      // PB
#define SPI2_MOSI    GPIO_Pin_15      // PB

#define SPI3_SCLK    GPIO_Pin_3      // PB
#define SPI3_MISO    GPIO_Pin_4      // PB
#define SPI3_MOSI    GPIO_Pin_5      // PB


/* functions */
void spi_conf( SPI_TypeDef* SPIx, u16 speed, SPI_MODE mode, u32 SPI_DIR );
uint8_t spi_full_duplex_trans_byte( SPI_TypeDef *SPIx, uint8_t byte );

#ifdef __cplusplus
}
#endif
		
#endif

