/************************************************
 * @file lcd_st7735.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-08
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef LCD_144_ST7735_144_H
#define LCD_144_ST7735_144_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_hal.h"

/* definations */
#define ST7735_NOMAL                0xC8u
#define ST7735_RIGHT_TO_LEFT        0x68u
#define ST7735_LEFT_TO_RIGHT        0xA8u
#define ST7735_UPSIDE_DOWN          0x08u
#define ST7735_DIRECTION            ST7735_RIGHT_TO_LEFT

#define LCD_144_W   128
#define LCD_144_H   128

#define LCD_ST7735_SPI              SPI2

#define LCD_144_ST7735_MOSI         GPIO_PIN_15
#define LCD_144_ST7735_MISO         NULL
#define LCD_144_ST7735_CLK          GPIO_PIN_13
#define LCD_144_ST7735_SPI_PIN      LCD_144_ST7735_MOSI | LCD_144_ST7735_MISO | LCD_144_ST7735_CLK

#define LCD_144_ST7735_CS           GPIO_PIN_12     // PB12
#define LCD_144_ST7735_RST          GPIO_PIN_14     // PB14
#define LCD_144_ST7735_DC           GPIO_PIN_6      // PC6
#define LCD_144_ST7735_BLK          GPIO_PIN_7      // PC7


#define	LCD_144_ST7735_CS_SET  	    GPIOB->BSRR |= (uint32_t)LCD_144_ST7735_CS
#define	LCD_144_ST7735_CS_CLR  	    GPIOB->BSRR |= (uint32_t)LCD_144_ST7735_CS <<16

#define	LCD_144_ST7735_RST_SET      GPIOB->BSRR |= (uint32_t)LCD_144_ST7735_RST
#define	LCD_144_ST7735_RST_CLR  	GPIOB->BSRR |= (uint32_t)LCD_144_ST7735_RST <<16

#define	LCD_144_ST7735_DC_SET  	    GPIOC->BSRR |= (uint32_t)LCD_144_ST7735_DC  
#define	LCD_144_ST7735_DC_CLR  	    GPIOC->BSRR |= (uint32_t)LCD_144_ST7735_DC <<16

#define	LCD_144_ST7735_BLK_SET  	GPIOC->BSRR |= (uint32_t)LCD_144_ST7735_BLK
#define	LCD_144_ST7735_BLK_CLR  	GPIOC->BSRR |= (uint32_t)LCD_144_ST7735_BLK <<16

typedef enum {
    ST7735_CMD  = 0,
    ST7735_DAT     ,
} ST7735_DC_OPT;

/* function */
void lcd_st7735_init( void );

void lcd_st7735_trans_byte( uint8_t byte );

void lcd_st7735_send_cmd( uint8_t cmd );
void lcd_st7735_send_dat( uint8_t dat );
void lcd_st7735_send( uint8_t byte, ST7735_DC_OPT opt );

void lcd_st7735_hw_reset(void);
void lcd_st7735_powerUp( void );
void lcd_st7735_powerDown( void );
void lcd_st7735_displayOn( void );
void lcd_st7735_displayOff( void );

void lcd_st7735_set_region( uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye );
void lcd_st7735_draw_pixel( uint16_t x, uint16_t y, uint16_t color );
void lcd_st7735_send_pixel_dat( uint16_t color );
void lcd_st7735_clear_with( uint16_t color );


#ifdef __cplusplus
}
#endif

#endif
