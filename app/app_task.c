#include "app_task.h"
#include "lvgl.h"
#include "pro_conf.h"
#include "stm32f4xx_conf.h"
#include "led.h"

static rt_thread_t u_threadx[APP_THREAD_NUM] = {RT_NULL};
static struct rt_thread u_static_threadx[APP_THREAD_NUM];

/* led闪烁线程 */
#define LED_THREAD_NAME         "led_blink"         // 线程名
#define LED_BLINK_STACK_SIZE    160                  // 线程栈大小
#define LED_BLINK_TIME_SLICE    1                   // 线程时间片
#define LED_BLINK_PRIOROTY      20                  // 线程优先级
static rt_thread_t *led_blink_th = &u_threadx[0];      // 从线程堆分配线程
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
#define LVGL_TICK_STACK_SIZE    160                // 线程栈大小
#define LVGL_TICK_TIME_SLICE    5                   // 线程时间片
#define LVGL_TICK_PRIOROTY      10                  // 线程优先级
static rt_thread_t *lvgl_tick_th = &u_threadx[1];      // 从线程堆分配线程
static void lvgl_tick_thread( void *param )
{
    param = param;
    while ( 1 ) {
        lv_tick_inc(LVGL_TICK);
        rt_thread_mdelay(LVGL_TICK);
    }
}

/* lvgl task handler线程 */
#define LVGL_TASK_THREAD_NAME   "lvgl_task"                         // 线程名
#define LVGL_TASK_STACK_SIZE    2048                                // 线程栈大小
#define LVGL_TASK_TIME_SLICE    10                                  // 线程时间片
#define LVGL_TASK_PRIOROTY      10                                  // 线程优先级
ALIGN(RT_ALIGN_SIZE) static u8 lvgl_task_stk[LVGL_TASK_STACK_SIZE]; // 线程栈
static struct rt_thread *lvgl_task_th_s = &u_static_threadx[3];     // 从线程堆分配线程
static void lvgl_task_thread( void *param )
{
    param = param;
    while ( 1 ) {
        lv_task_handler();
        rt_thread_mdelay(LVGL_TICK);
    }
}

/* sd卡检测线程 */
#define SD_DETECT_THREAD_NAME   "sd_detect"                         // 线程名
#define SD_DETECT_STACK_SIZE    512                                 // 线程栈大小
#define SD_DETECT_TIME_SLICE    10                                  // 线程时间片
#define SD_DETECT_PRIOROTY      9                                   // 线程优先级
#define SD_DETECT_TIMER_TIME    1000                                // 定时时间
ALIGN(RT_ALIGN_SIZE) static u8 sd_detect_stk[SD_DETECT_STACK_SIZE]; // 线程栈
static struct rt_thread *sd_detect_th_s = &u_static_threadx[4];     // 从线程堆分配线程
u8 sd_detect_flag = 0;
extern FATFS   fs_lv[2];
extern FRESULT fr_lv[2];
static void sd_detect_thread( void *param )
{
    param = param;
    while ( 1 ) {
        if ( SD_Detect() == SD_PRESENT ) {
            if ( !sd_detect_flag ) {
                sd_detect_flag = 1;
                if ( fr_lv[SD_SDIO_INDEX] != FR_OK ) {
                    fr_lv[SD_SDIO_INDEX] = f_mount( &fs_lv[SD_SDIO_INDEX], "SD_SDIO:", 1 );
                    if ( fr_lv[SD_SDIO_INDEX] != FR_OK ) {
                        DEBUG_PRINT( "sd card mounted error. (%d)\n", fr_lv[SD_SDIO_INDEX] );
                        goto rt_delay;
                    }
                }
                DEBUG_PRINT( "sd card inserted.\n" );
            }
        } else {
            if( sd_detect_flag ) {
                sd_detect_flag = 0;
                fr_lv[SD_SDIO_INDEX] = FR_NOT_READY;
                DEBUG_PRINT( "sd card ejected.\n" );
            }
        }
        rt_delay:
        rt_thread_mdelay(SD_DETECT_TIMER_TIME);
    }
}

int app_create_task( void )
{
    
    rt_err_t err = RT_ERROR;
    
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
    err = rt_thread_init(  
        lvgl_task_th_s,                 
        LVGL_TASK_THREAD_NAME,          /*线程名字*/ 
        lvgl_task_thread,               /*线程入口函数*/
        RT_NULL,                        /*线程入口函数*/
        lvgl_task_stk,                  /*线程栈*/
        LVGL_TASK_STACK_SIZE,           /*线程栈大小*/
        LVGL_TASK_PRIOROTY,
        LVGL_TASK_TIME_SLICE
    );
    if ( err == RT_EOK ) 
        rt_thread_startup (lvgl_task_th_s);
    else
        rt_kprintf( "create thread \"%s\" error. (%d)\n", LVGL_TASK_THREAD_NAME, err );

    /*  创建sd卡检测线程 */
    err = rt_thread_init(  
        sd_detect_th_s,                 
        SD_DETECT_THREAD_NAME,          /*线程名字*/ 
        sd_detect_thread,               /*线程入口函数*/
        RT_NULL,                        /*线程入口函数*/
        sd_detect_stk,                  /*线程栈*/
        SD_DETECT_STACK_SIZE,           /*线程栈大小*/
        SD_DETECT_PRIOROTY,
        SD_DETECT_TIME_SLICE
    );
    if ( err == RT_EOK ) 
        rt_thread_startup (sd_detect_th_s);
    else
        rt_kprintf( "create thread \"%s\" error. (%d)\n", SD_DETECT_THREAD_NAME, err );
    
    return 0;
}
