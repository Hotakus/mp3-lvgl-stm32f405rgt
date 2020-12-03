/************************************************
 * @file main.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-01
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/
#include "main.h"
#include "jpeglib.h"

void rgb888_to_rgb565(unsigned char* rgb888_buf, int rgb888_size, uint16_t *rgb565_buf)
{
    uint8_t  Red = 0 ;
    uint8_t  Green = 0 ;
    uint8_t  Blue = 0 ;
    uint32_t count = 0 ;
 
    if(rgb888_buf == NULL || rgb888_size <= 0 || rgb565_buf == NULL) {
        printf("Invail input parameter in %s\n", __FUNCTION__) ;
        return;
    }
 
    for(uint32_t i = 0; i<rgb888_size; i += 3) {
        Red   = rgb888_buf[i] >> 3;
        Green = rgb888_buf[i+1] >> 2 ;
        Blue  = rgb888_buf[i+2] >> 3;
        rgb565_buf[count++] = ( (Red<<11)|(Green<<5)|(Blue) ) ;
    }
}

int libjpeg_decompress( int argc, char **argv )
{
    if ( argc != 2 ) {
        DEBUG_PRINT( "Usage : libjpeg_test <jpg file>\n" );
        return -1;
    }

    struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

    JFILE in_file;
    FRESULT ifres = f_open( &in_file, argv[1], FA_READ | FA_OPEN_EXISTING );
    if ( ifres != FR_OK ) {
        DEBUG_PRINT( "Open jpg file : %s error (%d)\n", argv[1], ifres );
        return -1;
    }
    jpeg_stdio_src(&cinfo, &in_file);

    jpeg_read_header(&cinfo, TRUE);
    DEBUG_PRINT("image_width = %d\n", cinfo.image_width);
	DEBUG_PRINT("image_height = %d\n", cinfo.image_height);
	DEBUG_PRINT("num_components = %d\n", cinfo.num_components);

    cinfo.scale_num = 1;
    cinfo.scale_denom = 10;

    jpeg_start_decompress(&cinfo);

    DEBUG_PRINT("output_width = %d\n", cinfo.output_width);
	DEBUG_PRINT("output_height = %d\n", cinfo.output_height);
	DEBUG_PRINT("output_components = %d\n", cinfo.output_components);


    uint32_t raw_stride = cinfo.output_width * cinfo.output_components;

    uint8_t *line_buf = (uint8_t*)MALLOC( raw_stride );
    uint16_t *rgb565_buf = (uint16_t*)MALLOC( cinfo.output_width * sizeof(uint16_t) );


    scr_opr_handler *scr = scr_get_opr_handler();
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines( &cinfo, &line_buf, 1 );
        rgb888_to_rgb565( line_buf, raw_stride, rgb565_buf );
        for ( uint32_t x = 0; x < cinfo.output_width; x += 1 ) {
            scr->draw_pixel( x, cinfo.output_scanline-1, rgb565_buf[x] );
        }
    }


    FREE( line_buf );
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    f_close( &in_file );
}
MSH_CMD_EXPORT( libjpeg_decompress, libjpeg_test );

int libjpeg_compress( int argc, char **argv )
{

}

int main()
{
    return 0;
}

