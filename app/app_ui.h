#ifndef APP_UI_H
#define APP_UI_H

/* includes */
#include "stm32f4xx_conf.h"
#include "app_typedef.h"
#include "pro_conf.h"

/* definations */
#define APP_UI_NUM      10      // app 的最大UI数量
#define APP_UI_LAYER    5       // app 的最大UI层级

typedef enum {
    APP_UI_REGISTER_OK      = 0 ,
    APP_UI_REGISTER_ERR         ,
    APP_UI_REGISTER_UI_FULL     ,
    
    APP_UI_UNREGISTER_OK      = 0 ,
    APP_UI_UNREGISTER_ERR         ,
} ui_register_stat;

/* functions */
#if defined(USE_LV_EX)
void app_create_example(void);
#endif
void app_create_ui( const char *ui_name );

ui_register_stat app_ui_register( const app_ui *ui );
ui_register_stat app_ui_unregister( const char *ui_name );

#endif
