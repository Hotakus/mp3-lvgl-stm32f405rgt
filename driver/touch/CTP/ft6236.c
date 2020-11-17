#include "iic_conf.h"
#include "ft6236.h"

/************************************************
 * @brief FUNCTION PROPOTYPE
 ************************************************/
static void ctp_ft6236_gpio(void);
static void ctp_ft6236_start(void);
static void ctp_ft6236_stop (void);
static void ctp_ft6236_stop (void);
static void ctp_ft6236_send_addr( u8 addr );
static void ctp_ft6236_read_reg( u8 reg_addr, u8 *val, u32 len );

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
    
    ctp_g.GPIO_Mode     = GPIO_Mode_IN;
    ctp_g.GPIO_Pin      = FT6236_INT;
    ctp_g.GPIO_PuPd     = GPIO_PuPd_UP;
    ctp_g.GPIO_Speed    = GPIO_Speed_100MHz;
    GPIO_Init( GPIOA, &ctp_g );
    
    
    
}

void ctp_ft6236_init( void )
{
    ctp_ft6236_gpio();
    i2c_conf( I2C1, 100, 0x94 );
    
    
    
}


static void ctp_ft6236_start(void)
{
    
}

static void ctp_ft6236_stop (void)
{
    
}

static void ctp_ft6236_send_addr( u8 addr )
{
    
}

static void ctp_ft6236_read_reg( u8 reg_addr, u8 *val, u32 len )
{
    
}




