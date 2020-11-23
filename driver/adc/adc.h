/************************************************
 * @file adc.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-23
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef ADC_H
#define ADC_H

/* includes */
#include "stm32f4xx_conf.h"
#include "arm_math.h"
#include <stdio.h>

/* definations */

/* functions */
void adc_init( ADC_TypeDef *ADCx );
uint16_t adc_get_value( ADC_TypeDef *ADCx, uint8_t adc_channel );

#endif
