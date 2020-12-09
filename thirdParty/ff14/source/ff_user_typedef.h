/************************************************
 * @file ff_user_typedef.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-10
 * 
 * @copyright Copyright (c) 2020
 * 
*************************************************/
#ifndef __FF_USER_TYPEDEF_H
#define __FF_USER_TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32f4xx_hal.h"
#include "ff.h"


/* definations */
typedef enum {
    SCAN_OPT_ALL        = 0x00,     // 遍历所有
    SCAN_OPT_NO_HIDEN         ,     // 遍历除隐藏文件夹外的所有
    SCAN_OPT_CUR_DIR          ,     // 仅遍历当前文件夹
} SCAN_OPT;

/************************************************
 * @brief Path type
 ************************************************/
typedef enum {
    PATH_DEV_NAME = 0,    // 设备名
    PATH_MAIN        ,    // 主路径
    PATH_FILE_NAME   ,    // 文件名
    PATH_FILE_TYPE   ,    // 文件类型
} PATH_TYPE;

/* 文件列表结构体 */
typedef struct list_file_t {
    uint16_t file_cnt;      // 列表文件内容数量
    const char *type;  // 列表文件类型
} list_file_t;

typedef struct  {
  HAL_StatusTypeDef (*init) (void);
  HAL_StatusTypeDef (*read) ( uint8_t *buff, LBA_t sector, UINT count );
  HAL_StatusTypeDef (*write) ( uint8_t *buff, LBA_t sector, UINT count );
} fatfs_dev_opr_t;

#ifdef __cplusplus
}
#endif
		
#endif

