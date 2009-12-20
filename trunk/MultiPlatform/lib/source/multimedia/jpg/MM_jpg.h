/***************************************************************************

		MM_jpg.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-28 14:11:08

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _MM_jpg_h_
#define _MM_jpg_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct MM_IMAGE_JPG MM_IMAGE_JPG;

//JPEG File
MM_IMAGE_JPG * MM_jpg_open( u8 *path );
int MM_jpg_read( MM_IMAGE_JPG * jpg, void *buf, int width, int height );
int MM_jpg_write( MM_IMAGE_JPG * jpg, void *buf, int width, int height );
int MM_jpg_readline( MM_IMAGE_JPG * jpg, void *buf, int scanline );
void MM_jpg_close( MM_IMAGE_JPG *jpg );
//获取图片大小
//返回非0--读取失败, 返回0--成功
int MM_jpg_size( MM_IMAGE_JPG * jpg, int * width, int * height  );

#ifdef __cplusplus
}
#endif

#endif
