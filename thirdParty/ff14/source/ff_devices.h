/************************************************
 * @file ff_devices.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-10
 * 
 * @copyright Copyright (c) 2020
 * 
*************************************************/
#ifndef __FF_DEVICES_H
#define __FF_DEVICES_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "ff_user_typedef.h"

extern fatfs_dev_opr_t fs_dev[FF_VOLUMES];

void fatfs_dev_init(void);

#ifdef __cplusplus
}
#endif

#endif // !__FF_DEVICES_H
