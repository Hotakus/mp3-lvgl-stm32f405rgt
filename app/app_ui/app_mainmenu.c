#include "app_mainmenu.h"

/**********************
 *  DEFINE
 **********************/
#define THIS_UI_NAME    "mainmenu"
#define ICONS_ROW       2
#define ICONS_COL       3

#define ICONS_W  70
#define ICONS_H  70

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
 * @brief icons_path[x][y]
 * x : row
 * y : col
 ************************************************/
static icon_t icons_path[ICONS_ROW][ICONS_COL] = {
    /* row 1 */
    {
        {
            .name     = "music",
            .rel_path = "S:/mp3_icons/music_icon_rel.bin",
            .pr_path  = "S:/mp3_icons/music_icon_pr.bin" ,
        },
        {
            .name     = "calendar",
            .rel_path = "S:/mp3_icons/Calendar_icon_rel.bin",
            .pr_path  = "S:/mp3_icons/Calendar_icon_pr.bin" ,
        },
        {
            .rel_path = NULL,
            .pr_path  = NULL,
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

static lv_obj_t * icons_container = NULL;
static lv_obj_t * icons_obj[ICONS_ROW][ICONS_COL];

 /**********************
 *  FUNCTIONS
 **********************/
/************************************************
 * @brief 创建mainmenu
 ************************************************/
static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    
    if(event == LV_EVENT_CLICKED) {
        printf("Clicked\n");
    } else if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Toggled\n");
    }
}

static void mainmenu_create(void)
{

//    lv_obj_t * img = lv_img_create( lv_scr_act(), NULL );
//    lv_img_set_src( img, "S:/earp.bin" );
//    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);

    
    icons_container = lv_cont_create( lv_scr_act(), NULL );
    /* 计算容器的size */
    lv_obj_set_size( icons_container, 
        ICONS_COL*ICONS_W + ICONS_MARGIN_X*(ICONS_COL-1), 
        ICONS_ROW*ICONS_H + ICONS_MARGIN_Y*(ICONS_ROW-1) 
    );
    lv_obj_set_pos( icons_container, 35, 40 );
    lv_obj_set_style_local_bg_opa( icons_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0 );
    lv_obj_set_style_local_border_opa( icons_container, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0 );
    
    lv_obj_t * music_btn = lv_imgbtn_create( icons_container, NULL );
    lv_imgbtn_set_src( music_btn, LV_BTN_STATE_RELEASED, icons_path[0][0].rel_path );
    lv_imgbtn_set_src( music_btn, LV_BTN_STATE_PRESSED, icons_path[0][0].pr_path );
    lv_obj_align(music_btn, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    
}

/************************************************
 * @brief 移除mainmenu
 ************************************************/
static void mainmenu_remove(void)
{
    lv_obj_del( icons_container );
}

app_ui_t *mainmenu_ui_get( void )
{
    return &app_mainmenu;
}

