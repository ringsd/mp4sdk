/***************************************************************************

		MM_ico.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-21 10:09:48

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _MM_ico_h_
#define _MM_ico_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct MM_IMAGE_ICO MM_IMAGE_ICO;

MM_IMAGE_ICO * MM_ico_open( u8 *file );
void MM_ico_close( MM_IMAGE_ICO * image_ico );
int MM_ico_read( MM_IMAGE_ICO * image_ico, void *buf, int width, int height );

void MM_ico_test( void );

#ifdef __cplusplus
}
#endif

#endif
