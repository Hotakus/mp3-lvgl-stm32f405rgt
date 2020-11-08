#ifndef LCD_144_ST7735_144_H
#define LCD_144_ST7735_144_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"
#include "systick.h"

/* definations */

#define LCD_144_W   128
#define LCD_144_H   128

#define LCD_144_ST7735_SPI          SPI2

#define LCD_144_ST7735_MOSI         GPIO_Pin_15
#define LCD_144_ST7735_MISO         NULL
#define LCD_144_ST7735_CLK          GPIO_Pin_13
#define LCD_144_ST7735_SPI_PIN      LCD_144_ST7735_MOSI | LCD_144_ST7735_MISO | LCD_144_ST7735_CLK

#define LCD_144_ST7735_CS           GPIO_Pin_12     // PB12
#define LCD_144_ST7735_RST          GPIO_Pin_14     // PB14
#define LCD_144_ST7735_DC           GPIO_Pin_6      // PC6
#define LCD_144_ST7735_BLK          GPIO_Pin_7      // PC7


#define	LCD_144_ST7735_CS_SET  	    GPIOB->BSRRL |= LCD_144_ST7735_CS
#define	LCD_144_ST7735_CS_CLR  	    GPIOB->BSRRH |= LCD_144_ST7735_CS

#define	LCD_144_ST7735_RST_SET      GPIOB->BSRRL |= LCD_144_ST7735_RST
#define	LCD_144_ST7735_RST_CLR  	GPIOB->BSRRH |= LCD_144_ST7735_RST

#define	LCD_144_ST7735_DC_SET  	    GPIOC->BSRRL |= LCD_144_ST7735_DC  
#define	LCD_144_ST7735_DC_CLR  	    GPIOC->BSRRH |= LCD_144_ST7735_DC

#define	LCD_144_ST7735_BLK_SET  	GPIOC->BSRRL |= LCD_144_ST7735_BLK
#define	LCD_144_ST7735_BLK_CLR  	GPIOC->BSRRH |= LCD_144_ST7735_BLK

typedef enum {
    ST7735_CMD  = 0,
    ST7735_DAT     ,
} ST7735_DC_OPT;

/* function */
void lcd_144_init( void );

u8 lcd_144_trans_byte( u8 byte );

void lcd_144_send_cmd( u8 cmd );
void lcd_144_send_dat( u8 dat );
void lcd_144_send( u8 byte, ST7735_DC_OPT opt );

void lcd_144_hw_reset(void);
void lcd_144_powerUp( void );
void lcd_144_powerDown( void );
void lcd_144_displayOn( void );
void lcd_144_displayOff( void );

void lcd_144_set_region( u16 xs, u16 ys, u16 xe, u16 ye );
void lcd_144_draw_pixel( u16 x, u16 y, u16 color );
void lcd_144_send_pixel_dat( u16 color );
void lcd_144_clear_with( u16 color );


#ifdef __cplusplus
}
#endif

#endif
