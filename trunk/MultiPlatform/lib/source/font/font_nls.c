/***************************************************************************

		font_nls.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-24 22:04:21

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>
#include "font_nls.h"

extern char xfont_nls_path[];		//外部全局变量,提供nls文件目录,以'\'结尾的路径

enum{
	NLS_TYPE_DOUBLE,
	NLS_TYPE_SINGLE,
};

static int nls_type = NLS_TYPE_DOUBLE;
static int single_char_div = 0x80;

struct XFONT_NLS {
	XFONT_CODE_PAGE code_page;
	int single_char_div;
	int nls_type;
	u16 * nls_table;
	u16 * u2a_table;
	NLS_FILE_NAME * name;
};

//加载nls文件
//0成功
static int xfont_nls_load( const char * fname, XFONT_NLS * hnls )
{
	FILE *fp = fopen( fname, "rb" );
	u16 * nls_table;
	int single_char_div;
	int nls_type;
	
	if( fp )
	{
		u32 length;
		//加载映射表
		fseek( fp, 0 , SEEK_END );
		length = ftell( fp );
		fseek( fp, 0 , SEEK_SET );
		if( length > 0x100 )	//根据文件大小判断字节类型
		{
			if( (nls_table = malloc( 0x8000 * sizeof(u16) )) == NULL )
				goto err;
			fread( nls_table, sizeof(u16), 0x8000 , fp);
			nls_type = NLS_TYPE_DOUBLE;
			single_char_div = 0x80;
		}
		else
		{
			if( (nls_table = malloc( 0x80 * sizeof(u16) )) == NULL )
				goto err;
			fread( nls_table, sizeof(u16), 0x80 , fp);
			nls_type = NLS_TYPE_SINGLE;
			single_char_div = 0x100;
		}
		fclose( fp );
		fp = NULL;
		
		hnls->u2a_table = NULL;
		hnls->nls_table = nls_table;
		hnls->nls_type = nls_type;
		hnls->single_char_div = single_char_div;

		return 0;
	}
err:
	if( fp )
		fclose( fp );
	return -1;
}

u16 xfont_nls_a2u( XFONT_NLS * hnls, u16 achar )
{
	if( achar < 0x8000 || hnls == NULL )
	{
		return achar;
	}
	else
	{
		if( hnls->nls_type == NLS_TYPE_DOUBLE )
		{
			return hnls->nls_table[ achar-0x8000 ];
		}
		else
			return hnls->nls_table[ achar - 0x80 ];
	}
}

int xfont_nls_unicode_to_utf8( wchar_t uchar, unsigned char * utfs )
{
    if( uchar < 0x80 )
    {
        utfs[0] = (unsigned char)uchar;
        return 1;
    }
    else
    if( uchar < 0x800 )
    {
        utfs[0] = 0xC | (uchar >> 6);
        utfs[1] = 0x8 | (uchar & 0x3F);
        return 2;
    }
    else
    if( uchar < 0x10000 )
    {
        utfs[0] = 0xE | (uchar >> 12);
        utfs[1] = 0x8 | ((uchar >> 6) & 0x3F);
        utfs[2] = 0x8 | (uchar & 0x3F);
        return 3;
    }
#if UNICODE_IS_32_BIT
    else
    if( uchar < 0x200000 )
    {
        utfs[0] = 0xF | (uchar >> 18);
        utfs[1] = 0x8 | ((uchar >> 12) & 0x3F);
        utfs[2] = 0x8 | ((uchar >> 6) & 0x3F);
        utfs[3] = 0x8 | (uchar & 0x3F);
        return 4;
    }
    else
    if( uchar < 0x4000000 )
    {
        utfs[0] = 0xF8 | (uchar >> 24);
        utfs[1] = 0x8 | ((uchar >> 18) & 0x3F);
        utfs[2] = 0x8 | ((uchar >> 12) & 0x3F);
        utfs[3] = 0x8 | ((uchar >> 6) & 0x3F);
        utfs[4] = 0x8 | (uchar & 0x3F);
        return 5;
    }
    else
    if( uchar < 0x7FFFFFFF )
    {
        utfs[0] = 0xFC | (uchar >> 30);
        utfs[1] = 0x8 | ((uchar >> 24) & 0x3F);
        utfs[2] = 0x8 | ((uchar >> 18) & 0x3F);
        utfs[3] = 0x8 | ((uchar >> 12) & 0x3F);
        utfs[4] = 0x8 | ((uchar >> 6) & 0x3F);
        utfs[5] = 0x8 | (uchar & 0x3F);
        return 6;
    }
#endif
    else
    {
        return 0;
    }
}

int xfont_nls_utf8_to_unicode( const unsigned char * utfs, wchar_t * uchar )
{
    if( utfs[0] < 0x80 )
    {
        *uchar = utfs[0];
        return 1;
    }
    else
    if( (utfs[0] & 0xE0) == 0xC0 )
    {
        *uchar = ((wchar_t)(utfs[0] & 0x1F) << 6) |
                  (wchar_t)(utfs[1] & 0x3F);
        return 2;
    }
    else
    if( (utfs[0] & 0xF0) == 0xE0 )
    {
        *uchar = ((wchar_t)(utfs[0] & 0x0F) << 12) |
                 ((wchar_t)(utfs[1] & 0x3F) << 6) |
                  (wchar_t)(utfs[2] & 0x3F);
        return 3;
    }
#if UNICODE_IS_32_BIT
    else
    if( (utfs[0] & 0xF8) == 0xF0 )
    {
        *uchar = ((wchar_t)(utfs[0] & 0x07) << 18) |
                 ((wchar_t)(utfs[1] & 0x3F) << 12) |
                 ((wchar_t)(utfs[2] & 0x3F) <<  6) |
                  (wchar_t)(utfs[3] & 0x3F);
        return 4;
    }
    else
    if( (utfs[0] & 0xFC) == 0xF8 )
    {
        *uchar = ((wchar_t)(utfs[0] & 0x03) << 24) |
                 ((wchar_t)(utfs[1] & 0x3F) << 18) |
                 ((wchar_t)(utfs[2] & 0x3F) << 12) |
                 ((wchar_t)(utfs[3] & 0x3F) <<  6) |
                  (wchar_t)(utfs[4] & 0x3F);
        return 5;
    }
    else
    if( (utfs[0] & 0xFE) == 0xFC )
    {
        *uchar = ((wchar_t)(utfs[0] & 0x01) << 30) |
                 ((wchar_t)(utfs[1] & 0x3F) << 24) |
                 ((wchar_t)(utfs[2] & 0x3F) << 18) |
                 ((wchar_t)(utfs[3] & 0x3F) << 12) |
                 ((wchar_t)(utfs[4] & 0x3F) <<  6) |
                  (wchar_t)(utfs[5] & 0x3F);
        return 6;
    }
#endif
    else
    {
        return 0;
    }
}

static void build_u2a_table( XFONT_NLS * hnls )
{
	int i;
	int max_count;
	/* 填转换表为0xFF, 全部无效的转换项 */
	memset( hnls->u2a_table, 0xFF, sizeof(u16) * 0x10000 );
	/* 初始化ASCII部分 */
	for( i = 0; i < 0x80; i++ )
	{
		hnls->u2a_table[i] = i;
	}
	if( hnls->nls_type == NLS_TYPE_SINGLE )
		max_count = 0x80;
	else
		max_count = 0x8000;
	for( i = 0; i < max_count; i++ )
	{
		int dat = i+max_count;
		if( hnls->u2a_table[hnls->nls_table[i]] == 0xFFFF )
		    hnls->u2a_table[hnls->nls_table[i]] = ((dat>>8)&0xFF)|((dat<<8)&0xFF00);
	}
}

