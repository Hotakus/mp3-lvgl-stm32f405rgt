#include "LCD_144_ST7735144.h"
#include "spi_conf.h"

void lcd_gpio( void ) {
    GPIO_InitTypeDef LCD_144_ST7735_io;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    LCD_144_ST7735_io.GPIO_Pin     = LCD_144_ST7735_CS;              
    LCD_144_ST7735_io.GPIO_Mode    = GPIO_Mode_OUT;    
    LCD_144_ST7735_io.GPIO_OType   = GPIO_OType_PP;    
    LCD_144_ST7735_io.GPIO_Speed   = GPIO_Speed_100MHz;
    LCD_144_ST7735_io.GPIO_PuPd    = GPIO_PuPd_UP;     
    GPIO_Init( GPIOB, &LCD_144_ST7735_io );   
    
    LCD_144_ST7735_io.GPIO_Pin     = LCD_144_ST7735_DC | LCD_144_ST7735_BLK;              
    LCD_144_ST7735_io.GPIO_Mode    = GPIO_Mode_OUT;    
    LCD_144_ST7735_io.GPIO_OType   = GPIO_OType_PP;    
    LCD_144_ST7735_io.GPIO_Speed   = GPIO_Speed_100MHz;
    LCD_144_ST7735_io.GPIO_PuPd    = GPIO_PuPd_UP;     
    GPIO_Init( GPIOC, &LCD_144_ST7735_io );

}

void lcd_init( void ) {
    spi_conf( LCD_144_ST7735_SPI, SPI_BaudRatePrescaler_2, SPI_MODE_4, SPI_Direction_2Lines_FullDuplex );
    lcd_gpio();
    
    lcd_reset();

    lcd_powerUp();
    
    lcd_send_cmd(0x11);
    
    //ST7735R Frame Rate
	lcd_send_cmd(0xB1);
	lcd_send_dat(0x01);
	lcd_send_dat(0x2C);
	lcd_send_dat(0x2D);

	lcd_send_cmd(0xB2);
	lcd_send_dat(0x01);
	lcd_send_dat(0x2C);
	lcd_send_dat(0x2D);

	lcd_send_cmd(0xB3);
	lcd_send_dat(0x01);
	lcd_send_dat(0x2C);
	lcd_send_dat(0x2D);
	lcd_send_dat(0x01);
	lcd_send_dat(0x2C);
	lcd_send_dat(0x2D);

	lcd_send_cmd(0xB4); //Column inversion
	lcd_send_dat(0x07);

	//ST7735R Power Sequence
	lcd_send_cmd(0xC0);
	lcd_send_dat(0xA2);
	lcd_send_dat(0x02);
	lcd_send_dat(0x84);
	lcd_send_cmd(0xC1);
	lcd_send_dat(0xC5);

	lcd_send_cmd(0xC2);
	lcd_send_dat(0x0A);
	lcd_send_dat(0x00);

	lcd_send_cmd(0xC3);
	lcd_send_dat(0x8A);
	lcd_send_dat(0x2A);
	lcd_send_cmd(0xC4);
	lcd_send_dat(0x8A);
	lcd_send_dat(0xEE);

	lcd_send_cmd(0xC5); //VCOM
	lcd_send_dat(0x0E);

	lcd_send_cmd(0x36); //MX, MY, RGB mode
	lcd_send_dat(0xC8);

	//ST7735R Gamma Sequence
	lcd_send_cmd(0xe0);
	lcd_send_dat(0x0f);
	lcd_send_dat(0x1a);
	lcd_send_dat(0x0f);
	lcd_send_dat(0x18);
	lcd_send_dat(0x2f);
	lcd_send_dat(0x28);
	lcd_send_dat(0x20);
	lcd_send_dat(0x22);
	lcd_send_dat(0x1f);
	lcd_send_dat(0x1b);
	lcd_send_dat(0x23);
	lcd_send_dat(0x37);
	lcd_send_dat(0x00);
	lcd_send_dat(0x07);
	lcd_send_dat(0x02);
	lcd_send_dat(0x10);

	lcd_send_cmd(0xe1);
	lcd_send_dat(0x0f);
	lcd_send_dat(0x1b);
	lcd_send_dat(0x0f);
	lcd_send_dat(0x17);
	lcd_send_dat(0x33);
	lcd_send_dat(0x2c);
	lcd_send_dat(0x29);
	lcd_send_dat(0x2e);
	lcd_send_dat(0x30);
	lcd_send_dat(0x30);
	lcd_send_dat(0x39);
	lcd_send_dat(0x3f);
	lcd_send_dat(0x00);
	lcd_send_dat(0x07);
	lcd_send_dat(0x03);
	lcd_send_dat(0x10);

	lcd_send_cmd(0x2a);
	lcd_send_dat(0x00);
	lcd_send_dat(0x00);
	lcd_send_dat(0x00);
	lcd_send_dat(0x7f);

	lcd_send_cmd(0x2b);
	lcd_send_dat(0x00);
	lcd_send_dat(0x00);
	lcd_send_dat(0x00);
	lcd_send_dat(0x9f);

	lcd_send_cmd(0xF0); //Enable test command
	lcd_send_dat(0x01);
	lcd_send_cmd(0xF6); //Disable ram power save mode
	lcd_send_dat(0x00);

	lcd_send_cmd(0x3A); //65k mode
	lcd_send_dat(0x05);

	lcd_send_cmd(0x29);//Display on
    
    lcd_clear( 0x5555 );
    
}

