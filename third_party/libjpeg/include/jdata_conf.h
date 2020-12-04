
/************************************************
 * @file jdata_conf.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-04
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "ff.h"

/* if you use SDCard, you should be adjust the value to 512 or lower */
#define INPUT_BUF_SIZE  4096	/* choose an efficiently fread'able size */

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
