/************************************************
 * @file libjpeg_user.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-04
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef LIBJPEG_USER_H
#define LIBJPEG_USER_H

/* includes */
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include "pro_conf.h"


/* definations */

/* functions */
void rgb888_to_rgb565(unsigned char* rgb888_buf, int rgb888_size, uint16_t *rgb565_buf);
int libjpeg_decompress( int argc, char **argv );

#endif
