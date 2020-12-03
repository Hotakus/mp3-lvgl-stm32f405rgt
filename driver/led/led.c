/************************************************
 * @file led.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-04
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "led.h"

void led_conf( GPIO_TypeDef *GPIOx, u32 pin ) {
    GPIO_InitTypeDef io;
    
    if ( GPIOx == GPIOA ) {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    } else if ( GPIOx == GPIOB ) {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    } else if ( GPIOx == GPIOC ) {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    } else if ( GPIOx == GPIOD ) {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    } else if ( GPIOx == GPIOE ) {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    }
    
    io.GPIO_Pin     = pin;              
    io.GPIO_Mode    = GPIO_Mode_OUT;    
    io.GPIO_OType   = GPIO_OType_PP;    
    io.GPIO_Speed   = GPIO_Speed_100MHz;
    io.GPIO_PuPd    = GPIO_PuPd_UP;     
    GPIO_Init( GPIOx, &io );            
}

void led_on( GPIO_TypeDef *GPIOx, u32 led_io ) {
    GPIOx->BSRRH |= led_io;
}

void led_off( GPIO_TypeDef *GPIOx, u32 led_io ) {
    GPIOx->BSRRL |= led_io;
}
