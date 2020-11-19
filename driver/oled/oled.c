#include "oled.h"
#include "usart.h"
#include "oledfont.h"
#include "pro_conf.h"
#include "iic_conf.h"

/* OLED 显存 */
uint8_t OLED_GRAM[8][128];

/***************************
*   i2c init
****************************/
void oled_i2c_init( I2C_TypeDef * I2Cx, u16 i2c_clk, uint8_t own_addr ) {
    i2c_conf( I2Cx, i2c_clk, own_addr );

    oled_off();
    
	oled_send(OLED_ADDR, 0xA1,OLED_CMD);    //set segment remap 
	oled_send(OLED_ADDR, 0xA6,OLED_CMD);    //--normal / reverse
                                            
	oled_send(OLED_ADDR, 0xA8,OLED_CMD);    //--set multiplex ratio(1 to 64)
	oled_send(OLED_ADDR, 0x3F,OLED_CMD);    //--1/32 duty
                                            
	oled_send(OLED_ADDR, 0xC8,OLED_CMD);    //Com scan direction
	oled_send(OLED_ADDR, 0xD3,OLED_CMD);    //-set display offset
	oled_send(OLED_ADDR, 0x00,OLED_CMD);    //
                                            
	oled_send(OLED_ADDR, 0xD5,OLED_CMD);    //set osc division
	oled_send(OLED_ADDR, 0x80,OLED_CMD);    //
                                            
	oled_send(OLED_ADDR, 0xD8,OLED_CMD);    //set area color mode off
	oled_send(OLED_ADDR, 0x05,OLED_CMD);    //
                                            
	oled_send(OLED_ADDR, 0xD9,OLED_CMD);    //Set Pre-Charge Period
	oled_send(OLED_ADDR, 0xF1,OLED_CMD);    //
                                            
	oled_send(OLED_ADDR, 0xDA,OLED_CMD);    //set com pin configuartion
	oled_send(OLED_ADDR, 0x12,OLED_CMD);    //
                                            
	oled_send(OLED_ADDR, 0xDB,OLED_CMD);    //set Vcomh
	oled_send(OLED_ADDR, 0x30,OLED_CMD);    //
                                            
	oled_send(OLED_ADDR, 0x8D,OLED_CMD);    //set charge pump enable
	oled_send(OLED_ADDR, 0x14,OLED_CMD);    //

    oled_brightness( 0xFF );
	oled_on();
    
    /* 初始化GRAM */
    memset( OLED_GRAM, 0x00, sizeof(OLED_GRAM) );
    oled_flush_with( (uint8_t*)OLED_GRAM );

    show_char( (uint8_t*)&zh_ch[0], 16, 16, 70, 48 );
    show_char( (uint8_t*)&zh_ch[1], 16, 16, 70+16, 48 );
    show_char( (uint8_t*)&zh_ch[2], 16, 16, 70+32, 48 );
    
    show_picture( (uint8_t*)&IMG[0], 64, 32, 0, 0 );
    
    show_str( (uint8_t*)"Hello!!!", FONT_8x16, 0, 64-16 );

    oled_flush_with( (uint8_t*)OLED_GRAM );
}


/***************************
*   函数:       oled_i2c_send_byte
*   功能:       往OLED发送字节
*   byte:       字节
****************************/
void oled_i2c_send_byte( uint8_t byte ) {
    i2c_send_bytes( OLED_I2C, &byte, 1, 0xFFF );
}

/***************************
*   函数:       oled_send
*   功能:       往OLED发送命令或数据
*   slave_addr: OLED地址
*   dat:        命令或数据
*   opt:        命令或数据opt
****************************/
void oled_send( uint8_t slave_addr, uint8_t dat, OLED_OPT opt ) {

    ErrorStatus err = ERROR;

    err = i2c_generate_start( OLED_I2C, 0xFFF );
    if ( err != SUCCESS )
        return;

    i2c_send_7bitAddr( OLED_I2C, I2C_Direction_Transmitter, OLED_ADDR, 0xFFF );
    if ( err != SUCCESS )
        return;
    
    if ( opt == OLED_CMD ) {
        oled_i2c_send_byte( 0x00 );
    } else {
        oled_i2c_send_byte( 0x40 );
    }
    
    oled_i2c_send_byte( dat );
    
    i2c_generate_stop( OLED_I2C );
}


/***************************
*   函数:       oled_clean
*   功能:       清除屏幕
****************************/
void oled_clean( void ) {
    uint8_t x, page;
    
    oled_send( OLED_ADDR, 0x00, OLED_CMD );
    oled_send( OLED_ADDR, 0x10, OLED_CMD );
    
    for ( page = 0; page < 8; page++ ) {
        oled_send( OLED_ADDR, 0xB0|page, OLED_CMD );
        for ( x = 0; x < 128; x++ ) {
            oled_send( OLED_ADDR, 0, OLED_DATA );
        }
    }
}

