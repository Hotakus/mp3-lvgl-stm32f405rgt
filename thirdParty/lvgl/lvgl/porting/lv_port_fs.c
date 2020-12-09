/**
 * @file lv_port_fs.c
 *
 */

 /*Copy this file as "lv_port_fs.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_fs.h"
#include "ff_user.h"
#include "w25qxx.h"
#include "sdio.h"
#include "ff_devices.h"
 /*********************
  *      DEFINES
  *********************/

  /**********************
   *      TYPEDEFS
   **********************/

   /* Create a type to store the required data about your file.
    * If you are using a File System library
    * it already should have a File type.
    * For example FatFS has `FIL`. In this case use `typedef FIL file_t`
    */
typedef FIL file_t;

/*Similarly to `file_t` create a type for directory reading too */
typedef DIR dir_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void fs_init(void);

static lv_fs_res_t fs_open(lv_fs_drv_t* drv, void* file_p, const char* path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close(lv_fs_drv_t* drv, void* file_p);
static lv_fs_res_t fs_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br);
static lv_fs_res_t fs_write(lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw);
static lv_fs_res_t fs_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos);
static lv_fs_res_t fs_size(lv_fs_drv_t* drv, void* file_p, uint32_t* size_p);
static lv_fs_res_t fs_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p);
static lv_fs_res_t fs_remove(lv_fs_drv_t* drv, const char* path);
static lv_fs_res_t fs_trunc(lv_fs_drv_t* drv, void* file_p);
static lv_fs_res_t fs_rename(lv_fs_drv_t* drv, const char* oldname, const char* newname);
static lv_fs_res_t fs_free(lv_fs_drv_t* drv, uint32_t* total_p, uint32_t* free_p);
static lv_fs_res_t fs_dir_open(lv_fs_drv_t* drv, void* rddir_p, const char* path);
static lv_fs_res_t fs_dir_read(lv_fs_drv_t* drv, void* rddir_p, char* fn);
static lv_fs_res_t fs_dir_close(lv_fs_drv_t* drv, void* rddir_p);

/**********************
 *  STATIC VARIABLES
 **********************/

