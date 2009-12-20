/***************************************************************************

		ft2.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-01 16:23:27

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#define _ft2_c_
#include <global.h>
#include <ft2build.h>
#include "ft2.h"
#include FT_FREETYPE_H

FT_Face face; /* face对象的句柄 */

void ft2_read( FT_UInt charcode )
{
	FT_UInt glyph_index;
	FT_Error error;
	FT_Int32 load_flags = FT_LOAD_DEFAULT ;
	glyph_index = FT_Get_Char_Index( face, charcode ); 
	error = FT_Load_Glyph( 
			face, /* face对象的句柄 */ 
			glyph_index, /* 字形索引 */ 
			load_flags ); /* 装载标志，参考下面 */ 
	if( face->glyph->format != FT_GLYPH_FORMAT_BITMAP )
	{
		error = FT_Render_Glyph( face->glyph, /* 字形槽 */ 
		FT_RENDER_MODE_NORMAL ); /* 渲染模式 */
	}
	//ft2_draw( face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap_left, face->glyph->bitmap_top );
}

FT_Library library = NULL;

void ft2_deinit( void )
{
	if( library )
	{
		FT_Done_FreeType( library );
		library = NULL;
	}
}

int ft2_init( const char *fontpath, int font_size )
{
	FT_Error error;

	error = FT_Init_FreeType ( &library ); 
	if ( error ) 
	{ 
		printf("FT_Init_FreeType error!\n");
		return 0;
	}
	error = FT_New_Face( library, fontpath, 0, &face ); 
	if ( error == FT_Err_Unknown_File_Format ) 
	{ 
		printf("FT_Err_Unknown_File_Format\n");
		ft2_deinit();
		return 0;
	} 
	else if ( error ) 
	{ 
		printf("Error Type File !\n");
		ft2_deinit();
		return 0;
	} 
	return 1;
}


typedef struct XFONT_FT2{
	FT_Library lib;
	FT_Face face;
	int font_size;
} XFONT_FT2;

const char * ft2_font_name( XFONT_FT2 * xfont )
{
	return xfont->face->family_name;
}

void xfont_freetype2_close( XFONT_FT2 * xfont )
{
	if( xfont->face )
	{
		FT_Done_Face( xfont->face );
	}
	if( xfont->lib )
	{
		FT_Done_FreeType( xfont->lib );
	}
	free( xfont );
}

XFONT_FT2 * xfont_freetype2_open( const char * path )
{
	XFONT_FT2 * ft2;
	FT_Error error;
	ft2 = malloc( sizeof(XFONT_FT2) );
	if( ft2 == NULL )
	{
		return NULL;
	}
	ft2->lib = NULL;
	ft2->face = NULL;
	ft2->font_size = 0;

	error = FT_Init_FreeType ( &ft2->lib ); 
	if ( error ) 
	{ 
		printf("FT_Init_FreeType error!\n");
		xfont_freetype2_close( ft2 );
		return NULL;
	}
	error = FT_New_Face( ft2->lib, path, 0, &ft2->face ); 
	if ( error == FT_Err_Unknown_File_Format ) 
	{ 
		printf("FT_Err_Unknown_File_Format\n");
		xfont_freetype2_close( ft2 );
		return NULL;
	} 
	else if ( error ) 
	{ 
		printf("Error Type File !\n");
		xfont_freetype2_close( ft2 );
		return NULL;
	}
	
	return ft2;
}

int x_offset = 1;

static void copy_bitmap( u8 * dst, u8 * src, int dw, int dh, int sw, int sh, int strx, int stry )
{
	int x, y;
	//if( strx < 0 )
		strx = 0;
	for( y = 0; y < dh; y++ )
	{
		if( y >= sh + stry || y < stry )
		{
			for( x = 0; x < dw; x++ )
			{
				*dst++ = 0x0;
			}
		}
		else
		{
			for( x = 0; x < strx; x++ )
			{
				*dst++ = 0x0;
			}
			for( ; x < sw+strx; x++ )
			{
				*dst++ = *src++;
			}
			for( ; x < dw; x++ )
			{
				*dst++ = 0x0;
			}
		}
	}
}

static int xfont_freetype2_read_internal( XFONT_FT2 * xfont, int code, int size, int style, u8 * buf, int limit, XFONT_INFO * info, int read )
{
	FT_UInt glyph_index;
	FT_Error error;
	FT_Int32 load_flags = FT_LOAD_DEFAULT ;
	int len;
	
	FT_Face face = xfont->face;

	if( xfont->font_size != size )
	{
		FT_Set_Pixel_Sizes( 
			face, /* face对象句柄 */ 
			0, /* 象素宽度 */ 
			size ); /* 象素高度 */ 
		xfont->font_size = size;
	}
	
	glyph_index = FT_Get_Char_Index( face, code ); 
	error = FT_Load_Glyph( 
			face, /* face对象的句柄 */ 
			glyph_index, /* 字形索引 */ 
			load_flags ); /* 装载标志，参考下面 */ 
	if( face->glyph->format != FT_GLYPH_FORMAT_BITMAP )
	{
		error = FT_Render_Glyph( face->glyph, /* 字形槽 */ 
		FT_RENDER_MODE_NORMAL ); /* 渲染模式 */
	}
	//ft2_draw( face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap_left, face->glyph->bitmap_top );
	len = face->glyph->bitmap.width * face->glyph->bitmap.rows;
	if( len > limit )
		len = limit;
	// *width = face->glyph->bitmap.width;
	info->font_width = face->glyph->advance.x >> 6;
	info->bitmap_width = face->glyph->bitmap.width;
	info->bitmap_height = face->glyph->bitmap.rows;
	info->bitmap_left = face->glyph->bitmap_left;
	info->bitmap_top = face->glyph->bitmap_top;
	//memcpy( buf, face->glyph->bitmap.buffer, len );
	if( info->bitmap_width < face->glyph->bitmap.width )
		info->bitmap_width = face->glyph->bitmap.width;
	
	if( !read )
		return 0;
	
	memcpy( buf, face->glyph->bitmap.buffer, len );
	//copy_bitmap( buf, face->glyph->bitmap.buffer, *width, *height, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap_left, size - face->glyph->bitmap_top );
	//copy_bitmap( buf, face->glyph->bitmap.buffer, *width, *height, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, size - face->glyph->bitmap.rows );
	//printf( "%c %d %d %d %d\n", code, *width, *height, face->glyph->bitmap_left, face->glyph->bitmap_top );
	//printf( "%c %d %d\n", code, face->glyph->bitmap_top, face->glyph->bitmap.rows );
	return 0;
}

int xfont_freetype2_read( XFONT_FT2 * xfont, int code, int size, int style, u8 * buf, int limit, XFONT_INFO * info )
{
	return xfont_freetype2_read_internal( xfont, code, size, style, buf, limit, info, 1 );
}

int xfont_freetype2_getsize( XFONT_FT2 * xfont, int code, int size, int style, XFONT_INFO * info )
{
	return xfont_freetype2_read_internal( xfont, code, size, style, NULL, 0, info, 0 );
}