/***************************
*   函数:       oled_flush_with
*   功能:       通过指定的OLED_GRAM刷新屏幕
*   gram:       GRAM首地址
****************************/
void oled_flush_with( uint8_t *gram ) {
    uint8_t x, page;

    oled_send( OLED_ADDR, 0x00, OLED_CMD );
    oled_send( OLED_ADDR, 0x10, OLED_CMD );

    for ( page = 0; page < 8; page++ ) {
        oled_send( OLED_ADDR, 0xB0|page, OLED_CMD );
        for ( x = 0; x < 128; x++ ) {
            oled_send( OLED_ADDR, *gram++, OLED_DATA );
        }
    }
}

/***************************
*   函数:       oled_set_pos
*   功能:       往OLED_GRAM写入点坐标
*   x:          x坐标
*   y:          y坐标
*   val:        写入的值，1 或 0
****************************/
void oled_set_pos( uint8_t x, uint8_t y, uint8_t val ) {
    if ( x >= 128 && y >= 64 )
        return;
    
    uint8_t page = y>>3;       // get page
    uint8_t pd   = 1<<(y%8);  // one column page data
    
    if ( val ) {
        OLED_GRAM[page][x] |= pd;
    } else {
        OLED_GRAM[page][x] &=~ pd;
    }
    
}

/***************************
*   函数:       oled_draw_point
*   功能:       画点并显示
*   x:          x坐标
*   y:          y坐标
*   val:        写入的值，1 或 0
****************************/
void oled_draw_point( uint8_t x, uint8_t y, uint8_t val ) {
    oled_set_pos( x, y, val );
    oled_flush_with( (uint8_t*)OLED_GRAM );
}

/***************************
*   函数:       show_char
*   功能:       调用指定字体显示字符(可显示中文)
*   width:      字符的宽度
*   hight:      字符的高度
*   x:          x坐标
*   y:          y坐标
****************************/
void show_char( uint8_t *font_buf, uint8_t width, uint8_t hight, uint8_t x, uint8_t y ) {
    
    uint8_t w, h;
    
    uint8_t page = y>>3;       // get page
    
    if ( page >= 8 )
        page = 7;
    
    for ( h = 0; h < (hight>>3); h++ ) {
        for ( w = 0; w < width; w++ ) {
            OLED_GRAM[page][x+w] |= *font_buf++;
        }
        page++;
    }
}

/***************************
*   函数:       show_str
*   功能:       调用指定字体显示字符串
*   str:        字符串指针
*   font_type:  调用的字体，在头文件中有定义
*   x:          x坐标
*   y:          y坐标
****************************/
void show_str( uint8_t *str, FONT_TYPE font_type, uint8_t x, uint8_t y ) {
    
    uint8_t pos = 0;

    while ( str[pos] != '\0' ) {
        if ( font_type == FONT_6x8 ) {
            show_char( (uint8_t*)&F6x8[str[pos]-' '], 6, 8, x+(pos<<3), y );
        } else if ( font_type == FONT_8x16 ) {
            show_char( (uint8_t*)&F8x16[str[pos]-' '], 8, 16, x+(pos<<3), y );
        }
        pos++;
    }
    
    oled_flush_with( (uint8_t*)OLED_GRAM );
}

/***************************
*   函数:       show_picture
*   功能:       显示任意尺寸图片
*   pic:        图片buf指针
*   width:      图片的宽度
*   hight:      图片的高度
*   x:          x坐标
*   y:          y坐标
****************************/
void show_picture( uint8_t *pic, uint8_t width, uint8_t hight, uint8_t x, uint8_t y ) {
    show_char( pic, width, hight, x, y );
}

/***************************
*   函数:       oled_on
*   功能:       打开OLED显示
****************************/
void oled_on( void ) {
    oled_send(OLED_ADDR, 0xAF,OLED_CMD);    //--turn on oled panel
}

/***************************
*   函数:       oled_off
*   功能:       关闭OLED显示
****************************/
void oled_off( void ) {
    oled_send(OLED_ADDR, 0xAE,OLED_CMD);    //--turn off oled panel
}

/***************************
*   函数:       oled_brightness
*   功能:       调节OLED亮度
****************************/
void oled_brightness( uint8_t lm ) {
    oled_send(OLED_ADDR, 0x81,OLED_CMD);    // set brightness
    oled_send(OLED_ADDR, lm  ,OLED_CMD);
}

