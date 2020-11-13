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
u8 cur_char;

void USART1_IRQHandler( void ) {
#if USER_USE_UCOS == 1
    OSIntEnter();
#endif
    if ( USART_GetITStatus( USART1, USART_IT_RXNE ) ) {
        cur_char = my_getc();
        //my_putc( cur_char );
        USART_ClearITPendingBit( USART1, USART_IT_RXNE );
    }
    if(USART_GetFlagStatus( USART1, USART_FLAG_ORE ) != RESET)
    {
        USART_ClearFlag( USART1, USART_FLAG_ORE );
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
#if USER_USE_RTTHREAD == 1
    rt_interrupt_enter();
#endif 
    SD_ProcessIRQSrc();  
#if USER_USE_RTTHREAD == 1
  rt_interrupt_leave();
#endif 
}   
void SD_SDIO_DMA_IRQHANDLER(void)
{
#if USER_USE_RTTHREAD == 1
    rt_interrupt_enter();
#endif 
    SD_ProcessDMAIRQ();  
#if USER_USE_RTTHREAD == 1
  rt_interrupt_leave();
#endif 
} 
