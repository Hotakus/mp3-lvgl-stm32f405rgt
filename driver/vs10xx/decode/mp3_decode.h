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
#include "stm32f4xx_hal.h"
#include "arm_math.h"
#include <stdio.h>

/************************************************
 * @brief TYPEDEF
 ************************************************/
/* MP3 相关定义 */
typedef struct mp3_ID3Vx_header_t
{
    uint8_t IDx[3];       /* 保存的值比如为"ID3"表示是ID3V2 */
    uint8_t version;      /* 如果是ID3V2.3则保存3,如果是ID3V2.4则保存4 */
    uint8_t revision;     /* 副版本号 */
    uint8_t flag;         /* 存放标志的字节 */
    uint8_t IDx_size[4];  /* 整个 IDxVx 的大小，除去本结构体的 10 个字节 */
    /* 只有后面 7 位有用 */
} mp3_ID3Vx_header_t;

/* 帧头 */
typedef struct mp3_fh_t {
    uint8_t sync:8;                        //同步信息1

    uint8_t crc:1;                         // CRC校验
    uint8_t layer: 2;                       //层
    uint8_t version:2;                      //版本
    uint8_t sync2:3;                        //同步信息2

    uint8_t private:1;                      //保留字
    uint8_t padding:1;                     //帧长调节
    uint8_t sampling_frequency:2;          //采样频率
    uint8_t bitrate_index:4;               //位率

    uint8_t emphasis:2;                    //强调模式
    uint8_t original:1;                      //原版标志
    uint8_t copyright:1;                    // 版权
    uint8_t mode_extension:2;              //扩充模式
    uint8_t mode:2;                         //声道模式
} mp3_fh_t;


/* 用户关注的MP3信息 */
typedef struct mp3_info_t {
    mp3_ID3Vx_header_t id3v2;
    mp3_fh_t fh;
    size_t   file_size;
    size_t   file_size_splited; // 去除ID3V2和ID3V2后大小， 也就是所有帧大小
    size_t   id3v2_size;        // ID3V2 size
    size_t   h_frame_pos;       // 帧头偏移地址
    size_t   frame_spos;        
    size_t   frame_epos;
    size_t   sample_rate;
    uint16_t bitrates;          // 比特率 kbps
    uint16_t play_times;        // 播放时间
    uint16_t frame_size;        // 单帧大小 byte



} mp3_info_t;

/************************************************
 * @brief mp3 tag
 ************************************************/
typedef struct mp3_tag_header_t {
    uint8_t name[4];
    uint8_t size[4];
    uint8_t flag[2];
} mp3_tag_header_t;

typedef struct mp3_tag_t {
    mp3_tag_header_t header;
    size_t pos;
    size_t size;
} mp3_tag_t;

/************************************************
 * @brief DEFINE
 ************************************************/



/************************************************
 * @brief FUNCTION PROTOTYPE
 ************************************************/
void mp3_analyse_id3v2( mp3_info_t *mp3 );
void mp3_analyse_frame_header( mp3_info_t *mp3 );
void mp3_analyse_bitrate( mp3_info_t *mp3 );
void mp3_analyse_tag( mp3_tag_t *tag );
void mp3_analyse(const char *mp3_file_path, mp3_info_t *mp3 );
mp3_tag_t *mp3_find_tag_pos( const char *mp3_file_path, const char *tag_name );

void extract_mp3_pic( const char *mp3_file_path, const char *out_path );


#endif
