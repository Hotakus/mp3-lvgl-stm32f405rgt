#include "ips_st7789.h"


void ips_st7789_gpio( void ) {
    
    GPIO_InitTypeDef ips_st7789_g;
    SPI_InitTypeDef  ips_st7789_s;
    
#if IPS_ST7789_USE_SPI == 1
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE );
#elif IPS_ST7789_USE_SPI == 2
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI2, ENABLE );
#elif IPS_ST7789_USE_SPI == 3
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE );
#endif
    

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
#if IPS_ST7789_USE_SPI != 1
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
#endif    
    
    GPIO_PinAFConfig( IPS_ST7789_SPI_GPIO, IPS_ST7789_SCLK_SRC, IPS_ST7789_SPI_AF );
    GPIO_PinAFConfig( IPS_ST7789_SPI_GPIO, IPS_ST7789_MOSI_SRC, IPS_ST7789_SPI_AF );
    
    ips_st7789_g.GPIO_Mode     = GPIO_Mode_AF;
    ips_st7789_g.GPIO_Pin      = IPS_ST7789_MOSI | IPS_ST7789_SCLK;
    ips_st7789_g.GPIO_OType    = GPIO_OType_PP;
    ips_st7789_g.GPIO_PuPd     = GPIO_PuPd_UP;
    ips_st7789_g.GPIO_Speed    = GPIO_Speed_100MHz;
    GPIO_Init( IPS_ST7789_SPI_GPIO, &ips_st7789_g );
    
    ips_st7789_g.GPIO_Mode     = GPIO_Mode_OUT;
    ips_st7789_g.GPIO_Pin      = IPS_ST7789_DC | IPS_ST7789_RES | IPS_ST7789_BLK;
    GPIO_Init( GPIOA, &ips_st7789_g );
    
    ips_st7789_s.SPI_CPHA                  = SPI_CPHA_2Edge;
    ips_st7789_s.SPI_CPOL                  = SPI_CPOL_High;
    ips_st7789_s.SPI_Mode                  = SPI_Mode_Master;
    ips_st7789_s.SPI_DataSize              = SPI_DataSize_8b;
    ips_st7789_s.SPI_FirstBit              = SPI_FirstBit_MSB;
    ips_st7789_s.SPI_Direction             = SPI_Direction_2Lines_FullDuplex;
    ips_st7789_s.SPI_BaudRatePrescaler     = SPI_BaudRatePrescaler_2;
    ips_st7789_s.SPI_NSS                   = SPI_NSS_Soft;
    SPI_Init( IPS_ST7789_SPI, &ips_st7789_s );
    SPI_Cmd( IPS_ST7789_SPI, ENABLE );

}

void ips_st7789_init( void ) {
    
    ips_st7789_gpio();
//    DELAY(150);
//    ips_st7789_sw_reset();
//    DELAY(150);
    ips_st7789_hw_reset();
    

    ips_st7789_send_cmd(0x36);
    
#if USE_HORIZONTAL == 0
    ips_st7789_send_dat(0x00);
#elif USE_HORIZONTAL == 1
    ips_st7789_send_dat(0xC0);
#elif USE_HORIZONTAL == 2
    ips_st7789_send_dat(0xC0);
#elif USE_HORIZONTAL ==3 
    ips_st7789_send_dat(0xA0);
#endif
    
    ips_st7789_send_cmd(0x3A); 
    ips_st7789_send_dat(0x05);
    
    ips_st7789_send_cmd(0xB2);
    ips_st7789_send_dat(0x0C);
    ips_st7789_send_dat(0x0C);
    ips_st7789_send_dat(0x00);
    ips_st7789_send_dat(0x33);
    ips_st7789_send_dat(0x33); 
    
    ips_st7789_send_cmd(0xB7); 
    ips_st7789_send_dat(0x35);  
    
    ips_st7789_send_cmd(0xBB);
    ips_st7789_send_dat(0x19);
    
    ips_st7789_send_cmd(0xC0);
    ips_st7789_send_dat(0x2C);
    
    ips_st7789_send_cmd(0xC2);
    ips_st7789_send_dat(0x01);
    
    ips_st7789_send_cmd(0xC3);
    ips_st7789_send_dat(0x12);   
    
    ips_st7789_send_cmd(0xC4);
    ips_st7789_send_dat(0x20);  
    
    ips_st7789_send_cmd(0xC6); 
    ips_st7789_send_dat(0x0F);    
    
    ips_st7789_send_cmd(0xD0); 
    ips_st7789_send_dat(0xA4);
    ips_st7789_send_dat(0xA1);
    
    ips_st7789_send_cmd(0xE0);
    ips_st7789_send_dat(0xD0);
    ips_st7789_send_dat(0x04);
    ips_st7789_send_dat(0x0D);
    ips_st7789_send_dat(0x11);
    ips_st7789_send_dat(0x13);
    ips_st7789_send_dat(0x2B);
    ips_st7789_send_dat(0x3F);
    ips_st7789_send_dat(0x54);
    ips_st7789_send_dat(0x4C);
    ips_st7789_send_dat(0x18);
    ips_st7789_send_dat(0x0D);
    ips_st7789_send_dat(0x0B);
    ips_st7789_send_dat(0x1F);
    ips_st7789_send_dat(0x23);
    
    ips_st7789_send_cmd(0xE1);
    ips_st7789_send_dat(0xD0);
    ips_st7789_send_dat(0x04);
    ips_st7789_send_dat(0x0C);
    ips_st7789_send_dat(0x11);
    ips_st7789_send_dat(0x13);
    ips_st7789_send_dat(0x2C);
    ips_st7789_send_dat(0x3F);
    ips_st7789_send_dat(0x44);
    ips_st7789_send_dat(0x51);
    ips_st7789_send_dat(0x2F);
    ips_st7789_send_dat(0x1F);
    ips_st7789_send_dat(0x1F);
    ips_st7789_send_dat(0x20);
    ips_st7789_send_dat(0x23);
    
    ips_st7789_send_cmd(0x21); 
    
    ips_st7789_displayOn();
    
    ips_st7789_clear_with( 0x07f0 );
    
}


