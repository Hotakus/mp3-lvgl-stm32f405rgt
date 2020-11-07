#include "main.h"

/* 设备初始化 */
void device_init( void ) 
{
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_0 );
    usart_conf( USART1, 2000000 );
    delay_init();
    
    w25qxx_init();
    
//    fatfs_test( "SPIF" );
    fatfs_test( "SD_SDIO" );
}

/* 主函数 */
int main () 
{
    device_init();
    
    led_conf( GPIOA, GPIO_Pin_8 );
    led_on( GPIOA, GPIO_Pin_8 );
    while(1){
        led_on( GPIOA, GPIO_Pin_8 );
        delay_ms(500);
        led_off( GPIOA, GPIO_Pin_8 );
        delay_ms(500);
    }

}

