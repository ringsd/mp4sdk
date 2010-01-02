/***************************************************************************

		image_cache.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-26 12:44:42

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _image_cache_h_
#define _image_cache_h_

#ifdef	__cplusplus
extern "C" {
#endif

//通过基于行的cache读原始图片
typedef struct IMAGE_CACHE_READLINE_BLOCK{
	int length;
	int line;
	int empty;
	struct IMAGE_CACHE_READLINE_BLOCK * next;
	u32 data[1];
} IMAGE_CACHE_READLINE_BLOCK;

typedef struct IMAGE_CACHE_READLINE{
	MM_IMAGE * image;
	int width;
	int height;
	int blocks;		//缓存块数量
	int memory_low; //低内存空间标志
	IMAGE_CACHE_READLINE_BLOCK * first;
	IMAGE_CACHE_READLINE_BLOCK * last;
	IMAGE_CACHE_READLINE_BLOCK * temp;
} IMAGE_CACHE_READLINE;

IMAGE_CACHE_READLINE * image_cache_readline_open( MM_IMAGE * image );
void image_cache_readline_read_setempty( IMAGE_CACHE_READLINE * handle, int sy, int dh, int ch );
void image_cache_readline_read( IMAGE_CACHE_READLINE * handle, u32 * dst, int line );
void image_cache_readline_close( IMAGE_CACHE_READLINE * handle );

#ifdef __cplusplus
}
#endif

#endif
