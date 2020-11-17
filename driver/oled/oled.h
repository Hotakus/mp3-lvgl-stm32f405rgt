#ifndef OLED_H
#define OLED_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_conf.h"
#include "iic_conf.h"
#include <stdio.h>

/* definations */
#define OLED_I2C    I2C1
#define OLED_ADDR   0x78

typedef enum {
    OLED_CMD     =0,
    OLED_DATA    ,
} OLED_OPT;

typedef enum {
    FONT_6x8     = 0,
    FONT_8x16       ,
} FONT_TYPE;


/* functions */
void oled_i2c_init( I2C_TypeDef * I2Cx, u16 i2c_clk, u8 own_addr );

void oled_i2c_start( I2C_TypeDef * I2Cx );
void oled_i2c_stop( I2C_TypeDef * I2Cx );

void oled_i2c_send_byte( u8 byte );
void oled_send( u8 slave_addr, u8 dat, OLED_OPT opt );


void oled_clean( void );
void oled_flush_with( u8 *gram );
void oled_set_pos( u8 x, u8 y, u8 val );
void oled_draw_point( u8 x, u8 y, u8 val );

void show_char( u8 *font_buf, u8 width, u8 hight, u8 x, u8 y );
void show_str( u8 *str, FONT_TYPE font_type, u8 x, u8 y );
void show_picture( u8 *pic, u8 width, u8 hight, u8 x, u8 y );

void oled_on( void );
void oled_off( void );
void oled_brightness( u8 lm );

#ifdef __cplusplus
}
#endif
		
#endif