u8 lcd_trans_byte( u8 byte ) {
    while ( !( LCD_144_ST7735_SPI->SR & SPI_I2S_FLAG_TXE ) );
    LCD_144_ST7735_SPI->DR = byte;
    while ( !( LCD_144_ST7735_SPI->SR & SPI_I2S_FLAG_RXNE ) );
    return LCD_144_ST7735_SPI->DR;
}

void lcd_trans_half( u16 half ) {
    lcd_send_dat( half>>8 );
    lcd_send_dat( half&0x0F );
}

void lcd_send_cmd( u8 cmd ) {

    LCD_144_ST7735_DC_CLR;
    LCD_144_ST7735_CLR;
    lcd_trans_byte( cmd );
    LCD_144_ST7735_CS_SET;
}


void lcd_send_dat( u8 dat ) {
    LCD_144_ST7735_DC_SET;
    LCD_144_ST7735_CLR;
    lcd_trans_byte( dat );
    LCD_144_ST7735_CS_SET;
}

void lcd_reset(void) {
    LCD_144_ST7735_RST_CLR;
    delay_ms(50);
    LCD_144_ST7735_RST_SET;
    delay_ms(50);
}

void lcd_powerUp( void ) {
    lcd_send_cmd( 0x11 );
    delay_ms(10);
    LCD_144_ST7735_BLK_SET;
    lcd_send_cmd( 0x29 );
}

void lcd_powerDown( void ) {
    lcd_send_cmd( 0x28 );
    lcd_send_cmd( 0x12 );
    delay_ms(100);
}


void lcd_setRegion( u16 xs, u16 ys, u16 xe, u16 ye) {
	lcd_send_cmd(0x2a);
	lcd_send_dat(0x00);
	lcd_send_dat(xs + 2);
	lcd_send_dat(0x00);
	lcd_send_dat(xe + 2);

	lcd_send_cmd(0x2b);
	lcd_send_dat(0x00);
	lcd_send_dat(ys + 3);
	lcd_send_dat(0x00);
	lcd_send_dat(ye + 3);

	lcd_send_cmd(0x2c);
}

void lcd_setPos( u16 x, u16 y ) {
    lcd_setRegion( x, y, x, y );
}

void lcd_clear(u16 color) {
	u8 x, y;
	lcd_setRegion(0, 0, 128 - 1, 128 - 1);
	lcd_send_cmd(0x2C);
	for(y = 0; y < 128; y++) {
        for(x = 0; x < 128; x++) {
			lcd_trans_half( color );
		}
    }
}


void lcd_drawPoint( u16 x, u16 y, u16 color ) {
    lcd_setPos( x, y );
    lcd_trans_half( color );
}

