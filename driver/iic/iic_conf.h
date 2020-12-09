/************************************************
 * @file iic_conf.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-09
 * 
 * @copyright Copyright (c) 2020
 * 
*************************************************/
#ifndef IIC_CONF_H
#define IIC_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "pro_conf.h"

/* definations */
#define I2C_RETRY_TIMES     0xFF
#define I2C_TIMEOUT         0xFFFFFFFF  

#define I2C1_SCL    GPIO_PIN_6
#define I2C1_SDA    GPIO_PIN_7

#define I2C2_SCL    GPIO_PIN_10
#define I2C2_SDA    GPIO_PIN_11

/* functions */

void i2c_conf(I2C_HandleTypeDef* h_i2c, uint16_t i2c_clk, uint8_t own_addr);

HAL_StatusTypeDef i2c_send( I2C_HandleTypeDef* h_i2c, uint16_t dev_addr, uint8_t *send_buf, size_t size );
HAL_StatusTypeDef i2c_read( I2C_HandleTypeDef* h_i2c, uint16_t dev_addr, uint8_t *rec_buf, size_t size );

#ifdef __cplusplus
}
#endif
		
#endif

