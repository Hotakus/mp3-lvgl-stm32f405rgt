#include "usart.h"
#include "nvic_conf.h"
#include "dma_conf.h"
#include <rtthread.h>

static void usart_gpio( void ) {
    GPIO_InitTypeDef uio;
    
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource9,  GPIO_AF_USART1 );
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource10, GPIO_AF_USART1 );
    
    uio.GPIO_Pin        = USART1_PIN_TX | USART1_PIN_RX;
    uio.GPIO_Mode       = GPIO_Mode_AF;
    uio.GPIO_OType      = GPIO_OType_OD;
    uio.GPIO_PuPd       = GPIO_PuPd_UP;
    uio.GPIO_Speed      = GPIO_Speed_100MHz;
    GPIO_Init( GPIOA, &uio );
    
}

void usart_conf( USART_TypeDef* USARTx, u32 baud_rate ) {
    USART_InitTypeDef us_s;
    
    usart_gpio();

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
    
    us_s.USART_Mode                 = USART_Mode_Tx | USART_Mode_Rx;
    us_s.USART_BaudRate             = baud_rate;
    us_s.USART_WordLength           = USART_WordLength_8b;
    us_s.USART_Parity               = USART_Parity_No;
    us_s.USART_StopBits             = USART_StopBits_1;
    us_s.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    USART_Init( USARTx, &us_s );
    nvic_conf( USART1_IRQn, 0, 0, ENABLE );
    USART_ITConfig( USARTx, USART_IT_RXNE, ENABLE );
    USART_Cmd( USARTx, ENABLE );
}

void rt_usart_conf( USART_TypeDef* USARTx, u32 baud_rate )
{
    USART_InitTypeDef us_s;
    
    usart_gpio();

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
    
    us_s.USART_Mode                 = USART_Mode_Tx | USART_Mode_Rx;
    us_s.USART_BaudRate             = baud_rate;
    us_s.USART_WordLength           = USART_WordLength_8b;
    us_s.USART_Parity               = USART_Parity_No;
    us_s.USART_StopBits             = USART_StopBits_1;
    us_s.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    USART_Init( USARTx, &us_s );
    USART_Cmd( USARTx, ENABLE );
}
INIT_BOARD_EXPORT(rt_usart_conf);

void my_putc( uint8_t ch ) 
{
    while ( !( USART1->SR & USART_FLAG_TC ) );
    USART1->DR = ch;
}

uint8_t my_getc( void ) 
{
    while ( !( USART1->SR & USART_FLAG_RXNE ) );
    return USART1->DR;
}

PUTCHAR_PROTOTYPE 
{
    if ( ch == '\n' ) {
        my_putc( '\r' );
    }
    my_putc( (uint8_t)ch );
    return ch;
}

GETCHAR_PROTOTYPE 
{
    return my_getc();
}

