/***************************************************************************

		image_zoom.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-26 12:56:57

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _image_zoom_h_
#define _image_zoom_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct IMAGE_ZOOM{
	BITMAP_ZOOM * bitmap_zoom;
	MM_IMAGE * image;
	IMAGE_CACHE_READLINE * cache_line;
	u32 * buffer;
	int width;
	int height;
	int real_width;	//图片的实际大小
	int real_height;
	int random_enable; //该图像句柄支持随机读取
} IMAGE_ZOOM;

void image_zoom_read( IMAGE_ZOOM * image_zoom, BITMAP_ZOOM_DIRECTION direction, u32 * dst, int sx, int sy, int cw, int ch, int dw, int dh, int quick );
IMAGE_ZOOM * image_zoom_open( MM_IMAGE * image );
void image_zoom_close( IMAGE_ZOOM * image_zoom );

#define USE_PREVIEW	//加载时动态预览

#ifdef __cplusplus
}
#endif

#endif
