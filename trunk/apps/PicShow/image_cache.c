/***************************************************************************

		image_cache.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-26 12:44:26

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <global.h>
#include "MM_Image.H"
#include "image_cache.h"

IMAGE_CACHE_READLINE * image_cache_readline_open( MM_IMAGE * image )
{
	IMAGE_CACHE_READLINE * handle = malloc( sizeof( IMAGE_CACHE_READLINE ) );
	if( handle == NULL ) goto err;
	handle->first = NULL;
	handle->last = NULL;
	handle->temp = NULL;
	handle->image = image;
	handle->memory_low = 0;
	handle->blocks = 0;
	MM_image_size	( image, &handle->width, &handle->height  );
	return handle;
err:
	return NULL;
}

static IMAGE_CACHE_READLINE_BLOCK *
 image_cache_readline_choose_block( IMAGE_CACHE_READLINE * handle )
{
	IMAGE_CACHE_READLINE_BLOCK * block = handle->first;
	IMAGE_CACHE_READLINE_BLOCK * max_block = block;
	IMAGE_CACHE_READLINE_BLOCK * min_block = block;
	while( block )
	{
		if( block->empty )
			return block;
		if( block->line > max_block->line )
			max_block = block;
		if( block->line < min_block->line )
			min_block = block;
		block = block->next;
	}
	return min_block;
}

//快速清除不需要的cache块 sy = 原图y坐标, dh = 缩放目标图高度, ch = 缩放窗口高度
void image_cache_readline_read_setempty( IMAGE_CACHE_READLINE * handle, int sy, int dh, int ch )
{
	IMAGE_CACHE_READLINE_BLOCK * block = handle->first;
	while( block )
	{
		if( block->line < sy ||
			block->line > sy + ch*handle->height/dh ||
			( block->line*dh/handle->height == (block->line+1)*dh/handle->height ) )
			block->empty = 1;
		else
			block->empty = 0;
		block = block->next;
	}
}

#define MEMORY_RESERVE 0x100000
#define MAX_BLOCKS 1024

void image_cache_readline_read( IMAGE_CACHE_READLINE * handle, u32 * dst, int line )
{
	IMAGE_CACHE_READLINE_BLOCK * block = handle->temp;
	//IMAGE_CACHE_READLINE_BLOCK * temp = handle->temp;
	while( block != NULL )	//从上一次的块开始搜索
	{
		if( block->line == line )
			break;
		block = block->next;
	}
	if( block == NULL )	//从头开始搜索
	{
		block = handle->first;
		while( block != handle->temp )
		{
			if( block->line == line )
				break;
			block = block->next;
		}
		if( block == handle->temp )
			block = NULL;
	}
	if ( block == NULL )
	{
		if( handle->memory_low == 0 && handle->blocks < MAX_BLOCKS )
		{
			//测试空闲内存
			block = malloc( handle->width*4 + sizeof( IMAGE_CACHE_READLINE_BLOCK ) + MEMORY_RESERVE );
			if( block )	//空间足够,建立一个新的block
			{
				free( block );
				block = malloc( handle->width*4 + sizeof( IMAGE_CACHE_READLINE_BLOCK ) );
				if( handle->last )					//加入链表
					handle->last->next = block;
				handle->last = block;
				if( handle->first == NULL )
					handle->first = block;
				block->next = NULL;
				handle->blocks++;
				//printf( "%d\n", handle->blocks );
			}
			else
				handle->memory_low = 1;
		}
		if( block == NULL )	//无空闲内存,使用已有的block
		{
			block = image_cache_readline_choose_block( handle );
		}
		if( block == NULL )
			goto err;
		//printf( "%d\n", line );
		MM_image_readline( handle->image, block->data, line );
		block->empty = 0;
		block->length = handle->width * 4;
		block->line = line;
	}
	memcpy( dst, block->data, block->length );
	handle->temp = block;	//下一次从这里开始搜索
err:
	return;
}

void image_cache_readline_close( IMAGE_CACHE_READLINE * handle )
{
	IMAGE_CACHE_READLINE_BLOCK * block = handle->first;
	while( block )
	{
		IMAGE_CACHE_READLINE_BLOCK * tmpblock = block->next;
		free( block );
		block = tmpblock;
	}
	free( handle );
}


