/************************************************
 * @file main.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pro_conf.h"

/* std */
#include "stm32f4xx_conf.h"
#include "arm_math.h"

/* driver include files */
#include "usart.h"
#include "systick.h"
#include "w25qxx.h"
#include "rtc.h"
#include "spi_conf.h"
#include "nvic_conf.h"
#include "led.h"
#include "ff_user.h"
#include "screen_conf.h"
#include "file_trans.h"

#if USER_USE_APP == 1
#include "app_main.h"
#endif
    
/* functions */



#ifdef __cplusplus
}
#endif

#endif
