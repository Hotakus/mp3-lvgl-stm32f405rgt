#include "app_ui.h"


/**********************
 *  STATIC PROTOTYPES
 **********************/
 static void lv_ex_label_1(void);
 static void lv_ex_spinner_1(void);
 
 
/**********************
 *  STATIC VARIABLE
 **********************/
static u8 reg_ui_num = 0;
static app_ui *app_ui_s[APP_UI_NUM];

 
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
ui_register_stat app_ui_register( app_ui *ui )
{

    if ( reg_ui_num == APP_UI_NUM )
        return APP_UI_REGISTER_UI_FULL;
    
    for ( u8 i = 0; i < APP_UI_NUM; i++ )  {
        if ( app_ui_s[i] != NULL )
            continue;
        
        app_ui_s[i] = ui;
        
        reg_ui_num += 1;
        return APP_UI_REGISTER_OK;
    }
    
    return APP_UI_REGISTER_OK;
}

ui_register_stat app_ui_unregister( app_ui *ui )
{
    for ( u8 i = 0; i < APP_UI_NUM; i++ ) {
        if ( STRCMP( app_ui_s[i]->ui_name, ui->ui_name ) != 0 )
            continue;
        app_ui_s[i] = NULL;
        reg_ui_num -= 1;
        return APP_UI_UNREGISTER_OK;
    }
    
    return APP_UI_UNREGISTER_OK;
}

/************************************************
 * @brief 根据ui名创建已注册的ui
 * 
 * @param ui_name 
 ************************************************/
void app_create_ui( const char *ui_name )
{

    for ( u8 i = 0; i < APP_UI_NUM; i++ )  {
        if ( app_ui_s[i]->ui_name == NULL )
            continue;
        if ( STRCMP( app_ui_s[i]->ui_name, ui_name ) == 0 )
            app_ui_s[i]->ctl_h->create();
        break;
    }

}

/************************************************
 * @brief 根据ui名删除已注册的ui
 * 
 * @param ui_name 
 ************************************************/
void app_delete_ui( const char *ui_name )
{
    for ( u8 i = 0; i < APP_UI_NUM; i++ )  {
        if ( STRCMP( app_ui_s[i]->ui_name, ui_name ) == 0 ) {
            app_ui_s[i]->ctl_h->remove();
            return;
        }
    }
}

/************************************************
 * @brief 创建所有已注册的ui
 ************************************************/
void app_ui_init(void)
{
    
    for ( u8 i = 0; i < APP_UI_NUM; i++ ) {
        app_ui_s[i] = NULL;
    }

    app_ui_register( mainmenu_ui_get() );
    

    for ( u8 i = 0; i < APP_UI_NUM; i++ )  {
        if ( app_ui_s[i] == NULL )
            continue;
        app_ui_s[i]->ctl_h->create();
    }
}


