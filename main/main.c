#include "main.h"

#define APP_THREAD_NUM  5
static rt_thread_t threadx[APP_THREAD_NUM] = {RT_NULL};

/* led闪烁线程 */
#define LED_THREAD_NAME         "led_blink"         // 线程名
#define LED_BLINK_STACK_SIZE    512                 // 线程栈大小
#define LED_BLINK_TIME_SLICE    1                   // 线程时间片
#define LED_BLINK_PRIOROTY      1                   // 线程优先级
static rt_thread_t *led_blink_t = &threadx[0];      // 从线程堆分配线程


static void led_blink_thread( void *param )
{
    DEBUG_PRINT( "param: %s\n", (u8*)param );
    
    while ( 1 ) {
        led_on( GPIOA, GPIO_Pin_8 );
        rt_thread_mdelay( 500 );
        led_off( GPIOA, GPIO_Pin_8 );
        rt_thread_mdelay( 500 );
    }
}

/* 设备初始化 */
static void device_init( void ) 
{
//    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_0 );
//    usart_conf( USART1, 2000000 );
//    delay_init();
    
    w25qxx_init();
    fatfs_test( "SD_SDIO" );
}

 
/* 主函数 */
int main () 
{
    device_init();
    
    led_conf( GPIOA, GPIO_Pin_8 );
    led_on( GPIOA, GPIO_Pin_8 );
    

    rt_thread_t t = rt_thread_find( "main" );
    
    DEBUG_PRINT( "current thread: %s.\n", t->name );
    
    /*  创建led闪烁线程 */
    *led_blink_t = rt_thread_create( 
        LED_THREAD_NAME,        /*线程名字*/                    
        led_blink_thread,       /*线程入口函数*/
        RT_NULL,                /*线程入口函数参数*/
        LED_BLINK_STACK_SIZE,   /*线程栈大小*/
        LED_BLINK_PRIOROTY ,    /*线程优先级*/
        LED_BLINK_TIME_SLICE    /*线程时间片*/
    );               
    
    if(led_blink_t !=RT_NULL)
        rt_thread_startup (*led_blink_t);
    else 
        return -1;
    
    while(1) {
        
        rt_thread_mdelay(500);
    }

}

