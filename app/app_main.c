#include "app_main.h"
#include "pro_conf.h"

static void lvgl_init( void ) 
{
    lv_init();
    lv_port_disp_init();        // 显示器初始化
    lv_port_indev_init();       // 输入设备初始化
    lv_port_fs_init();          // 文件系统设备初始化
}

int app_init( void )
{
    lvgl_init();
    
#if defined(USE_LV_EX)
    app_create_example();
#endif
    
    app_create_task();
    
    return 0;
    
}
INIT_APP_EXPORT(app_init);  // 导出为APP，rtthread启动时会自动调用这个函数执行


