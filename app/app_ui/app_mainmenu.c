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

static app_ui app_mainmenu = {
    .ui_name = THIS_UI_NAME,
    .ctl_h = &ctl_handler,
};
static u8 register_flag = 0;
 /**********************
 *  FUNCTIONS
 **********************/
static void mainmenu_create(void)
{

    /*Create a LED and switch it OFF*/
lv_obj_t * led1 = lv_led_create(lv_scr_act(), NULL);
lv_obj_align(led1, NULL, LV_ALIGN_CENTER, -10, 0);
lv_led_off(led1);
/*Copy the previous LED and set a brightness*/
lv_obj_t * led2 = lv_led_create(lv_scr_act(), led1);
lv_obj_align(led2, NULL, LV_ALIGN_CENTER, 0, 0);
lv_led_set_bright(led2, 190);
/*Copy the previous LED and switch it ON*/
lv_obj_t * led3 = lv_led_create(lv_scr_act(), led1);
lv_obj_align(led3, NULL, LV_ALIGN_CENTER, 0, 0);
lv_led_on(led3);
    
}

static void mainmenu_remove(void)
{

}

app_ui *mainmenu_ui_get( void )
{
    return &app_mainmenu;
}

