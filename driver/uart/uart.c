/************************************************
 * @file uart.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-04
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "uart.h"
#include "sys_conf.h"
#include "stm32f4xx_hal.h"

static void uart_gpio( void );

static UART_HandleTypeDef h_uart1;

static void uart_gpio( void ) {
    GPIO_InitTypeDef uio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    uio.Alternate   = GPIO_AF7_USART1;
    uio.Pin         = USART1_PIN_RX | USART1_PIN_TX;
    uio.Mode        = GPIO_MODE_AF_PP;
    uio.Pull        = GPIO_NOPULL;
    uio.Speed       = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init( USART1_PORT, &uio );
    
}

void uart1_conf( uint32_t baud_rate ) {

    uart_gpio();

    __HAL_RCC_USART1_CLK_ENABLE();

    h_uart1.Instance            = USART1;
    h_uart1.Init.BaudRate       = baud_rate;
    h_uart1.Init.WordLength     = UART_WORDLENGTH_8B;
    h_uart1.Init.StopBits       = UART_STOPBITS_1;
    h_uart1.Init.Parity         = UART_PARITY_NONE;
    h_uart1.Init.Mode           = UART_MODE_TX_RX;
    h_uart1.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
    h_uart1.Init.OverSampling   = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&h_uart1) != HAL_OK) {
        Error_Handler();
    }

}

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

