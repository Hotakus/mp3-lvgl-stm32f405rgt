#ifndef IPS_ST7789_ST7789_H
#define IPS_ST7789_ST7789_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"
#include "pro_conf.h"

/* Screen's size */
#define IPS_ST7789_W   240
#define IPS_ST7789_H   240

/* Screen's display direction */
#define USE_HORIZONTAL 0

/* definations */
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

#define IPS_ST7789_DC          GPIO_Pin_1      // PA
#define IPS_ST7789_RES         GPIO_Pin_2      // PA
#define IPS_ST7789_BLK         GPIO_Pin_3      // PA

#define IPS_ST7789_DC_HIGH     GPIOA->BSRRL |= IPS_ST7789_DC
#define IPS_ST7789_DC_LOW      GPIOA->BSRRH |= IPS_ST7789_DC

#define IPS_ST7789_RES_HIGH    GPIOA->BSRRL |= IPS_ST7789_RES
#define IPS_ST7789_RES_LOW     GPIOA->BSRRH |= IPS_ST7789_RES

#define IPS_ST7789_BLK_HIGH    GPIOA->BSRRL |= IPS_ST7789_BLK
#define IPS_ST7789_BLK_LOW     GPIOA->BSRRH |= IPS_ST7789_BLK


typedef enum {
    ST7789_CMD  = 0,
    ST7789_DAT     ,
} ST7789_DC_OPT;

/* 设置点的命令 */

/* functions */
/* 初始函数 */
        void ips_st7789_init( void );
/* 绘图函数 */
        void ips_st7789_set_region( u16 xs, u16 ys, u16 xe, u16 ye );
        void ips_st7789_draw_point( u16 x, u16 y, u16 color );
        void ips_st7789_send_pixel_dat( u16 color );
        void ips_st7789_clear_with( u16 color );
/* 其他功能函数 */
        void ips_st7789_sw_reset( void );
        void ips_st7789_hw_reset( void );
        void ips_st7789_displayOn( void );
        void ips_st7789_displayOff( void );
        void ips_st7789_powerDown( void );
        void ips_st7789_powerUp( void );
		
#ifdef __cplusplus
}
#endif
		
#endif
