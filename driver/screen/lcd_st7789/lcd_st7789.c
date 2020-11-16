#include "lcd_st7789.h"

static void lcd_st7789_gpio( void );
static void lcd_st7789_send_cmd( u8 cmd );
static void lcd_st7789_send_dat( u8 dat );
static void lcd_st7789_trans_byte( u8 byte );
static void lcd_st7789_send( u8 byte, ST7789_DC_OPT opt );

static void lcd_st7789_gpio( void ) 
{
#if (USER_USE_SCR == USE_SCR_LCD_ST7789_130)
    GPIO_InitTypeDef lcd_st7789_g;
    SPI_InitTypeDef  lcd_st7789_s;
#if IPS_ST7789_USE_SPI == 1
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE );
#elif IPS_ST7789_USE_SPI == 2
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI2, ENABLE );
#elif IPS_ST7789_USE_SPI == 3
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE );
#endif
    
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
#if IPS_ST7789_USE_SPI != 1 && (USER_USE_SCR == USE_SCR_LCD_ST7789_130)
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
#endif    
    
    GPIO_PinAFConfig( IPS_ST7789_SPI_GPIO, IPS_ST7789_SCLK_SRC, IPS_ST7789_SPI_AF );
    GPIO_PinAFConfig( IPS_ST7789_SPI_GPIO, IPS_ST7789_MOSI_SRC, IPS_ST7789_SPI_AF );
    
    lcd_st7789_g.GPIO_Mode     = GPIO_Mode_AF;
    lcd_st7789_g.GPIO_Pin      = IPS_ST7789_MOSI | IPS_ST7789_SCLK;
    lcd_st7789_g.GPIO_OType    = GPIO_OType_PP;
    lcd_st7789_g.GPIO_PuPd     = GPIO_PuPd_UP;
    lcd_st7789_g.GPIO_Speed    = GPIO_Speed_100MHz;
    GPIO_Init( IPS_ST7789_SPI_GPIO, &lcd_st7789_g );
    
    lcd_st7789_g.GPIO_Mode     = GPIO_Mode_OUT;
    lcd_st7789_g.GPIO_Pin      = ST7789_DC | ST7789_RST | LCD_LEDK;
    GPIO_Init( GPIOA, &lcd_st7789_g );
    
    lcd_st7789_s.SPI_CPHA                  = SPI_CPHA_2Edge;
    lcd_st7789_s.SPI_CPOL                  = SPI_CPOL_High;
    lcd_st7789_s.SPI_Mode                  = SPI_Mode_Master;
    lcd_st7789_s.SPI_DataSize              = SPI_DataSize_8b;
    lcd_st7789_s.SPI_FirstBit              = SPI_FirstBit_MSB;
    lcd_st7789_s.SPI_Direction             = SPI_Direction_1Line_Tx;
    lcd_st7789_s.SPI_BaudRatePrescaler     = SPI_BaudRatePrescaler_2;
    lcd_st7789_s.SPI_NSS                   = SPI_NSS_Soft;
    SPI_Init( IPS_ST7789_SPI, &lcd_st7789_s );
    SPI_Cmd( IPS_ST7789_SPI, ENABLE );
    
#elif (USER_USE_SCR == USE_SCR_LCD_ST7789_200)

    GPIO_InitTypeDef lcd_st7789_mcu_g;
    
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
    
    lcd_st7789_mcu_g.GPIO_Mode  = GPIO_Mode_OUT;
    lcd_st7789_mcu_g.GPIO_Pin   = ST7789_CS | ST7789_DC | ST7789_WR | ST7789_RD | LCD_LEDK;
    lcd_st7789_mcu_g.GPIO_Speed = GPIO_Speed_100MHz;
    lcd_st7789_mcu_g.GPIO_OType = GPIO_OType_PP;
    lcd_st7789_mcu_g.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init( ST7789_CTL_GPIO, &lcd_st7789_mcu_g );
    
    lcd_st7789_mcu_g.GPIO_Mode  = GPIO_Mode_OUT;
    lcd_st7789_mcu_g.GPIO_Pin   = LCD_ST7789_DAT_PIN;
    lcd_st7789_mcu_g.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init( ST7789_DAT_GPIO, &lcd_st7789_mcu_g );
    
    lcd_st7789_mcu_g.GPIO_Mode  = GPIO_Mode_OUT;
    lcd_st7789_mcu_g.GPIO_Pin   = ST7789_RST;
    lcd_st7789_mcu_g.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init( GPIOB, &lcd_st7789_mcu_g );

