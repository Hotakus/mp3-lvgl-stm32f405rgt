#ifndef IIC_CONF_H
#define IIC_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>

/* definations */
#define I2C_RETRY_TIMES     5  

#define I2C1_SCL    GPIO_Pin_6
#define I2C1_SDA    GPIO_Pin_7

/* functions */
void i2c_conf( I2C_TypeDef * I2Cx, u16 i2c_clk, uint8_t own_addr );
ErrorStatus i2c_check_event( I2C_TypeDef *I2Cx, uint32_t i2c_event, uint32_t timeout );
ErrorStatus i2c_generate_start( I2C_TypeDef *I2Cx, uint32_t timeout );
void i2c_generate_stop ( I2C_TypeDef *I2Cx );
ErrorStatus i2c_send_7bitAddr( I2C_TypeDef *I2Cx, uint32_t dir, uint8_t addr, uint32_t timeout );
ErrorStatus i2c_send_bytes( I2C_TypeDef *I2Cx, uint8_t *byte, uint32_t len, uint32_t timeout );
ErrorStatus i2c_read_bytes( I2C_TypeDef *I2Cx, uint8_t *byte, uint32_t len, uint32_t timeout );


#ifdef __cplusplus
}
#endif
		
#endif

