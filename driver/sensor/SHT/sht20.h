/************************************************
 * @file sht20.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-30
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef SHT20_H
#define SHT20_H

/* includes */
#include "stm32f4xx_hal.h"
#include "pro_conf.h"
#include <stdio.h>


/* definations */
#define SHT20_I2C   I2C1
#define SHT20_ADDR  0x80

/* functions */
void sht_init( void );
HAL_StatusTypeDef sht_read_user_reg( uint8_t *dat );
HAL_StatusTypeDef sht_writ_user_reg( uint8_t *dat );

double sht_read_RH_TEMP( uint8_t which );

#endif
