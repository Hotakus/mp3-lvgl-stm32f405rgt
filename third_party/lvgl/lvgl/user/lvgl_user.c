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
    /*Check whether the type `src` is known by the decoder*/
    if ( is_jpeg( (const char *)src ) != true )
        return LV_RES_INV;

    cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

    FRESULT ifres = f_open( &jpeg_in_file, src, FA_READ | FA_OPEN_EXISTING );
    if ( ifres != FR_OK ) {
        DEBUG_PRINT( "Open jpg file : %s error (%d)\n", src, ifres );
        return LV_RES_INV;
    }
    jpeg_stdio_src(&cinfo, &jpeg_in_file);
    jpeg_read_header(&cinfo, TRUE);

    /* Read the PNG header and find `width` and `height` */
    header->cf = LV_IMG_CF_RAW_ALPHA;
    header->w  = cinfo.image_width;
    header->h  = cinfo.image_height;

    return LV_RES_OK;
}

static lv_res_t jpeg_decoder_open(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc)
{
    if ( is_jpeg( (const char *)dsc->src ) != true )
        return LV_RES_INV;

    dsc->img_data = NULL;

    return LV_RES_OK;
}

static void jpeg_decoder_close(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc)
{
    f_close( &jpeg_in_file );
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
    jpeg_read_scanlines( &cinfo, &buf, 1 );
}
