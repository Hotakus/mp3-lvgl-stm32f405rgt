/************************************************
 * @file user_cmd.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#include "stm32f4xx_hal.h"
#include "sdio.h"
#include "pro_conf.h"
#include "screen_conf.h"
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include <lvgl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rtc.h"
 //#include "oled.h"
 //#include "ft6236.h"
#include "vs10xx.h"
#include "mp3_decode.h"

#if defined(RT_USING_FINSH)

 /************************************************
  * @brief cmd for msh
  ************************************************/

static void lcd_test(int argc, char** argv)
{
  scr_opr_handler* scr = scr_get_opr_handler();

  if (argc == 2) {
    switch (*argv[1]) {
    case 'r':
      scr->clear(0xF800);
      break;
    case 'g':
      scr->clear(0x07F0);
      break;
    case 'b':
      scr->clear(0x001F);
      break;
    default:
      scr->clear(0xF800);
      break;
    }
  } else
    scr->clear(0xF800);

}
MSH_CMD_EXPORT(lcd_test, lcd test);

#if USER_USE_FATFS == 1
#include "ff_user.h"
#include "file_trans.h"
/************************************************
 * @brief 扫描目录下文件
 *
 * @param argc
 * @param argv
 ************************************************/
static void scan_file(int argc, char** argv)
{

  scan_catalog(argv[1], SCAN_OPT_CUR_DIR);

}
MSH_CMD_EXPORT(scan_file, list files in sd);

/************************************************
 * @brief 显示文件属性
 *
 * @param argc
 * @param argv
 ************************************************/
static void file_info(int argc, char** argv)
{
  show_file_info(argv[1]);
}
MSH_CMD_EXPORT(file_info, get info which file);

/************************************************
 * @brief 查看文件内容
 *
 * @param argc
 * @param argv
 ************************************************/
static void rtt_cat(int argc, char** argv)
{

  uint32_t btr = 0;
  uint32_t br = 0;
  uint8_t* ch_buf = NULL;

  if (argc < 2) {
    rt_kprintf("usage rtt_cat {file} btr\n");
    return;
  } else if (argc == 2) {
    btr = sizeof(ch_buf);
  } else if (argc == 3) {
    btr = atoi(argv[2]);
  }
  rt_kprintf("btr: %d\n", btr);
  ch_buf = (uint8_t*)MALLOC(sizeof(uint8_t) * btr);

  FRESULT cat_fres;
  FIL     cat_fil;

  cat_fres = f_open(&cat_fil, argv[1], FA_READ | FA_OPEN_EXISTING);
  if (cat_fres != FR_OK) {
    rt_kprintf("open '%s' error. (%d)\n", get_file_name(argv[1]), cat_fres);
    f_close(&cat_fil);
    return;
  }

  cat_fres = f_lseek(&cat_fil, 0);
  if (cat_fres != FR_OK) {
    rt_kprintf("seek begin error. (%d)\n", cat_fres);
    f_close(&cat_fil);
    return;
  }

  cat_fres = f_read(&cat_fil, ch_buf, btr, &br);
  if (cat_fres != FR_OK) {
    rt_kprintf("read '%s' error. (%d)\n", get_file_name(argv[1]), cat_fres);
    f_close(&cat_fil);
    return;
  }

  if (br) {
    rt_kprintf("br: %d\n", br);
    for (uint32_t i = 0; i < br; i++)
      my_putc(ch_buf[i]);
  } else {
    rt_kprintf("Null to read\n");
  }

  rt_free(ch_buf);
  f_close(&cat_fil);

}
MSH_CMD_EXPORT(rtt_cat, show file content);

/************************************************
 * @brief 格式化存储设备
 *
 * @param argc
 * @param argv
 ************************************************/
