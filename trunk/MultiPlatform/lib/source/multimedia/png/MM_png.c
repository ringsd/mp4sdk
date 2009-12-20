/***************************************************************************

		MM_png.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-07-25 21:34:19

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#define PNG_INTERNAL
#define PNG_NO_EXTERN

#include <global.h>
#include <png.h>
#include "MM_png.h"

#define ERROR -1
#define OK 0

struct MM_IMAGE_PNG{
	FILE * fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep bitmap;
	int width;
	int height;
	int number_passes;
	int memory_less;
};

static void user_error_fn( png_structp png_ptr, png_const_charp const_charp )
{
}

static void user_warning_fn( png_structp png_ptr, png_const_charp const_charp )
{
}

static int read_chunk_callback(png_structp ptr, png_unknown_chunkp chunk)
{
       /* The unknown chunk structure contains your
          chunk data, along with similar data for any other
          unknown chunks: */

       /* Note that libpng has already taken care of
          the CRC handling */

       /* put your code here.  Search for your chunk in the
          unknown chunk structure, process it, and return one
          of the following: */

       return 0; /* chunk had an error */
}
    
MM_IMAGE_PNG *		MM_png_open	( u8 *path )
{
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_infop end_info = NULL;
	png_voidp user_error_ptr = NULL;
	png_unknown_chunkp user_chunk_ptr = NULL;
	char header[8];
	int number = 8;
	int is_png;
	png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_method, number_passes;
    MM_IMAGE_PNG * image;
	
	FILE *fp = fopen(path, "rb");
    if (!fp)
		goto err;
    
    image = malloc(sizeof(MM_IMAGE_PNG));
    if( !image )
		goto err;
    
    fread(header, 1, number, fp);
    is_png = !png_sig_cmp(header, 0, number);
    if (!is_png)
		goto err;
    
	png_ptr = png_create_read_struct
		(PNG_LIBPNG_VER_STRING, (png_voidp)user_error_ptr,
		user_error_fn, user_warning_fn);
	if (!png_ptr)
		goto err;
		
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		goto err;
    
    end_info = png_create_info_struct(png_ptr);
	if (!end_info)
		goto err;
    
	if (setjmp(png_jmpbuf(png_ptr)))
		goto err;
	
	png_init_io(png_ptr, fp);
	
	png_set_sig_bytes(png_ptr, number);
	
	png_set_read_user_chunk_fn(png_ptr, user_chunk_ptr,
        read_chunk_callback);
    
    png_read_info(png_ptr, info_ptr);
    
    png_get_IHDR(png_ptr, info_ptr, &width, &height,
       &bit_depth, &color_type, &interlace_type,
       &compression_type, &filter_method);
	
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);
		
	if (bit_depth < 8)
		png_set_packing(png_ptr);
       
    if (color_type == PNG_COLOR_TYPE_GRAY &&
        bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
        
	if (color_type == PNG_COLOR_TYPE_GRAY || color_type== PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);
	
	//if (color_type == PNG_COLOR_TYPE_RGB)
    //    png_set_filler(png_ptr, filler, PNG_FILLER_BEFORE);
        
	if (color_type == PNG_COLOR_TYPE_RGB)
        png_set_filler(png_ptr, 0, PNG_FILLER_AFTER);
    
    //if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
     //   png_set_swap_alpha(png_ptr);
    
    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        png_set_bgr(png_ptr);
    
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
	}
    //if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    //    png_set_swap_alpha(png_ptr);
        
    //png_set_filler(png_ptr, filler, PNG_TRANSFORM_SWAP_ALPHA);
    
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
	}
	
	if(bit_depth == 16)
		png_set_strip_16(png_ptr);
		
	number_passes = png_set_interlace_handling(png_ptr);
	
    png_read_update_info(png_ptr,info_ptr);
	
	image->fp = fp;
	image->info_ptr = info_ptr;
	image->png_ptr = png_ptr;
	image->number_passes = number_passes;
	image->width = width;
	image->height = height;
	image->bitmap = NULL;
	image->memory_less = 0;
	
	/*
	if( number_passes > 1 )
	{
		int pass = number_passes - 1;
		u8 * buf = malloc( width * 4 );
		while( pass-- )
		{
			int i = height;
			while( i-- )
			{
				png_read_rows(image->png_ptr, &buf, NULL, 1);
			}
		}
		free( buf );
	}
	*/
	return image;
	
