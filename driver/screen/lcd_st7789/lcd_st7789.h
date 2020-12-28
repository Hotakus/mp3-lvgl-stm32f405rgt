#ifndef LCD_ST7789_H
#define LCD_ST7789_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_hal.h"
#include "pro_conf.h"


/* definations */
#define ST7789_NOMAL            0xC8u
#define ST7789_RIGHT_TO_LEFT    0x68u
#define ST7789_LEFT_TO_RIGHT    0xA8u
#define ST7789_UPSIDE_DOWN      0x08u
#define ST7789_DIRECTION        ST7789_NOMAL

#if (USER_USE_SCR == USE_SCR_LCD_ST7789_130)
/* Screen's display direction */
#define ST7789_USE_HORIZONTAL 0

#define ST7789_SCR_X    240
#define ST7789_SCR_Y    240

#define IPS_ST7789_USE_SPI     1
#if  (IPS_ST7789_USE_SPI==1) 
#define IPS_ST7789_SPI        SPI1
#define IPS_ST7789_SPI_GPIO   GPIOA
#define IPS_ST7789_SPI_AF     GPIO_AF_SPI1   
#define IPS_ST7789_SCLK       GPIO_PIN_5       // PA
#define IPS_ST7789_MOSI       GPIO_PIN_7       // PA
#define IPS_ST7789_SCLK_SRC   GPIO_PinSource5  
#define IPS_ST7789_MOSI_SRC   GPIO_PinSource7  
#elif  (IPS_ST7789_USE_SPI==2) 
#define IPS_ST7789_SPI        SPI2
#define IPS_ST7789_SPI_GPIO   GPIOB
#define IPS_ST7789_SPI_AF     GPIO_AF_SPI2
#define IPS_ST7789_SCLK       GPIO_PIN_13       // PB
#define IPS_ST7789_MOSI       GPIO_PIN_15       // PB
#define IPS_ST7789_SCLK_SRC   GPIO_PinSource13  
#define IPS_ST7789_MOSI_SRC   GPIO_PinSource15 
#elif  (IPS_ST7789_USE_SPI==3) 
#define IPS_ST7789_SPI        SPI3
#define IPS_ST7789_SPI_AF     GPIO_AF_SPI3    
#define IPS_ST7789_SPI_GPIO   GPIOB
#define IPS_ST7789_SCLK       GPIO_PIN_3       // PB
#define IPS_ST7789_MOSI       GPIO_PIN_5       // PB
#define IPS_ST7789_SCLK_SRC   GPIO_PinSource3  
#define IPS_ST7789_MOSI_SRC   GPIO_PinSource5  
#endif

#if  (IPS_ST7789_USE_SPI != 0) && (USER_USE_SCR == USE_SCR_LCD_ST7789_130)
#define ST7789_DC          GPIO_PIN_1      // PA
#define ST7789_RST         GPIO_PIN_2      // PA
#define LCD_LEDK           GPIO_PIN_3      // PA

#define ST7789_DC_HIGH     GPIOA->BSRR = ST7789_DC
#define ST7789_DC_LOW      GPIOA->BSRR = ST7789_DC

#define ST7789_RST_HIGH    GPIOA->BSRR = ST7789_RST
#define ST7789_RST_LOW     GPIOA->BSRR = ST7789_RST

#define ST7789_BLK_HIGH    GPIOA->BSRR = LCD_LEDK
#define ST7789_BLK_LOW     GPIOA->BSRR = LCD_LEDK
#endif

#elif  (USER_USE_SCR == USE_SCR_LCD_ST7789_200)

#define ST7789_SCR_X    240
#define ST7789_SCR_Y    320

// use mcu interface
#define ST7789_CS                   GPIO_PIN_1  // PA
#define ST7789_DC                   GPIO_PIN_2  // PA
#define ST7789_WR                   GPIO_PIN_3  // PA
#define ST7789_RD                   GPIO_PIN_4  // PA
#define LCD_LEDK                    GPIO_PIN_8  // PA
#define ST7789_RST                  GPIO_PIN_0  // PB

#define LCD_ST7789_DAT_0            GPIO_PIN_8  // PB
#define LCD_ST7789_DAT_1            GPIO_PIN_9  // PB
#define LCD_ST7789_DAT_2            GPIO_PIN_10 // PB
#define LCD_ST7789_DAT_3            GPIO_PIN_11 // PB
#define LCD_ST7789_DAT_4            GPIO_PIN_12 // PB
#define LCD_ST7789_DAT_5            GPIO_PIN_13 // PB
#define LCD_ST7789_DAT_6            GPIO_PIN_14 // PB
#define LCD_ST7789_DAT_7            GPIO_PIN_15 // PB
#define LCD_ST7789_DAT_PIN          ( LCD_ST7789_DAT_0 | LCD_ST7789_DAT_1 | LCD_ST7789_DAT_2 | LCD_ST7789_DAT_3 | \
                                      LCD_ST7789_DAT_4 | LCD_ST7789_DAT_5 | LCD_ST7789_DAT_6 | LCD_ST7789_DAT_7 )
#define ST7789_CTL_GPIO             GPIOA
#define ST7789_DAT_GPIO             GPIOB

W25QXX_CS_PORT->BSRR = ((uint32_t)W25QXX_CS)<<16

#define ST7789_CS_HIGH              ST7789_CTL_GPIO->BSRR  = ((uint32_t)ST7789_CS)
#define ST7789_CS_LOW               ST7789_CTL_GPIO->BSRR  = ((uint32_t)ST7789_CS)<<16

#define ST7789_DC_HIGH              ST7789_CTL_GPIO->BSRR  = ((uint32_t)ST7789_DC)
#define ST7789_DC_LOW               ST7789_CTL_GPIO->BSRR  = ((uint32_t)ST7789_DC)<<16

#define ST7789_WR_HIGH              ST7789_CTL_GPIO->BSRR  = ((uint32_t)ST7789_WR)
#define ST7789_WR_LOW               ST7789_CTL_GPIO->BSRR  = ((uint32_t)ST7789_WR)<<16

#define ST7789_RD_HIGH              ST7789_CTL_GPIO->BSRR  = ((uint32_t)ST7789_RD)
#define ST7789_RD_LOW               ST7789_CTL_GPIO->BSRR  = ((uint32_t)ST7789_RD)<<16

#define ST7789_BLK_HIGH             ST7789_CTL_GPIO->BSRR  = ((uint32_t)LCD_LEDK)
#define ST7789_BLK_LOW              ST7789_CTL_GPIO->BSRR  = ((uint32_t)LCD_LEDK)<<16

#define ST7789_RST_HIGH             GPIOB->BSRR  = ((uint32_t)ST7789_RST)
#define ST7789_RST_LOW              GPIOB->BSRR  = ((uint32_t)ST7789_RST)<<16


#else       // 处理不使用屏幕的情况

#define ST7789_BLK_HIGH    
#define ST7789_BLK_LOW     

#define ST7789_RST_HIGH             
#define ST7789_RST_LOW              

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
        void lcd_st7789_set_region( uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye );
        void lcd_st7789_draw_point( uint16_t x, uint16_t y, uint16_t color );
        void lcd_st7789_send_pixel_dat( uint16_t color );
        void lcd_st7789_clear_with( uint16_t color );
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
