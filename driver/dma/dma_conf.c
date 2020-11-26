#include "dma_conf.h"
#include <string.h>

DMA_InitTypeDef dma_s[2];

void dma_conf( uint8_t dma_num, u32 periph_addr, u32 ch, u32 dir, u32 prio ) {
    
    dma_s[dma_num].DMA_Channel               = ch;
    dma_s[dma_num].DMA_DIR                   = dir;
    dma_s[dma_num].DMA_Priority              = prio;
    dma_s[dma_num].DMA_PeripheralBaseAddr    = periph_addr;
    dma_s[dma_num].DMA_Mode                  = DMA_Mode_Normal;
    dma_s[dma_num].DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
    dma_s[dma_num].DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    dma_s[dma_num].DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte; 
    dma_s[dma_num].DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
    dma_s[dma_num].DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    dma_s[dma_num].DMA_MemoryInc             = DMA_MemoryInc_Enable;  
    dma_s[dma_num].DMA_FIFOMode              = DMA_FIFOMode_Disable;
    dma_s[dma_num].DMA_FIFOThreshold         = DMA_FIFOThreshold_Full;
}


void dma_on( uint8_t dma_num, DMA_Stream_TypeDef *dma_stream, uint8_t* mem_buf, u32 bl ) {
    u32 bl_t = 0;
    if ( bl == 0 ) {
        bl_t = strlen( (const char*)mem_buf );
    }
    
    while( DMA_GetCmdStatus( dma_stream ) == ENABLE );
    
    dma_s[dma_num].DMA_Memory0BaseAddr       = (u32)mem_buf;
    dma_s[dma_num].DMA_BufferSize            = (bl==0)?(bl_t):bl;
    DMA_Init( dma_stream, &dma_s[dma_num] );

    DMA_Cmd( dma_stream, ENABLE );
}

void dma_off( DMA_Stream_TypeDef *dma_stream ) {
    DMA_Cmd( dma_stream, DISABLE );
    DMA_DeInit( dma_stream );
}


void spi1_dma_trans( uint8_t *pSendBuf, uint8_t *pRecBuf, u32 bl ) {

    DMA_DeInit( DMA2_Stream3 );
    DMA_DeInit( DMA2_Stream0 );
    
    dma_conf( 0, (u32)&(SPI1->DR), DMA_Channel_3, DMA_DIR_MemoryToPeripheral, DMA_Priority_VeryHigh );
    dma_conf( 1, (u32)&(SPI1->DR), DMA_Channel_3, DMA_DIR_PeripheralToMemory, DMA_Priority_VeryHigh );
    
    dma_s[0].DMA_Memory0BaseAddr       = (u32)pSendBuf;
    dma_s[0].DMA_BufferSize            = bl;
    
    dma_s[1].DMA_Memory0BaseAddr       = (u32)pRecBuf;
    dma_s[1].DMA_BufferSize            = bl;
    
    DMA_Init( DMA2_Stream3, &dma_s[0] );
    DMA_Init( DMA2_Stream0, &dma_s[1] );
    
    DMA_Cmd( DMA2_Stream3, ENABLE );
    DMA_Cmd( DMA2_Stream0, ENABLE );
    
    while( DMA_GetCmdStatus( DMA2_Stream3 ) == ENABLE );
    while( DMA_GetCmdStatus( DMA2_Stream0 ) == ENABLE );
    
}

