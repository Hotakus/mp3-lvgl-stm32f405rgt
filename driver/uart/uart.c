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

static UART_HandleTypeDef h_uart1;



void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  if (uartHandle->Instance == USART1) {

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if (uartHandle->Instance == USART1) {

    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

  }
}


void uart1_conf(uint32_t baud_rate)
{

  __HAL_RCC_USART1_CLK_ENABLE();

  h_uart1.Instance = USART1;
  h_uart1.Init.BaudRate = baud_rate;
  h_uart1.Init.WordLength = UART_WORDLENGTH_8B;
  h_uart1.Init.StopBits = UART_STOPBITS_1;
  h_uart1.Init.Parity = UART_PARITY_NONE;
  h_uart1.Init.Mode = UART_MODE_TX_RX;
  h_uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  h_uart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&h_uart1) != HAL_OK) {
    Error_Handler();
  }


}

void my_putc(uint8_t ch)
{
  while (!(USART1->SR & USART_FLAG_TXE));
  USART1->DR = ch;
  // CDC_Transmit_FS( &ch, 1 );
}

uint8_t my_getc(void)
{
  while (!(USART1->SR & USART_FLAG_RXNE));
  return USART1->DR;
}

PUTCHAR_PROTOTYPE
{
  if (ch == '\n')
    my_putc('\r');
  my_putc((uint8_t)ch);
  return ch;
}

GETCHAR_PROTOTYPE
{
  return my_getc();
}