FATFS   fs_lv[2] = { NULL, NULL };
FRESULT fr_lv[2] = { FR_NOT_READY, FR_NOT_READY };
static uint8_t dev_conf_status[2] = { 0 };

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_fs_init(void)
{
    /*----------------------------------------------------
     * Initialize your storage device and File System
     * -------------------------------------------------*/
  fs_init();

  /*---------------------------------------------------
   * Register the file system interface  in LittlevGL
   *--------------------------------------------------*/

  if (dev_conf_status[SD_SDIO_INDEX] == 1 && dev_conf_status[SPIF_INDEX] == 1)
    return;

/* Add a simple drive to open images */
  static lv_fs_drv_t fs_drv[2];
  lv_fs_drv_init(&fs_drv[SD_SDIO_INDEX]);
  lv_fs_drv_init(&fs_drv[SPIF_INDEX]);

  /*Set up fields...*/
  /* For SD card */

  fs_drv[0].file_size = sizeof(file_t);
  fs_drv[0].letter = 'S';
  fs_drv[0].open_cb = fs_open;
  fs_drv[0].close_cb = fs_close;
  fs_drv[0].read_cb = fs_read;
  fs_drv[0].write_cb = fs_write;
  fs_drv[0].seek_cb = fs_seek;
  fs_drv[0].tell_cb = fs_tell;
  fs_drv[0].free_space_cb = fs_free;
  fs_drv[0].size_cb = fs_size;
  fs_drv[0].remove_cb = fs_remove;
  fs_drv[0].rename_cb = fs_rename;
  fs_drv[0].trunc_cb = fs_trunc;
  fs_drv[0].rddir_size = sizeof(dir_t);
  fs_drv[0].dir_close_cb = fs_dir_close;
  fs_drv[0].dir_open_cb = fs_dir_open;
  fs_drv[0].dir_read_cb = fs_dir_read;
  lv_fs_drv_register(&fs_drv[0]);

  /* For SPI FLASH */

  fs_drv[1].file_size = sizeof(file_t);
  fs_drv[1].letter = 'F';
  fs_drv[1].open_cb = fs_open;
  fs_drv[1].close_cb = fs_close;
  fs_drv[1].read_cb = fs_read;
  fs_drv[1].write_cb = fs_write;
  fs_drv[1].seek_cb = fs_seek;
  fs_drv[1].tell_cb = fs_tell;
  fs_drv[1].free_space_cb = fs_free;
  fs_drv[1].size_cb = fs_size;
  fs_drv[1].remove_cb = fs_remove;
  fs_drv[1].rename_cb = fs_rename;
  fs_drv[1].trunc_cb = fs_trunc;
  fs_drv[1].rddir_size = sizeof(dir_t);
  fs_drv[1].dir_close_cb = fs_dir_close;
  fs_drv[1].dir_open_cb = fs_dir_open;
  fs_drv[1].dir_read_cb = fs_dir_read;
  lv_fs_drv_register(&fs_drv[1]);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

 /* Initialize your Storage device and File system. */
static void fs_init(void)
{

  fatfs_dev_init();

  if (fr_lv[SPIF_INDEX] != FR_OK) {
    fr_lv[SPIF_INDEX] = f_mount(&fs_lv[SPIF_INDEX], "SPIF:", 1);
    if (fr_lv[SPIF_INDEX] != FR_OK) {
      DEBUG_PRINT("spi flash mount error. (fr: %d)\n", fr_lv[SPIF_INDEX]);
    }
    DEBUG_PRINT("spi flash mount successfully.\n\n");
  }

  /*E.g. for FatFS initalize the SD card and FatFS itself*/
  /*You code here*/
  if (fr_lv[SD_SDIO_INDEX] != FR_OK) {
    fr_lv[SD_SDIO_INDEX] = f_mount(&fs_lv[SD_SDIO_INDEX], "SD_SDIO:", 1);
    if (fr_lv[SD_SDIO_INDEX] != FR_OK) {
      DEBUG_PRINT("sd card mount error. (fr: %d)\n", fr_lv[SD_SDIO_INDEX]);
      if (fr_lv[SD_SDIO_INDEX] == FR_NOT_READY)
        DEBUG_PRINT("no sd card.\n");
    } else
      DEBUG_PRINT("sd card mount successfully.\n");
  }



  if (fr_lv[SD_SDIO_INDEX] == FR_OK) {
    sd_show_card_info();
  }

  if (fr_lv[SPIF_INDEX] == FR_OK) {
    extern w25qxx_feature_s w25qxx;
    printf("SPI FLASH:\n");
    printf("Capacity : %d Bytes\nJEDECID  : %X\n",
      w25qxx.capacity,
      w25qxx.JEDECID
    );
  }

}


/**
 * Open a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param path path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_open(lv_fs_drv_t* drv, void* file_p, const char* path, lv_fs_mode_t mode)
{
  char pathBuf[128] = { 0 };
  uint8_t opt_mode = 0;

  // 根据传入的参数判断是什么存储设备
  switch (drv->letter) {
  case 'S':       // SD card
    sprintf(pathBuf, "SD_SDIO:/%s", path);
    break;
  case 'F':       // SPI FALSH
    sprintf(pathBuf, "SPIF:/%s", path);
    break;
  default:
    printf("No drive %c\n", drv->letter);
    return LV_FS_RES_NOT_EX;
  }

  /* 判断文件操作方法 */
  if (mode == LV_FS_MODE_WR) {
      /*Open a file for write*/
      /* Add your code here*/

    opt_mode = FA_OPEN_ALWAYS | FA_WRITE;
  } else if (mode == LV_FS_MODE_RD) {
      /*Open a file for read*/
      /* Add your code here*/

    opt_mode = FA_OPEN_EXISTING | FA_READ;
  } else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) {
      /*Open a file for read and write*/
      /* Add your code here*/

    opt_mode = FA_WRITE | FA_READ;
  }

  /* 调用FatFs的函数 */

  FRESULT fres = f_open(file_p, pathBuf, opt_mode);

  if (fres != FR_OK) {
    printf("f_open error (%d)\n", fres);
    return LV_FS_RES_NOT_IMP;
  } else
    return LV_FS_RES_OK;

}


/**
 * Close an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_close(lv_fs_drv_t* drv, void* file_p)
{

    /* Add your code here*/
  if (f_close(file_p) != FR_OK) {

    return LV_FS_RES_NOT_IMP;
  } else {

    return LV_FS_RES_OK;
  }

}

/**
 * Read data from an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br)
{

    /* Add your code here*/
  FRESULT fres = f_read((FIL*)file_p, buf, btr, br);
  if (fres != FR_OK) {
    printf("f_read error (%d)\n", fres);
    return LV_FS_RES_NOT_IMP;
  } else {

    return LV_FS_RES_OK;
  }
}

/**
 * Write into a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param buf pointer to a buffer with the bytes to write
 * @param btr Bytes To Write
 * @param br the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw)
{
    /* Add your code here*/
  FRESULT fres = f_write((FIL*)file_p, buf, btw, bw);
  if (fres != FR_OK) {
    printf("f_read error (%d)\n", fres);
    return LV_FS_RES_NOT_IMP;
  } else
    return LV_FS_RES_OK;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos)
{

    /* Add your code here*/
  FRESULT fres = f_lseek((FIL*)file_p, pos);
  if (fres != FR_OK) {
    printf("f_lseek error (%d)\n", fres);
    return LV_FS_RES_NOT_IMP;
  } else {

    return LV_FS_RES_OK;
  }
}

/**
 * Give the size of a file bytes
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param size pointer to a variable to store the size
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_size(lv_fs_drv_t* drv, void* file_p, uint32_t* size_p)
{
  lv_fs_res_t res = LV_FS_RES_NOT_IMP;

  /* Add your code here*/
  *size_p = f_size((FIL*)file_p);
  res = LV_FS_RES_OK;

  return res;
}
/**
 * Give the position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p)
{
    /* Add your code here*/
  *pos_p = f_tell((FIL*)file_p);
  return LV_FS_RES_OK;
}

