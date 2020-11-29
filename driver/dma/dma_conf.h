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

void dma_mem2mem( DMA_TypeDef *DMAx, DMA_Stream_TypeDef *stream, uint32_t ch, uint32_t prio,uint8_t *src, uint8_t *dst, uint32_t size );
void dma_on( uint8_t dma_num, DMA_Stream_TypeDef *dma_stream, uint8_t* mem_buf, uint32_t bl );
void dma_off( DMA_Stream_TypeDef *dma_stream );

void dma_test(void);

#ifdef __cplusplus
}
#endif
		
#endif

