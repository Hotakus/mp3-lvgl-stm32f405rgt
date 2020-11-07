#include "isr_funcs.h"
#include "dma_conf.h"
#include "pro_conf.h"
#include "usart.h"
#if USER_USE_UCOS == 1
/* ucos */
#include "ucos_inc.h"
#endif

/*
*   USART1 RX ISR function
*/
#define USART_BUF_SIZE  256
u8 usart_rx_buf[USART_BUF_SIZE] = {0};
u16 urp = 0;

void USART1_IRQHandler( void ) {
    
#if USER_USE_UCOS == 1
    OSIntEnter();
#endif
    if ( USART_GetITStatus( USART1, USART_IT_RXNE ) ) {
        usart_rx_buf[urp] = my_getc();
        my_putc( usart_rx_buf[urp] );
        urp++;
        
        USART_ClearITPendingBit( USART1, USART_IT_RXNE );
    }
    
#if USER_USE_UCOS == 1
    OSIntExit();
#endif
    
}

/*
*   USART1 DMA TX ISR function
*/
void DMA2_Stream7_IRQHandler( void ) {
#if USER_USE_UCOS == 1
    OSIntEnter();
#endif
    if( DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7) ) {  
        DMA_ClearITPendingBit( DMA2_Stream7, DMA_IT_TCIF7 );  
        DMA_Cmd( DMA2_Stream7, DISABLE );
        //dma_off(DMA2_Stream7);
    }
#if USER_USE_UCOS == 1
    OSIntExit();
#endif
}


void TIM4_IRQHandler( void ) {
    if ( TIM_GetITStatus( TIM4, TIM_IT_Update ) != RESET ) {
        ticks_decrease();
        TIM_ClearITPendingBit( TIM4, TIM_IT_Update );
    }
}

void SDIO_IRQHandler(void)
{
    SD_ProcessIRQSrc();  
}   
void SD_SDIO_DMA_IRQHANDLER(void)
{
    SD_ProcessDMAIRQ();  
} 
