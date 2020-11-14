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
#include "pro_conf.h"
#include "stm32f4xx_conf.h"
#include "lcd_144_st7735.h"
#include "lcd_st7789.h"

/* function */
typedef struct {
    void        ( *init )            ( void );
    
    void        ( *set_region )      ( u16 xs, u16 ys, u16 xe, u16 ye );

    void        ( *draw_pixel )      ( u16 x, u16 y, u16 color );
    void        ( *send_pixel_dat )  ( u16 color );
    
    void        ( *clear )           ( u16 color );
    
    void        ( *hw_reset )        ( void );
    void        ( *sw_reset )        ( void );
    
    void        ( *display_on )      ( void );
    void        ( *display_off )     ( void );
    
    void        ( *power_down )      ( void );
    void        ( *power_up )        ( void );
    
    u16 SCR_X_MAX;
    u16 SCR_Y_MAX;
    
} scr_opr_handler;


scr_opr_handler *scr_get_opr_handler(void);
    

#ifdef __cplusplus
}
#endif

#endif
