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
    /* With GCC/RAISONANCE, small DEBUG_PRINT (option LD Linker->Libraries->Small DEBUG_PRINT
     set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
    #define GETCHAR_PROTOTYPE int fgetc( FILE *f )
#endif /* __GNUC__ */

/* functions */
void usart_conf( USART_TypeDef* USARTx, u32 baud_rate );
void my_putc( u8 ch );
u8 my_getc( void );
void usart_dma_tx( u8 *buf );

#define DEBUG_PRINT     printf

#endif
