#include "app_main.h"
#include "pro_conf.h"

extern FATFS   fs_lv[2];
extern FRESULT fr_lv[2];

static struct rt_thread app_init_thread;
ALIGN(4) static u8 app_init_stk[APP_INIT_STK_SIZE]; 

static void lvgl_init( void ) 
{
    lv_init();
    lv_port_disp_init();        // 显示器初始化
    lv_port_indev_init();       // 输入设备初始化
    lv_port_fs_init();          // 文件系统设备初始化
}

void app_init( void *param )
{
    lvgl_init();
    
#if defined(USE_LV_EX)
    app_create_example();
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
}
INIT_APP_EXPORT(app_create_init_thread);  // 导出为APP，rtthread启动时会自动调用这个函数执行


