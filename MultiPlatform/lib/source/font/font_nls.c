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

static void build_u2a_table( XFONT_NLS * hnls )
{
	int i;
	int max_count;
	/* 清空转换表 */
	memset( hnls->u2a_table, 0, sizeof(u16) * 0x10000 );
	/* 初始化ASCII部分 */
	for( i = 0; i < 0x80; i++ )
	{
		hnls->u2a_table[i] = i;
	}
	if( nls_type == NLS_TYPE_SINGLE )
		max_count = 0x80;
	else
		max_count = 0x8000;
	for( i = 0; i < max_count; i++ )
	{
		int dat = i+max_count;
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


