/************************************************
 * @file iic_conf.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-18
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/

#include "iic_conf.h"
#include "usart.h"
#include "pro_conf.h"

static I2C_InitTypeDef  i2c_s;
static GPIO_InitTypeDef i2cg_s;
static u16 i2c_clk_m = 1000;

/************************************************
 * @brief I2C configure
 * 
 * @param I2Cx 
 * @param i2c_clk 
 * @param own_addr 
 ************************************************/
void i2c_conf( I2C_TypeDef * I2Cx, u16 i2c_clk, uint8_t own_addr ) {
    
    if ( i2c_clk > 400 ) {
        DEBUG_PRINT( "i2c clk error\n" );
        return;
    }
    
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
    
    GPIO_PinAFConfig( GPIOB, GPIO_PinSource6, GPIO_AF_I2C1 );
    GPIO_PinAFConfig( GPIOB, GPIO_PinSource7, GPIO_AF_I2C1 );
    
    i2c_s.I2C_Mode                  = I2C_Mode_I2C;
    i2c_s.I2C_ClockSpeed            = i2c_clk_m * i2c_clk;
    i2c_s.I2C_Ack                   = I2C_Ack_Enable;
    i2c_s.I2C_DutyCycle             = I2C_DutyCycle_2;
    i2c_s.I2C_AcknowledgedAddress   = I2C_AcknowledgedAddress_7bit;
    i2c_s.I2C_OwnAddress1           = own_addr;
    I2C_AcknowledgeConfig( I2Cx, ENABLE );
    I2C_Init( I2Cx, &i2c_s );
    I2C_Cmd( I2Cx, ENABLE );
    I2C1->CR1 |= 0x80;              // 解除限速
    
    i2cg_s.GPIO_Mode            = GPIO_Mode_AF;
    i2cg_s.GPIO_Pin             = I2C1_SCL | I2C1_SDA;
    i2cg_s.GPIO_OType           = GPIO_OType_OD;
    i2cg_s.GPIO_PuPd            = GPIO_PuPd_UP;
    i2cg_s.GPIO_Speed           = GPIO_Speed_100MHz;
    GPIO_Init( GPIOB, &i2cg_s );
}

/************************************************
 * @brief check i2c event
 * 
 * @param I2Cx 
 * @param i2c_event 
 * @param timeout 
 * @return ErrorStatus 
 ************************************************/
ErrorStatus i2c_check_event( I2C_TypeDef *I2Cx, uint32_t i2c_event, uint32_t timeout )
{
    ErrorStatus err = ERROR;
    do {
        err = I2C_CheckEvent( I2Cx, i2c_event );
    } while ( --timeout && err != SUCCESS );
    return err;
}

/************************************************
 * @brief generate an i2c start condition
 * 
 * @param I2Cx 
 * @param timeout 
 * @return ErrorStatus 
 ************************************************/
ErrorStatus i2c_generate_start( I2C_TypeDef *I2Cx, uint32_t timeout )
{
    ErrorStatus err = ERROR;

    I2C_GenerateSTART( I2Cx, ENABLE );
    err = i2c_check_event( I2Cx, I2C_EVENT_MASTER_MODE_SELECT, timeout );
    if ( err != SUCCESS ) {
        DEBUG_PRINT( "I2C generate start condition error.\n" );
        i2c_generate_stop( I2Cx );
    }
    return err;
}

/************************************************
 * @brief generate an i2c stop condition
 * 
 * @param I2Cx 
 ************************************************/
void i2c_generate_stop ( I2C_TypeDef *I2Cx )
{
    I2C_GenerateSTOP( I2Cx, ENABLE );
}

/************************************************
 * @brief send a 7bit address
 * 
 * @param I2Cx 
 * @param addr 
 * @param timeout 
 * @return ErrorStatus 
 ************************************************/
ErrorStatus i2c_send_7bitAddr( I2C_TypeDef *I2Cx, uint32_t dir, uint8_t addr, uint32_t timeout )
{
    ErrorStatus err = ERROR;
    uint8_t retry = I2C_RETRY_TIMES;

    I2C_Send7bitAddress( I2Cx, addr, dir );
    if ( dir == I2C_Direction_Transmitter ) {
        err = i2c_check_event( I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, timeout );
    } else {
        err = i2c_check_event( I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED, timeout );
    }
    if ( err != SUCCESS ) {
        DEBUG_PRINT( "I2C send 7bit address error. (0x%02x)\n", addr );
        i2c_generate_stop( I2Cx );
        return err;
    }
    return err;
}

/************************************************
 * @brief i2c send byte
 * 
 * @param I2Cx 
 * @param byte 
 * @param timeout 
 * @return ErrorStatus 
 ************************************************/
ErrorStatus i2c_send_bytes( I2C_TypeDef *I2Cx, uint8_t *byte, uint32_t len, uint32_t timeout )
{
    ErrorStatus err = ERROR;
    uint8_t *pd = byte;

    if ( !len )
        return ERROR;

    do {
        I2Cx->DR = *pd;
        err = i2c_check_event( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING, timeout );
        if ( err != SUCCESS ) {
            i2c_generate_stop( I2Cx );
            return err;
        } else if ( err == SUCCESS && len != 1 )
            pd += 1;
    } while ( --len );

    return err;
}

/************************************************
 * @brief i2c receive n bytes
 * 
 * @param I2Cx 
 * @param byte 
 * @param len 
 * @param timeout 
 * @return ErrorStatus 
 ************************************************/
ErrorStatus i2c_read_bytes( I2C_TypeDef *I2Cx, uint8_t *byte, uint32_t len, uint32_t timeout )
{
    ErrorStatus err = ERROR;
    uint8_t retry = I2C_RETRY_TIMES;
    uint8_t *pd = byte;
    
    if ( !len )
        return ERROR;

    do {
        err = i2c_check_event( I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED, timeout );
        if ( err != SUCCESS ) {
            i2c_generate_stop( I2Cx );
            *byte = 0xFF;
            return err;
        } else {
            *byte = (uint8_t)I2Cx->DR;
            pd += 1;
        }
    } while ( --len );
    
    return err;
}
