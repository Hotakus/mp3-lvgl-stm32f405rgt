/************************************************
 * @file app_ui.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-23
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
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

static const char * mp3_sd_bg_path      = "S:/icons/mp3_bg.bin";
static lv_obj_t * mp3_backgroud = NULL;


#if USE_LV_EX == 1
void lv_ex_bar_1(void)
{
    lv_obj_t * bar1 = lv_bar_create(lv_scr_act(), NULL);
    lv_obj_set_size(bar1, 128, 20);
    lv_bar_set_range( bar1, 0, 10000 );
    lv_bar_set_anim_time( bar1, 500 );
    lv_obj_align(bar1, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_bar_set_value(bar1, 3215, LV_ANIM_ON);

    lv_obj_set_style_local_bg_color( bar1, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE( 0x88, 0xab, 0xba ) );
    lv_obj_set_style_local_radius( bar1, LV_BAR_PART_BG, LV_STATE_DEFAULT, 7 );
    lv_obj_set_style_local_radius( bar1, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, 7 );
}

void app_create_example(void)
{

    // lv_obj_t * label = lv_label_create( lv_scr_act(), NULL );
    // lv_label_set_text( label, "fetive.\nfestive2d\nfestivsafxe3" );
    // lv_label_set_align( label, LV_LABEL_ALIGN_CENTER );

    lv_ex_bar_1();

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
    if ( ui == NULL )
        return APP_UI_REGISTER_ERR;
    // 如果已注册ui数量大于等于APP_UI_NUM则返回已满状态
    if ( aums.reg_ui_num >= APP_UI_NUM )
        return APP_UI_REGISTER_UI_FULL;
    
    for ( uint8_t i = 0; i < APP_UI_NUM; i++ )  {
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
    if ( ui == NULL )
        return APP_UI_UNREGISTER_ERR;
    for ( uint8_t i = 0; i < APP_UI_NUM; i++ ) {
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
    for ( uint8_t i = 0; i < APP_UI_NUM; i++ )  {
        if ( aums.app_ui_s[i] == NULL )
            continue;
        if ( aums.app_ui_s[i]  == ui ) {
            aums.app_ui_layer[aums.next_ui_layer] = aums.app_ui_s[i];
            aums.app_ui_layer[aums.next_ui_layer]->ctl_h->create();
            aums.cur_ui_layer = aums.next_ui_layer;
            aums.next_ui_layer += 1;
            return;
        } else
            continue;
    }
    DEBUG_PRINT( "No ui object to create: %s\n", ui->ui_name );
}

/************************************************
 * @brief 层级管理块返回上一层
 * 
 * @param ui 
 ************************************************/
void app_ui_return( void )
{
    /* 防止顶层UI被删除 */
    if ( aums.next_ui_layer == 0 )
        return;
    if ( aums.cur_ui_layer == 0 )
        return;
    
    aums.app_ui_layer[aums.cur_ui_layer]->ctl_h->remove();     // 删除当前层界面
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
    app_ui_t *ui = NULL;

    for ( uint8_t i = 0; i < APP_UI_NUM; i++ ) {
        aums.app_ui_s[i] = NULL;
    }

    /* 创建背景 */
    LV_IMG_DECLARE(mp3_bg);
    mp3_backgroud = lv_img_create( lv_scr_act(), NULL );
    lv_img_set_src( mp3_backgroud, mp3_sd_bg_path );
    //lv_img_set_src( mp3_backgroud, &mp3_bg );
    lv_obj_align(mp3_backgroud, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    
    /* 创建状态栏 */
    ui = status_bar_ui_get();
    app_ui_register( ui );
    ui->ctl_h->create();        // 常驻ui不装入ui控制块

    /* 创建mainmenu */
    ui = mainmenu_ui_get();
    app_ui_register( ui );
    app_create_ui( ui );

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

/************************************************
 * @brief 通过ui名找到已注册ui
 * 
 * @param ui_name 
 * @return app_ui_t* 
 ************************************************/
app_ui_t *app_find_ui( const char *ui_name )
{
    return NULL;
}
