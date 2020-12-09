/************************************************
 * @file screen_conf.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef SCREEN_CONF_H
#define SCREEN_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "screen_conf.h"
#include "stm32f4xx_hal.h"
#include "lcd_st7735.h"
#include "lcd_st7789.h"

/* function */
typedef struct {
    void        ( *init )            ( void );
    
    void        ( *set_region )      ( uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye );

    void        ( *draw_pixel )      ( uint16_t x, uint16_t y, uint16_t color );
    void        ( *send_pixel_dat )  ( uint16_t color );
    
    void        ( *clear )           ( uint16_t color );
    
    void        ( *hw_reset )        ( void );
    void        ( *sw_reset )        ( void );
    
    void        ( *display_on )      ( void );
    void        ( *display_off )     ( void );
    
    void        ( *power_down )      ( void );
    void        ( *power_up )        ( void );
    
    uint16_t SCR_X_MAX;
    uint16_t SCR_Y_MAX;
    
} scr_opr_handler;


scr_opr_handler *scr_get_opr_handler(void);
    

#ifdef __cplusplus
}
#endif

#endif
