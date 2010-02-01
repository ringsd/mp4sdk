/***************************************************************************

		xfont.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-01 13:06:34

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#define _font_c_
#include <global.h>
#include <string.h>
#include "ft2.h"
#include "ccbmf.h"
#include "font_cache.h"
#include "font_ascii.h"

#ifdef _MBCS
#define stricmp _stricmp
#endif

struct XFONT {
	void * handle;
	void * (*xfont_open)( const char * path );
	int (*xfont_read)( void * xfont, int code, int size, int style, u8 * buf, int limit, XFONT_INFO * info );
	int (*xfont_getsize)( void * xfont, int code, int size, int style, XFONT_INFO * info );
	const char * (*xfont_name)( void * xfont );
	void (*xfont_close)( void * xfont );
	char *		extname;
};

XFONT xfont_types[] = {
{			NULL,		xfont_freetype2_open,			xfont_freetype2_read,			xfont_freetype2_getsize,	ft2_font_name,	xfont_freetype2_close,		"TTF"		},
{			NULL,		xfont_freetype2_open,			xfont_freetype2_read,			xfont_freetype2_getsize,	ft2_font_name,	xfont_freetype2_close,		"TTC"		},
{			NULL,		ccbmf_open,						ccbmf_read,						ccbmf_getsize,				ccbmf_name,		ccbmf_close,				"BMF"		},
{			NULL,		NULL,			NULL,			NULL,		NULL,		NULL		},	//END_BLOCK
};

//shell提供的字体默认目录路径,以'\'结尾(当传入的路径仅仅为文件名时,就在该目录中找)
extern char xfont_fonts_path[];

XFONT * xfont_open( const char * path )
{
	int i;
	char * dot;
	char tpath[ MAX_PATH ];
	
	if( (strrchr( path, '\\' ) == 0) &&		//判断路径是否仅仅包括文件名
		(strrchr( path, '/' ) == 0) )
	{
		strcpy( tpath, xfont_fonts_path );
		strcat( tpath, path );
		path = tpath;
	}
	
	dot = strrchr( path, '.' );
	if( dot == NULL )
		return NULL;
	dot++;
	i = 0;

	while( xfont_types[i].extname )
	{
		if( stricmp( dot, xfont_types[i].extname ) == 0 )
		{
			XFONT * xfont = malloc( sizeof(XFONT) );
			if( xfont )
			{
				xfont->xfont_open	= xfont_types[i].xfont_open;
				xfont->xfont_read	= xfont_types[i].xfont_read;
				xfont->xfont_close	= xfont_types[i].xfont_close;
				xfont->xfont_getsize	= xfont_types[i].xfont_getsize;
				xfont->xfont_name   = xfont_types[i].xfont_name;
				xfont->handle	= xfont->xfont_open( path );
				if( xfont->handle == NULL )
				{
					free(xfont);
					xfont = NULL;
				}
			}
			return xfont;
		}
		i++;
	}
	return NULL;
}

XFONT_CACHE * cache_handle = NULL;

int xfont_cache_init( void )
{
	cache_handle = xfont_cache_open( xfont_cache_memory_size, xfont_cache_font_max_size );
	if( cache_handle )
		return 0;
	return -1;
}

int xfont_cache_deinit( void )
{
	if( cache_handle == NULL )
		return -1;
	xfont_cache_close( cache_handle );
	cache_handle = NULL;
	return 0;
}

int xfont_uread( XFONT * xfont, int code, int size, int style, u8 * buf, int limit, XFONT_INFO * info )
{
	if( xfont == NULL )		//传入NULL指针,使用内置8*16点阵英文字库
	{
		display_byte_gray8( code, buf, 8 );
		info->bitmap_height = 16;
		info->font_width = 8;
		info->bitmap_left = 0;
		info->bitmap_top = 16;
		info->bitmap_width = 8;
		return 0;
	}
	
	//cache读取,暂时只对GUI默认大小的字体起效
	if( cache_handle && (size == gui_default_font_size) )
	{
		char * data;
		if( (data = xfont_cache_read( cache_handle, xfont, info, code, size, style )) != NULL )
		{
			memcpy( buf, data, size*size );
			return 0;
		}
		else
		{
			if( xfont->xfont_read( xfont->handle, code, size, style, buf, limit, info ) == 0 )
			{
				xfont_cache_write( cache_handle, xfont, info, code, size, style, buf );
				return 0;
			}
			return -1;
		}
	}
	return xfont->xfont_read( xfont->handle, code, size, style, buf, limit, info );
}

int xfont_getsize( XFONT * xfont, int code, int size, int style, XFONT_INFO * info )
{
	return xfont->xfont_getsize( xfont->handle, code, size, style, info );
}

const char * xfont_name( XFONT * xfont )
{
	return xfont->xfont_name( xfont->handle );
}

void xfont_close( XFONT * xfont )
{
	xfont->xfont_close( xfont->handle );
	free( xfont );
}

#define MAX_WIDTH 256
#define MAX_HEIGHT 256

static void xfont_udraw_rgb565( 
    XFONT * xfont, int code, int size, int style, u16 color, u16 bgcolor,
    COLOR_MODE color_mode, XFONT_BLEND_MODE blending_mode,
    int startx, int starty, int pitch,
    void * buf, int * width, int * height, int limit
)
{
	int x, y, endx, endy, line_add;
	u16 * dst;
	u8 * src;
	XFONT_INFO info;
	u32 fc_r = ( color & 0xF800 );
	u32 fc_g = ( color & 0x07E0 );
	u32 fc_b = ( color & 0x001F );
	u8 graybuf[ MAX_WIDTH*MAX_HEIGHT ];
	
	if( color_mode != LCD_R5G6B5 )
		return;
	
	xfont_uread( xfont, code, size, style, graybuf, MAX_WIDTH*MAX_HEIGHT, &info );
	endx = info.bitmap_width + startx;
	endy = info.bitmap_height + starty;
	src = graybuf;
	dst = (u16*)buf + pitch * (starty-info.bitmap_top) + startx + info.bitmap_left;
	line_add = pitch - (endx - startx);
	
	for( y = starty; y < endy; y ++ )
	{
		if( (u32)dst >= (u32)((u32)buf + limit) )
			break;
		for( x = startx; x < endx; x ++ )
		{ 
			int gray = *src;
			int gray_bg = 256 - gray;
			u32 data;
			
			switch( blending_mode )
			{
				case XFONT_BLEND_MODE_COVER:
					data = bgcolor;
					break;
				case XFONT_BLEND_MODE_ALPHA:
					data = *dst;
					break;
				default:
					goto err;
			}
			
			if( gray == 256 )
			{
				*dst = color;
			}
			else if( gray > 0 )
			{
				u32 r = (	( data ) * ( gray_bg ) + 
								( fc_r * gray ) ) & 0xF80000;
				u32 g = (	( data & 0x07E0 ) * ( gray_bg ) + 
								( fc_g * gray ) ) & 0x07E000;
				u32 b = (	( data & 0x001F )	* ( gray_bg ) + 
								( fc_b * gray ) ) & 0x001F00;
				*dst = (u16)((r | g | b)>>8);
			}
			src++;
			dst++;
		}
		dst += line_add;
	}
err:
	*width = info.font_width;
	*height = info.bitmap_height;
}

static void xfont_udraw_rgb555( 
    XFONT * xfont, int code, int size, int style, u16 color, u16 bgcolor,
    COLOR_MODE color_mode, XFONT_BLEND_MODE blending_mode,
    int startx, int starty, int pitch,
    void * buf, int * width, int * height, int limit
)
{
	int x, y, endx, endy, line_add;
	u16 * dst;
	u8 * src;
	XFONT_INFO info;
	u32 fc_r = ( color & 0x7C00 );
	u32 fc_g = ( color & 0x03E0 );
	u32 fc_b = ( color & 0x001F );
	u16 fc_a = ( color & 0x8000 );
	u8 graybuf[ MAX_WIDTH*MAX_HEIGHT ];
	
	if( color_mode != LCD_A1R5G5B5 )
		return;
	
	xfont_uread( xfont, code, size, style, graybuf, MAX_WIDTH*MAX_HEIGHT, &info );
	endx = info.bitmap_width + startx;
	endy = info.bitmap_height + starty;
	src = graybuf;
	dst = (u16*)buf + pitch * (starty-info.bitmap_top) + startx + info.bitmap_left;
	line_add = pitch - (endx - startx);
	
	for( y = starty; y < endy; y ++ )
	{
		if( (u32)dst >= (u32)((u32)buf + limit) )
			break;
		for( x = startx; x < endx; x ++ )
		{ 
			int gray = *src;
			int gray_bg = 256 - gray;
			u32 data;
			
			switch( blending_mode )
			{
				case XFONT_BLEND_MODE_COVER:
					data = bgcolor;
					break;
				case XFONT_BLEND_MODE_ALPHA:
					data = *dst;
					break;
				default:
					goto err;
			}
			
			if( gray == 256 )
			{
				*dst = color;
			}
			else if( gray > 0 )
			{
				u32 r = (	( data ) * ( gray_bg ) + 
								( fc_r * gray ) ) & 0x7C0000;
				u32 g = (	( data & 0x07E0 ) * ( gray_bg ) + 
								( fc_g * gray ) ) & 0x03E000;
				u32 b = (	( data & 0x001F )	* ( gray_bg ) + 
								( fc_b * gray ) ) & 0x001F00;
				*dst = (u16)((r | g | b)>>8) | fc_a;
			}
			src++;
			dst++;
		}
		dst += line_add;
	}
err:
	*width = info.font_width;
	*height = info.bitmap_height;
}

static void xfont_udraw_rgb888
                (   XFONT * xfont, int code, int size, int style, u32 color, u32 bgcolor,
                    COLOR_MODE color_mode, XFONT_BLEND_MODE blending_mode,
                    int startx, int starty, int pitch,
                    void * buf, int * width, int * height, int limit
                )
{
	int x, y, endx, endy, line_add;
	u32 *dst;
	u8 * src;
	XFONT_INFO info;
	u32 fc_r = ( color & 0xFF0000 );
	u32 fc_g = ( color & 0xFF00 );
	u32 fc_b = ( color & 0xFF );
	u8 graybuf[ MAX_WIDTH*MAX_HEIGHT ];
	
	if( color_mode != LCD_A8R8G8B8 )
		return;
	
	xfont_uread( xfont, code, size, style, graybuf, MAX_WIDTH*MAX_HEIGHT, &info );
	endx = info.bitmap_width + startx;
	endy = info.bitmap_height + starty;
	src = graybuf;
	dst = (u32*)buf + pitch * (starty-info.bitmap_top) + startx + info.bitmap_left;
	line_add = pitch - (endx - startx);
	
	for( y = starty; y < endy; y ++ )
	{
		if( (u32)dst >= (u32)((u32)buf + limit) )
			break;
		for( x = startx; x < endx; x ++ )
		{ 
			int gray = *src;
			u32 data;
			int gray_bg = 255-gray;
			switch( blending_mode )
			{
				case XFONT_BLEND_MODE_COVER:
					data = bgcolor;
					break;
				case XFONT_BLEND_MODE_ALPHA:
					data = *dst;
					break;
				default:
					goto err;
			}
			
			if( gray == 255 )
			{
				*dst = color;
			}
			else if( gray > 0 )
			{
				register int mask = 0xFF;
				u32 r = (	( data & (mask<<16) ) * ( gray_bg ) + 
								( fc_r * gray ) ) & (mask<<24);
				u32 g = (	( data & (mask<<8) ) * ( gray_bg ) + 
								( fc_g * gray ) ) & (mask<<16);
				u32 b = (	( data & mask )	* ( gray_bg ) + 
								( fc_b * gray ) );
				*dst = ((r | g | b)>>8) | (mask & (mask<<24));
			}
			src++;
			dst++;
		}
		dst += line_add;
	}
err:
	*width = info.font_width;
	*height = info.bitmap_height;
}

void xfont_udraw
                (   XFONT * xfont, int code, int size, int style, u32 color, u32 bgcolor,
                    COLOR_MODE color_mode, XFONT_BLEND_MODE blending_mode,
                    int startx, int starty, int pitch,
                    void * buf, int * width, int * height, int limit
                )
{
    switch( color_mode )
    {
        case LCD_A8R8G8B8:
            xfont_udraw_rgb888
                (   xfont, code, size, style, color, bgcolor,
                    color_mode, blending_mode,
                    startx, starty, pitch,
                    buf, width, height,limit
                );
            break;
        case LCD_R5G6B5:
            xfont_udraw_rgb565
                (   xfont, code, size, style, (u16)color, (u16)bgcolor,
                    color_mode, blending_mode,
                    startx, starty, pitch,
                    buf, width, height,limit
                );
            break;
        case LCD_A1R5G5B5:
            xfont_udraw_rgb555
                (   xfont, code, size, style, (u16)color, (u16)bgcolor,
                    color_mode, blending_mode,
                    startx, starty, pitch,
                    buf, width, height,limit
                );
            break;
    }
}
