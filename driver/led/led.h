#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"

/* definations */

/* function */
void led_conf( GPIO_TypeDef *GPIOx, u32 pin );
void led_on( GPIO_TypeDef *GPIOx, u32 led_io );
void led_off( GPIO_TypeDef *GPIOx, u32 led_io );

#ifdef __cplusplus
}
#endif

#endif
