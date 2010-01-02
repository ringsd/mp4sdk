/***************************************************************************

		bitmap_zoom.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-26 12:53:13

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <global.h>
#include "bitmap_zoom.h"

#define LIMIT_WIDTH 32
#define LIMIT_HEIGHT 32

static void bitmap_zoom_2x( u32 * dst, u32 * src, int sw, int sh )
{
	u32 * pix, * tmp;
	int dw, dh, x, y;
	dw = sw/2;
	dh = sh/2;

	pix = dst;
	for( y = 0; y < dh; y ++ )
	{
		tmp = src;
		for( x = 0; x < dw; x ++ )
		{
			u32 p00 = tmp[0];
			u32 p01 = tmp[1];
			u32 p10 = tmp[sw];
			u32 p11 = tmp[sw+1];
			*pix = ( (p00>>2) & 0x3F3F3F3F ) + ( (p01>>2) & 0x3F3F3F3F ) + ( (p10>>2) & 0x3F3F3F3F ) + ( (p11>>2) & 0x3F3F3F3F );
			tmp += 2;
			pix ++;
		}
		src += sw * 2;
	}

}

BITMAP_ZOOM * bitmap_zoom_open( u32 * bitmap, int width, int height )
{
	BITMAP_ZOOM * rootzoom, * zoom;
	rootzoom = malloc( sizeof(BITMAP_ZOOM) );
	if( rootzoom == NULL )
		goto err;
	rootzoom->data = bitmap;
	rootzoom->height = height;
	rootzoom->width = width;
	rootzoom->next = NULL;
	
	zoom = rootzoom;
	while( width > LIMIT_WIDTH || height > LIMIT_HEIGHT )
	{
		BITMAP_ZOOM * tzoom;
		u32 *buf;
		tzoom = malloc( sizeof(BITMAP_ZOOM) );
		if( tzoom == NULL )
			goto err;
		buf = malloc( width * height );
		if( buf == NULL )
			goto err;
		bitmap_zoom_2x( buf, zoom->data, width, height );
		width = width/2;
		height = height/2;
		tzoom->width = width;
		tzoom->height = height;
		tzoom->data = buf;
		tzoom->next = NULL;
		zoom->next = tzoom;
		zoom = tzoom;
	}

	return rootzoom;
err:
	zoom = rootzoom;
	while( zoom )
	{
		BITMAP_ZOOM * tzoom = zoom->next;
		if( zoom->data != NULL && zoom->data != bitmap )
			free( zoom->data );
		free( zoom );
		zoom = tzoom;
	}
	return NULL;
}

void bitmap_zoom_close( BITMAP_ZOOM * zoom )
{
	BITMAP_ZOOM * tzoom;
	tzoom = zoom->next;
	free( zoom );
	zoom = tzoom;
	while( zoom )
	{
		tzoom = zoom->next;
		if( zoom->data )
			free( zoom->data );
		free( zoom );
		zoom = tzoom;
	}
}

//正常显示画面
static void bitmap_zoom_read_normal( BITMAP_ZOOM * zoom, u32 * dst, int sx, int sy, int cw, int ch, int dw, int dh )
{
	int x, y;
	u32 * line_start , * limit_line, * min_line;
	u32 * limit_src;
	int pitchx, pitchy, sw, sh;

	while( zoom->width > dw )
	{
		if( zoom->next == NULL )
			break;
		if( zoom->next->width < dw )
			break;
		zoom = zoom->next;
		sx = sx/2;
		sy = sy/2;
	}
	
	sw = zoom->width;
	sh = zoom->height;
	limit_src  = zoom->data + sw * sh;
	line_start = zoom->data + sx + sy * sw;
	limit_line = zoom->data + sw + sy * sw;
	min_line   = zoom->data + sy * sw;
	pitchy = 0;
	for( y = 0; y < ch; y++ )
	{
		u32 *src = line_start;
		
		if( src + sw >= limit_src )
		{
			for( x = 0; x < cw; x++ )
			{
				dst[x] = 0;
			}
			dst += cw;
		}
		else
		{
			register u32 pixel = 0;
			pitchx = dw;
			for( x = 0; x < cw; x++ )
			{
				int count = 0;
				pitchx += sw;
				while( pitchx >= dw )
				{
					if( src >= limit_line || src < min_line || src < zoom->data || src >= limit_src )
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
			line_start += sw;
			limit_line += sw;
			min_line   += sw;
			pitchy -= dh;
		}
		
	}

}

//旋转180度显示画面
static void bitmap_zoom_read_right180( BITMAP_ZOOM * zoom, u32 * in_dst, int sx, int sy, int cw, int ch, int dw, int dh )
{
	int x, y;
	u32 * line_start , * limit_line, * min_line;
	u32 * limit_src;
	u32 * dst;
	int pitchx, pitchy, sw, sh;

	while( zoom->width > dw )
	{
		if( zoom->next == NULL )
			break;
		if( zoom->next->width < dw )
			break;
		zoom = zoom->next;
		sx = sx/2;
		sy = sy/2;
	}
	
	sw = zoom->width;
	sh = zoom->height;
	limit_src  = zoom->data + sw * sh;
	line_start = zoom->data + sx + sy * sw;
	limit_line = zoom->data + sw + sy * sw;
	min_line   = zoom->data + sy * sw;
	pitchy = 0;
	for( y = 0; y < ch; y++ )
	{
		u32 *src = line_start;
		dst = in_dst + (ch - y)*cw - 1;
		if( src + sw >= limit_src )
		{
			for( x = 0; x < cw; x++ )
			{
				*dst-- = 0;
			}
		}
		else
		{
			register u32 pixel = 0;
			pitchx = dw;
			for( x = 0; x < cw; x++ )
			{
				int count = 0;
				pitchx += sw;
				while( pitchx >= dw )
				{
					if( src >= limit_line || src < min_line || src < zoom->data || src >= limit_src )
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
				dst--;
			}
		}
		
		pitchy += sh;
		while( pitchy >= dh )
		{
			line_start += sw;
			limit_line += sw;
			min_line   += sw;
			pitchy -= dh;
		}
		
	}

}

//顺时针旋转270度显示(sy sx顺序相反,使得显示正确)
static void bitmap_zoom_read_right270( BITMAP_ZOOM * zoom, u32 * in_dst, int sy, int sx, int cw, int ch, int dw, int dh )
{
	int x, y;
	u32 * line_start , * limit_line, * min_line;
	u32 * limit_src;
	u32 * dst;
	int pitchx, pitchy, sw, sh;

	while( zoom->width > dw )
	{
		if( zoom->next == NULL )
			break;
		if( zoom->next->width < dw )
			break;
		zoom = zoom->next;
		sx = sx/2;
		sy = sy/2;
	}
	
	sw = zoom->width;
	sh = zoom->height;
	limit_src  = zoom->data + sw * sh;
	line_start = zoom->data + sx + sy * sw;
	limit_line = zoom->data + sw + sy * sw;
	min_line   = zoom->data + sy * sw;
	pitchy = 0;
	for( y = 0; y < cw; y++ )
	{
		u32 *src = line_start;
		dst = in_dst + cw*(ch-1) + y;
		if( src + sw >= limit_src )
		{
			for( x = 0; x < ch; x++ )
			{
				dst[0] = 0;
				dst -= cw;			//上移一行
			}
		}
		else
		{
			register u32 pixel = 0;
			pitchx = dw;
			for( x = 0; x < ch; x++ )
			{
				int count = 0;
				pitchx += sw;
				while( pitchx >= dw )
				{
					if( src >= limit_line || src < min_line || src < zoom->data || src >= limit_src )
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
				dst -= cw;			//上移一行
			}
		}
		
		pitchy += sh;
		while( pitchy >= dh )
		{
			line_start += sw;
			limit_line += sw;
			min_line   += sw;
			pitchy -= dh;
		}
		
	}

}

//顺时针旋转90度
static void bitmap_zoom_read_right90( BITMAP_ZOOM * zoom, u32 * in_dst, int sy, int sx, int cw, int ch, int dw, int dh )
{
	int x, y;
	u32 * line_start , * limit_line, * min_line;
	u32 * limit_src;
	u32 * dst;
	int pitchx, pitchy, sw, sh;

	while( zoom->width > dw )
	{
		if( zoom->next == NULL )
			break;
		if( zoom->next->width < dw )
			break;
		zoom = zoom->next;
		sx = sx/2;
		sy = sy/2;
	}
	
	sw = zoom->width;
	sh = zoom->height;
	limit_src  = zoom->data + sw * sh;
	line_start = zoom->data + sx + sy * sw;
	limit_line = zoom->data + sw + sy * sw;
	min_line   = zoom->data + sy * sw;
	pitchy = 0;
	for( y = 0; y < cw; y++ )
	{
		u32 *src = line_start;
		dst = in_dst + cw - y - 1;
		if( src + sw >= limit_src )
		{
			for( x = 0; x < ch; x++ )
			{
				dst[0] = 0;
				dst += cw;
			}
		}
		else
		{
			register u32 pixel = 0;
			pitchx = dw;
			for( x = 0; x < ch; x++ )
			{
				int count = 0;
				pitchx += sw;
				while( pitchx >= dw )
				{
					if( src >= limit_line || src < min_line || src < zoom->data || src >= limit_src )
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
				dst += cw;			//下移一行
			}
		}
		
		pitchy += sh;
		while( pitchy >= dh )
		{
			line_start += sw;
			limit_line += sw;
			min_line   += sw;
			pitchy -= dh;
		}
		
	}

}

void bitmap_zoom_read( BITMAP_ZOOM * zoom, BITMAP_ZOOM_DIRECTION direction, u32 * dst, int sx, int sy, int cw, int ch, int dw, int dh )
{
	switch( direction )
	{
		case BITMAP_ZOOM_RIGHT0:
			bitmap_zoom_read_normal( zoom, dst, sx, sy, cw, ch, dw, dh );
			break;
		case BITMAP_ZOOM_RIGHT90:
			bitmap_zoom_read_right90( zoom, dst, sx, sy, cw, ch, dw, dh );
			break;
		case BITMAP_ZOOM_RIGHT180:
			bitmap_zoom_read_right180( zoom, dst, sx, sy, cw, ch, dw, dh );
			break;
		case BITMAP_ZOOM_RIGHT270:
			bitmap_zoom_read_right270( zoom, dst, sx, sy, cw, ch, dw, dh );
			break;
		default:
			break;
	}
}

