/************************************************
 * @file adc.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-11
 * 
 * @copyright Copyright (c) 2020
 * 
*************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

extern ADC_HandleTypeDef hadc1;


uint16_t adc_get(uint8_t avg);

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