u8 ips_st7789_trans_byte( u8 byte )
{
    while ( !( IPS_ST7789_SPI->SR & SPI_I2S_FLAG_TXE ) );
    IPS_ST7789_SPI->DR = byte;
    while ( !( IPS_ST7789_SPI->SR & SPI_I2S_FLAG_RXNE ) );
    return IPS_ST7789_SPI->DR; 
}

void ips_st7789_send_cmd( u8 cmd )
{
    IPS_ST7789_DC_LOW;
    ips_st7789_trans_byte( cmd );
}

void ips_st7789_send_dat( u8 dat )
{
    IPS_ST7789_DC_HIGH;
    ips_st7789_trans_byte( dat );
}

void ips_st7789_send( u8 byte, ST7789_DC_OPT opt )
{
    if ( opt == ST7789_CMD )
        ips_st7789_send_cmd( byte );
    else if ( opt == ST7789_DAT )
        ips_st7789_send_dat( byte );
    else
        printf( "send opt error.\n" );
}

void ips_st7789_set_region( u16 xs, u16 ys, u16 xe, u16 ye )
{
    ips_st7789_send_cmd( 0x2a );
	ips_st7789_send_dat( 0x00 );
	ips_st7789_send_dat( xs );
	ips_st7789_send_dat( 0x00 );
	ips_st7789_send_dat( xe );
	
    ips_st7789_send_cmd( 0x2b );
	ips_st7789_send_dat( 0x00 );
	ips_st7789_send_dat( ys );
	ips_st7789_send_dat( 0x00 );
	ips_st7789_send_dat( ye );
	
	ips_st7789_send_cmd( 0x2c );
}

void ips_st7789_draw_point( u16 x, u16 y, u16 color )
{
    ips_st7789_set_region( x, y, x, y );
    ips_st7789_send_pixel_dat( color );
}

void ips_st7789_send_pixel_dat( u16 color )
{
    ips_st7789_send_dat( color >> 8 );
    ips_st7789_send_dat( color&0xFF );
}

void ips_st7789_clear_with( u16 color )
{
    u8 x, y;
    ips_st7789_set_region( 0, 0, IPS_ST7789_W-1, IPS_ST7789_H-1 );
    for ( y = 0; y < IPS_ST7789_H; y++ ) {
        for ( x = 0; x < IPS_ST7789_W; x++ ) {
            ips_st7789_send_pixel_dat( color );
        }
    }
}


void ips_st7789_sw_reset( void )
{
    ips_st7789_send_cmd( 0x01 );
    DELAY(200);
}

void ips_st7789_hw_reset( void )
{
    IPS_ST7789_RES_HIGH;
    DELAY(100);
    IPS_ST7789_RES_LOW;
    DELAY(100);
    IPS_ST7789_RES_HIGH;
    DELAY(100);
}

void ips_st7789_displayOn( void )
{
    ips_st7789_powerUp();
    IPS_ST7789_BLK_HIGH;
}

void ips_st7789_displayOff( void )
{
    IPS_ST7789_BLK_LOW;
    ips_st7789_powerDown();
}

void ips_st7789_powerDown( void )
{
    ips_st7789_send_cmd(0x12); 
    ips_st7789_send_cmd(0x28);
}

void ips_st7789_powerUp( void )
{
    ips_st7789_send_cmd(0x11); 
    ips_st7789_send_cmd(0x29);
}