u16 xfont_nls_u2a( XFONT_NLS * hnls, u16 uchar )
{
	if( !hnls->u2a_table )
	{
		hnls->u2a_table = malloc( sizeof(u16) * 0x10000 );;
		if( !hnls->u2a_table )
			goto err;
		build_u2a_table(hnls);	
	}
	return hnls->u2a_table[uchar];
err:
	if( hnls->u2a_table )
		free(hnls->u2a_table);
	printf( "memory not enough!\n" );
	return 0;
}

/* unicode 转为本地ansi扩展码，对于不在当前代码页中的字符,ansiex码是如下格式
 * 返回值是ansiex码长度
 * 0b 11111111
 * 0b 1000xxxx  unicode bit [15:12]
 * 0b 10xxxxxx          bit [11:06]
 * 0b 10xxxxxx          bit [05:00]
 */
int xfont_nls_unicode_to_ansiex( XFONT_NLS * hnls, u16 uchar, unsigned char * ansiex )
{
    u16 achar = xfont_nls_u2a( hnls, uchar );
    if( achar < 0x80 )
    {
        ansiex[0] = (unsigned char)achar;
        return 1;
    }
    else
    if( achar < 0xFF00 )
    {
        ansiex[0] = (unsigned char)achar;
        ansiex[1] = achar >> 8;
        return 2;
    }
    else   // 字符不在当前代码页中
    {
        ansiex[0] = 0xFF;
        ansiex[1] = 0x80 | (uchar >> 12);
        ansiex[2] = 0x80 | ((uchar >> 6) & 0x3F);
        ansiex[3] = 0x80 | (uchar & 0x3F);
        return 4;
    }
}

