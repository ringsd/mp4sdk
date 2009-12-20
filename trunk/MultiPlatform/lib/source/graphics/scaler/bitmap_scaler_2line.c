/***************************************************************************

		bitmap_scaler_2line.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-11-29 18:35:19

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/

#include <global.h>
#include "bitmap_scaler_2line.h"

#define BLOCK_LINES 256

struct BITMAP_2LINE{
    u32 * block_ptr;
    u32 * block_raw_ptr;		//预缩小原图缓存
    int max_lines;
    int block_next_line;
    int block_start_line;
    int block_width;
    int block_height;
    int block_divide;
    int (*readline)( int line, void * buffer,void * param );
    void * param;
    
    //从模式
    u32 * slave_ptr;
    int slave_divide;
    int slave_next_line;
    int slave_start_line;
    int slave_width;
    int slave_height;
};

BITMAP_2LINE * bitmap_2line_init
( int width, int height, int divide, int (*readline)( int line, void * buffer,void * param ), void * param )
{
    BITMAP_2LINE * bitmap_2line;
    bitmap_2line = malloc( sizeof(BITMAP_2LINE) );
    
    if( bitmap_2line == NULL )
        goto err;
        
    bitmap_2line->block_width = width;
	bitmap_2line->block_height = height;
	bitmap_2line->block_next_line = 0;
	bitmap_2line->block_start_line = 0;
	bitmap_2line->block_divide = divide;
	bitmap_2line->max_lines = height;
	
	if( divide > 1 )		// 预缩小缓存
	{
		bitmap_2line->block_raw_ptr = malloc(width * divide * 4);
		if( bitmap_2line->block_raw_ptr == NULL )
		    goto err2;
	}
	else
	    bitmap_2line->block_raw_ptr = NULL;
	    
	bitmap_2line->block_width = width/divide;
	bitmap_2line->block_ptr = malloc( width*BLOCK_LINES*4 );
	if( bitmap_2line->block_ptr == NULL )
	    goto err3;
	bitmap_2line->slave_ptr = NULL;
	bitmap_2line->slave_divide = 1;
	
	bitmap_2line->readline = readline;
	bitmap_2line->param = param;
	
    return bitmap_2line;
err3:
    free( bitmap_2line->block_raw_ptr );
err2:
    free( bitmap_2line );
err:
    return NULL;
}

/* 读取block_divide行,合并到一行中
 * int line 原图行
 */
static void img2line_divide( BITMAP_2LINE * bitmap_2line, u32 * dst, int line )
{
	int i, x, y;
	int raw_width = bitmap_2line->block_width * bitmap_2line->block_divide;
	int raw_divide = bitmap_2line->block_divide * bitmap_2line->block_divide;
	int block_width = bitmap_2line->block_width;
	int block_divide = bitmap_2line->block_divide;
	u32 * raw_ptr = bitmap_2line->block_raw_ptr;
	/* 读取raw数据 */
	for( i = 0; i < bitmap_2line->block_divide; i++ )
	{
        bitmap_2line->readline( line+i, raw_ptr, bitmap_2line->param );
		raw_ptr += raw_width;
	}
	raw_ptr = bitmap_2line->block_raw_ptr;
	/* 合并到一行中 */
	for( x = 0; x < block_width; x++ )
	{
		u32 r = 0, g = 0, b = 0;
		for( y = 0; y < block_divide; y++ )
		{
			for( i = 0; i < block_divide; i++ )
			{
				u32 pixel = raw_ptr[i+y*raw_width];
				r += (pixel>>16)&0xFF;
				g += (pixel>>8)&0xFF;
				b += pixel&0xFF;
			}
		}
		raw_ptr += block_divide;
		*dst++ = ((r/raw_divide)<<16)|((g/raw_divide)<<8)|(b/raw_divide);
	}
}

u32 * bitmap_2line_getline( BITMAP_2LINE * bitmap_2line, int line )
{
    if( line < bitmap_2line->block_start_line + bitmap_2line->block_next_line - 1 && line >= bitmap_2line->block_start_line )
	{
		return bitmap_2line->block_ptr + (line-bitmap_2line->block_start_line)*bitmap_2line->block_width;
	}
	while( bitmap_2line->block_start_line + bitmap_2line->block_next_line - 1 <= line )
	{
		if( bitmap_2line->block_next_line >= BLOCK_LINES )
		{	/* 根据slave的分频比, block滚动时, 保留相应的数据量 */
			memcpy( bitmap_2line->block_ptr, bitmap_2line->block_ptr + (BLOCK_LINES-bitmap_2line->slave_divide) * bitmap_2line->block_width, bitmap_2line->block_width * 4 * bitmap_2line->slave_divide );
			bitmap_2line->block_next_line = bitmap_2line->slave_divide;
			bitmap_2line->block_start_line += BLOCK_LINES - bitmap_2line->slave_divide;
		}
		if( bitmap_2line->block_divide > 1 )
		{
			img2line_divide( bitmap_2line, bitmap_2line->block_ptr + bitmap_2line->block_next_line * bitmap_2line->block_width, 
							( bitmap_2line->block_start_line + bitmap_2line->block_next_line ) * bitmap_2line->block_divide );
		}
		else if( bitmap_2line->block_start_line + bitmap_2line->block_next_line < bitmap_2line->max_lines )
		{
            bitmap_2line->readline( bitmap_2line->block_start_line + bitmap_2line->block_next_line, 
                                    bitmap_2line->block_ptr + bitmap_2line->block_next_line * bitmap_2line->block_width, bitmap_2line->param );
		}
		bitmap_2line->block_next_line++;
	}
	return bitmap_2line->block_ptr + (line - bitmap_2line->block_start_line) * bitmap_2line->block_width;
}

int bitmap_2line_deinit( BITMAP_2LINE * bitmap_2line )
{
    if( bitmap_2line->block_raw_ptr )
        free( bitmap_2line->block_raw_ptr );
    if( bitmap_2line->block_ptr )
        free( bitmap_2line->block_ptr );
    free( bitmap_2line );
    return 0;
}