/**
 * Delete a file
 * @param drv pointer to a driver where this function belongs
 * @param path path of the file to delete
 * @return  LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_remove(lv_fs_drv_t* drv, const char* path)
{
  return LV_FS_RES_OK;
}

/**
 * Truncate the file size to the current position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to an 'ufs_file_t' variable. (opened with lv_fs_open )
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_trunc(lv_fs_drv_t* drv, void* file_p)
{
  return LV_FS_RES_OK;
}

/**
 * Rename a file
 * @param drv pointer to a driver where this function belongs
 * @param oldname path to the file
 * @param newname path with the new name
 * @return LV_FS_RES_OK or any error from 'fs_res_t'
 */
static lv_fs_res_t fs_rename(lv_fs_drv_t* drv, const char* oldname, const char* newname)
{
  return LV_FS_RES_OK;
}

/**
 * Get the free and total size of a driver in kB
 * @param drv pointer to a driver where this function belongs
 * @param letter the driver letter
 * @param total_p pointer to store the total size [kB]
 * @param free_p pointer to store the free size [kB]
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_free(lv_fs_drv_t* drv, uint32_t* total_p, uint32_t* free_p)
{
  lv_fs_res_t res = LV_FS_RES_NOT_IMP;
  FRESULT fres = FR_INVALID_PARAMETER;
  char pathBuf[10] = { 0 };

  uint32_t t_size = 0;
  uint32_t f_size = 0;

  // 根据传入的参数判断是什么存储设备
  switch (drv->letter) {
  case 'S':       // SD card
    strcpy(pathBuf, "SD_SDIO:/");
    break;
  case 'F':       // SPI FALSH
    strcpy(pathBuf, "SPIF:/");
    break;
  default:
    printf("No drive %c\n", drv->letter);
    return LV_FS_RES_NOT_EX;
  }

  /* Add your code here*/
  fres = exf_getfree(pathBuf, &t_size, &f_size);
  if (fres != FR_OK)
    res = LV_FS_RES_NOT_IMP;
  else {
    *total_p = t_size;
    *free_p = f_size;
    printf("t_size: %d\tf_size: %d\n", t_size, f_size);
    res = LV_FS_RES_OK;
  }

  return res;
}

/**
 * Initialize a 'fs_read_dir_t' variable for directory reading
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to a 'fs_read_dir_t' variable
 * @param path path to a directory
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_open(lv_fs_drv_t* drv, void* rddir_p, const char* path)
{
    // lv_fs_res_t res = LV_FS_RES_NOT_IMP;
    // FRESULT fres = FR_INVALID_PARAMETER;
    // char pathBuf[256] = {0};

    // /* Add your code here*/
    // // 根据传入的参数判断是什么存储设备
    // switch ( drv->letter ) {
    // case 'S':       // SD card
    //     sprintf( pathBuf, "SD_SDIO:/%s", path );
    //     break;
    // case 'F':       // SPI FALSH
    //     sprintf( pathBuf, "SPIF:/%s", path );
    //     break;
    // default:
    //     printf( "No drive %c\n", drv->letter );
    //     return LV_FS_RES_NOT_EX;
    // }

    // fres = f_opendir( (DIR*)rddir_p, pathBuf );
    // if ( fres != FR_OK ) 
    //     res = LV_FS_RES_NOT_IMP;
    // else 
    //     res = LV_FS_RES_OK;

    // return res;

  return LV_FS_RES_OK;
}

/**
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to an initialized 'fs_read_dir_t' variable
 * @param fn pointer to a buffer to store the filename
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_read(lv_fs_drv_t* drv, void* rddir_p, char* fn)
{
    // lv_fs_res_t res = LV_FS_RES_NOT_IMP;
    // FRESULT fres = FR_INVALID_PARAMETER;
    // FILINFO fno;

    // /* Add your code here*/
    // fn = fn; 

    // fres = f_readdir( (DIR*)rddir_p, &fno );
    // if ( fres != FR_OK ) {
    //     res = LV_FS_RES_NOT_IMP;
    // } else {
    //     fn = fno.fname;
    //     res = LV_FS_RES_OK;
    // }

    // return res;

  return LV_FS_RES_OK;
}

/**
 * Close the directory reading
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to an initialized 'fs_read_dir_t' variable
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_close(lv_fs_drv_t* drv, void* rddir_p)
{
    // lv_fs_res_t res = LV_FS_RES_NOT_IMP;
    // FRESULT fres = FR_INVALID_PARAMETER;

    // /* Add your code here*/
    // fres = f_closedir( (DIR*)rddir_p );
    // if ( fres != FR_OK ) 
    //     res = LV_FS_RES_NOT_IMP;
    // else {
    //     res = LV_FS_RES_OK;
    // }

    // return res;

  return LV_FS_RES_OK;
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
