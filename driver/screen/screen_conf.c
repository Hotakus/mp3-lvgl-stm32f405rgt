/************************************************
 * @file screen_conf.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "screen_conf.h"
#include "pro_conf.h"
#include "lcd_st7789.h"

static scr_opr_handler scr_opr_s = {
#if (USER_USE_SCR == USE_SCR_LCD_ST7789_130) || (USER_USE_SCR == USE_SCR_LCD_ST7789_200)
    .init               = lcd_st7789_init,

    .set_region         = lcd_st7789_set_region,
    .draw_pixel         = lcd_st7789_draw_point,
    .send_pixel_dat     = lcd_st7789_send_pixel_dat,
    .clear              = lcd_st7789_clear_with,

    .hw_reset           = lcd_st7789_hw_reset,
    .sw_reset           = lcd_st7789_sw_reset,

    .display_on         = lcd_st7789_displayOn,
    .display_off        = lcd_st7789_displayOff,
        
    .power_down         = lcd_st7789_powerDown,
    .power_up           = lcd_st7789_powerUp,
    
    .SCR_X_MAX          = ST7789_SCR_X,
    .SCR_Y_MAX          = ST7789_SCR_X,
#elif (USER_USE_SCR == USE_SCR_LCD_144_ST7735)
    .init               = lcd_st7735_init,
    
    .set_region         = lcd_st7735_set_region,
    
    .draw_pixel         = lcd_st7735_draw_pixel,
    .send_pixel_dat     = lcd_st7735_send_pixel_dat,
    
    .clear              = lcd_st7735_clear_with,
    
    .hw_reset           = lcd_st7735_hw_reset,
    .sw_reset           = NULL,
    
    .display_on         = lcd_st7735_displayOn,
    .display_off        = lcd_st7735_displayOff,
    
    .power_down         = lcd_st7735_powerDown,
    .power_up           = lcd_st7735_powerUp,
    
    .SCR_X_MAX          = 128,
    .SCR_Y_MAX          = 128,
#endif
};

scr_opr_handler *scr_get_opr_handler(void)
{
    return &scr_opr_s;
}

