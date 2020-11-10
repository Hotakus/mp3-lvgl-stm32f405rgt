#include "app_ui.h"
#include "pro_conf.h"

/**********************
 *  STATIC PROTOTYPES
 **********************/

void lv_ex_label_1(void)
{
    lv_obj_t * label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_recolor(label2, true);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SROLL_CIRC); /*Circular scroll*/
    lv_obj_set_width(label2, 120);
    lv_label_set_text(label2, "#ff0000 Hello# #00ff00 world ! Suhouyu.#");
    lv_obj_align(label2, NULL, LV_ALIGN_CENTER, 0, 0);
}

void lv_ex_spinner_1(void)
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

#if defined(USE_LV_EX)
void app_create_example(void)
{
    lv_ex_label_1();
    lv_ex_spinner_1();
}
#endif

void app_create_ui( const u8 *ui_name )
{

    
    
    
}

