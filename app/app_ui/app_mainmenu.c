/************************************************
 * @file app_mainmenu.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-26
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "app_mainmenu.h"

#include "app_music.h"
#include "app_calendar.h"
#include "app_folder.h"
#include "app_calculator.h"
#include "app_env.h"
#include "app_setting.h"

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME    "mainmenu"
#define ICONS_ROW       2
#define ICONS_COL       3

#define ICONS_W         70
#define ICONS_H         70

#define ICONS_MARGIN_X  20
#define ICONS_MARGIN_Y  30

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

/************************************************
 * @brief icons[x][y]
 * x : row
 * y : col
 ************************************************/
static icon_t icons[ICONS_ROW][ICONS_COL] = {
    /* row 1 */
    {
        {
            .en_name  = "music",       
            .cn_name  = "音乐",
            .rel_path = "S:/icons/mainmenu/music_icon_rel.bin",
            .pr_path  = "S:/icons/mainmenu/music_icon_pr.bin" ,
        },
        {
            .en_name  = "calendar",
            .cn_name  = "日历",
            .rel_path = "S:/icons/mainmenu/calendar_icon_rel.bin",
            .pr_path  = "S:/icons/mainmenu/calendar_icon_pr.bin" ,
        },
        {
            .en_name  = "folder",
            .cn_name  = "文件夹",
            .rel_path = "S:/icons/mainmenu/folder_icon_rel.bin",
            .pr_path  = "S:/icons/mainmenu/folder_icon_pr.bin" ,
        },
    },
    /* row 2 */
    {
        {
            .rel_path = NULL,
            .pr_path  = NULL,
        },
        {
            .rel_path = NULL,
            .pr_path  = NULL,
        },
        {
            .rel_path = NULL,
            .pr_path  = NULL,
        },
    },

};

static lv_obj_t * obj_container = NULL;           // icons顶层容器

 /**********************
 *  FUNCTIONS
 **********************/
/************************************************
 * @brief 事件句柄
 * 
 * @param obj       
 * @param event 
 ************************************************/
static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    app_ui_t * ui = NULL;
    switch (event) {
    case LV_EVENT_CLICKED:
        if ( STRCMP( obj->user_data, "music" ) == 0 ) {
            ui = music_ui_get();
        } else if ( STRCMP( obj->user_data, "calendar" ) == 0 ) {
            ui = calendar_ui_get();
        } else if ( STRCMP( obj->user_data, "folder" ) == 0 ) {
            ui = folder_ui_get();
        } else if ( STRCMP( obj->user_data, "calculator" ) == 0 ) {
            ui = calculator_ui_get();
        } else if ( STRCMP( obj->user_data, "evn" ) == 0 ) {
            ui = env_ui_get();
        } else if ( STRCMP( obj->user_data, "setting" ) == 0 ) {
            ui = setting_ui_get();
        }
        break;
    default:
        break;
    }

    if ( event == LV_EVENT_CLICKED && ui != NULL ) {
        app_ui_register( ui );
        app_create_ui( ui );
    }
    
}

/************************************************
 * @brief 创建mainmenu
 ************************************************/
static void mainmenu_create(void)
{

    /* 创建mainmenu的icons的容器 */
    obj_container = lv_cont_create( lv_scr_act(), NULL );
    /* 计算容器的size */
    lv_obj_set_size( obj_container, 
        ICONS_COL*ICONS_W + ICONS_MARGIN_X*(ICONS_COL-1), 
        ICONS_ROW*ICONS_H + ICONS_MARGIN_Y*(ICONS_ROW-1) 
    );
    lv_obj_set_pos( obj_container, 35, 40 );
    lv_obj_set_style_local_bg_opa( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0 );
    lv_obj_set_style_local_border_opa( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0 );
    
    for ( uint8_t row = 0; row < ICONS_ROW; row++ ) {
        for ( uint8_t col = 0; col < ICONS_COL; col++ ) {
            /* 找到合法成员 */
            if( icons[row][col].rel_path == NULL || icons[row][col].pr_path == NULL )
                continue;
            /* 创建imgbtn并自动对齐 */
            icons[row][col].btn = lv_imgbtn_create( obj_container, NULL );
            icons[row][col].btn->user_data = icons[row][col].en_name;
            lv_imgbtn_set_src( icons[row][col].btn, LV_BTN_STATE_RELEASED, icons[row][col].rel_path );
            lv_imgbtn_set_src( icons[row][col].btn, LV_BTN_STATE_PRESSED, icons[row][col].pr_path );
            lv_obj_set_pos( icons[row][col].btn, (ICONS_W+ICONS_MARGIN_X)*col, (ICONS_H+ICONS_MARGIN_Y)*row );
            lv_obj_set_event_cb( icons[row][col].btn, event_handler );
            /* 为每个icon创建标签 */
            icons[row][col].label = lv_label_create( obj_container, NULL );
            lv_obj_set_style_local_text_font( 
                icons[row][col].label, 
                LV_LABEL_PART_MAIN, 
                LV_STATE_DEFAULT, 
                &ariblk_14
            );
            lv_obj_set_style_local_text_color( icons[row][col].label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE );
            lv_label_set_text( icons[row][col].label, icons[row][col].en_name );
            lv_obj_align(icons[row][col].label, icons[row][col].btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        }
    }
}

/************************************************
 * @brief 移除mainmenu
 ************************************************/
static void mainmenu_remove(void)
{
//    for ( uint8_t row = 0; row < ICONS_ROW; row++ ) {
//        for ( uint8_t col = 0; col < ICONS_COL; col++ ) {
//            /* 找到合法成员 */
//            if ( icons[row][col].btn != NULL ) {
//                lv_obj_del( icons[row][col].btn );
//                icons[row][col].btn->user_data = NULL;
//            }
//            if ( icons[row][col].label != NULL )
//                lv_obj_del( icons[row][col].label );
//        }
//    }
    lv_obj_del( obj_container );
}

app_ui_t *mainmenu_ui_get( void )
{
    return &app_mainmenu;
}

