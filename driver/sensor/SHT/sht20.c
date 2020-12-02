/************************************************
 * @file sht20.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-30
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "sht20.h"
#include "iic_conf.h"


static double RH = 0;
static double TEMP = 0;

static ErrorStatus sht_reset( void );

/************************************************
 * @brief reset sht
 * 
 * @return ErrorStatus 
 ************************************************/
static ErrorStatus sht_reset( void )
{
    ErrorStatus err = ERROR;

    err = i2c_generate_start( SHT20_I2C, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;
    err = i2c_send_7bitAddr( SHT20_I2C, I2C_Direction_Transmitter, SHT20_ADDR, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;    
    uint8_t reg = 0xFE;
    err = i2c_send_bytes( SHT20_I2C, &reg, 1, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;  
    i2c_generate_stop( SHT20_I2C );
    
    return err;
}

/************************************************
 * @brief sht init
 ************************************************/
void sht_init( void )
{
    static uint8_t flag = 0;
    if ( !flag ) {
        i2c_conf( SHT20_I2C, 400, 0x94 );
        flag = 1;
    }
    
    sht_reset();
    uint8_t dat = 0x3;
    sht_writ_user_reg( &dat );
    dat = 0;
    sht_read_user_reg( &dat );
    DEBUG_PRINT( "ureg: %x\n", dat );

    sht_read_RH_TEMP( 0 );
    sht_read_RH_TEMP( 1 );

}

/************************************************
 * @brief 读sht的user寄存器
 * 
 * @param dat 
 * @return ErrorStatus 
 ************************************************/
ErrorStatus sht_read_user_reg( uint8_t *dat )
{
    ErrorStatus err = ERROR;

    err = i2c_generate_start( SHT20_I2C, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;
    err = i2c_send_7bitAddr( SHT20_I2C, I2C_Direction_Transmitter, SHT20_ADDR, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;    
    uint8_t reg = 0xE7;
    err = i2c_send_bytes( SHT20_I2C, &reg, 1, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;  

    err = i2c_generate_start( SHT20_I2C, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;
    err = i2c_send_7bitAddr( SHT20_I2C, I2C_Direction_Receiver, SHT20_ADDR, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;    
    err = i2c_read_bytes( SHT20_I2C, dat, 1, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;
    
    I2C_AcknowledgeConfig(SHT20_I2C, DISABLE);
	i2c_generate_stop( SHT20_I2C );
    
    return err;
}

/************************************************
 * @brief 写user寄存器
 * 
 * @param dat 
 * @return ErrorStatus 
 ************************************************/
ErrorStatus sht_writ_user_reg( uint8_t *dat )
{
    ErrorStatus err = ERROR;

    err = i2c_generate_start( SHT20_I2C, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;
    err = i2c_send_7bitAddr( SHT20_I2C, I2C_Direction_Transmitter, SHT20_ADDR, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;    
    uint8_t reg = 0xE6;
    err = i2c_send_bytes( SHT20_I2C, &reg, 1, 0xFFFF );
    if ( err != SUCCESS ) 
        return err; 
    err = i2c_send_bytes( SHT20_I2C, dat, 1, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;
    i2c_generate_stop( SHT20_I2C );
    
    return err;
}


/************************************************
 * @brief 读温湿度
 * 
 * @param which 0:RH  1:TEMP
 * @return double 
 ************************************************/
double sht_read_RH_TEMP( uint8_t which )
{
    ErrorStatus err = ERROR;
    uint8_t buf[3] = {0};
    uint8_t cmd = 0;
    uint8_t delay_time = 0;

    if ( !which ) {
        cmd = 0xF5;
        delay_time = 25;
    } else if ( which ) {
        cmd = 0xF3;
        delay_time = 80;
    }

    

    err = i2c_generate_start( SHT20_I2C, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;
    err = i2c_send_7bitAddr( SHT20_I2C, I2C_Direction_Transmitter, SHT20_ADDR, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;    
    err = i2c_send_bytes( SHT20_I2C, &cmd, 1, 0xFFFF );
    if ( err != SUCCESS ) 
        return err; 

    for ( uint16_t i = 0; i < 3400; i++ );
    i2c_generate_stop( SHT20_I2C );

    DELAY( delay_time );

    err = i2c_generate_start( SHT20_I2C, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;
    err = i2c_send_7bitAddr( SHT20_I2C, I2C_Direction_Receiver, SHT20_ADDR, 0xFFFF );
    if ( err != SUCCESS ) 
        return err;

    err = i2c_check_event( SHT20_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED, 0xFFFF );
    if ( err != SUCCESS ) {
        printf( "e 1\n" );
    } else {
        buf[0] = (uint8_t)SHT20_I2C->DR;
        I2C_AcknowledgeConfig( SHT20_I2C, ENABLE );
        buf[1] = (uint8_t)SHT20_I2C->DR;
        I2C_AcknowledgeConfig( SHT20_I2C, ENABLE );
        buf[2] = (uint8_t)SHT20_I2C->DR;
        I2C_AcknowledgeConfig( SHT20_I2C, DISABLE );
    }

	i2c_generate_stop( SHT20_I2C );

    uint16_t res_bin = 0;
    res_bin = buf[0]<<6;
    res_bin |= buf[1]>>2;

    if ( !which ) {
        RH = 125*(res_bin<<2)/(1<<16) - 6;
        printf( "RH : %0.2f %%\n", RH );
        return RH;
    } else if ( which ) {
        TEMP = 175.72*(res_bin<<2)/(1<<16) - 46.85;
        printf( "TEMP : %0.2f ℃\n", TEMP );
        return TEMP;
    }
    
    return 0xFF;
}
