/***************************************************************************

		MM_ico.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-21 10:09:43

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#define _MM_ico_c_
#include <global.h>
#include "msicon.h"
#include "MM_imager.h"
#include "MM_ico.h"

struct MM_IMAGE_ICO{
	ico_reader_t *ico;
	i_io_glue_t *ig;
};

MM_IMAGE_ICO * MM_ico_open( u8 *file )
{
	i_io_glue_t *ig;
	int error;
	ico_reader_t *ico;
	MM_IMAGE_ICO *image_ico;
	
	ig = malloc( sizeof(i_io_glue_t) );
	if( ig == NULL )
		return NULL;
		
	ig->readcb = img_fread;					//初始化虚拟接口
	ig->writecb = img_fwrite;
	ig->seekcb = img_fseek;
	ig->closecb = img_fclose;
	ig->sizecb = img_size;
	ig->exdata = fopen( file, "rb" );
	
	if( ig->exdata == NULL )
		return NULL;

	ico = ico_reader_open( ig, &error );
	
	if( ico == NULL )
	{
		fclose( ig->exdata );
		return NULL;
	}
	
	image_ico = malloc( sizeof(MM_IMAGE_ICO) );
	image_ico->ico = ico;
	image_ico->ig = ig;
	return image_ico;
}

int MM_ico_read( MM_IMAGE_ICO * image_ico, void *buf, int width, int height )
{
	int count, i, error;
	ico_image_t  *image;
	ico_reader_t *ico = image_ico->ico;
	GA_RECT srcblock, dstblock;
	int max_choose = -1;
	int opti_choose = -1;
	int max_width = 0, max_height = 0;
	int min_diff_width = 100000, min_diff_height = 100000;

	dstblock.left = 0;
	dstblock.right = width - 1;
	dstblock.top = 0;
	dstblock.bottom = height - 1;
	
	count = ico_image_count( ico );
	for( i = 0; i < count; i++ )
	{
		image = ico_image_read( ico, i, &error );
		if( image )
		{
			if( min_diff_width > abs(image->width - width) )
			{
				min_diff_width = abs(image->width - width);
				opti_choose = i;
			}

			if( min_diff_height > abs(image->height - height) )
			{
				min_diff_height = abs(image->height - height);
				opti_choose = i;
			}

			if( image->width > max_width && image->height > max_height)
			{
				 max_choose = i;
				 max_width = image->width;
				 max_height = image->height;
			}

			printf( "bit:%d width:%d height:%d\n", image->bit_count, image->width, image->height );

			ico_image_release( image );
		}
	}
	if( opti_choose == -1 )
		opti_choose = max_choose;
	printf( "opti_choose=%d\n", opti_choose );
	image = ico_image_read( ico, opti_choose, &error );

	if( image )
	{
		srcblock.left = 0;
		srcblock.right = image->width - 1;
		srcblock.top = 0;
		srcblock.bottom = image->height - 1;

		switch( image->bit_count )
		{
			case 4:
				GA_RectCopy_Bit4_A8R8G8B8_MASK( &srcblock, &srcblock, image->image_data, &dstblock, &dstblock, buf, image->palette, image->mask_data );
				break;
			case 8:
				GA_RectCopy_Bit8_A8R8G8B8_MASK( &srcblock, &srcblock, image->image_data, &dstblock, &dstblock, buf, image->palette, image->mask_data );
				break;
			case 24:
				break;
			case 32:
				GA_RectCopy_A8R8G8B8( &srcblock, &srcblock, image->image_data, &dstblock, &dstblock, buf );
				break;
		}
		printf( "bit:%d width:%d height:%d\n", image->bit_count, image->width, image->height );
		ico_image_release( image );
	}

	return 0;
}

void MM_ico_close( MM_IMAGE_ICO * image_ico )
{
	ico_reader_close( image_ico->ico );
	fclose( image_ico->ig->exdata );
	free( image_ico->ig );
	free( image_ico );
}

#if 0
void MM_ico_test( void )
{
	int width = 64, height = 64;
	u8 *buf = malloc( width * height * 4 );
	GA_RECT srcblock, wndblock;
	MM_IMAGE_ICO * image_ico;
	srcblock.left = 0;
	srcblock.right = width;
	srcblock.top = 0;
	srcblock.bottom = height;
	wndblock.left = 0;
	wndblock.right = screen_get_width();
	wndblock.top = 0;
	wndblock.bottom = screen_get_height();

	image_ico = MM_ico_open("test.ico");
	MM_ico_read( image_ico, buf, width, height );
	MM_ico_close( image_ico );
	GA_RectCopy_A8R8G8B8( &srcblock, &srcblock, buf, &srcblock, &wndblock, lcd_bufferui() );
	lcd_updateui();
	free( buf );
	menu_get_key();
}
#endif
//---------------------MultiMedia Image END--------------------------------

