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
lv_obj_t *background_img;
lv_obj_t *background_img_2;

/************************************************
 * @brief 创建mainmenu
 ************************************************/
static void mainmenu_create(void)
{
    LV_IMG_DECLARE( sd );
//    background_img = lv_img_create( lv_scr_act(), NULL );
//    lv_img_set_src(background_img, "S:/.mp3_icon/sd.bin" );
//    lv_obj_align(background_img, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    
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

