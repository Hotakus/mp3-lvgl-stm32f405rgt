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
void oled_i2c_init( I2C_TypeDef * I2Cx, u16 i2c_clk, uint8_t own_addr );

void oled_i2c_send_byte( uint8_t byte );
void oled_send( uint8_t slave_addr, uint8_t dat, OLED_OPT opt );


void oled_clean( void );
void oled_flush_with( uint8_t *gram );
void oled_set_pos( uint8_t x, uint8_t y, uint8_t val );
void oled_draw_point( uint8_t x, uint8_t y, uint8_t val );

void show_char( uint8_t *font_buf, uint8_t width, uint8_t hight, uint8_t x, uint8_t y );
void show_str( uint8_t *str, FONT_TYPE font_type, uint8_t x, uint8_t y );
void show_picture( uint8_t *pic, uint8_t width, uint8_t hight, uint8_t x, uint8_t y );

void oled_on( void );
void oled_off( void );
void oled_brightness( uint8_t lm );

#ifdef __cplusplus
}
#endif
		
#endif

