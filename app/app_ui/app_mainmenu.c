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

static app_ui_t app_mainmenu = {
    .ui_name = THIS_UI_NAME,
    .ctl_h = &ctl_handler,
};

static lv_point_t tileview_valid_pos[] = {
    {0, 0}, 
    {0, 1}
};
static lv_obj_t * tileview = NULL;

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
    
    tileview = lv_tileview_create( lv_scr_act(), NULL );
    lv_obj_set_size( tileview, 128, 108 );
    lv_obj_set_pos( tileview, 0, 20 );
    lv_tileview_set_valid_positions( tileview, tileview_valid_pos, 2 );
    lv_tileview_set_edge_flash(tileview, true);
    
    lv_obj_t * tile1 = lv_obj_create(tileview, NULL);
    lv_obj_set_size(tile1, 128, 108);
    lv_tileview_add_element(tileview, tile1);
    
    /*Tile1: just a label*/
    lv_obj_t * label = lv_label_create(tile1, NULL);
    lv_label_set_text(label, "Scroll down");
    lv_obj_set_size( label, 128, 108 );
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    
//    lv_obj_t * list = lv_list_create(tileview, NULL);
//    lv_obj_set_size( list, 128, 108 );
//    lv_obj_set_pos(list, 0, 108);
//    lv_list_set_scroll_propagation(list, true);
//    lv_list_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_DRAG);
//    lv_list_add_btn(list, NULL, "One");
//    lv_list_add_btn(list, NULL, "Two");
//    lv_list_add_btn(list, NULL, "Three");
    
    lv_obj_t * preload = lv_spinner_create(tileview, NULL);
    lv_obj_set_pos(preload, 0, 128);
    lv_obj_set_size(preload, 50, 80);
}

/************************************************
 * @brief 移除mainmenu
 ************************************************/
static void mainmenu_remove(void)
{
    
    
    
    
}

app_ui_t *mainmenu_ui_get( void )
{
    return &app_mainmenu;
}

