/************************************************
 * @file lcd_st7735.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-08
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "lcd_st7735.h"
#include "pro_conf.h"
#include "spi_conf.h"

static SPI_HandleTypeDef h_st7735_spi;

void lcd_gpio( void ) {
    GPIO_InitTypeDef lcd_st7735_io_s;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_SPI2_CLK_ENABLE();
    
    lcd_st7735_io_s.Pin     = LCD_144_ST7735_CS | LCD_144_ST7735_RST;
    lcd_st7735_io_s.Mode    = GPIO_MODE_OUTPUT_PP;
    lcd_st7735_io_s.Pull    = GPIO_PULLUP;
    lcd_st7735_io_s.Speed   = GPIO_SPEED_FREQ_VERY_HIGH;  
    HAL_GPIO_Init( GPIOB, &lcd_st7735_io_s );
    
    lcd_st7735_io_s.Pin     = LCD_144_ST7735_DC | LCD_144_ST7735_BLK;
    lcd_st7735_io_s.Mode    = GPIO_MODE_OUTPUT_PP;
    lcd_st7735_io_s.Pull    = GPIO_PULLUP;
    lcd_st7735_io_s.Speed   = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init( GPIOC, &lcd_st7735_io_s );
    
    /* SPI */
    lcd_st7735_io_s.Pin       = LCD_144_ST7735_MOSI | LCD_144_ST7735_CLK;
    lcd_st7735_io_s.Mode      = GPIO_MODE_AF_PP;
    lcd_st7735_io_s.Pull      = GPIO_PULLUP;
    lcd_st7735_io_s.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    lcd_st7735_io_s.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init( GPIOB, &lcd_st7735_io_s );

    h_st7735_spi.Instance               = LCD_ST7735_SPI;
    h_st7735_spi.Init.CLKPhase          = SPI_PHASE_2EDGE;
    h_st7735_spi.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    h_st7735_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    h_st7735_spi.Init.Mode              = SPI_MODE_MASTER;
    h_st7735_spi.Init.NSS               = SPI_NSS_SOFT;
    h_st7735_spi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    h_st7735_spi.Init.DataSize          = SPI_DATASIZE_8BIT;
    h_st7735_spi.Init.Direction         = SPI_DIRECTION_1LINE;
    h_st7735_spi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    h_st7735_spi.Init.TIMode            = SPI_TIMODE_DISABLE;
    if ( HAL_SPI_Init( &h_st7735_spi ) != HAL_OK ) {
        Error_Handler();
    }
    __HAL_SPI_ENABLE(&h_st7735_spi);
    
    
    LCD_144_ST7735_CS_SET;
    LCD_144_ST7735_BLK_SET;
    
}

void lcd_st7735_hw_reset(void)
{
    LCD_144_ST7735_RST_CLR;
    DELAY( 10 );
    LCD_144_ST7735_RST_SET;
    DELAY( 10 );
}

