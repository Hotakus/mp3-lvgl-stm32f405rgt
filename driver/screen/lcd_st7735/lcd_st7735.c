/************************************************
 * @file lcd_144_st7735.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "lcd_st7735.h"
#include "pro_conf.h"
#include "spi_conf.h"

void lcd_gpio( void ) {
    GPIO_InitTypeDef lcd_144_io_s;
    GPIO_InitTypeDef spig_s;
    SPI_InitTypeDef  spi_s;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE);
    
    GPIO_PinAFConfig( GPIOB, GPIO_PinSource13, GPIO_AF_SPI2 );
    GPIO_PinAFConfig( GPIOB, GPIO_PinSource15, GPIO_AF_SPI2 );
    
    lcd_144_io_s.GPIO_Pin     = LCD_144_ST7735_CS | LCD_144_ST7735_RST;              
    lcd_144_io_s.GPIO_Mode    = GPIO_Mode_OUT;    
    lcd_144_io_s.GPIO_OType   = GPIO_OType_PP;    
    lcd_144_io_s.GPIO_Speed   = GPIO_Speed_100MHz;
    lcd_144_io_s.GPIO_PuPd    = GPIO_PuPd_UP;     
    GPIO_Init( GPIOB, &lcd_144_io_s );   
    
    lcd_144_io_s.GPIO_Pin     = LCD_144_ST7735_DC | LCD_144_ST7735_BLK;              
    lcd_144_io_s.GPIO_Mode    = GPIO_Mode_OUT;    
    lcd_144_io_s.GPIO_OType   = GPIO_OType_PP;    
    lcd_144_io_s.GPIO_Speed   = GPIO_Speed_100MHz;
    lcd_144_io_s.GPIO_PuPd    = GPIO_PuPd_UP;     
    GPIO_Init( GPIOC, &lcd_144_io_s );
    
    /* SPI conf */
//    spi_s.SPI_CPOL      = SPI_CPOL_High;
//    spi_s.SPI_CPHA      = SPI_CPHA_2Edge;
//    spi_s.SPI_Mode              = SPI_Mode_Master;
//    spi_s.SPI_FirstBit          = SPI_FirstBit_MSB;
//    spi_s.SPI_Direction         = SPI_Direction_Tx;
//    spi_s.SPI_DataSize          = SPI_DataSize_8b;
//    spi_s.SPI_NSS               = SPI_NSS_Soft;
//    spi_s.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; 
//    SPI_Init( LCD_144_ST7735_SPI, &spi_s );
//    SPI_Cmd( LCD_144_ST7735_SPI, ENABLE );
//    
//    spig_s.GPIO_Pin          = LCD_144_ST7735_SPI_PIN;
//    spig_s.GPIO_Mode         = GPIO_Mode_AF;
//    spig_s.GPIO_OType        = GPIO_OType_PP;
//    spig_s.GPIO_PuPd         = GPIO_PuPd_UP;
//    spig_s.GPIO_Speed        = GPIO_Speed_100MHz;
//    GPIO_Init( GPIOB, &spig_s );

    spi_conf( LCD_144_ST7735_SPI, SPI_BaudRatePrescaler_2, SPI_MODE_4, SPI_Direction_Tx );
    
    LCD_144_ST7735_CS_SET;
    LCD_144_ST7735_BLK_SET;
    
}

void lcd_144_hw_reset(void)
{
    LCD_144_ST7735_RST_CLR;
    DELAY( 10 );
    LCD_144_ST7735_RST_SET;
    DELAY( 10 );
}

