/***************************************************************************

		font_nls.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-24 22:04:25

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _font_nls_h_
#define _font_nls_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct NLS_FILE_NAME {
	XFONT_CODE_PAGE code_page;
	char * fname;
} NLS_FILE_NAME;

typedef struct XFONT_NLS XFONT_NLS;
int xfont_nls_close( XFONT_NLS * hnls );
XFONT_NLS * xfont_nls_open( XFONT_CODE_PAGE code_page );
u16 xfont_nls_a2u( XFONT_NLS * hnls, u16 achar );
u16 xfont_nls_u2a( XFONT_NLS * hnls, u16 uchar );

#ifdef __cplusplus
}
#endif

#endif
