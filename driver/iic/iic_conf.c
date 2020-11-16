#include "iic_conf.h"
#include "usart.h"

static I2C_InitTypeDef  i2c_s;
static GPIO_InitTypeDef i2cg_s;
static u16 i2c_clk_m = 1000;

void i2c_conf( I2C_TypeDef * I2Cx, u16 i2c_clk, u8 own_addr ) {
    
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


