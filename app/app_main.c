#include "app_main.h"
#include "pro_conf.h"

/************************************************
 * @brief EXTERN VARIABLE
 ************************************************/
extern FATFS   fs_lv[2];
extern FRESULT fr_lv[2];

/************************************************
 * @brief STATIC VARIABLE
 ************************************************/
static struct rt_thread app_init_thread;
ALIGN(4) static uint8_t app_init_stk[APP_INIT_STK_SIZE]; 

/************************************************
 * @brief FUNCTION PROPOTYPE
 ************************************************/
static void lvgl_init( void );


/************************************************
 * @brief FUNCTION REALIZED
 ************************************************/
static void lvgl_init( void ) 
{
    lv_init();
    lv_port_disp_init();        // 显示器初始化
//    lv_port_indev_init();       // 输入设备初始化
    lv_port_fs_init();          // 文件系统设备初始化
}

/************************************************
 * @brief APP 入口函数
 * 
 * @param param 
 ************************************************/
void app_init( void *param )
{
    lvgl_init();
    
#if USE_LV_EX == 1
    app_create_example();
#else
    app_ui_init();
#endif

    app_create_task();
}

int app_create_init_thread(void)
{
    rt_err_t err = rt_thread_init(
        &app_init_thread, 
        "app_init_th",
        app_init,
        RT_NULL,
        app_init_stk,
        APP_INIT_STK_SIZE,
        1, 1
    );
    if ( err == RT_EOK ) {
        rt_thread_startup( &app_init_thread );
    } else {
        rt_kprintf( "app init err.\n" );
        rt_thread_detach( &app_init_thread );
        return -1;
    }
    return 0;
}
INIT_APP_EXPORT(app_create_init_thread);  // 导出为APP，rtthread启动时会自动调用这个函数执行


