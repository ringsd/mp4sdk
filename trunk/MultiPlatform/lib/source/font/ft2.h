/***************************************************************************

		ft2.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2008-12-05 19:51:29

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _ft2_h_
#define _ft2_h_

#ifdef	__cplusplus
extern "C" {
#endif

//data -- 256ª“∂»Õº
void ft2_draw( char *data, int width, int height, int left, int top );
int ft2_init( const char *fontpath, int font_size );
void ft2_deinit( void );
void ft2_read( unsigned int charcode );
void ft2_char_size( int size );

#ifndef _ft2_c_
#define XFONT_FT2 void
XFONT_FT2 * xfont_freetype2_open( const char * path );
int xfont_freetype2_read( XFONT_FT2 * xfont, int code, int size, int style, u8 * buf, int limit, XFONT_INFO * info );
void xfont_freetype2_close( XFONT_FT2 * xfont );
const char * ft2_font_name( XFONT_FT2 * xfont );
int xfont_freetype2_getsize( XFONT_FT2 * xfont, int code, int size, int style, XFONT_INFO * info );
#endif

#ifdef __cplusplus
}
#endif

#endif
