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
#include "ff.h"

/************************************************
 * @brief STATIC PROTOTYPE
 ************************************************/


/************************************************
 * @brief STATIC VARIABLE
 ************************************************/

/************************************************
 * @brief 计算ID3V2 （包括前10字节）
 ************************************************/
void mp3_analyse_id3v2( mp3_info_t *mp3 )
{
    mp3->id3v2_size  = (mp3->id3v2.IDx_size[3]&0x7F);
    mp3->id3v2_size |= (mp3->id3v2.IDx_size[2]&0x7F) << 7;
    mp3->id3v2_size |= (mp3->id3v2.IDx_size[1]&0x7F) << 14;
    mp3->id3v2_size |= (mp3->id3v2.IDx_size[0]&0x7F) << 21;
    mp3->id3v2_size += 10;   // 算上头10字节
}

/************************************************
 * @brief 分析帧头
 ************************************************/
void mp3_analyse_frame_header( mp3_info_t *mp3 )
{

    mp3_analyse_bitrate( mp3 );
    if ( mp3->bitrates )
        mp3->play_times = mp3->file_size_splited / (mp3->bitrates>>3) / 1000;

    /* 计算单帧长 */
    /* 如果layer为layer1 */
    if ( mp3->fh.layer == 0x03 ) {         //  如果layer为layer1
        mp3->frame_size = 384;
    } else if ( mp3->fh.layer == 0x02 ) {  //  如果layer为layer2 
        mp3->frame_size = 1152;
    } else if ( mp3->fh.layer == 0x01 ) {  //  如果layer为layer3 
        if ( mp3->fh.version == 0x03 ) {   //  如果MPEG1
            mp3->frame_size = 1152;
        } else 
            mp3->frame_size = 576;
    } 
    
}

/************************************************
 * @brief 计算比特率 (单位kbps)
 ************************************************/
void mp3_analyse_bitrate( mp3_info_t *mp3 )
{
    uint8_t inc_var = 0;    // 递增量
    uint8_t base = 32;       // 基值

    /* 如果等于0x03，则为MPEG1, 否则MPEG2或MPEG2.5 */
    if ( mp3->fh.version == 0x03 ) {
        /* layer 1 */
        if ( mp3->fh.layer == 0x03 ) {
            inc_var = 32;
        } else if ( mp3->fh.layer == 0x02 ) {
            inc_var = 16;
        } else if ( mp3->fh.layer == 0x01 ) {
            inc_var = 8;
        }
    } else if ( mp3->fh.version == 0x02 || mp3->fh.version == 0x00 ) {

    } 

    if ( mp3->fh.bitrate_index ) 
        mp3->bitrates = base + ( inc_var * (mp3->fh.bitrate_index+3) );
    else 
        mp3->bitrates = 0;
}

/************************************************
 * @brief 读取MP3文件并进行文件分析
 * 
 * @param mp3_file_path mp3文件路径
 * @param mp3 mp3_info_t 
 ************************************************/
void mp3_analyse(const char *mp3_file_path, mp3_info_t *mp3 )
{
    FIL *mp3_fil = (FIL*)MALLOC( sizeof(FIL) );
    FRESULT res = f_open( mp3_fil, mp3_file_path, FA_OPEN_EXISTING | FA_READ );
    if ( res != FR_OK ) {
        DEBUG_PRINT( "open mp3 file : %s error.\n", mp3_file_path );
        return;
    }

    /* 分析ID3V2头 */
    f_lseek( mp3_fil, 0 );
    f_read( mp3_fil, &mp3->id3v2, 10, NULL );
    mp3_analyse_id3v2( mp3 );

    /* 得到去除头信息的帧总大小 */
    mp3->file_size = f_size( mp3_fil );
    mp3->file_size_splited = mp3->file_size - mp3->id3v2_size - 128;
    mp3->h_frame_pos = mp3->id3v2_size;

    /* 分析帧头 */
    f_lseek( mp3_fil, mp3->h_frame_pos );
    f_read( mp3_fil, &mp3->fh, 4, NULL );
    mp3_analyse_frame_header( mp3 );

    
    f_close( mp3_fil );
    FREE(mp3_fil);
}

/************************************************
 * @brief 分析传入的Tag
 * 
 * @param tag 
 ************************************************/
void mp3_mp3_analyse_tag( mp3_tag_t *tag )
{
    tag->size  = tag->header.size[3];
    tag->size |= tag->header.size[2]<<8;
    tag->size |= tag->header.size[1]<<16;
    tag->size |= tag->header.size[0]<<24;
}

