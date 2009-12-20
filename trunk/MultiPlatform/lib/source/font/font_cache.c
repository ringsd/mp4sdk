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
	struct XFONT_CACHE_BLOCK * prev;
	struct XFONT_CACHE_BLOCK * next;
	int index;
	XFONT_INFO info;
	XFONT * xfont;
	int size;
	int style;
	char data[1];
} XFONT_CACHE_BLOCK;

struct XFONT_CACHE{
	int blockcount;
	int max_cache_size;
	int max_font_size;
	int used_size;
	XFONT_CACHE_BLOCK * first;
	XFONT_CACHE_BLOCK * last;
	XFONT_CACHE_BLOCK * block_index[ 0x10000 ];
};

XFONT_CACHE * xfont_cache_open( int max_cache_size, int max_font_size )
{
	int i;
	XFONT_CACHE * handle = malloc( sizeof(XFONT_CACHE) );
	if( handle == NULL ) goto err;
	handle->max_cache_size = max_cache_size;
	handle->max_font_size = max_font_size;
	handle->used_size = 0;
	handle->blockcount = 0;
	handle->last = NULL;
	handle->first = NULL;
	for( i = 0; i < 0x10000; i++ )
		handle->block_index[i] = NULL;
	return handle;
err:
	return NULL;
}

int xfont_cache_close( XFONT_CACHE * handle )
{
	int i;
	for( i = 0; i < 0x10000; i++ )
	{
		if( handle->block_index[i] )
			free( handle->block_index[i] );
	}
	free( handle );
	return 0;
}

char * xfont_cache_read( XFONT_CACHE * handle, XFONT * xfont, XFONT_INFO * info, int code, int size, int style )
{
	XFONT_CACHE_BLOCK * block = handle->block_index[code];
	if( block )				//已缓存,读取并挂到队首
	{
		if( block->xfont != xfont ||
			block->size != size ||
			block->style != style )
			goto err;
		#if 0
		if( handle->first != block )	//判断队首是否自己
		{
			if( block->prev )						//当前块的前后项链接起来
				block->prev->next = block->next;
			if( block->next )
				block->next->prev = block->prev;
				
			handle->first->prev = block;	//原队首变为第二项
			
			block->prev = NULL;				//挂接到队首
			block->next = handle->first;
			handle->first = block;
		}
		#endif
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
		
	if( handle->block_index[code] != NULL )				//当前编码已经缓存,则直接返回
		return 0;
	
	if( handle->used_size < handle->max_cache_size )	//小于最大内存,申请新的块,并挂在队尾
	{
		int blocksize = sizeof(XFONT_CACHE_BLOCK) + (handle->max_font_size*handle->max_font_size) - 1;
		block = malloc( blocksize );
		if( block == NULL )
		{
			return -1;			//在限定的内存范围内出现了内存不足,返回错误
		}
		if( handle->first == NULL )
			handle->first = block;
		handle->blockcount++;
		handle->used_size += blocksize;
		//printf( "%d\n", handle->blockcount );
	}
	else					//不再申请新的内存,替换已有的块
	{
		block = handle->first;		//取队列第一块
		handle->first = block->next;	//第二块变成队首
		if( block->next )
			block->next->prev = NULL;	//队首的前项设为NULL
			
		handle->block_index[block->index] = NULL;	//在表中清空这一块
	}
	memcpy( block->data, buffer, size*size );	//复制数据到缓存
	memcpy( &block->info, info, sizeof(XFONT_INFO) );	//复制INFO到缓存
	block->xfont = xfont;
	block->size = size;
	block->style = style;
	block->next = NULL;
	block->prev = handle->last;						//挂载到队尾
	if( handle->last )
		handle->last->next = block;
	handle->last = block;
	block->index = code;
	handle->block_index[code] = block;
	
	return 0;
}


