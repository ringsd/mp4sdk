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

/* unicode 转为本地ansi扩展码，对于不在当前代码页中的字符,ansiex码是如下格式
 * 返回值是ansiex码长度
 * 0b 11111111
 * 0b 1000xxxx  unicode bit [15:12]
 * 0b 10xxxxxx          bit [11:06]
 * 0b 10xxxxxx          bit [05:00]
 */
int xfont_nls_unicode_to_ansiex( XFONT_NLS * hnls, u16 uchar, unsigned char * ansiex );

/*  本地ansi扩展码转为unicode，对于不在当前代码页中的字符,ansiex码是如下格式
 * 返回值是ansiex码长度
 * 0b 11111111
 * 0b 1000xxxx  unicode bit [15:12]
 * 0b 10xxxxxx          bit [11:06]
 * 0b 10xxxxxx          bit [05:00]
 */
int xfont_nls_ansiex_to_unicode( XFONT_NLS * hnls, const unsigned char * ansiex, u16 * uchar );

/*
 * unicode字符串转ansiex字符串
 * 返回值, 已转换的unicode字符串长度
 */
int xfont_nls_str_u2ax( XFONT_NLS * nls, unsigned char * ansiex, const u16 * uchar, int limit );

/* ansiex字符串转unicode字符串
 * 返回值, 已转换的ansiex字符串长度
 */
int xfont_nls_str_ax2u( XFONT_NLS * nls, u16 * uchar, const unsigned char * ansiex, int limit );

#ifdef __cplusplus
}
#endif

#endif
