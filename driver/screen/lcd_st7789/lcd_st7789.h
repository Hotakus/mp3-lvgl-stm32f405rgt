#ifndef LCD_ST7789_H
#define LCD_ST7789_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"
#include "pro_conf.h"


/* definations */

#if (USER_USE_SCR == USE_SCR_LCD_ST7789_130)
/* Screen's display direction */
#define USE_HORIZONTAL 0

#define ST7789_SCR_X    240
#define ST7789_SCR_Y    240

#define IPS_ST7789_USE_SPI     1
#if  (IPS_ST7789_USE_SPI==1) 
#define IPS_ST7789_SPI        SPI1
#define IPS_ST7789_SPI_GPIO   GPIOA
#define IPS_ST7789_SPI_AF     GPIO_AF_SPI1   
#define IPS_ST7789_SCLK       GPIO_Pin_5       // PA
#define IPS_ST7789_MOSI       GPIO_Pin_7       // PA
#define IPS_ST7789_SCLK_SRC   GPIO_PinSource5  
#define IPS_ST7789_MOSI_SRC   GPIO_PinSource7  
#elif  (IPS_ST7789_USE_SPI==2) 
#define IPS_ST7789_SPI        SPI2
#define IPS_ST7789_SPI_GPIO   GPIOB
#define IPS_ST7789_SPI_AF     GPIO_AF_SPI2
#define IPS_ST7789_SCLK       GPIO_Pin_13       // PB
#define IPS_ST7789_MOSI       GPIO_Pin_15       // PB
#define IPS_ST7789_SCLK_SRC   GPIO_PinSource13  
#define IPS_ST7789_MOSI_SRC   GPIO_PinSource15 
#elif  (IPS_ST7789_USE_SPI==3) 
#define IPS_ST7789_SPI        SPI3
#define IPS_ST7789_SPI_AF     GPIO_AF_SPI3    
#define IPS_ST7789_SPI_GPIO   GPIOB
#define IPS_ST7789_SCLK       GPIO_Pin_3       // PB
#define IPS_ST7789_MOSI       GPIO_Pin_5       // PB
#define IPS_ST7789_SCLK_SRC   GPIO_PinSource3  
#define IPS_ST7789_MOSI_SRC   GPIO_PinSource5  
#endif

#if  (IPS_ST7789_USE_SPI != 0) && (USER_USE_SCR == USE_SCR_LCD_ST7789_130)
#define ST7789_DC          GPIO_Pin_1      // PA
#define ST7789_RST         GPIO_Pin_2      // PA
#define LCD_LEDK           GPIO_Pin_3      // PA

#define ST7789_DC_HIGH     GPIOA->BSRRL |= ST7789_DC
#define ST7789_DC_LOW      GPIOA->BSRRH |= ST7789_DC

#define ST7789_RST_HIGH    GPIOA->BSRRL |= ST7789_RST
#define ST7789_RST_LOW     GPIOA->BSRRH |= ST7789_RST

#define LCD_LEDK_HIGH      GPIOA->BSRRL |= LCD_LEDK
#define LCD_LEDK_LOW       GPIOA->BSRRH |= LCD_LEDK
#endif

#elif  (USER_USE_SCR == USE_SCR_LCD_ST7789_200)

#define ST7789_SCR_X    240
#define ST7789_SCR_Y    320

// use mcu interface
#define ST7789_CS                   GPIO_Pin_1  // PA
#define ST7789_DC                   GPIO_Pin_2  // PA
#define ST7789_WR                   GPIO_Pin_3  // PA
#define ST7789_RD                   GPIO_Pin_4  // PA
#define LCD_LEDK                    GPIO_Pin_11 // PA
#define ST7789_RST                  GPIO_Pin_0  // PB

#define LCD_ST7789_DAT_0            GPIO_Pin_8  // PB
#define LCD_ST7789_DAT_1            GPIO_Pin_9  // PB
#define LCD_ST7789_DAT_2            GPIO_Pin_10 // PB
#define LCD_ST7789_DAT_3            GPIO_Pin_11 // PB
#define LCD_ST7789_DAT_4            GPIO_Pin_12 // PB
#define LCD_ST7789_DAT_5            GPIO_Pin_13 // PB
#define LCD_ST7789_DAT_6            GPIO_Pin_14 // PB
#define LCD_ST7789_DAT_7            GPIO_Pin_15 // PB
#define LCD_ST7789_DAT_PIN          ( LCD_ST7789_DAT_0 | LCD_ST7789_DAT_1 | LCD_ST7789_DAT_2 | LCD_ST7789_DAT_3 | \
                                      LCD_ST7789_DAT_4 | LCD_ST7789_DAT_5 | LCD_ST7789_DAT_6 | LCD_ST7789_DAT_7 )
#define ST7789_CTL_GPIO     GPIOA
#define ST7789_DAT_GPIO     GPIOB

#define ST7789_CS_HIGH              GPIOA->BSRR |= ST7789_CS
#define ST7789_CS_LOW               GPIOA->BRR  |= ST7789_CS

#define ST7789_DC_HIGH              GPIOA->BSRR |= ST7789_DC
#define ST7789_DC_LOW               GPIOA->BRR  |= ST7789_DC

#define ST7789_WR_HIGH              GPIOA->BSRR |= ST7789_WR
#define ST7789_WR_LOW               GPIOA->BRR  |= ST7789_WR

#define ST7789_RD_HIGH              GPIOA->BSRR |= ST7789_RD
#define ST7789_RD_LOW               GPIOA->BRR  |= ST7789_RD

#define ST7789_RST_HIGH             GPIOB->BSRR |= ST7789_RST
#define ST7789_RST_LOW              GPIOB->BRR  |= ST7789_RST

#define ST7789_BLK_HIGH             GPIOA->BSRR |= LCD_LEDK
#define ST7789_BLK_LOW              GPIOA->BRR  |= LCD_LEDK

#endif



typedef enum {
    ST7789_CMD  = 0,
    ST7789_DAT     ,
} ST7789_DC_OPT;

/* 设置点的命令 */

/* functions */
/* 初始函数 */
        void lcd_st7789_init( void );
/* 绘图函数 */
        void lcd_st7789_set_region( u16 xs, u16 ys, u16 xe, u16 ye );
        void lcd_st7789_draw_point( u16 x, u16 y, u16 color );
        void lcd_st7789_send_pixel_dat( u16 color );
        void lcd_st7789_clear_with( u16 color );
/* 其他功能函数 */
        void lcd_st7789_sw_reset( void );
        void lcd_st7789_hw_reset( void );
        void lcd_st7789_displayOn( void );
        void lcd_st7789_displayOff( void );
        void lcd_st7789_powerDown( void );
        void lcd_st7789_powerUp( void );
		
#ifdef __cplusplus
}
#endif
		
#endif
