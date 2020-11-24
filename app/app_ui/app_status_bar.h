/************************************************
 * @file app_status_bar.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-23
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef APP_STATUS_BAR_H
#define APP_STATUS_BAR_H

/* includes */
#include "stm32f4xx_conf.h"
#include "app_typedef.h"
#include "pro_conf.h"
#include "app_ui.h"

/* definations */
#define BATTERY_CALCULATE_CNT       10
#define FULL_CHARGE                 2.05

#define NO_BATTERY      1

/* functions */
app_ui_t *status_bar_ui_get( void );

void status_bar_time_update( void );
void status_bar_attach_info_update( const char *attach_info );
void status_bar_bat_info_update( uint8_t now );

void attach_info_create( void );
void attach_info_delete( void );

void time_info_create( void );
void time_info_delete( void );

uint16_t battery_info_statistic( void );
void battery_info_create( void );
void battery_info_delete( void );

#endif
