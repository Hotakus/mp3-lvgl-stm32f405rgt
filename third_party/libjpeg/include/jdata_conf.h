/**
 ******************************************************************************
  * File Name          : jdata_conf.h
  * Description        : This file provides header to "jdata_conf.h" module.
  *                      It implements also file based read/write functions.
  *
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/

/*Stdio is chosen for File storage*/
#include <stdio.h>
#include "pro_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*This defines the memory allocation methods.*/
#define JMALLOC   malloc
#define JFREE     free

/*This defines the File data manager type.*/
#define JFILE            FIL

size_t jf_writ(JFILE *file, uint8_t *buf, uint32_t sizeofbuf);
size_t jf_read(JFILE *file, uint8_t *buf, uint32_t sizeofbuf);

inline size_t jf_read(JFILE *file, uint8_t *buf, uint32_t sizeofbuf)
{
    uint32_t br = 0;
    f_read( file, buf, sizeofbuf, &br );
    return br;
}

inline size_t jf_writ(JFILE *file, uint8_t *buf, uint32_t sizeofbuf)
{
    uint32_t bw = 0;
    f_read( file, buf, sizeofbuf, &bw );
    return bw;
}

#define JFREAD(file,buf,sizeofbuf)  jf_read(file,buf,sizeofbuf)
#define JFWRITE(file,buf,sizeofbuf)  jf_writ(file,buf,sizeofbuf)

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
