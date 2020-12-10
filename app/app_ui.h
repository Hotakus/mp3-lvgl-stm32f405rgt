#ifndef APP_UI_H
#define APP_UI_H

/* includes */
#include "stm32f4xx_hal.h"
#include "app_typedef.h"

/* ui */
#include "app_mainmenu.h"
#include "app_status_bar.h"

/* definations */


typedef enum {
    APP_UI_REGISTER_OK      = 0 ,
    APP_UI_REGISTER_ERR         ,
    APP_UI_REGISTER_UI_FULL     ,
    
    APP_UI_UNREGISTER_OK    = 0 ,
    APP_UI_UNREGISTER_ERR       ,
} ui_register_stat;

/* functions */
void app_ui_init(void);
app_ui_managed_t *app_get_ui_layer_b(void);

#if USE_LV_EX == 1
void app_create_example(void);
#endif

void app_create_ui( app_ui_t *ui );
void app_ui_return( void );

ui_register_stat app_ui_register( app_ui_t *ui );
ui_register_stat app_ui_unregister( app_ui_t *ui );

app_ui_t *app_find_ui( const char *ui_name );

#endif
