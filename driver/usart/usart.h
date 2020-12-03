/************************************************
 * @file usart.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-04
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef USART_H
#define USART_H

/* includes */
#include "stm32f4xx_conf.h"
#include "arm_math.h"
#include <stdio.h>
#include "systick.h"

/* definations */
#define USART1_PIN_RX   GPIO_Pin_10
#define USART1_PIN_TX   GPIO_Pin_9

#ifdef __GNUC__
    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
    #define GETCHAR_PROTOTYPE int fgetc( FILE *f )
#endif /* __GNUC__ */

/* functions */
void usart_conf( USART_TypeDef* USARTx, u32 baud_rate );
void rt_usart_conf( USART_TypeDef* USARTx, u32 baud_rate );
void my_putc( uint8_t ch );
uint8_t my_getc( void );


#endif
