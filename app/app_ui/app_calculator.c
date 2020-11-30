/************************************************
 * @file app_calculator.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-26
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "app_calculator.h"

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME    "calculator"

typedef enum opr_ch_mode_t {
    OPR_CH_MODE_NOMAL = 0,
    OPR_CH_MODE_PROG     ,
} opr_ch_mode_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void calculator_create(void);
static void calculator_remove(void);

static void opr_ch_mode_alter( opr_ch_mode_t mode );

 /**********************
 *  STATIC VARIABLE
 **********************/
static app_ui_ctl_handler ctl_handler = {
    .create = calculator_create,
    .remove = calculator_remove,
};

static app_ui_t app_calculator = {
    .ui_name = THIS_UI_NAME,
    .ctl_h = &ctl_handler,
};


static lv_obj_t * obj_container = NULL;             // 顶层容器
static lv_obj_t * mode_dropdown = NULL;             // 模式选择下拉菜单

/* 结果显示区 */
static lv_obj_t * res_show_area = NULL; // 顶层容器
static lv_obj_t * res_history = NULL;
static lv_obj_t * res_current = NULL;

/* 通用按键区 */
static lv_obj_t * com_btn_area = NULL;      // 顶层容器

static opr_ch_mode_t opr_ch_mode = OPR_CH_MODE_NOMAL;   // 计算符号列表模式
static lv_obj_t * opr_ch_list = NULL;                   // 计算符号列表
static lv_obj_t * opr_ch_obj[4] = {NULL};               // 计算符号实体
static lv_obj_t * nor_ext_ch_obj[4] = {NULL};           // 普通模式拓展符号实体
static lv_obj_t * prog_ext_ch_obj[4] = {NULL};          // 程序员模式拓展符号实体

static lv_obj_t * num_area = NULL;          // 数字容器
static lv_obj_t * num_obj[12] = {NULL};     // 数字实体

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

        break;
    default:
        break;
    }

}

/************************************************
 * @brief 创建calculator
 ************************************************/
static void calculator_create(void)
{
    obj_container = lv_cont_create( lv_scr_act(), NULL );
    lv_obj_set_size( obj_container, 320, 220 );
    lv_obj_set_pos( obj_container, 0, 20 );
    lv_obj_set_style_local_radius( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 10 );
    lv_obj_set_style_local_bg_color( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE( 0x90, 0x42, 0x42 ) );
    lv_obj_set_style_local_border_width( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 2 );
    lv_obj_set_style_local_border_color( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE( 0x4d, 0x16, 0x16 ) );

    res_show_area = lv_cont_create( obj_container, NULL );
    res_history   = lv_label_create( res_show_area, NULL );
    res_current   = lv_label_create( res_show_area, NULL );

    com_btn_area  = lv_cont_create( obj_container, NULL );
    num_area      = lv_cont_create( com_btn_area, NULL );
    for ( uint8_t n = 0; n < (sizeof(num_obj)/sizeof(num_obj[0])); n++ ) {
        num_obj[n] = lv_btn_create( num_area, NULL );
    }

    opr_ch_list = lv_page_create( com_btn_area, NULL );
    opr_ch_mode_alter( opr_ch_mode );

}

/************************************************
 * @brief 移除calculator
 ************************************************/
static void calculator_remove(void)
{
    lv_obj_del( obj_container );
    
}

app_ui_t *calculator_ui_get( void )
{
    return &app_calculator;
}

/************************************************
 * @brief 更改计算器模式
 * 
 * @param mode 
 ************************************************/
static void opr_ch_mode_alter( opr_ch_mode_t mode )
{

}
