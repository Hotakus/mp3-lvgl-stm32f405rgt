/************************************************
 * @file lvgl_user.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-04
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "lvgl_user.h"
#include "lvgl.h"
#include "jpeglib.h"
#include "libjpeg_user.h"

static lv_img_decoder_t * jpeg_dec = NULL;     // JPEG文件decoder
static lv_img_decoder_t * png_dec = NULL;      // PNG文件decoder

static uint8_t jpeg_flag = 0;
static struct jpeg_decompress_struct cinfo;
static struct jpeg_error_mgr jerr;
static JFILE  jpeg_in_file;
static lv_res_t jpeg_decoder_info(lv_img_decoder_t * decoder, const void * src, lv_img_header_t * header);
static lv_res_t jpeg_decoder_open(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc);
static void jpeg_decoder_close(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc);
static lv_res_t jpeg_decoder_read_line(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t* dsc, lv_coord_t x,lv_coord_t y, lv_coord_t len, uint8_t * buf);


static lv_res_t png_decoder_info(lv_img_decoder_t * decoder, const void * src, lv_img_header_t * header);

/************************************************
 * @brief 用户 Decoder 的初始化函数
 ************************************************/
void lvgl_user_decoder_init( void )
{
    jpeg_dec = lv_img_decoder_create();
    lv_img_decoder_set_info_cb(jpeg_dec, jpeg_decoder_info);
    lv_img_decoder_set_open_cb(jpeg_dec, jpeg_decoder_open);
    lv_img_decoder_set_close_cb(jpeg_dec, jpeg_decoder_close);
    lv_img_decoder_set_read_line_cb(jpeg_dec, jpeg_decoder_read_line );
}

static lv_res_t jpeg_decoder_info(lv_img_decoder_t * decoder, const void * src, lv_img_header_t * header)
{

    if ( is_jpeg( (const char *)src ) != true )
       return LV_RES_INV;

    cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

    lv_fs_res_t ifres = lv_fs_open( &jpeg_in_file, src, LV_FS_MODE_RD );
    if ( ifres != FR_OK ) {
        DEBUG_PRINT( "Open jpg file : %s error (%d)\n", src, ifres );
        return LV_RES_INV;
    }
    jpeg_stdio_src(&cinfo, &jpeg_in_file);
    jpeg_read_header(&cinfo, TRUE);
    

    header->cf = LV_IMG_CF_TRUE_COLOR;
    header->w  = cinfo.image_width;
    header->h  = cinfo.image_height;

    lv_fs_close( &jpeg_in_file );
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return LV_RES_OK;
}

static lv_res_t jpeg_decoder_open(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc)
{

   if ( is_jpeg( (const char *)dsc->src ) != true )
       return LV_RES_INV;

    dsc->img_data = NULL;

    cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

    lv_fs_res_t ifres = lv_fs_open( &jpeg_in_file, dsc->src, LV_FS_MODE_RD );
    if ( ifres != FR_OK ) {
        DEBUG_PRINT( "Open jpg file : %s error (%d)\n", dsc->src, ifres );
        return LV_RES_INV;
    }
    jpeg_stdio_src(&cinfo, &jpeg_in_file);
    jpeg_read_header(&cinfo, TRUE);

    jpeg_start_decompress(&cinfo);

    return LV_RES_OK;
}

static void jpeg_decoder_close(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc)
{
    lv_fs_close( &jpeg_in_file );
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
}

static lv_res_t jpeg_decoder_read_line(
    lv_img_decoder_t * decoder, 
    lv_img_decoder_dsc_t* dsc, 
    lv_coord_t x,
    lv_coord_t y, 
    lv_coord_t len, 
    uint8_t * buf
)
{
    uint32_t raw_stride = cinfo.output_width * cinfo.output_components;
    uint8_t *line_buf = (uint8_t*)lv_mem_alloc( raw_stride );
    uint16_t *rgb565_buf = (uint16_t*)lv_mem_alloc( cinfo.output_width * sizeof(uint16_t) );

    jpeg_read_scanlines( &cinfo, &line_buf, 1 );
    rgb888_to_rgb565( line_buf, raw_stride, rgb565_buf );
    for ( uint32_t x = 0, k = 0; x < cinfo.output_width*2; x += 2 , k += 1 ) {
        buf[x] = rgb565_buf[k]&0xFF;
        buf[x+1] = rgb565_buf[k]>>8;
    }

    lv_mem_free( line_buf );
    lv_mem_free( rgb565_buf );
    return LV_RES_OK;
}
