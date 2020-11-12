#ifndef APP_UI_H
#define APP_UI_H

/* includes */
#include "stm32f4xx_conf.h"
#include "app_typedef.h"
#include "pro_conf.h"

/* ui */
#include "app_mainmenu.h"

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
void app_ui_init(void);
app_ui *app_get_ui_obj_s(void);

#if USE_LV_EX == 1
void app_create_example(void);
#endif

void app_create_ui( const char *ui_name );
void app_delete_ui( const char *ui_name );

ui_register_stat app_ui_register( app_ui *ui );
ui_register_stat app_ui_unregister( app_ui *ui );

#endif
