#include "main.h"

#define APP_INIT_STK_SIZE   (1<<10)
static struct rt_thread app_init_thread;
ALIGN(4) static u8 app_init_stk[APP_INIT_STK_SIZE]; 

int main () 
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
        return -1;
    }
}


