/***************************************************************************

		font_cache.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-05-02 20:22:30

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _font_cache_h_
#define _font_cache_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct XFONT_CACHE XFONT_CACHE;

XFONT_CACHE * xfont_cache_open( int max_cache_size, int max_font_size );

int xfont_cache_close( XFONT_CACHE * handle );

char * xfont_cache_read( XFONT_CACHE * handle, XFONT * xfont, XFONT_INFO * info, int code, int size, int style );

int xfont_cache_write( XFONT_CACHE * handle, XFONT * xfont, XFONT_INFO * info, int code, int size, int style, u8 * buffer );

#ifdef __cplusplus
}
#endif

#endif