/*  本地ansi扩展码转为unicode，对于不在当前代码页中的字符,ansiex码是如下格式
 * 返回值是ansiex码长度
 * 0b 11111111
 * 0b 1000xxxx  unicode bit [15:12]
 * 0b 10xxxxxx          bit [11:06]
 * 0b 10xxxxxx          bit [05:00]
 */
int xfont_nls_ansiex_to_unicode( XFONT_NLS * hnls, const unsigned char * ansiex, u16 * uchar )
{
    if( ansiex[0] < 0x80 )
    {
        *uchar = xfont_nls_a2u( hnls, ansiex[0] );
        return 1;
    }
    else
    if( ansiex[0] < 0xFF )
    {
        *uchar = xfont_nls_a2u( hnls, ((u16)ansiex[0] << 8) | ansiex[1] );
        return 2;
    }
    else
    {
        *uchar = ((ansiex[1] & 0x0F) << 12) |
                 ((ansiex[2] & 0x3F) <<  6) |
                  (ansiex[3] & 0x3F);
        return 4;
    }
}

/*
 * unicode字符串转ansiex字符串
 * 返回值, 已转换的unicode字符串长度
 */
int xfont_nls_str_u2ax( XFONT_NLS * nls, unsigned char * ansiex, const u16 * uchar, int limit )
{
    int             length;
    unsigned char * pa    = ansiex;
    const u16 *     pu    = uchar;
    int             count = 0;
    
    while( *pu )
    {
        if( count + 5 >= limit )
            break;
        length = xfont_nls_unicode_to_ansiex( nls, *pu, pa );
        pa    += length;
        count += length;
        pu    ++;
    }
    *pa = 0;
    return pu - uchar;
}

/* ansiex字符串转unicode字符串
 * 返回值, 已转换的ansiex字符串长度
 */
int xfont_nls_str_ax2u( XFONT_NLS * nls, u16 * uchar, const unsigned char * ansiex, int limit )
{
    int                   length;
    const unsigned char * pa    = ansiex;
    u16 *                 pu    = uchar;
    int                   count = 0;
    
    while( *pa )
    {
        if( count + 2 >= limit )
            break;
        length   = xfont_nls_ansiex_to_unicode( nls, pa, pu );
        pa      += length;
        pu      ++;
        count   ++;
    }
    *pu = 0;
    return pa - ansiex;
}

XFONT_NLS * xfont_nls_open( XFONT_CODE_PAGE code_page )
{
	int i = 0;
	XFONT_NLS * hnls;
	
	if( (hnls = malloc( sizeof( XFONT_NLS ) )) == NULL )
		goto err;
	
	if( code_page == XFONT_CP_DEFAULT )
		code_page = xfont_default_codepage;
		
	while( nls_fname[i].fname )
	{
		if( nls_fname[i].code_page == code_page )
		{
			char path[ MAX_PATH ];
			strcpy( path, xfont_nls_path );
			strcat( path, nls_fname[i].fname );
			hnls->name = &nls_fname[i];
			if( xfont_nls_load( path, hnls ) )
				goto err;
			hnls->code_page = code_page;
			return hnls;
		}
		i++;
	}

err:
	if( hnls )
		free( hnls );
	return NULL;
}

int xfont_nls_close( XFONT_NLS * hnls )
{
	if( hnls == NULL )
		goto err;
	if( hnls->nls_table )
		free( hnls->nls_table );
	if( hnls->u2a_table )
		free( hnls->u2a_table );
	free( hnls );
	return 0;
err:
	return -1;
}