err:
	if( fp )
		fclose( fp );
	if( image )
		free( image );
	if( png_ptr )
		png_destroy_read_struct(&png_ptr,
           (png_infopp)NULL, (png_infopp)NULL);
    if( info_ptr )
		png_destroy_read_struct((png_structpp)NULL, &info_ptr,
			(png_infopp)NULL);
	return NULL;
}

int			MM_png_read		( MM_IMAGE_PNG * image, void *buf, int width, int height )
{
	return -1;
}

int			MM_png_write	( MM_IMAGE_PNG * image, void *buf, int width, int height )
{
	return -1;
}

void		MM_png_close	( MM_IMAGE_PNG * image )
{
	if( image->bitmap )
		free( image->bitmap );
	png_read_end(image->png_ptr, image->info_ptr);
    png_destroy_read_struct(&image->png_ptr, &image->info_ptr, (png_infopp)NULL);
    fclose( image->fp );
}

int 		MM_png_size		( MM_IMAGE_PNG * image, int * width, int * height  )
{
	if( image == NULL )
		goto err;
	*width = image->width;
	*height = image->height;
	return 0;
err:
	return -1;
}

int 		MM_png_readline	( MM_IMAGE_PNG * image, void *buf, int scanline )
{
	int pass = image->number_passes;
	/*
	if( pass <= 1 )
	{
		png_read_rows(image->png_ptr, (png_bytepp)&buf, NULL, 1);
		return 0;
	}
	if( image->now_pass == 0 )
	{
		png_bytep ptr = image->pass_buf;
		png_read_rows(image->png_ptr, (png_bytepp)&buf, NULL, 1);
		pass--;
		while( pass-- )
		{
			png_read_rows(image->png_ptr, &ptr, NULL, 1);
			ptr += image->width * 4;
		}
		image->now_pass++;
	}
	else
	{
		memcpy( buf, image->pass_buf+(image->now_pass-1)*image->width*4, image->width*4 );
		image->now_pass++;
		if( image->now_pass >= pass )
			image->now_pass = 0;
	}*/
	
	/* 第一次读取时会触发 */
	if( image->bitmap == NULL && image->memory_less == 0 && pass > 1 )
	{
		image->bitmap = malloc( image->width*image->height*4 );
		if( image->bitmap == NULL )
		{
			/* 内存不足, 跳过扫描过程 */
			int pass = image->number_passes - 1;
			png_bytep buf = malloc( image->width * 4 );
			image->memory_less = 1;
			
			if( buf == NULL )
				return -1;
			
			while( pass-- )
			{
				int i = image->height;
				while( i-- )
				{
					png_read_rows(image->png_ptr, &buf, NULL, 1);
				}
			}
			free( buf );
		}
		else
		{
			/* 读入整张图 */
			while( pass-- )
			{
				int i = image->height;
				png_bytep ptr = image->bitmap;
				while( i-- )
				{
					png_read_rows(image->png_ptr, &ptr, NULL, 1);
					ptr += image->width * 4;
				}
			}
		}
	}
	
	if( image->bitmap )
	{
		memcpy( buf, image->bitmap + scanline * image->width * 4, image->width * 4 );
	}
	else
	{
		png_bytep ptr = buf;
		png_read_rows(image->png_ptr, &ptr, NULL, 1);
		ptr += image->width * 4;
	}
	
	return 0;
}

