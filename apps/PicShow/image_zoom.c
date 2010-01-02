/***************************************************************************

		image_zoom.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-26 12:56:59

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <global.h>
#include "MM_Image.H"
#include "image_cache.h"
#include "bitmap_zoom.h"
#include "image_zoom.h"
#include "ps_common.h"

//以缩小[divide]倍的方式读取image一行
static void image_zoom_read_line( IMAGE_ZOOM * image_zoom, u32 * buf, int divide, int line )
{
	int width = image_zoom->real_width;
	int height = image_zoom->real_height;
	int i, j, real_line;
	int x, y;
	u32 * linebuffer = malloc( divide * width * 4 );
	u32 * pixel = linebuffer;
	
	if( linebuffer == NULL )
		goto err;
	
	real_line = line * divide;		//转换到原始图像行
	for( i = 0; i < divide; i++ )
		MM_image_readline( image_zoom->image, linebuffer + width * i, real_line + i );
	
	for( x = 0; x < image_zoom->width; x++ )
	{
		u32 r = 0, g = 0, b = 0;
		int div2 = divide*divide;
		for( i = 0; i < divide; i++ )
		{
			for( j = 0; j < divide; j++ )
			{
				r += (pixel[i+j*width] >> 16)&0xFF;
				g += (pixel[i+j*width] >> 8)&0xFF;
				b += (pixel[i+j*width])&0xFF;
			}
		}
		r = r / div2;
		g = g / div2;
		b = b / div2;
		pixel += divide;
		buf[ x ] = (r << 16) | (g << 8) | b;
	}
	
	free( linebuffer );
err:
	return;
}


//直接读取原始图像并缩放(用于内存不足时)
static void image_zoom_read_raw( IMAGE_ZOOM * image_zoom, u32 * dst, int sx, int sy, int cw, int ch, int dw, int dh )
{
	int x, y;
	u32 * line_start , * limit_line, * min_line;
	//u32 * limit_src;
	int pitchx, pitchy, sw, sh;
	u32 * line_buffer;
	//BITMAP_ZOOM * zoom = image_zoom->bitmap_zoom;
	
	sw = image_zoom->real_width;
	sh = image_zoom->real_height;
	
	line_buffer = malloc( sw * 4 );
	if( line_buffer == NULL ) goto err;
	
	image_cache_readline_read_setempty( image_zoom->cache_line, sy, dh, ch );
	//limit_src  = zoom->data + sw * sh;
	line_start = line_buffer + sx;
	limit_line = line_buffer + sw;
	min_line   = line_buffer;
	pitchy = 0;
	for( y = 0; y < ch; y++ )
	{
		u32 *src = line_start;
		/*
		if( src + sw >= limit_src )
		{
			for( x = 0; x < cw; x++ )
			{
				dst[x] = 0;
			}
			dst += cw;
		}
		else*/
		{
			register u32 pixel = 0;
			image_cache_readline_read( image_zoom->cache_line, line_buffer, (y+sy*dh/sh)*sh/dh );
			pitchx = dw;
			for( x = 0; x < cw; x++ )
			{
				int count = 0;
				pitchx += sw;
				while( pitchx >= dw )
				{
					if( src >= limit_line || src < min_line )
						pixel = 0;
					else if( count == 0 )
						pixel = *src;
					else
						pixel = ( (src[0]>>1) & 0x7F7F7F7F ) + ( (src[1]>>1) & 0x7F7F7F7F );
					src++;
					pitchx -= dw;
					count++;
				}
				*dst = pixel;
				dst++;
			}
		}
		
		pitchy += sh;
		while( pitchy >= dh )
		{
			//line_start += sw;
			//limit_line += sw;
			//min_line   += sw;
			pitchy -= dh;
		}
		
	}
	
	free( line_buffer );
err:
	return;
}

//读取缩放的图像(快速,自由缩放)
void image_zoom_read( IMAGE_ZOOM * image_zoom, BITMAP_ZOOM_DIRECTION direction, u32 * dst, int sx, int sy, int cw, int ch, int dw, int dh, int quick )
{
	if( dw <= image_zoom->width || image_zoom->width == image_zoom->real_width || quick || !image_zoom->random_enable )
		bitmap_zoom_read( image_zoom->bitmap_zoom, direction, dst, sx*image_zoom->width/image_zoom->real_width,
		 sy*image_zoom->height/image_zoom->real_height, cw, ch, dw, dh );
	else
		image_zoom_read_raw( image_zoom, dst, sx, sy, cw, ch, dw, dh );
}

//以缩小的方式读取image
static void image_zoom_load( IMAGE_ZOOM * image_zoom, u32 * buf, int divide )
{
	int i;
	
	#ifdef USE_PREVIEW
	u32 * screen = lcd_bufferui();
	int width = screen_get_width();
	int height = screen_get_height();
	u32 * screen_limit = screen + width * height;
	int copy_lines;
	#endif
	
	for( i = 0; i < image_zoom->height; i++ )
	{
		image_zoom_read_line( image_zoom, buf, divide, i );
		#ifdef USE_PREVIEW
		if( load_preview )
		{
			copy_lines = (i+1)*height/(image_zoom->height*divide) - i*height/(image_zoom->height*divide);
			if( copy_lines > 0 && screen < screen_limit )
			{
				int x, y;
				for( y = 0; y < max(copy_lines, divide); y++ )
				{
					for( x = 0; x < width; x++ )
					{
						register int off = x*image_zoom->height/height;
						if( off >= image_zoom->width )
							break;
						screen[x] = buf[off];//((buf[off]&0xFEFEFE) + (screen[x]&0xFEFEFE))>>1;
					}
					screen += width;
					if( i % 50 == 0 )
					{
						lprintf( 0, 0, "load : %d%%", i*100/image_zoom->height );
					}
				}
			}
		}
		#endif
		buf += image_zoom->width;
	}
}


IMAGE_ZOOM * image_zoom_open( MM_IMAGE * image )
{
	int width, height, divide = 1;
	u32 * buf;
	IMAGE_ZOOM * image_zoom = malloc( sizeof(IMAGE_ZOOM) );
	
	if( image_zoom == NULL )
		goto err;
		
	image_zoom->random_enable = MM_image_is_random_read( image );
		
	image_zoom->cache_line = image_cache_readline_open( image );	//打开一个cache句柄
	if( image_zoom->cache_line == NULL )
		goto err;
		
	MM_image_size	( image, &width, &height  );
	
	image_zoom->real_height = height;
	image_zoom->real_width = width;
	
	while( ( buf = malloc( (width * height * 4) * 4 / 3 ) ) == NULL )
	//while( (width * height * 4) * 4 / 3 > 1000000 )
	{
		width = width/2;
		height = height/2;
		divide = divide*2;
	}
	free( buf );		//前面申请的内存仅仅是用来查询内存空间用的,必须释放
	buf = malloc( (width * height * 4) );
	image_zoom->buffer = buf;
	image_zoom->height = height;
	image_zoom->width = width;
	image_zoom->image = image;
	image_zoom_load( image_zoom, buf, divide );
	image_zoom->bitmap_zoom = bitmap_zoom_open( buf, width, height );
	return image_zoom;
err:
	return NULL;
}

void image_zoom_close( IMAGE_ZOOM * image_zoom )
{
	image_cache_readline_close( image_zoom->cache_line );
	bitmap_zoom_close( image_zoom->bitmap_zoom );
	free( image_zoom->buffer );
	free( image_zoom );
}