void lcd_st7735_init( void )
{
    lcd_gpio();
    lcd_st7735_hw_reset();
    
    lcd_st7735_send(0x11, ST7735_CMD);//Sleep exit
		
    lcd_st7735_send(0x36, ST7735_CMD);
    lcd_st7735_send(ST7735_DIRECTION, ST7735_DAT);

	//ST7735R Frame Rate
	lcd_st7735_send(0xB1, ST7735_CMD); 
	lcd_st7735_send(0x01, ST7735_DAT); 
	lcd_st7735_send(0x2C, ST7735_DAT); 
	lcd_st7735_send(0x2D, ST7735_DAT); 

	lcd_st7735_send(0xB2, ST7735_CMD); 
	lcd_st7735_send(0x01, ST7735_DAT); 
	lcd_st7735_send(0x2C, ST7735_DAT); 
	lcd_st7735_send(0x2D, ST7735_DAT); 

	lcd_st7735_send(0xB3, ST7735_CMD); 
	lcd_st7735_send(0x01, ST7735_DAT); 
	lcd_st7735_send(0x2C, ST7735_DAT); 
	lcd_st7735_send(0x2D, ST7735_DAT); 
	lcd_st7735_send(0x01, ST7735_DAT); 
	lcd_st7735_send(0x2C, ST7735_DAT); 
	lcd_st7735_send(0x2D, ST7735_DAT); 
	
	lcd_st7735_send(0xB4, ST7735_CMD); //Column inversion 
	lcd_st7735_send(0x07, ST7735_DAT); 
	
	//ST7735R Power Sequence
	lcd_st7735_send(0xC0, ST7735_CMD); 
	lcd_st7735_send(0xA2, ST7735_DAT); 
	lcd_st7735_send(0x02, ST7735_DAT); 
	lcd_st7735_send(0x84, ST7735_DAT); 
	lcd_st7735_send(0xC1, ST7735_CMD); 
	lcd_st7735_send(0xC5, ST7735_DAT); 

	lcd_st7735_send(0xC2, ST7735_CMD); 
	lcd_st7735_send(0x0A, ST7735_DAT); 
	lcd_st7735_send(0x00, ST7735_DAT); 

	lcd_st7735_send(0xC3, ST7735_CMD); 
	lcd_st7735_send(0x8A, ST7735_DAT); 
	lcd_st7735_send(0x2A, ST7735_DAT); 
	lcd_st7735_send(0xC4, ST7735_CMD); 
	lcd_st7735_send(0x8A, ST7735_DAT); 
	lcd_st7735_send(0xEE, ST7735_DAT); 
	
	lcd_st7735_send(0xC5, ST7735_CMD); //VCOM 
	lcd_st7735_send(0x0E, ST7735_DAT); 
	
	// lcd_st7735_send(0x36, ST7735_CMD); //MX, MY, RGB mode 
	// lcd_st7735_send(0xC8, ST7735_DAT); //ÊúÆÁC8 ºáÆÁ08 A8	
	
	//ST7735R Gamma Sequence
	lcd_st7735_send(0xe0, ST7735_CMD); 
	lcd_st7735_send(0x0f, ST7735_DAT); 
	lcd_st7735_send(0x1a, ST7735_DAT); 
	lcd_st7735_send(0x0f, ST7735_DAT); 
	lcd_st7735_send(0x18, ST7735_DAT); 
	lcd_st7735_send(0x2f, ST7735_DAT); 
	lcd_st7735_send(0x28, ST7735_DAT); 
	lcd_st7735_send(0x20, ST7735_DAT); 
	lcd_st7735_send(0x22, ST7735_DAT); 
	lcd_st7735_send(0x1f, ST7735_DAT); 
	lcd_st7735_send(0x1b, ST7735_DAT); 
	lcd_st7735_send(0x23, ST7735_DAT); 
	lcd_st7735_send(0x37, ST7735_DAT); 
	lcd_st7735_send(0x00, ST7735_DAT); 	
	lcd_st7735_send(0x07, ST7735_DAT); 
	lcd_st7735_send(0x02, ST7735_DAT); 
	lcd_st7735_send(0x10, ST7735_DAT); 

	lcd_st7735_send(0xe1, ST7735_CMD); 
	lcd_st7735_send(0x0f, ST7735_DAT); 
	lcd_st7735_send(0x1b, ST7735_DAT); 
	lcd_st7735_send(0x0f, ST7735_DAT); 
	lcd_st7735_send(0x17, ST7735_DAT); 
	lcd_st7735_send(0x33, ST7735_DAT); 
	lcd_st7735_send(0x2c, ST7735_DAT); 
	lcd_st7735_send(0x29, ST7735_DAT); 
	lcd_st7735_send(0x2e, ST7735_DAT); 
	lcd_st7735_send(0x30, ST7735_DAT); 
	lcd_st7735_send(0x30, ST7735_DAT); 
	lcd_st7735_send(0x39, ST7735_DAT); 
	lcd_st7735_send(0x3f, ST7735_DAT); 
	lcd_st7735_send(0x00, ST7735_DAT); 
	lcd_st7735_send(0x07, ST7735_DAT); 
	lcd_st7735_send(0x03, ST7735_DAT); 
	lcd_st7735_send(0x10, ST7735_DAT);  
	
	lcd_st7735_send(0xF0, ST7735_CMD); //Enable test command  
	lcd_st7735_send(0x01, ST7735_DAT); 
	lcd_st7735_send(0xF6, ST7735_CMD); //Disable ram power save mode 
	lcd_st7735_send(0x00, ST7735_DAT); 
	
	lcd_st7735_send(0x3A, ST7735_CMD); //65k mode 
	lcd_st7735_send(0x05, ST7735_DAT); 

	lcd_st7735_powerUp();
    
    lcd_st7735_clear_with( 0xf800 );
    
}