static void rtt_mkfs(int argc, char** argv)
{

  FRESULT fres = FR_NOT_READY;

  uint8_t* works = NULL;

  if (argv[1][0] == 'F') {
    works = (uint8_t*)MALLOC(sizeof(uint8_t) * FF_MAX_SS);
    fres = f_mkfs("SPIF:", NULL, works, FF_MAX_SS);
    if (fres != FR_OK) {
      rt_kprintf("mkfs SPIF: %d\n", fres);
      return;
    }
  } else if (argv[1][0] == 'S') {
    works = (uint8_t*)MALLOC(sizeof(uint8_t) * FF_MIN_SS);
    fres = f_mkfs("SD_SDIO:", NULL, works, FF_MIN_SS);
    if (fres != FR_OK) {
      rt_kprintf("mkfs SD: %d\n", fres);
      return;
    }
  }

  rt_kprintf("mkfs done.\n");

  FREE(works);

}
MSH_CMD_EXPORT(rtt_mkfs, mkfs dev);

/************************************************
 * @brief 拷贝文件
 *
 * @param argc
 * @param argv
 ************************************************/
void rtt_cp(int argc, char** argv)
{
  if (argc != 3)
    return;
  rt_kprintf("trans begin.\n");
  if (file_trans(argv[1], argv[2]) != TRANS_STAT_OK)
    rt_kprintf("trans error.\n");
  rt_kprintf("trans end.\n");

}
MSH_CMD_EXPORT(rtt_cp, copy file);
#endif

#if USER_USE_LVGL == 1
static void lvgl_mem()
{
  lv_mem_monitor_t mem;
  lv_mem_monitor(&mem);

  DEBUG_PRINT("lv free_size  : %d KiB\n", mem.free_size);
  DEBUG_PRINT("lv total_size : %d KiB\n", mem.total_size);
  DEBUG_PRINT("lv max_used   : %d KiB\n", mem.max_used);
}
MSH_CMD_EXPORT(lvgl_mem, lvgl_mem);
/************************************************
 * @brief 在当前屏幕显示一张来自存储设备的图片
 ************************************************/
static void show_pic(int argc, char** argv)
{
  ALIGN(4) static lv_obj_t* pic = NULL;

  if (argc == 1)
    return;

  if (STRCMP(argv[1], "free") == 0 && pic != NULL) {
    lv_res_t res;
    uint8_t retry = 5;
    do {
      res = lv_obj_del(pic);
    } while (--retry && res != LV_RES_INV);
    if (res != LV_RES_INV) {
      rt_kprintf("delete obj error.\n");
    }
    pic = NULL;
    return;
  }

  if (pic == NULL) {
    pic = lv_img_create(lv_scr_act(), NULL);
  }

  lv_img_set_src(pic, argv[1]);
  lv_obj_align(pic, NULL, LV_ALIGN_CENTER, 0, 0);

}
MSH_CMD_EXPORT(show_pic, show a picture in screen from memory device.);
#endif

// /************************************************
//  * @brief oled 测试
//  ************************************************/
// static void oled_test(int argc, char** argv)
// {
//     static uint8_t flag = 0;
//     if (!flag)
//         oled_i2c_init(I2C1, 400, 0);
//     else {
//         extern uint8_t OLED_GRAM [8][128];
//         oled_clean();
//         show_str((uint8_t*)"Hello!!!", FONT_8x16, 0, 64 - 16);
//         oled_flush_with((uint8_t*)OLED_GRAM);
//     }
// }
// MSH_CMD_EXPORT(oled_test, oled test);

/************************************************
 * @brief vs10xx 测试
 ************************************************/
static void vs10xx_test(int argc, char** argv)
{
  vs10xx_init();
}
MSH_CMD_EXPORT(vs10xx_test, vs10xx test);

static void playback(int argc, char** argv)
{
  if (argc == 1) {
    vs10xx_playback("SD_SDIO:/test.mp3");
    return;
  } else if (argc > 2) {
    DEBUG_PRINT("usage : %s <file path>\n", __FUNCTION__);
    return;
  }

  vs10xx_playback(argv[1]);
}
MSH_CMD_EXPORT(playback, paly audio by vs10xx);

static void mp3_get_jpeg(int argc, char** argv)
{
  if (argc == 1)
    return;
  extract_mp3_pic("SD_SDIO:/test.mp3", "SD_SDIO:/test.jpg");
}
MSH_CMD_EXPORT(mp3_get_jpeg, get jpeg from mp3 file);


#endif