void lcd_144_init( void )
{
    lcd_gpio();
    lcd_144_hw_reset();
    
    lcd_144_send(0x11, ST7735_CMD);//Sleep exit
		
	//ST7735R Frame Rate
	lcd_144_send(0xB1, ST7735_CMD); 
	lcd_144_send(0x01, ST7735_DAT); 
	lcd_144_send(0x2C, ST7735_DAT); 
	lcd_144_send(0x2D, ST7735_DAT); 

	lcd_144_send(0xB2, ST7735_CMD); 
	lcd_144_send(0x01, ST7735_DAT); 
	lcd_144_send(0x2C, ST7735_DAT); 
	lcd_144_send(0x2D, ST7735_DAT); 

	lcd_144_send(0xB3, ST7735_CMD); 
	lcd_144_send(0x01, ST7735_DAT); 
	lcd_144_send(0x2C, ST7735_DAT); 
	lcd_144_send(0x2D, ST7735_DAT); 
	lcd_144_send(0x01, ST7735_DAT); 
	lcd_144_send(0x2C, ST7735_DAT); 
	lcd_144_send(0x2D, ST7735_DAT); 
	
	lcd_144_send(0xB4, ST7735_CMD); //Column inversion 
	lcd_144_send(0x07, ST7735_DAT); 
	
	//ST7735R Power Sequence
	lcd_144_send(0xC0, ST7735_CMD); 
	lcd_144_send(0xA2, ST7735_DAT); 
	lcd_144_send(0x02, ST7735_DAT); 
	lcd_144_send(0x84, ST7735_DAT); 
	lcd_144_send(0xC1, ST7735_CMD); 
	lcd_144_send(0xC5, ST7735_DAT); 

	lcd_144_send(0xC2, ST7735_CMD); 
	lcd_144_send(0x0A, ST7735_DAT); 
	lcd_144_send(0x00, ST7735_DAT); 

	lcd_144_send(0xC3, ST7735_CMD); 
	lcd_144_send(0x8A, ST7735_DAT); 
	lcd_144_send(0x2A, ST7735_DAT); 
	lcd_144_send(0xC4, ST7735_CMD); 
	lcd_144_send(0x8A, ST7735_DAT); 
	lcd_144_send(0xEE, ST7735_DAT); 
	
	lcd_144_send(0xC5, ST7735_CMD); //VCOM 
	lcd_144_send(0x0E, ST7735_DAT); 
	
	lcd_144_send(0x36, ST7735_CMD); //MX, MY, RGB mode 
	lcd_144_send(0xC8, ST7735_DAT); //ÊúÆÁC8 ºáÆÁ08 A8	
	
	//ST7735R Gamma Sequence
	lcd_144_send(0xe0, ST7735_CMD); 
	lcd_144_send(0x0f, ST7735_DAT); 
	lcd_144_send(0x1a, ST7735_DAT); 
	lcd_144_send(0x0f, ST7735_DAT); 
	lcd_144_send(0x18, ST7735_DAT); 
	lcd_144_send(0x2f, ST7735_DAT); 
	lcd_144_send(0x28, ST7735_DAT); 
	lcd_144_send(0x20, ST7735_DAT); 
	lcd_144_send(0x22, ST7735_DAT); 
	lcd_144_send(0x1f, ST7735_DAT); 
	lcd_144_send(0x1b, ST7735_DAT); 
	lcd_144_send(0x23, ST7735_DAT); 
	lcd_144_send(0x37, ST7735_DAT); 
	lcd_144_send(0x00, ST7735_DAT); 	
	lcd_144_send(0x07, ST7735_DAT); 
	lcd_144_send(0x02, ST7735_DAT); 
	lcd_144_send(0x10, ST7735_DAT); 

	lcd_144_send(0xe1, ST7735_CMD); 
	lcd_144_send(0x0f, ST7735_DAT); 
	lcd_144_send(0x1b, ST7735_DAT); 
	lcd_144_send(0x0f, ST7735_DAT); 
	lcd_144_send(0x17, ST7735_DAT); 
	lcd_144_send(0x33, ST7735_DAT); 
	lcd_144_send(0x2c, ST7735_DAT); 
	lcd_144_send(0x29, ST7735_DAT); 
	lcd_144_send(0x2e, ST7735_DAT); 
	lcd_144_send(0x30, ST7735_DAT); 
	lcd_144_send(0x30, ST7735_DAT); 
	lcd_144_send(0x39, ST7735_DAT); 
	lcd_144_send(0x3f, ST7735_DAT); 
	lcd_144_send(0x00, ST7735_DAT); 
	lcd_144_send(0x07, ST7735_DAT); 
	lcd_144_send(0x03, ST7735_DAT); 
	lcd_144_send(0x10, ST7735_DAT);  
	
	lcd_144_send(0xF0, ST7735_CMD); //Enable test command  
	lcd_144_send(0x01, ST7735_DAT); 
	lcd_144_send(0xF6, ST7735_CMD); //Disable ram power save mode 
	lcd_144_send(0x00, ST7735_DAT); 
	
	lcd_144_send(0x3A, ST7735_CMD); //65k mode 
	lcd_144_send(0x05, ST7735_DAT); 
	
	lcd_144_powerUp();
    
    lcd_144_clear_with( 0xffff );
    
}

