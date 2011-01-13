/***************************************************************************

		bitmap_zoom.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-26 12:53:21

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _bitmap_zoom_h_
#define _bitmap_zoom_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct BITMAP_ZOOM {
	int width;
	int height;
	u32 *data;
	struct BITMAP_ZOOM * next;
} BITMAP_ZOOM;

typedef enum BITMAP_ZOOM_DIRECTION{
	BITMAP_ZOOM_RIGHT0,
	BITMAP_ZOOM_RIGHT90,
	BITMAP_ZOOM_RIGHT180,
	BITMAP_ZOOM_RIGHT270,
} BITMAP_ZOOM_DIRECTION;

BITMAP_ZOOM * bitmap_zoom_open( u32 * bitmap, int width, int height );
void bitmap_zoom_close( BITMAP_ZOOM * zoom );
void bitmap_zoom_read( BITMAP_ZOOM * zoom, BITMAP_ZOOM_DIRECTION direction, u32 * dst, int sx, int sy, int cw, int ch, int dw, int dh );

#ifdef __cplusplus
}
#endif

#endif