#endif

}

void lcd_st7789_init( void ) {
    
    lcd_st7789_gpio();

    lcd_st7789_hw_reset();

    lcd_st7789_send_cmd(0x36);
    
#if USE_HORIZONTAL == 0
    lcd_st7789_send_dat(0x00);
#elif USE_HORIZONTAL == 1
    lcd_st7789_send_dat(0xC0);
#elif USE_HORIZONTAL == 2
    lcd_st7789_send_dat(0xC0);
#elif USE_HORIZONTAL ==3 
    lcd_st7789_send_dat(0xA0);
#endif
    
    lcd_st7789_send_cmd(0x3A); 
    lcd_st7789_send_dat(0x05);
    
    lcd_st7789_send_cmd(0xB2);
    lcd_st7789_send_dat(0x0C);
    lcd_st7789_send_dat(0x0C);
    lcd_st7789_send_dat(0x00);
    lcd_st7789_send_dat(0x33);
    lcd_st7789_send_dat(0x33); 
    
    lcd_st7789_send_cmd(0xB7); 
    lcd_st7789_send_dat(0x35);  
    
    lcd_st7789_send_cmd(0xBB);
    lcd_st7789_send_dat(0x19);
    
    lcd_st7789_send_cmd(0xC0);
    lcd_st7789_send_dat(0x2C);
    
    lcd_st7789_send_cmd(0xC2);
    lcd_st7789_send_dat(0x01);
    
    lcd_st7789_send_cmd(0xC3);
    lcd_st7789_send_dat(0x12);   
    
    lcd_st7789_send_cmd(0xC4);
    lcd_st7789_send_dat(0x20);  
    
    lcd_st7789_send_cmd(0xC6); 
    lcd_st7789_send_dat(0x0F);    
    
    lcd_st7789_send_cmd(0xD0); 
    lcd_st7789_send_dat(0xA4);
    lcd_st7789_send_dat(0xA1);
    
    lcd_st7789_send_cmd(0xE0);
    lcd_st7789_send_dat(0xD0);
    lcd_st7789_send_dat(0x04);
    lcd_st7789_send_dat(0x0D);
    lcd_st7789_send_dat(0x11);
    lcd_st7789_send_dat(0x13);
    lcd_st7789_send_dat(0x2B);
    lcd_st7789_send_dat(0x3F);
    lcd_st7789_send_dat(0x54);
    lcd_st7789_send_dat(0x4C);
    lcd_st7789_send_dat(0x18);
    lcd_st7789_send_dat(0x0D);
    lcd_st7789_send_dat(0x0B);
    lcd_st7789_send_dat(0x1F);
    lcd_st7789_send_dat(0x23);
    
    lcd_st7789_send_cmd(0xE1);
    lcd_st7789_send_dat(0xD0);
    lcd_st7789_send_dat(0x04);
    lcd_st7789_send_dat(0x0C);
    lcd_st7789_send_dat(0x11);
    lcd_st7789_send_dat(0x13);
    lcd_st7789_send_dat(0x2C);
    lcd_st7789_send_dat(0x3F);
    lcd_st7789_send_dat(0x44);
    lcd_st7789_send_dat(0x51);
    lcd_st7789_send_dat(0x2F);
    lcd_st7789_send_dat(0x1F);
    lcd_st7789_send_dat(0x1F);
    lcd_st7789_send_dat(0x20);
    lcd_st7789_send_dat(0x23);
    
    lcd_st7789_send_cmd(0x21); 
    
    lcd_st7789_displayOn();
    
    lcd_st7789_clear_with( 0x07f0 );
    
}


