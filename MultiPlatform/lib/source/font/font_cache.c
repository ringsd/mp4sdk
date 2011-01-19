/***************************************************************************

		font_cache.c

		为了避免cache的过程出现大量的碎片,
		cache模块仅仅对指定大小之内的字体做cache.
		每个block的大小都是相同的,这样也利于快速替换

		TIME LIST:
		CREATE	Rainy	2009-05-02 20:22:34

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>
#include "font_cache.h"

//#define MAX_CACHE_SIZE 0x200000			//最大占用的内存大小
//#define MAX_FONT_SIZE	32				//支持cache的最大字体大小

typedef struct XFONT_CACHE_BLOCK{
	int         index;
	XFONT_INFO  info;
	XFONT *     xfont;
	int         size;
	int         style;
	char *      data;
} XFONT_CACHE_BLOCK;

struct XFONT_CACHE{
	int total_blocks;
	int max_font_size;
	
	int block_cycle;
	XFONT_CACHE_BLOCK * blocks;
	char * datas;
	
	XFONT_CACHE_BLOCK * block_index[ 0x10000 ];
};

XFONT_CACHE * xfont_cache_open( int max_cache_size, int max_font_size )
{
	int                 i;
	XFONT_CACHE *       handle;
	int                 data_size;
	
	handle = malloc( sizeof(XFONT_CACHE) );
	if( handle == NULL )
	{
	    goto err;
	}
	
	handle->max_font_size = max_font_size;
	
	data_size   = max_font_size * max_font_size;
	
	handle->total_blocks = max_cache_size / data_size;
	
	handle->blocks = malloc( handle->total_blocks * sizeof(XFONT_CACHE_BLOCK) );
	
	if( handle->blocks == NULL )
	    goto err1;

	handle->block_cycle = 0;
	
	handle->datas = malloc( handle->total_blocks * data_size );
	
	if( handle->datas == NULL )
	    goto err2;
	
	for( i = 0; i < handle->total_blocks; i++ )
	{
	    handle->blocks[i].index = 0xFFFF;
	    handle->blocks[i].data = handle->datas + i * data_size;
	}
	
	for( i = 0; i < 0x10000; i++ )
	{
		handle->block_index[i] = NULL;
	}
	
	return handle;

err2:
    free( handle->blocks );
err1:
    free( handle );
err:
	return NULL;
}

int xfont_cache_close( XFONT_CACHE * handle )
{
	if( handle->blocks )
		free( handle->blocks );
    if( handle->datas )
        free( handle->datas );
	free( handle );
	return 0;
}

char * xfont_cache_read( XFONT_CACHE * handle, XFONT * xfont, XFONT_INFO * info, int code, int size, int style )
{
	XFONT_CACHE_BLOCK * block = handle->block_index[code];
	if( block )
	{
		if( block->xfont != xfont ||
			block->size != size ||
			block->style != style )
			goto err;
		memcpy( info, &block->info, sizeof(XFONT_INFO) );
		return block->data;
	}
err:
	return NULL;
}

int xfont_cache_write( XFONT_CACHE * handle, XFONT * xfont, XFONT_INFO * info, int code, int size, int style, u8 * buffer )
{
	XFONT_CACHE_BLOCK * block = NULL;
	
	if( size > handle->max_font_size )					//判断是否在cache支持的范围内
		return -1;
	
	block = &handle->blocks[ handle->block_cycle ];
	
	handle->block_index[ block->index ] = NULL;
	
	handle->block_cycle = (handle->block_cycle + 1) % handle->total_blocks;
	
	memcpy( block->data, buffer, size*size );	        //复制数据到缓存
	memcpy( &block->info, info, sizeof(XFONT_INFO) );	//复制INFO到缓存
	block->xfont = xfont;
	block->size = size;
	block->style = style;
	block->index = code;
	handle->block_index[code] = block;
	
	return 0;
}


