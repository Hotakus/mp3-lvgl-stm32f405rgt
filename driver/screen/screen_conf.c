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

static scr_opr_handler scr_opr_s = {
#if (USER_USE_SCR == USE_SCR_LCD_144_ST7735)
    .init           = lcd_144_init,

    .set_region     = lcd_144_set_region,

    .draw_pixel     = lcd_144_draw_pixel,
    .send_pixel_dat = lcd_144_send_pixel_dat,

    .clear          = lcd_144_clear_with,

    .hw_reset       = lcd_144_hw_reset,
    .sw_reset       = NULL,

    .display_on     = lcd_144_displayOn,
    .display_off    = lcd_144_displayOff,

    .power_down     = lcd_144_powerDown,
    .power_up       = lcd_144_powerUp,
    
    .SCR_X_MAX          = 128,
    .SCR_Y_MAX          = 128,
#endif
};

scr_opr_handler *scr_get_opr_handler(void)
{
    return &scr_opr_s;
}

