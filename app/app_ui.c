#include "app_ui.h"


/**********************
 *  STATIC VARIABLE
 **********************/
static u8 reg_ui_num = 0;
static app_ui app_ui_s[APP_UI_NUM];

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void lv_ex_label_1(void)
{
    lv_obj_t * label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_recolor(label2, true);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SROLL_CIRC); /*Circular scroll*/
    lv_obj_set_width(label2, 120);
    lv_label_set_text(label2, "#ff0000 Hello# #00ff00 world ! Suhouyu.#");
    lv_obj_align(label2, NULL, LV_ALIGN_CENTER, 0, 0);
}

static void lv_ex_spinner_1(void)
{
    /*Create a Preloader object*/
    lv_obj_t * preload = lv_spinner_create(lv_scr_act(), NULL);
    lv_obj_set_size(preload, 50, 50);
    lv_obj_align(preload, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    lv_obj_t * preload2 = lv_spinner_create(lv_scr_act(), NULL);
    lv_obj_set_size(preload2, 50, 50);
    lv_obj_align(preload2, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    
    lv_obj_t * preload3 = lv_spinner_create(lv_scr_act(), NULL);
    lv_obj_set_size(preload3, 50, 50);
    lv_obj_align(preload3, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    
    lv_obj_t * preload4 = lv_spinner_create(lv_scr_act(), NULL);
    lv_obj_set_size(preload4, 50, 50);
    lv_obj_align(preload4, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    
    lv_obj_t * preload5 = lv_spinner_create(lv_scr_act(), NULL);
    lv_obj_set_size(preload5, 50, 50);
    lv_obj_align(preload5, NULL, LV_ALIGN_CENTER, 0, 0);
    
}

#if USE_LV_EX == 1
void app_create_example(void)
{
    lv_ex_label_1();
    lv_ex_spinner_1();
}
#endif

/************************************************
 * @brief 注册ui
 * 
 * @param ui 
 * @return ui_register_stat 
 ************************************************/
ui_register_stat app_ui_register( const app_ui *ui )
{
    u8 i = 0;
    
    if ( reg_ui_num == APP_UI_NUM )
        return APP_UI_REGISTER_UI_FULL;
    
    for ( i = 0; i < APP_UI_NUM; i++ ) {
        if ( app_ui_s[i].ui_name != NULL )
            continue;
        
        app_ui_s[i].ui_name         = ui->ui_name;
        app_ui_s[i].ctl_h->create   = ui->ctl_h->create;
        app_ui_s[i].ctl_h->remove   = ui->ctl_h->remove;
        
        reg_ui_num += 1;
        return APP_UI_REGISTER_OK;
    }
    
    return APP_UI_REGISTER_OK;
}

ui_register_stat app_ui_unregister( const char *ui_name )
{
    u8 i = 0;

    for ( i = 0; i < APP_UI_NUM; i++ ) {
        if ( STRCMP( app_ui_s[i].ui_name, ui_name ) != 0 )
            return APP_UI_UNREGISTER_OK;
        app_ui_s[i].ui_name = NULL;
        app_ui_s[i].ctl_h   = NULL;
        reg_ui_num -= 1;
        return APP_UI_UNREGISTER_OK;
    }
    
    return APP_UI_UNREGISTER_OK;
}

void app_create_ui( const char *ui_name )
{

    
    
    
}