static void lcd_st7789_trans_byte( u8 byte )
{
#if (USER_USE_SCR == USE_SCR_LCD_ST7789_130)
    IPS_ST7789_SPI->DR = byte;
    while ( !( IPS_ST7789_SPI->SR & SPI_I2S_FLAG_TXE ) );
#elif (USER_USE_SCR == USE_SCR_LCD_ST7789_200)
    u16 dat = ((byte<<8)&0xFF00) | (GPIOB->IDR&0xFF);
    ST7789_DAT_GPIO->ODR = dat;
#endif
}

static void lcd_st7789_send_cmd( u8 cmd )
{
    
#if (USER_USE_SCR == USE_SCR_LCD_ST7789_130)
    ST7789_DC_LOW;
#elif (USER_USE_SCR == USE_SCR_LCD_ST7789_200)
    ST7789_CS_HIGH;
    ST7789_DC_LOW;
    ST7789_WR_LOW;
    ST7789_RD_HIGH;
    ST7789_CS_LOW;
#endif 
    
    lcd_st7789_trans_byte( cmd );
    
#if (USER_USE_SCR == USE_SCR_LCD_ST7789_200)
    ST7789_WR_HIGH;
#endif
}

static void lcd_st7789_send_dat( u8 dat )
{
#if (USER_USE_SCR == USE_SCR_LCD_ST7789_130)
    ST7789_DC_HIGH;
#elif (USER_USE_SCR == USE_SCR_LCD_ST7789_200)
    ST7789_CS_HIGH;
    ST7789_DC_HIGH;
    ST7789_WR_LOW;
    ST7789_RD_HIGH;
    ST7789_CS_LOW;
#endif 
    
    lcd_st7789_trans_byte( dat );
    
#if (USER_USE_SCR == USE_SCR_LCD_ST7789_200)
    ST7789_WR_HIGH;
#endif
}

static void lcd_st7789_send( u8 byte, ST7789_DC_OPT opt )
{
    if ( opt == ST7789_CMD )
        lcd_st7789_send_cmd( byte );
    else if ( opt == ST7789_DAT )
        lcd_st7789_send_dat( byte );
    else
        printf( "send opt error.\n" );
}

void lcd_st7789_set_region( u16 xs, u16 ys, u16 xe, u16 ye )
{
    lcd_st7789_send_cmd( 0x2a );
    lcd_st7789_send_pixel_dat( xs );
    lcd_st7789_send_pixel_dat( xe );
	
    lcd_st7789_send_cmd( 0x2b );
    lcd_st7789_send_pixel_dat( ys );
    lcd_st7789_send_pixel_dat( ye );
	
	lcd_st7789_send_cmd( 0x2c );
}

void lcd_st7789_draw_point( u16 x, u16 y, u16 color )
{
    lcd_st7789_set_region( x, y, x, y );
    lcd_st7789_send_pixel_dat( color );
}

void lcd_st7789_send_pixel_dat( u16 color )
{
    lcd_st7789_send_dat( color >> 8 );
    lcd_st7789_send_dat( color&0xFF );
}

void lcd_st7789_clear_with( u16 color )
{
    u16 x, y;
    lcd_st7789_set_region( 0, 0, 240-1, 320-1 );
    for ( y = 0; y < 320; y++ ) {
        for ( x = 0; x < 240; x++ ) {
            lcd_st7789_send_pixel_dat( color );
        }
    }
}


void lcd_st7789_sw_reset( void )
{
    lcd_st7789_send_cmd( 0x01 );
    DELAY(200);
}

void lcd_st7789_hw_reset( void )
{
    ST7789_RST_HIGH;
    DELAY(100);
    ST7789_RST_LOW;
    DELAY(100);
    ST7789_RST_HIGH;
    DELAY(100);
}

void lcd_st7789_displayOn( void )
{
    lcd_st7789_powerUp();
    ST7789_BLK_HIGH;
}

void lcd_st7789_displayOff( void )
{
    ST7789_BLK_LOW;
    lcd_st7789_powerDown();
}

void lcd_st7789_powerDown( void )
{
    lcd_st7789_send_cmd(0x10); 
    lcd_st7789_send_cmd(0x28);
}

void lcd_st7789_powerUp( void )
{
    lcd_st7789_send_cmd(0x11); 
    lcd_st7789_send_cmd(0x29);
}
