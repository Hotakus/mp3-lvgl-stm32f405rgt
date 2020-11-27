/************************************************
 * @file app_folder.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-26
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "app_folder.h"

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME    "folder"

#define ICONS_W         70
#define ICONS_H         70

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void folder_create(void);
static void folder_remove(void);

 /**********************
 *  STATIC VARIABLE
 **********************/
static app_ui_ctl_handler ctl_handler = {
    .create = folder_create,
    .remove = folder_remove,
};

static app_ui_t app_folder = {
    .ui_name = THIS_UI_NAME,
    .ctl_h = &ctl_handler,
};


static lv_obj_t * obj_container = NULL;           // 顶层容器

static icon_t sd_folder = {
    .en_name  = "SD Card",       
    .rel_path = "S:/icons/folder/sd_icon_rel.bin",
    .pr_path  = "S:/icons/folder/sd_icon_pr.bin" ,
};
static icon_t spif_folder = {
    .en_name  = "FLASH",       
    .rel_path = "S:/icons/folder/spif_icon_rel.bin",
    .pr_path  = "S:/icons/folder/spif_icon_pr.bin" ,
};

static lv_obj_t * sd_cap_bar = NULL;
static lv_obj_t * spif_cap_bar = NULL;

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
 * @brief 创建folder
 ************************************************/
static void folder_create(void)
{

    uint32_t sd_total = 0;
    uint32_t sd_free  = 0;
    double   sd_per;

    uint32_t spif_total = 0;
    uint32_t spif_free  = 0;
    double   spif_per;
    
    char lab_buf[50] = {0};

    obj_container = lv_cont_create( lv_scr_act(), NULL );
    lv_obj_set_size( obj_container, 320, 220 );
    lv_obj_set_pos( obj_container, 0, 20 );
    lv_obj_set_style_local_bg_opa( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0 );
    lv_obj_set_style_local_border_opa( obj_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0 );

    /* SD卡目录 */
    sd_folder.btn = lv_imgbtn_create( obj_container, NULL );
    lv_imgbtn_set_src( sd_folder.btn, LV_BTN_STATE_RELEASED, sd_folder.rel_path );
    lv_imgbtn_set_src( sd_folder.btn, LV_BTN_STATE_PRESSED, sd_folder.pr_path );
    lv_obj_set_pos( sd_folder.btn, 30, 15 );
    lv_obj_set_event_cb( sd_folder.btn, event_handler );
    sd_folder.label = lv_label_create( obj_container, NULL );
    lv_obj_align( sd_folder.label, sd_folder.btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0 );
    lv_obj_set_style_local_text_color( sd_folder.label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE );
    lv_obj_set_style_local_text_font( 
        sd_folder.label, 
        LV_LABEL_PART_MAIN, 
        LV_STATE_DEFAULT, 
        &ariblk_16
    );
    lv_fs_free_space( 'S', &sd_total, &sd_free );
    sprintf( lab_buf, 
        "%s\nCap : %02.2f\nFree : %02.2f",
        sd_folder.en_name,
        ( (double)sd_total/(1<<20) ),
        ( (double)sd_free/(1<<20) )
    );
    lv_label_set_text( sd_folder.label, lab_buf );
    lv_label_set_align( sd_folder.label, LV_LABEL_ALIGN_CENTER );

    /* SPI FLASH目录 */
    spif_folder.btn = lv_imgbtn_create( obj_container, sd_folder.btn );
    lv_imgbtn_set_src( spif_folder.btn, LV_BTN_STATE_RELEASED, spif_folder.rel_path );
    lv_imgbtn_set_src( spif_folder.btn, LV_BTN_STATE_PRESSED,  spif_folder.pr_path );
    lv_obj_set_pos( spif_folder.btn, 190, 15 );
    lv_obj_set_event_cb( spif_folder.btn, event_handler );
    spif_folder.label = lv_label_create( obj_container, sd_folder.label );
    lv_obj_align( spif_folder.label, spif_folder.btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0 );
    lv_fs_free_space( 'F', &spif_total, &spif_free );
    MEMSET( lab_buf, 0, sizeof(lab_buf)/sizeof(uint8_t) );
    sprintf( lab_buf, 
        "%s\nCap : %02.2f\nFree : %02.2f",
        spif_folder.en_name,
        ( (double)spif_total/(1<<20) ),
        ( (double)spif_free/(1<<20) )
    );
    lv_label_set_text( spif_folder.label, lab_buf );
    lv_label_set_align( spif_folder.label, LV_LABEL_ALIGN_CENTER );


    /* 创建 可视化 容量显示条 */
    sd_per   = (double)(sd_total-sd_free) / (double)sd_total;
    spif_per = (double)(spif_total-spif_free) / (double)spif_total;

    sd_cap_bar = lv_bar_create( obj_container, NULL );
    lv_obj_set_size( sd_cap_bar, 128, 20 );
    lv_bar_set_range( sd_cap_bar, 0, 1000 );
    lv_bar_set_anim_time( sd_cap_bar, 500 );
    lv_bar_set_value( sd_cap_bar, 1000*sd_per, LV_ANIM_ON );
    lv_obj_set_style_local_bg_color( sd_cap_bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_MAKE( 0x88, 0xab, 0xba ) );
    lv_obj_set_style_local_radius( sd_cap_bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, 7 );
    lv_obj_set_style_local_radius( sd_cap_bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, 7 );
    lv_obj_align( sd_cap_bar, sd_folder.label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0 );

    spif_cap_bar = lv_bar_create( obj_container, sd_cap_bar );
    lv_obj_set_size( spif_cap_bar, 128, 20 );
    lv_bar_set_range( spif_cap_bar, 0, 1000 );
    lv_bar_set_anim_time( spif_cap_bar, 500 );
    lv_bar_set_value( spif_cap_bar, 1000*spif_per, LV_ANIM_ON );
    lv_obj_align( spif_cap_bar, spif_folder.label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0 );
    
}

/************************************************
 * @brief 移除folder
 ************************************************/
static void folder_remove(void)
{
    lv_obj_del( obj_container );
}

app_ui_t *folder_ui_get( void )
{
    return &app_folder;
}

