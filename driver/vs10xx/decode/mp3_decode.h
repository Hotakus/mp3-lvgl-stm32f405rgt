/************************************************
 * @file mp3_decode.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-04
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#ifndef MP3_DECODE_H
#define MP3_DECODE_H

/* includes */
#include "stm32f4xx_conf.h"
#include "arm_math.h"
#include <stdio.h>

/************************************************
 * @brief TYPEDEF
 ************************************************/
/* MP3 相关定义 */
typedef struct mp3_ID3Vx_header_t
{
    unsigned char IDx[3];       /* 保存的值比如为"ID3"表示是ID3V2 */
    unsigned char version;      /* 如果是ID3V2.3则保存3,如果是ID3V2.4则保存4 */
    unsigned char revision;     /* 副版本号 */
    unsigned char flag;         /* 存放标志的字节 */
    unsigned char IDx_size[4];  /* 整个 IDxVx 的大小，除去本结构体的 10 个字节 */
    /* 只有后面 7 位有用 */
} mp3_ID3Vx_header_t;

/* 帧头 */
typedef struct FrameHeader_s {
    unsigned char sync:8;                        //同步信息1

    unsigned char crc:1;                         // CRC校验
    unsigned char layer: 2;                       //层
    unsigned char version:2;                      //版本
    unsigned char sync2:3;                        //同步信息2

    unsigned char private:1;                       //保留字
    unsigned char padding:1;                     //帧长调节
    unsigned char sampling_frequency:2;          //采样频率
    unsigned char bitrate_index:4;               //位率

    unsigned char emphasis:2;                    //强调模式
    unsigned char original:1;                      //原版标志
    unsigned char copyright:1;                    // 版权
    unsigned char mode_extension:2;              //扩充模式
    unsigned char mode:2;                         //声道模式
} FrameHeader_s;
typedef union mp3_fh_t
{
    FrameHeader_s fh;
    unsigned char buf[4];
} mp3_fh_t;

/* 用户关注的MP3信息 */
typedef struct mp3_info_t {
    size_t file_size;
    size_t file_size_splited;   // 去除ID3V2和ID3V2后大小， 也就是所有帧大小
    size_t h_frame_pos;         // 帧头偏移地址
    unsigned short bitrates;    // 比特率 kbps
    unsigned short play_times;  // 播放时间
    unsigned short frame_size;  // 单帧大小 byte
} mp3_info_t;



/************************************************
 * @brief DEFINE
 ************************************************/



/************************************************
 * @brief FUNCTION PROTOTYPE
 ************************************************/

#endif