/************************************************
 * @brief 根据tag名和MP3文件得到此tag
 * 
 * @param mp3_file_path 
 * @param tag_name 
 * @return mp3_tag_t* 
 ************************************************/
mp3_tag_t *mp3_find_tag_pos( const char *mp3_file_path, const char *tag_name )
{
    static mp3_tag_t tag;
    mp3_info_t mp3;
    FIL *mp3_fil = (FIL*)MALLOC( sizeof(FIL) );

    FRESULT res = f_open( mp3_fil, mp3_file_path, FA_OPEN_EXISTING | FA_READ );
    if ( res != FR_OK ) {
        DEBUG_PRINT( "open mp3 file : %s error.\n", mp3_file_path );
        return NULL;
    }

    f_lseek( mp3_fil, 0 );
    f_read( mp3_fil, &mp3.id3v2, 10, NULL );
    if ( MEMCMP( mp3.id3v2.IDx, "ID3", 3 ) != 0 ) {
        DEBUG_PRINT( "%s is not a mp3 file.\n", mp3_file_path );
        return NULL;
    }
    mp3_analyse_id3v2( &mp3 );
    
    tag.pos = 10;   // 跳过ID3V2前10字节
    while ( tag.pos < mp3.id3v2_size-10 ) {
        f_lseek( mp3_fil, tag.pos );
        f_read( mp3_fil, &tag.header, 10, NULL );
        // DEBUG_PRINT( "cur tag : %s\n", tag.header.name );
        mp3_mp3_analyse_tag( &tag );
        if ( MEMCMP( tag.header.name, tag_name, 4 ) == 0 )
            break;
        tag.pos += (tag.size+10);  // 指向下一个TAG
    }

    f_close( mp3_fil );
    FREE(mp3_fil);

    return &tag;  // 指向Tag的内容
}

/************************************************
 * @brief 从mp3文件中提取专辑图片
 * 
 * @param mp3_file_path 
 * @param out_path 
 ************************************************/
void extract_mp3_pic( const char *mp3_file_path, const char *out_path )
{
    size_t pic_spos = 0;
    size_t pic_epos = 0;
    mp3_tag_t *apic_tag;

    apic_tag = mp3_find_tag_pos( mp3_file_path, "APIC" );
    pic_spos = apic_tag->pos + 24;
    pic_epos = (apic_tag->pos + 14) + apic_tag->size;

    DEBUG_PRINT( "pic_spos : %x\n", pic_spos );
    DEBUG_PRINT( "pic_epos : %x\n", pic_epos );

     /* 创建文件 */
    size_t   jpeg_size = pic_epos - pic_spos;
    uint16_t jpeg_buf_size = 4096;
    uint8_t *jpeg_buf = (uint8_t *)MALLOC( sizeof(uint8_t)*jpeg_buf_size );

    uint32_t tt    = jpeg_size / jpeg_buf_size;
    uint32_t sdatn = jpeg_size % jpeg_buf_size;
    if ( sdatn )
        tt += 1;

    uint8_t retry = 0;
    FIL out_fil, mp3_fil;
    FRESULT src_fres, dest_fres;

    f_open( &mp3_fil, mp3_file_path, FA_READ | FA_OPEN_EXISTING );
    f_open( &out_fil, out_path, FA_WRITE | FA_OPEN_ALWAYS );

    f_lseek( &mp3_fil, pic_spos );
    f_lseek( &out_fil, 0 );

    Clock_Start();
    do {
        /* 传输 */
        retry = 5;
        do {
            src_fres   = f_read( &mp3_fil, jpeg_buf, (tt==1)?(sdatn):(jpeg_buf_size), NULL );
            dest_fres  = f_write( &out_fil, jpeg_buf, (tt==1)?(sdatn):(jpeg_buf_size), NULL );
        } while ( (dest_fres || src_fres) && --retry );
        if ( !retry ) {
            DEBUG_PRINT( "2 : trans error! ( %d %d )\n", dest_fres, src_fres  );
            goto trans_end;
        }
    } while ( --tt );

    
trans_end:
    f_close( &mp3_fil );
    f_close( &out_fil );
    FREE( jpeg_buf );

    double ttime = 0;
    double sec   = 0;
    double speed = 0;
    
    /* 计算传输速度 */
    ttime = Clock_End();
    sec   = ttime / 84000.0;
    speed = (double)jpeg_size / sec;
    printf( "\nsec         : %f s\n", sec  );
    printf( "preriod     : %d s\n", (int)ttime  );
    printf( "trans speed : %0.4f MiB/s\n", speed/(1<<20) );

}
