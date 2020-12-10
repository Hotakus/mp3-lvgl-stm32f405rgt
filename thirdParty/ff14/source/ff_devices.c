/************************************************
 * @file ff_devices.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-12-10
 *
 * @copyright Copyright (c) 2020
 *
*************************************************/
#include "ff_devices.h"

extern fatfs_dev_opr_t dev_sd;
extern fatfs_dev_opr_t dev_spif;

/************************************************
 * @brief FatFs Devices Operational structrue
*************************************************/
fatfs_dev_opr_t* fs_dev[FF_VOLUMES];

void fatfs_dev_init(void)
{
  fs_dev[0] = &dev_spif;
  fs_dev[1] = &dev_sd;
}
