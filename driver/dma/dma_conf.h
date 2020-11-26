#ifndef DMA_CONF_H
#define DMA_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>

/* definations */

/* functions */
void dma_conf( uint8_t dma_num, u32 periph_addr, u32 ch, u32 dir, u32 prio );
void dma_on( uint8_t dma_num, DMA_Stream_TypeDef *dma_stream, uint8_t* mem_buf, u32 bl );
void dma_off( DMA_Stream_TypeDef *dma_stream );

void spi1_dma_trans( uint8_t *pSendBuf, uint8_t *pRecBuf, u32 bl );

#ifdef __cplusplus
}
#endif
		
#endif

