/***************************************************************************

		MM_png.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-07-25 21:34:30

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _MM_png_h_
#define _MM_png_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct MM_IMAGE_PNG MM_IMAGE_PNG;

MM_IMAGE_PNG *		MM_png_open	( u8 *path );
int			MM_png_read		( MM_IMAGE_PNG * image, void *buf, int width, int height );
int			MM_png_write	( MM_IMAGE_PNG * image, void *buf, int width, int height );
void		MM_png_close	( MM_IMAGE_PNG * image );
int 		MM_png_size		( MM_IMAGE_PNG * image, int * width, int * height  );
int 		MM_png_readline	( MM_IMAGE_PNG * image, void *buf, int scanline );

#ifdef __cplusplus
}
#endif

#endif
