/************************************************
 * @file mp3_decode.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-04
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "mp3_decode.h"

/************************************************
 * @brief STATIC PROTOTYPE
 ************************************************/
static void analyse_id3v2( void );
static void analyse_frame_header( void );
static void analyse_bitrate( void );

/************************************************
 * @brief STATIC VARIABLE
 ************************************************/
static uint16_t bitrates_map[16][6] = {
    { 0, 0, 0, 0, 0, 0 },
    { 32, 32, 32, 0, 0, 0 },
    { 64, 48, 40, 0, 0, 0 },
    { 96, 56, 48, 0, 0, 0 },
    { 128, 64, 56, 0, 0, 0 },
    { 160, 80, 64, 0, 0, 0 },
    { 192, 96, 80, 0, 0, 0 },
    { 224, 112, 96, 0, 0, 0 },
    { 256, 128, 112, 0, 0, 0 },
    { 288, 160, 128, 0, 0, 0 },
    { 320, 192, 160, 0, 0, 0 },
    { 352, 224, 192, 0, 0, 0 },
    { 384, 256, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
};
static mp3_ID3Vx_header_t mp3_header;
static mp3_info_t mp3_info;
static mp3_fh_t mp3_fh;
static size_t mp3_id3v2_size = 0;

/************************************************
 * @brief 计算ID3V2 （包括前10字节）
 ************************************************/
static void analyse_id3v2(void)
{
    mp3_id3v2_size  = (mp3_header.IDx_size[3]&0x7F);
    mp3_id3v2_size |= (mp3_header.IDx_size[2]&0x7F) << 7;
    mp3_id3v2_size |= (mp3_header.IDx_size[1]&0x7F) << 14;
    mp3_id3v2_size |= (mp3_header.IDx_size[0]&0x7F) << 21;
    mp3_id3v2_size &= ~(0xF<<28); // 取前28位
    mp3_id3v2_size += 10;   // 算上头10字节
}

/************************************************
 * @brief 分析帧头
 ************************************************/
static void analyse_frame_header( void )
{
    analyse_bitrate();
    mp3_info.play_times = mp3_info.file_size_splited / (mp3_info.bitrates>>3) / 1000;

    /* 计算单帧长 */
    /* 如果layer为layer1 */
    if ( mp3_fh.fh.layer == 0x03 ) {         //  如果layer为layer1
        mp3_info.frame_size = 384;
    } else if ( mp3_fh.fh.layer == 0x02 ) {  //  如果layer为layer2 
        mp3_info.frame_size = 1152;
    } else if ( mp3_fh.fh.layer == 0x01 ) {  //  如果layer为layer3 
        if ( mp3_fh.fh.version == 0x03 ) {   //  如果MPEG1
            mp3_info.frame_size = 1152;
        } else 
            mp3_info.frame_size = 576;
    } 
    
}

/************************************************
 * @brief 从bitrate表中取出比特率值 (单位kbps)
 ************************************************/
static void analyse_bitrate( void )
{
    unsigned char x = 0;            // bitrate map coordinates x

    /* 如果等于0x03，则为MPEG1, 否则MPEG2或MPEG2.5 */
    if ( mp3_fh.fh.version == 0x03 ) {
        if ( mp3_fh.fh.layer == 0x03 )
            x = 0;
        if ( mp3_fh.fh.layer == 0x02 )
            x = 1;
        if ( mp3_fh.fh.layer == 0x01 )
            x = 2;
    } else if ( mp3_fh.fh.version == 0x03 || mp3_fh.fh.version == 0x00 ) {

    }

    mp3_info.bitrates = bitrates_map[mp3_fh.fh.bitrate_index][x];
}
