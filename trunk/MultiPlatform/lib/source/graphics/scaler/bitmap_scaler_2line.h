/***************************************************************************

		bitmap_scaler_2line.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-11-29 18:36:05

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/
#ifndef _bitmap_scaler_2line_h_
#define _bitmap_scaler_2line_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct BITMAP_2LINE BITMAP_2LINE;

u32 * bitmap_2line_getline( BITMAP_2LINE * bitmap_2line, int line );

int bitmap_2line_deinit( BITMAP_2LINE * bitmap_2line );

BITMAP_2LINE * bitmap_2line_init
( int width, int height, int divide, int (*readline)( int line, void * buffer, void * param ), void * param );

#ifdef __cplusplus
}
#endif

#endif
