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
 static void app_not_ui_handler(void);          // 无ui时跳转到这个handler

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

static const char * mp3_sd_bg_path      = "S:/mp3_icons/mp3_bg.bin";
static lv_obj_t * mp3_backgroud = NULL;


#if USE_LV_EX == 1
static void event_handler(lv_obj_t * obj, lv_event_t event);
static void lv_ex_label_1(void);
static void lv_ex_spinner_1(void);

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    printf( "event : %d\n", event );
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

void lv_ex_cpicker_1(void)
{
    lv_obj_t * cpicker;
    cpicker = lv_cpicker_create(lv_scr_act(), NULL);
    lv_obj_set_size(cpicker, 128, 128);
    lv_obj_align(cpicker, NULL, LV_ALIGN_CENTER, 0, 0);
}

void lv_ex_tileview_1(void)
{
    static lv_point_t valid_pos[] = {{0,0}, {0, 1}, {1,0}};
    lv_obj_t *tileview;
    tileview = lv_tileview_create(lv_scr_act(), NULL);
    lv_tileview_set_valid_positions(tileview, valid_pos, 3);
    lv_tileview_set_edge_flash(tileview, true);
    lv_obj_set_style_local_bg_color( tileview, LV_TILEVIEW_PART_EDGE_FLASH, LV_STATE_DEFAULT, LV_COLOR_BLUE );
    
    lv_obj_t * tile1 = lv_obj_create(tileview, NULL);
    lv_obj_set_size(tile1, LV_HOR_RES, LV_VER_RES);
    lv_tileview_add_element(tileview, tile1);
    
    /*Tile1: just a label*/
    lv_obj_t * label = lv_label_create(tile1, NULL);
    lv_label_set_text(label, "Scroll down");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    
    /*Tile2: a list*/
    lv_obj_t * list = lv_list_create(tileview, NULL);
    lv_obj_set_size(list, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(list, 0, LV_VER_RES);
    lv_list_set_scroll_propagation(list, true);
    lv_list_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_DRAG);
    lv_list_add_btn(list, NULL, "One");
    lv_list_add_btn(list, NULL, "Two");
    lv_list_add_btn(list, NULL, "Three");
//    lv_list_add_btn(list, NULL, "Four");
//    lv_list_add_btn(list, NULL, "Five");
//    lv_list_add_btn(list, NULL, "Six");
//    lv_list_add_btn(list, NULL, "Seven");
//    lv_list_add_btn(list, NULL, "Eight");
//    /*Tile3: a button*/
    lv_obj_t * tile3 = lv_obj_create(tileview, tile1);
    lv_obj_set_pos(tile3, LV_HOR_RES, LV_VER_RES);
    lv_tileview_add_element(tileview, tile3);
    
    lv_obj_t * btn = lv_btn_create(tile3, NULL);
    lv_obj_align(btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_tileview_add_element(tileview, btn);
    label = lv_label_create(btn, NULL);
    lv_label_set_text(label, "No scroll up");
    
    lv_obj_set_event_cb( tileview, event_handler );
    
    lv_tileview_set_anim_time( tileview, 50 );
    lv_list_set_anim_time( list, 50 );
    
}

void lv_ex_win_1(void)
{
    /*Create a window*/
    lv_obj_t * win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "Window title"); /*Set the title*/
    /*Add control button to the header*/
    lv_obj_t * close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE); /*Add␣
    ,!close button and use built-in close action*/
    lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);
    
    lv_win_add_btn(win, LV_SYMBOL_SETTINGS); /*Add a setup button*/
    /*Add some dummy content*/
    
    lv_obj_t * txt = lv_label_create(win, NULL);
    lv_label_set_text(
        txt, 
        "This is the content of the window"
        "You can add control buttons to\n"
        "the window header\n\n"
        "The content area becomes\n"
        "automatically scrollable is it's \n"
        "large enough.\n\n"
        " You can scroll the content\n"
        "See the scroll bar on the right!"
    );
    
    lv_label_set_long_mode( txt, LV_LABEL_LONG_SROLL );
    
    lv_win_set_header_height( win, 30);
    lv_win_set_drag( win, true );
    
}

void lv_ex_slider_1(void)
{
/*Create a slider*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_align(slider, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
    
    lv_obj_set_size( slider, 100, 50 );
    lv_obj_set_style_local_bg_color( slider, LV_SLIDER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_CYAN );
    lv_obj_set_style_local_bg_color( slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_ORANGE );
    lv_obj_set_style_local_bg_color( slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_COLOR_PURPLE );


}

void app_create_example(void)
{
//    lv_ex_label_1();
//    lv_ex_spinner_1();
    
//    lv_ex_btn_1();

    LV_IMG_DECLARE( music_icon_rel );
    LV_IMG_DECLARE( music_icon_pr );
    LV_IMG_DECLARE( Calendar_icon_rel );
    LV_IMG_DECLARE( Calendar_icon_pr );
    LV_IMG_DECLARE( mp3_demo );


    lv_ex_win_1();

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
    if ( ui == NULL )
        return APP_UI_UNREGISTER_ERR;
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
    
    aums.app_ui_layer[aums.cur_ui_layer]->ctl_h->remove();     // 删除上一层界面
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

    for ( u8 i = 0; i < APP_UI_NUM; i++ ) {
        aums.app_ui_s[i] = NULL;
    }

    
    /* 创建背景 */
    mp3_backgroud = lv_img_create( lv_scr_act(), NULL );
    lv_img_set_src( mp3_backgroud, mp3_sd_bg_path );
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

}
