/***************************************************************************

		MM_bmp.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-25 20:53:55

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _MM_bmp_h_
#define _MM_bmp_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct MM_IMAGE_BMP MM_IMAGE_BMP;

//Microsoft BMP File
MM_IMAGE_BMP * MM_bmp_open( u8 *path );
//返回1--读取失败, 返回0--成功
int MM_bmp_read( MM_IMAGE_BMP * bmp, void *buf, int width, int height );
int MM_bmp_readline	( MM_IMAGE_BMP * bmp, void *buf, int scanline );
//关闭bmp文件,返回值0:成功 -1失败
void MM_bmp_close( MM_IMAGE_BMP *bmp );
//获取图片大小
//返回非0--读取失败, 返回0--成功
int MM_bmp_size( MM_IMAGE_BMP * bmp, int * width, int * height  );

#ifdef __cplusplus
}
#endif

#endif
