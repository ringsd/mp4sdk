/***************************************************************************

		MM_Image.H

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-26 15:41:58

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _MM_Image_H_
#define _MM_Image_H_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct MM_IMAGE MM_IMAGE;

MM_IMAGE *	MM_image_open	( const u8 *path );
int			MM_image_read	( MM_IMAGE * image, void *buf, int width, int height );
int			MM_image_write	( MM_IMAGE * image, void *buf, int width, int height );
void		MM_image_close	( MM_IMAGE * image );
int 		MM_image_size	( MM_IMAGE * image, int * width, int * height  );
int 		MM_image_readline	( MM_IMAGE * image, void *buf, int scanline );
int 		MM_image_is_random_read	( MM_IMAGE * image );

#ifdef __cplusplus
}
#endif

#endif
