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
#include "ips_st7789.h"

static scr_opr_handler scr_opr_s = {
#if (USER_USE_SCR == USE_SCR_IPS_ST7789_ST7789)
    .init               = ips_st7789_init,

    .set_region         = ips_st7789_set_region,
    .draw_pixel         = ips_st7789_draw_point,
    .send_pixel_dat     = ips_st7789_send_pixel_dat,
    .clear              = ips_st7789_clear_with,

    .hw_reset           = ips_st7789_hw_reset,
    .sw_reset           = ips_st7789_sw_reset,

    .display_on         = ips_st7789_displayOn,
    .display_off        = ips_st7789_displayOff,
        
    .power_down         = ips_st7789_powerDown,
    .power_up           = ips_st7789_powerUp,
    
    .SCR_X_MAX          = IPS_ST7789_W,
    .SCR_Y_MAX          = IPS_ST7789_H,
#elif (USER_USE_SCR == USE_SCR_LCD_144_ST7735)
    .init               = lcd_144_init,
    
    .set_region         = lcd_144_set_region,
    
    .draw_pixel         = lcd_144_draw_pixel,
    .send_pixel_dat     = lcd_144_send_pixel_dat,
    
    .clear              = lcd_144_clear_with,
    
    .hw_reset           = lcd_144_hw_reset,
    .sw_reset           = NULL,
    
    .display_on         = lcd_144_displayOn,
    .display_off        = lcd_144_displayOff,
    
    .power_down         = lcd_144_powerDown,
    .power_up           = lcd_144_powerUp,
    
    .SCR_X_MAX          = 128,
    .SCR_Y_MAX          = 128,
#endif
};

scr_opr_handler *scr_get_opr_handler(void)
{
    return &scr_opr_s;
}

