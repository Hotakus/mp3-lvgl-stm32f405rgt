#include "main.h"

#define APP_THREAD_NUM  5
static rt_thread_t threadx[APP_THREAD_NUM] = {RT_NULL};

/* led闪烁线程 */
#define LED_THREAD_NAME         "led_blink"         // 线程名
#define LED_BLINK_STACK_SIZE    128                  // 线程栈大小
#define LED_BLINK_TIME_SLICE    1                   // 线程时间片
#define LED_BLINK_PRIOROTY      20                  // 线程优先级
static rt_thread_t *led_blink_th = &threadx[0];      // 从线程堆分配线程
static void led_blink_thread( void *param )
{
    param = param;
    led_conf( GPIOA, GPIO_Pin_8 );
    led_on( GPIOA, GPIO_Pin_8 );
    while ( 1 ) {
        led_on( GPIOA, GPIO_Pin_8 );
        rt_thread_mdelay( 500 );
        led_off( GPIOA, GPIO_Pin_8 );
        rt_thread_mdelay( 500 );
    }
}

/* lvgl tick线程 */
#define LVGL_TICK   10
#define LVGL_TICK_THREAD_NAME   "lvgl_tick"         // 线程名
#define LVGL_TICK_STACK_SIZE    128                // 线程栈大小
#define LVGL_TICK_TIME_SLICE    5                   // 线程时间片
#define LVGL_TICK_PRIOROTY      10                  // 线程优先级
static rt_thread_t *lvgl_tick_th = &threadx[1];      // 从线程堆分配线程
static void lvgl_tick_thread( void *param )
{
    param = param;
    while ( 1 ) {
        lv_tick_inc(LVGL_TICK);
        rt_thread_mdelay(LVGL_TICK);
    }
}

/* lvgl task handler线程 */
#define LVGL_TASK_THREAD_NAME   "lvgl_task"         // 线程名
#define LVGL_TASK_STACK_SIZE    2048                // 线程栈大小
#define LVGL_TASK_TIME_SLICE    10                   // 线程时间片
#define LVGL_TASK_PRIOROTY      10                  // 线程优先级
static rt_thread_t *lvgl_task_th = &threadx[2];      // 从线程堆分配线程
static void lvgl_task_thread( void *param )
{
    param = param;
    while ( 1 ) {
        lv_task_handler();
        rt_thread_mdelay(LVGL_TICK<<1);
    }
}


void lvgl_init( void ) 
{
    lv_init();
    lv_port_disp_init();        // 显示器初始化
//    lv_port_indev_init();       // 输入设备初始化
//    lv_port_fs_init();          // 文件系统设备初始化
}

void lv_ex_label_1(void)
{
    lv_obj_t * label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_BREAK); /*Break the long lines*/
    lv_label_set_recolor(label1, true); /*Enable re-coloring by␣,!commands in the text*/
    lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER); /*Center aligned lines*/
    lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label " "#00ff00 and# wrap long text automatically.");
    lv_obj_set_width(label1, 120);
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, -0);
    lv_obj_t * label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SROLL_CIRC); /*Circular scroll*/
    lv_obj_set_width(label2, 120);
    lv_label_set_text(label2, "It is a circularly scrolling text. ");
    lv_obj_align(label2, NULL, LV_ALIGN_CENTER, 0, 0);
}

/* 主函数 */
int main () 
{

    lvgl_init();

    lv_ex_label_1();
    
    /*  创建led闪烁线程 */
    *led_blink_th = rt_thread_create( 
        LED_THREAD_NAME,        /*线程名字*/                    
        led_blink_thread,       /*线程入口函数*/
        RT_NULL,                /*线程入口函数参数*/
        LED_BLINK_STACK_SIZE,   /*线程栈大小*/
        LED_BLINK_PRIOROTY ,    /*线程优先级*/
        LED_BLINK_TIME_SLICE    /*线程时间片*/
    );               
    if(led_blink_th !=RT_NULL)
        rt_thread_startup (*led_blink_th);
    else
        return -1;
    
    /*  创建lvgl tick线程 */
    *lvgl_tick_th = rt_thread_create( 
        LVGL_TICK_THREAD_NAME,        /*线程名字*/                    
        lvgl_tick_thread,             /*线程入口函数*/
        RT_NULL,                      /*线程入口函数参数*/
        LVGL_TICK_STACK_SIZE,         /*线程栈大小*/
        LVGL_TICK_PRIOROTY ,          /*线程优先级*/
        LVGL_TICK_TIME_SLICE          /*线程时间片*/
    );               
    if(lvgl_tick_th !=RT_NULL)
        rt_thread_startup (*lvgl_tick_th);
    else
        return -1;
    
    /*  创建lvgl task线程 */
    *lvgl_task_th = rt_thread_create( 
        LVGL_TASK_THREAD_NAME,        /*线程名字*/                    
        lvgl_task_thread,             /*线程入口函数*/
        RT_NULL,                      /*线程入口函数参数*/
        LVGL_TASK_STACK_SIZE,         /*线程栈大小*/
        LVGL_TASK_PRIOROTY ,          /*线程优先级*/
        LVGL_TASK_TIME_SLICE          /*线程时间片*/
    );               
    if(lvgl_task_th !=RT_NULL)
        rt_thread_startup (*lvgl_task_th);
    else
        return -1;
    
    while( 1 ) {
        rt_thread_mdelay(5000);
    }
}

