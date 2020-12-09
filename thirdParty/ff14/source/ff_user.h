/************************************************
 * @file ff_user.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-10
 * 
 * @copyright Copyright (c) 2020
 * 
*************************************************/
#ifndef __FF_USER_H
#define __FF_USER_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_hal.h"
#include "uart.h"
#include "ff_user_typedef.h"


/* functions */
FRESULT exf_getfree ( TCHAR *path, uint32_t *total, uint32_t *free );
FRESULT scan_catalog( TCHAR *path, SCAN_OPT opt );
FRESULT show_file_info( TCHAR *path );

list_file_t *list_indic_file( const char *dir_path, const char *file_type, uint8_t create_flag );

const char *path_get( const char *path, PATH_TYPE pt );
const char * get_dev_name(const char * path);
const char * get_real_path(const char * path);
const char * get_file_name(const char * path);
const char * get_file_type(const char * path);
void fatfs_test( char *dev );

#ifdef __cplusplus
}
#endif
		
#endif

