#ifndef IIC_CONF_H
#define IIC_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>

/* definations */
#define I2C1_SCL    GPIO_Pin_6
#define I2C1_SDA    GPIO_Pin_7

/* functions */
void i2c_conf( I2C_TypeDef * I2Cx, u16 i2c_clk, u8 own_addr );
ErrorStatus i2c_check_event( I2C_TypeDef *I2Cx, uint32_t i2c_event, uint32_t timeout );

#ifdef __cplusplus
}
#endif
		
#endif

