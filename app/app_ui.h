#ifndef APP_UI_H
#define APP_UI_H

/* includes */
#include "stm32f4xx_conf.h"
#include "app_typedef.h"

/* definations */
#define USE_LV_EX

/* functions */
#if defined(USE_LV_EX)
void app_create_example(void);
#endif
void app_create_ui( const u8 *ui_name );

#endif
