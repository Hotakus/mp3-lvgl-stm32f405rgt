#ifndef NVIC_CONF_H
#define NVIC_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>

/* definations */

/* functions */
void nvic_conf( uint8_t IRQx, u8 pre_prio, u8 sub_prio, FunctionalState stat );
void exti_conf( uint32_t EXTI_Linex, EXTITrigger_TypeDef trigger_method, FunctionalState stat );

#ifdef __cplusplus
}
#endif
		
#endif

