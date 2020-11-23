#include "app_mainmenu.h"

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME   "mainmenu"

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void mainmenu_create(void);
static void mainmenu_remove(void);

 /**********************
 *  STATIC VARIABLE
 **********************/
static app_ui_ctl_handler ctl_handler = {
    .create = mainmenu_create,
    .remove = mainmenu_remove,
};

static app_ui_t app_mainmenu = {
    .ui_name = THIS_UI_NAME,
    .ctl_h = &ctl_handler,
};

 /**********************
 *  FUNCTIONS
 **********************/
/************************************************
 * @brief 创建mainmenu
 ************************************************/
static void mainmenu_create(void)
{
    lv_obj_t * preload = lv_spinner_create(lv_scr_act(), NULL);
    lv_obj_set_size(preload, 50, 50);
    lv_obj_align(preload, NULL, LV_ALIGN_CENTER, 0, 0);
    
}

/************************************************
 * @brief 移除mainmenu
 ************************************************/
static void mainmenu_remove(void)
{
    
    
    
    app_ui_return();
}

app_ui_t *mainmenu_ui_get( void )
{
    return &app_mainmenu;
}

