#include "app_ui.h"


/**********************
 *  STATIC PROTOTYPES
 **********************/
 
/**********************
 *  STATIC VARIABLE
 **********************/
/************************************************
 * @brief 层级管理块
 ************************************************/
static app_ui_managed_t aums = {
    .reg_ui_num     = 0,
    .cur_ui_layer   = 0,
    .next_ui_layer  = 0,
};

#if USE_LV_EX == 1
static void event_handler(lv_obj_t * obj, lv_event_t event);
static void lv_ex_label_1(void);
static void lv_ex_spinner_1(void);

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        printf("Clicked\n");
    } else if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Toggled\n");
    }
}

static void lv_ex_label_1(void)
{
    lv_obj_t * label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_recolor(label2, true);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SROLL_CIRC); /*Circular scroll*/
    lv_obj_set_width(label2, 240);
    lv_label_set_text(label2, "#ff0000 Hello# #00ff00 world !# Trisuborn.  #ff7700 Hello# #00ffff world !# Trisuborn.");
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
    
}

void lv_ex_btn_1(void)
{
    lv_obj_t * btn2 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn2, event_handler);
    lv_btn_set_checkable(btn2, true);
    lv_obj_set_size( btn2, 50, 50 );
    lv_btn_toggle(btn2);
    lv_obj_align(btn2, NULL, LV_ALIGN_CENTER, 0, 0);
    
    lv_obj_t * label;
    label = lv_label_create(btn2, NULL);
    lv_label_set_recolor( label, true );
    lv_label_set_text(label, "#ff0000 Togg##00ff00 led#");
}

void app_create_example(void)
{
//    lv_ex_label_1();
//    lv_ex_spinner_1();
    
    lv_ex_btn_1();
    
}
#endif

/************************************************
 * @brief 注册ui
 * 
 * @param ui 
 * @return ui_register_stat 
 ************************************************/
ui_register_stat app_ui_register( app_ui_t *ui )
{

    if ( aums.reg_ui_num == APP_UI_NUM )
        return APP_UI_REGISTER_UI_FULL;
    
    for ( u8 i = 0; i < APP_UI_NUM; i++ )  {
        if ( aums.app_ui_s[i] != NULL )
            continue;
        
        aums.app_ui_s[i] = ui;
        
        aums.reg_ui_num += 1;
        return APP_UI_REGISTER_OK;
    }
    
    return APP_UI_REGISTER_OK;
}

/************************************************
 * @brief 注销ui
 * 
 * @param ui 
 * @return ui_register_stat 
 ************************************************/
ui_register_stat app_ui_unregister( app_ui_t *ui )
{
    for ( u8 i = 0; i < APP_UI_NUM; i++ ) {
        if ( aums.app_ui_s[i] != ui )
            continue;
        aums.app_ui_s[i] = NULL;
        aums.reg_ui_num -= 1;
        return APP_UI_UNREGISTER_OK;
    }
    
    return APP_UI_UNREGISTER_OK;
}

/************************************************
 * @brief 从已注册的ui对象找到相应的ui对象后 
 *        将ui对象装入ui层级管理块并创建 
 * 
 * @param ui 
 ************************************************/
void app_create_ui( app_ui_t *ui )
{
    for ( u8 i = 0; i < APP_UI_NUM; i++ )  {
        if ( aums.app_ui_s[i] == NULL )
            continue;
        if ( aums.app_ui_s[i]  == ui ) {
            aums.app_ui_layer[aums.next_ui_layer] = aums.app_ui_s[i];
            aums.app_ui_layer[aums.next_ui_layer]->ctl_h->create();
            aums.cur_ui_layer = aums.next_ui_layer;
            aums.next_ui_layer += 1;
        } else
            DEBUG_PRINT( "No ui object: %s\n", ui->ui_name );
        return;
    }
}

/************************************************
 * @brief 层级管理块返回上一层
 * 
 * @param ui 
 ************************************************/
void app_ui_return( void )
{
    
    if ( aums.next_ui_layer == 0 )
        return;
    if ( aums.cur_ui_layer == 0 )
        return;
    
    aums.app_ui_layer[aums.cur_ui_layer]->ctl_h->remove();
    aums.app_ui_layer[aums.cur_ui_layer] = NULL;
    aums.next_ui_layer -= 1;
    aums.cur_ui_layer  -= 1;
    aums.app_ui_layer[aums.cur_ui_layer]->ctl_h->create();     // 创建上一层界面

}

/************************************************
 * @brief 创建mainmenu 和 可配置相关ui组件
 ************************************************/
void app_ui_init(void)
{
    
    for ( u8 i = 0; i < APP_UI_NUM; i++ ) {
        aums.app_ui_s[i] = NULL;
    }

    /* 创建mainmenu */
    app_ui_register( mainmenu_ui_get() );
    app_create_ui( mainmenu_ui_get() );
    
    DEBUG_PRINT( "%s\n", aums.app_ui_layer[aums.cur_ui_layer]->ui_name );
    
    //lv_ex_spinner_1();

}

/************************************************
 * @brief 获取ui层级管理块 app_ui_managed_t aums
 * 
 * @return app_ui_managed_t* 
 ************************************************/
app_ui_managed_t *app_get_ui_layer_b(void)
{
    return &aums;
}