u8 lcd_144_trans_byte( u8 byte )
{
    while ( !( LCD_144_ST7735_SPI->SR & SPI_I2S_FLAG_TXE ) );
    LCD_144_ST7735_SPI->DR = byte;
    while ( !( LCD_144_ST7735_SPI->SR & SPI_I2S_FLAG_RXNE ) );
    return (u8)LCD_144_ST7735_SPI->DR;
}

void lcd_144_send_cmd( u8 cmd )
{
    LCD_144_ST7735_DC_CLR;
    LCD_144_ST7735_CS_CLR;
    lcd_144_trans_byte( cmd );
    LCD_144_ST7735_CS_SET;
}

void lcd_144_send_dat( u8 dat )
{
    LCD_144_ST7735_DC_SET;
    LCD_144_ST7735_CS_CLR;
    lcd_144_trans_byte( dat );
    LCD_144_ST7735_CS_SET;
}

void lcd_144_send( u8 byte, ST7735_DC_OPT opt )
{
    if ( opt == ST7735_CMD )
        lcd_144_send_cmd( byte );
    else if ( opt == ST7735_DAT )
        lcd_144_send_dat( byte );
    else
        DEBUG_PRINT( "send opt error.\n" );
    
}

void lcd_144_powerUp( void )
{
    lcd_144_send(0x11, ST7735_CMD);//Sleep exit
    lcd_144_displayOn();
    LCD_144_ST7735_BLK_SET;
}

void lcd_144_powerDown( void )
{
    lcd_144_send(0x12, ST7735_CMD);//Sleep enter
    lcd_144_displayOff();
    LCD_144_ST7735_BLK_CLR;
}

void lcd_144_displayOn( void )
{
    lcd_144_send(0x29, ST7735_CMD);//Display on
}

void lcd_144_displayOff( void )
{
    lcd_144_send(0x28, ST7735_CMD);//Display off
}

void lcd_144_set_region( u16 xs, u16 ys, u16 xe, u16 ye )
{
    lcd_144_send_cmd( 0x2a );
	lcd_144_send_dat( 0x00 );
	lcd_144_send_dat( xs+2 );
	lcd_144_send_dat( 0x00 );
	lcd_144_send_dat( xe+2 );
	
    lcd_144_send_cmd( 0x2b );
	lcd_144_send_dat( 0x00 );
	lcd_144_send_dat( ys+3 );
	lcd_144_send_dat( 0x00 );
	lcd_144_send_dat( ye+3 );
	
	lcd_144_send_cmd( 0x2c );
}

void lcd_144_draw_pixel( u16 x, u16 y, u16 color )
{
    lcd_144_set_region( x, y, x, y );
    lcd_144_send_pixel_dat( color );
}

void lcd_144_send_pixel_dat( u16 color )
{
    lcd_144_send_dat( color>>8 );
    lcd_144_send_dat( color&0xff );
}

void lcd_144_clear_with( u16 color )
{
    u16 x, y;
    
    lcd_144_set_region( 0, 0, LCD_144_W-1, LCD_144_H-1 );
    
    for ( y = 0; y < LCD_144_H; y++ ) {
        for ( x = 0; x < LCD_144_W; x++ ) {
//            lcd_144_draw_pixel( x, y, color );
            lcd_144_send_pixel_dat( color );
        }
    }
}


