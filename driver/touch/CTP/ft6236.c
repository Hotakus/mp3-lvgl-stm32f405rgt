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
 * @brief STATIC VARIABLE
 ************************************************/
static touch_coordinate_s touch_point[FT6236_MAX_TOUCH_NUM];

/************************************************
 * @brief GLOBAL VARIABLE
 ************************************************/
uint8_t ft6236_init_flag = 0;
uint8_t ft6236_error_times;

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
    uint8_t val = 0x0;
    
    ft6236_init_flag = 0;
    
    ctp_ft6236_gpio();
    ctp_ft6236_reset();
    i2c_conf( I2C1, 400, 0x94 );

    val = 0;
    ctp_ft6236_writ_reg( FT_DEVIDE_MODE, &val, 1 );
    val = 22;
    ctp_ft6236_writ_reg( FT_ID_G_THGROUP, &val, 1 );
    val = 12;
    ctp_ft6236_writ_reg( FT_ID_G_PERIODACTIVE, &val, 1 );
    val = 0;
    ctp_ft6236_writ_reg( FT_ID_G_MODE, &val, 1 );

    ft6236_init_flag = 1;

    
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
    i2c_generate_start( FT6236_I2C, 0xFFFF );
    if ( err != SUCCESS )
        return;
    // 发送地址+读
    i2c_send_7bitAddr( FT6236_I2C, I2C_Direction_Receiver, FT6236_ADDR, 0xFFFF );
    if ( err != SUCCESS )
        return;
    i2c_read_bytes( FT6236_I2C, val, 1, 0xFFFF );
    if ( err != SUCCESS )
        return;
	/*非应答*/
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	i2c_generate_stop( FT6236_I2C );

//    I2C_AcknowledgeConfig(I2C1, ENABLE);
//    /*①产生起始信号*/
//	I2C_GenerateSTART(I2C1, ENABLE);
//	/*检测EV5并清除标志*/
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//	/*②发送从机地址，写选通*/
//	I2C_Send7bitAddress(I2C1, FT6236_ADDR, I2C_Direction_Transmitter);
//	/*检测EV6并清除标志*/
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//	/*③发送要读取数据的地址*/
//	I2C_SendData(I2C1, reg_addr);
//	/*检测EV8并清除标志*/
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//	/*④重新发送起始信号 */
//	I2C_GenerateSTART(I2C1, ENABLE);
//	/*检测EV5并清除标志*/
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//	/*⑤发送从机设备地址，读选通*/
//	I2C_Send7bitAddress(I2C1, FT6236_ADDR, I2C_Direction_Receiver);
//	/*检测EV6并清除标志*/
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//	/*⑥检测EV7，然后读取数据清除标志*/
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); 
//	*val = I2C_ReceiveData(I2C1);
//	/*⑦非应答*/
//	I2C_AcknowledgeConfig(I2C1, DISABLE);
//        /*⑧产生停止信号*/
//	I2C_GenerateSTOP(I2C1, ENABLE);
    
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
    err = i2c_generate_start( FT6236_I2C, 0xFFFF );
    if ( err != SUCCESS )
        return;
    // 发送写命令
    i2c_send_7bitAddr( FT6236_I2C, I2C_Direction_Transmitter, (FT6236_ADDR | FT6236_WRIT), 0xFFFF );
    if ( err != SUCCESS )
        return;
    
    i2c_send_bytes( FT6236_I2C, &reg_addr, 1, 0xFFFF );
    if ( err != SUCCESS )
        return;
    
    i2c_send_bytes( FT6236_I2C, val, len, 0xFFFF );
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


void ctp_ft6236_scan( void )
{
    uint8_t val = 0;
    uint16_t x, y;
    ctp_ft6236_get_coordinate( &x, &y, FT_TP1 );

}

void ctp_ft6236_get_coordinate( uint16_t *x, uint16_t *y, uint8_t TPx )
{

    uint8_t xh, xl;
    uint8_t yh, yl;
    
    ctp_ft6236_read_reg( TPx  , &xh, 1 );  // 读触摸点状态和x高4位
    DELAY(1);
    ctp_ft6236_read_reg( TPx+1, &xl, 1 );  // 读x低8位
    DELAY(1);
    ctp_ft6236_read_reg( TPx+2, &yh, 1 );  // 读y高4位
    DELAY(1);
    ctp_ft6236_read_reg( TPx+3, &yl, 1 );  // 读y低8位

    
    *x  = (xh&0x0F)<<8;
    *x |= xl;
    
    *y  = (yh&0x0F)<<8;
    *y |= yl;

    // 判断是否误读
    if ( *x > FT6236_MAX_X )
        *x = FT6236_MAX_X;
    if ( *y > FT6236_MAX_X )
        *y = FT6236_MAX_Y;

    DEBUG_PRINT( "x : %03d y : %03d\n", *x, *y );
    
}

touch_gesture_t cp_ft6236_get_gesture( void )
{
    uint8_t gesture = 0;
    ctp_ft6236_read_reg( FT_GEST_ID, &gesture, 1 );
    return (touch_gesture_t)gesture;
}



