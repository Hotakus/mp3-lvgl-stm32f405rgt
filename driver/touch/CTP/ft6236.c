#include "iic_conf.h"
#include "ft6236.h"
#include "pro_conf.h"
#include "nvic_conf.h"

/************************************************
 * @brief FUNCTION PROPOTYPE
 ************************************************/
static void ctp_ft6236_gpio(void);
static void ctp_ft6236_reset(void);

/************************************************
 * @brief FUNCTION REALIZED
 ************************************************/
static void ctp_ft6236_gpio(void) 
{
    GPIO_InitTypeDef ctp_g;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
    
    ctp_g.GPIO_Mode     = GPIO_Mode_OUT;
    ctp_g.GPIO_Pin      = FT6236_RST;
    ctp_g.GPIO_OType    = GPIO_OType_PP;
    ctp_g.GPIO_PuPd     = GPIO_PuPd_UP;
    ctp_g.GPIO_Speed    = GPIO_Speed_100MHz;
    GPIO_Init( GPIOA, &ctp_g );
    
//    ctp_g.GPIO_Mode     = GPIO_Mode_IN;
//    ctp_g.GPIO_Pin      = FT6236_INT;
//    ctp_g.GPIO_PuPd     = GPIO_PuPd_UP;
//    ctp_g.GPIO_Speed    = GPIO_Speed_100MHz;
//    GPIO_Init( GPIOA, &ctp_g );
}

/************************************************
 * @brief ft6236 initialized function
 ************************************************/
void ctp_ft6236_init( void )
{
    ctp_ft6236_gpio();
    ctp_ft6236_reset();
    i2c_conf( I2C1, 400, 0x94 );
    
    uint8_t val = 0x0;

    val = 0;
    ctp_ft6236_writ_reg( FT_DEVIDE_MODE, &val, 1 );
    val = 22;
    ctp_ft6236_writ_reg( FT_ID_G_THGROUP, &val, 1 );
    val = 12;
    ctp_ft6236_writ_reg( FT_ID_G_PERIODACTIVE, &val, 1 );
    val = 0;
    ctp_ft6236_writ_reg( FT_ID_G_MODE, &val, 1 );

    exti_conf( EXTI_Line1, EXTI_Trigger_Falling, ENABLE );
    nvic_conf( EXTI1_IRQn, 1, 2, ENABLE );
}

/************************************************
 * @brief read x bytes from register indicated
 * 
 * @param reg_addr 
 * @param val 
 * @param len 
 ************************************************/
void ctp_ft6236_read_reg( uint8_t reg_addr, uint8_t *val, u32 len )
{
    ErrorStatus err = ERROR;

    // 开始
    err = i2c_generate_start( FT6236_I2C, 0xFFFF );
    if ( err != SUCCESS )
        return;
    // 发送写命令
    i2c_send_7bitAddr( FT6236_I2C, I2C_Direction_Transmitter, (FT6236_ADDR | FT6236_WRIT), 0xFFFF );
    if ( err != SUCCESS )
        return;
    // 发送要读的寄存器地址
    i2c_send_bytes( FT6236_I2C, &reg_addr, 1, 0xFFFF );
    if ( err != SUCCESS )
        return;
    i2c_generate_stop( FT6236_I2C );
    
    /*重新发送起始信号 */
    i2c_generate_start( FT6236_I2C, 0xFFF );
    if ( err != SUCCESS )
        return;
    // 发送地址+读
    i2c_send_7bitAddr( FT6236_I2C, I2C_Direction_Receiver, FT6236_ADDR, 0xFFFF );
    if ( err != SUCCESS )
        return;
    i2c_read_bytes( FT6236_I2C, val, 1, 0xFFF );
    if ( err != SUCCESS )
        return;
	/*非应答*/
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	i2c_generate_stop( FT6236_I2C );
    
}

/************************************************
 * @brief write x bytes to register indicated
 * 
 * @param reg_addr 
 * @param val 
 * @param len 
 ************************************************/
void ctp_ft6236_writ_reg( uint8_t reg_addr, uint8_t *val, u32 len )
{
    ErrorStatus err = ERROR;

    printf( "write 0x%02x to reg 0x%02x\n", *val, reg_addr );
    
    // 开始
    err = i2c_generate_start( FT6236_I2C, 0xFFF );
    if ( err != SUCCESS )
        return;
    // 发送写命令
    i2c_send_7bitAddr( FT6236_I2C, I2C_Direction_Transmitter, (FT6236_ADDR | FT6236_WRIT), 0xFFF );
    if ( err != SUCCESS )
        return;
    
    i2c_send_bytes( FT6236_I2C, &reg_addr, 1, 0xFFF );
    if ( err != SUCCESS )
        return;
    
    i2c_send_bytes( FT6236_I2C, val, len, 0xFFF );
    if ( err != SUCCESS )
        return;
    
    i2c_generate_stop( FT6236_I2C );
}

/************************************************
 * @brief ft6236 reset
 ************************************************/
static void ctp_ft6236_reset(void)
{
    FT6236_RST_LOW;
    DELAY( 100 );
    FT6236_RST_HIGH;
    DELAY( 100 );
}