void lcd_st7735_trans_byte( uint8_t byte )
{
    spi_one_line_tx_trans_byte( &h_st7735_spi, byte );
}

void lcd_st7735_send_cmd( uint8_t cmd )
{
    LCD_144_ST7735_DC_CLR;
    LCD_144_ST7735_CS_CLR;
    lcd_st7735_trans_byte( cmd );
    LCD_144_ST7735_CS_SET;
}

void lcd_st7735_send_dat( uint8_t dat )
{
    LCD_144_ST7735_DC_SET;
    LCD_144_ST7735_CS_CLR;
    lcd_st7735_trans_byte( dat );
    LCD_144_ST7735_CS_SET;
}

void lcd_st7735_send( uint8_t byte, ST7735_DC_OPT opt )
{
    if ( opt == ST7735_CMD )
        lcd_st7735_send_cmd( byte );
    else if ( opt == ST7735_DAT )
        lcd_st7735_send_dat( byte );
    else
        DEBUG_PRINT( "send opt error.\n" );
    
}

void lcd_st7735_powerUp( void )
{
    lcd_st7735_send(0x11, ST7735_CMD);//Sleep exit
    lcd_st7735_displayOn();
    LCD_144_ST7735_BLK_SET;
}

void lcd_st7735_powerDown( void )
{
    lcd_st7735_send(0x12, ST7735_CMD);//Sleep enter
    lcd_st7735_displayOff();
    LCD_144_ST7735_BLK_CLR;
}

void lcd_st7735_displayOn( void )
{
    lcd_st7735_send(0x29, ST7735_CMD);//Display on
}

void lcd_st7735_displayOff( void )
{
    lcd_st7735_send(0x28, ST7735_CMD);//Display off
}

void lcd_st7735_set_region( uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye )
{
    lcd_st7735_send_cmd( 0x2a );
	lcd_st7735_send_dat( 0x00 );
	lcd_st7735_send_dat( xs+2 );
	lcd_st7735_send_dat( 0x00 );
	lcd_st7735_send_dat( xe+2 );
	
    lcd_st7735_send_cmd( 0x2b );
	lcd_st7735_send_dat( 0x00 );
	lcd_st7735_send_dat( ys+3 );
	lcd_st7735_send_dat( 0x00 );
	lcd_st7735_send_dat( ye+3 );
	
	lcd_st7735_send_cmd( 0x2c );
}

void lcd_st7735_draw_pixel( uint16_t x, uint16_t y, uint16_t color )
{
    lcd_st7735_set_region( x, y, x, y );
    lcd_st7735_send_pixel_dat( color );
}

void lcd_st7735_send_pixel_dat( uint16_t color )
{
    lcd_st7735_send_dat( color>>8 );
    lcd_st7735_send_dat( color&0xff );
}

void lcd_st7735_clear_with( uint16_t color )
{
    uint16_t x, y;
    
    lcd_st7735_set_region( 0, 0, LCD_144_W-1, LCD_144_H-1 );
    
    for ( y = 0; y < LCD_144_H; y++ ) {
        for ( x = 0; x < LCD_144_W; x++ ) {
//            lcd_st7735_draw_pixel( x, y, color );
            lcd_st7735_send_pixel_dat( color );
        }
    }
}
