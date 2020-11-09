#include "app_ui.h"
#include "pro_conf.h"

/**********************
 *  STATIC PROTOTYPES
 **********************/

void lv_ex_label_1(void)
{
//    lv_obj_t * label1 = lv_label_create(lv_scr_act(), NULL);
//    lv_label_set_long_mode(label1, LV_LABEL_LONG_BREAK); /*Break the long lines*/
//    lv_label_set_recolor(label1, true); /*Enable re-coloring by␣,!commands in the text*/
//    lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER); /*Center aligned lines*/
//    lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label " "#00ff00 and# wrap long text automatically.");
//    lv_obj_set_width(label1, 120);
//    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, -0);
    
    
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
    lv_obj_align(preload, NULL, LV_ALIGN_CENTER, 0, 0);
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

