#include <global.h>
#include "font.h"

static XFONT * textout_font = NULL;
static int textout_size = 16;
static int textout_style = XFONT_STYLE_NORMAL;
static XFONT_NLS * textout_nls = NULL;
static int textout_line_height = 16;
static int textout_word_spacing = 0;

int textout_deinit( void )
{
	int errcode = -1;
	if( textout_font )
	{
		xfont_close( textout_font );
		textout_font = NULL;
		errcode = 0;
	}
	if( textout_nls )
	{
		xfont_nls_close( textout_nls );
		textout_nls = NULL;
		errcode = 0;
	}
	return errcode;
}

int textout_init( char * fname, int size, int line_height, int word_spacing, int style, XFONT_CODE_PAGE codepage )
{
	if( (textout_font = xfont_open( fname )) == NULL )
		goto err; 
	if( (textout_nls = xfont_nls_open( XFONT_CP_936 )) == NULL )
		goto err;
	textout_size = size;
	textout_style = style;
	textout_line_height = line_height;
	textout_word_spacing = word_spacing;
	return 0;
err:
	textout_deinit();
	return -1;
}

STRING_INFO * mtextout_line( u32 * buf, int bufwidth, int bufheight, const char *string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
	static STRING_INFO info;
	int scr_width, scr_height, font_width, font_height;
	int limit;
	u16 ucode;
	u16 acode;
	int x = min_x;
	int y = min_y;
	const u8 * str = (u8 *)string;
	scr_width = bufwidth;
	scr_height = bufheight;
	if( max_y > scr_height )
		max_y = scr_height;
	limit = (max_x + (scr_width * max_y)) * 4;
	
	while( *str )
	{
		if( *str & 0x80 )
		{
			acode = str[0];
			acode = (acode << 8) | str[1];
			str += 2;
		}
		else
		{
			acode = *str;
			str ++;
		}
			
		ucode = xfont_nls_a2u( textout_nls, acode );
		
		xfont_udraw( textout_font, ucode, textout_size, textout_style, fontcolor, bgcolor, LCD_A8R8G8B8, (bgcolor>>24) ? XFONT_BLEND_MODE_ALPHA : XFONT_BLEND_MODE_COVER,
			 x, y+textout_line_height, scr_width, 
			 buf, &font_width, &font_height, limit );

		x += font_width+textout_word_spacing;
		if( x >= scr_width || x >= max_x )
			break;
			
		if( *str == '\r' )
		{
			str++;
		}
		if( *str == '\n' )
		{
			str++;
			break;
		}
	}
	
	info.height = textout_line_height;
	info.width = x-min_x;
	info.length = (const char *)str-string;
	
	return &info;
}

STRING_INFO * mtextout16_line_colormode( u16 * buf, int bufwidth, int bufheight, const char *string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor, COLOR_MODE color_mode )
{
	static STRING_INFO info;
	int scr_width, scr_height, font_width, font_height;
	int limit;
	u16 ucode;
	u16 acode;
	int x = min_x;
	int y = min_y;
	const u8 * str = (u8 *)string;
	scr_width = bufwidth;
	scr_height = bufheight;
	if( max_y > scr_height )
		max_y = scr_height;
	limit = (max_x + (scr_width * max_y)) * 2;
	
	while( *str )
	{
		if( *str & 0x80 )
		{
			acode = str[0];
			acode = (acode << 8) | str[1];
			str += 2;
		}
		else
		{
			acode = *str;
			str ++;
		}
			
		ucode = xfont_nls_a2u( textout_nls, acode );
		
		xfont_udraw( textout_font, ucode, textout_size, textout_style, fontcolor, bgcolor, color_mode, XFONT_BLEND_MODE_ALPHA,
			 x, y+textout_line_height, scr_width, 
			 buf, &font_width, &font_height, limit );

		x += font_width+textout_word_spacing;
		if( x >= scr_width || x >= max_x )
			break;
			
		if( *str == '\r' )
		{
			str++;
		}
		if( *str == '\n' )
		{
			str++;
			break;
		}
	}
	
	info.height = textout_line_height;
	info.width = x-min_x;
	info.length = (const char *)str-string;
	
	return &info;
}

STRING_INFO * mtextoutW_line( u32 * buf, int bufwidth, int bufheight, const wchar_t * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
	static STRING_INFO info;
	int scr_width, scr_height, font_width, font_height;
	int limit;
	int x = min_x;
	int y = min_y;
	const wchar_t * str = (wchar_t *)string;
	scr_width = bufwidth;
	scr_height = bufheight;
	if( max_y > scr_height )
		max_y = scr_height;
	limit = (max_x + (scr_width * max_y)) * 4;
	
	while( *str )
	{
		xfont_udraw( textout_font, *str, textout_size, textout_style, fontcolor, bgcolor, LCD_A8R8G8B8, (bgcolor>>24) ? XFONT_BLEND_MODE_ALPHA : XFONT_BLEND_MODE_COVER,
			 x, y+textout_line_height, scr_width, 
			 buf, &font_width, &font_height, limit );

		x += font_width+textout_word_spacing;
		if( x >= scr_width || x >= max_x )
			break;
		str++;
		if( *str == '\r' )
		{
			str++;
		}
		if( *str == '\n' )
		{
			str++;
			break;
		}
	}
	
	info.height = textout_line_height;
	info.width = x-min_x;
	info.length = str-string;
	
	return &info;
}

static STRING_INFO * mtextoutW16_line_colormode( u16 * buf, int bufwidth, int bufheight, const wchar_t * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor, COLOR_MODE color_mode )
{
	static STRING_INFO info;
	int scr_width, scr_height, font_width, font_height;
	int limit;
	int x = min_x;
	int y = min_y;
	const wchar_t * str = (wchar_t *)string;
	scr_width = bufwidth;
	scr_height = bufheight;
	if( max_y > scr_height )
		max_y = scr_height;
	limit = (max_x + (scr_width * max_y)) * 2;
	
	while( *str )
	{
		xfont_udraw( textout_font, *str, textout_size, textout_style, fontcolor, bgcolor, color_mode, XFONT_BLEND_MODE_ALPHA,
			 x, y+textout_line_height, scr_width, 
			 buf, &font_width, &font_height, limit );

		x += font_width+textout_word_spacing;
		if( x >= scr_width || x >= max_x )
			break;
		str++;
		if( *str == '\r' )
		{
			str++;
		}
		if( *str == '\n' )
		{
			str++;
			break;
		}
	}
	
	info.height = textout_line_height;
	info.width = x-min_x;
	info.length = str-string;
	
	return &info;
}

STRING_INFO * mtextout16_line( u16 * buf, int bufwidth, int bufheight, const char *string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
    return mtextout16_line_colormode( buf, bufwidth, bufheight, string, min_x, min_y, max_x, max_y, fontcolor, bgcolor, LCD_R5G6B5 );
}

STRING_INFO * mtextoutW16_line( u16 * buf, int bufwidth, int bufheight, const wchar_t * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
    return mtextoutW16_line_colormode( buf, bufwidth, bufheight, string, min_x, min_y, max_x, max_y, fontcolor, bgcolor, LCD_R5G6B5 );
}

STRING_INFO * mtextout555_line( u16 * buf, int bufwidth, int bufheight, const char *string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
    return mtextout16_line_colormode( buf, bufwidth, bufheight, string, min_x, min_y, max_x, max_y, fontcolor, bgcolor, LCD_A1R5G5B5 );
}

STRING_INFO * mtextoutW555_line( u16 * buf, int bufwidth, int bufheight, const wchar_t * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
    return mtextoutW16_line_colormode( buf, bufwidth, bufheight, string, min_x, min_y, max_x, max_y, fontcolor, bgcolor, LCD_A1R5G5B5 );
}

STRING_INFO * textout_line( const char *string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
	return mtextout_line( lcd_bufferui(), screen_get_width(), screen_get_height(), string, min_x, min_y, max_x, max_y, fontcolor, bgcolor );
}

STRING_INFO * textoutW_line( const wchar_t *string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
	return mtextoutW_line( lcd_bufferui(), screen_get_width(), screen_get_height(), string, min_x, min_y, max_x, max_y, fontcolor, bgcolor );
}

STRING_INFO * mtextout( u32 *buf, int bufwidth, int bufheight, const char * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
	static STRING_INFO info, *pinfo;
	const char * pstr = string;
	int x = min_x, y = min_y;
	int max_width = 0;
	while( y < max_y )
	{
		pinfo = mtextout_line( buf, bufwidth, bufheight, pstr, x, y, max_x, max_y, fontcolor, bgcolor );
		if( pinfo->length == 0 )
			break;
		y += pinfo->height;
		pstr += pinfo->length;
		if( max_width < pinfo->width )
			max_width = pinfo->width;
	}
	info.width = max_width;
	info.height = y-min_y;
	info.length = pstr - string;
	return &info;
}

STRING_INFO * mtextoutW( u32 *buf, int bufwidth, int bufheight, const wchar_t * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
	static STRING_INFO info, *pinfo;
	const wchar_t * pstr = string;
	int x = min_x, y = min_y;
	int max_width = 0;
	while( y < max_y )
	{
		pinfo = mtextoutW_line( buf, bufwidth, bufheight, pstr, x, y, max_x, max_y, fontcolor, bgcolor );
		if( pinfo->length == 0 )
			break;
		y += pinfo->height;
		pstr += pinfo->length;
		if( max_width < pinfo->width )
			max_width = pinfo->width;
	}
	info.width = max_width;
	info.height = y-min_y;
	info.length = pstr - string;
	return &info;
}

STRING_INFO * mtextout16_colormode( u16 *buf, int bufwidth, int bufheight, const char * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor, COLOR_MODE color_mode )
{
	static STRING_INFO info, *pinfo;
	const char * pstr = string;
	int x = min_x, y = min_y;
	int max_width = 0;
	while( y < max_y )
	{
		pinfo = mtextout16_line_colormode( buf, bufwidth, bufheight, pstr, x, y, max_x, max_y, fontcolor, bgcolor, color_mode );
		if( pinfo->length == 0 )
			break;
		y += pinfo->height;
		pstr += pinfo->length;
		if( max_width < pinfo->width )
			max_width = pinfo->width;
	}
	info.width = max_width;
	info.height = y-min_y;
	info.length = pstr - string;
	return &info;
}

STRING_INFO * mtextoutW16_colormode( u16 *buf, int bufwidth, int bufheight, const wchar_t * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor, COLOR_MODE color_mode )
{
	static STRING_INFO info, *pinfo;
	const wchar_t * pstr = string;
	int x = min_x, y = min_y;
	int max_width = 0;
	while( y < max_y )
	{
		pinfo = mtextoutW16_line_colormode( buf, bufwidth, bufheight, pstr, x, y, max_x, max_y, fontcolor, bgcolor, color_mode );
		if( pinfo->length == 0 )
			break;
		y += pinfo->height;
		pstr += pinfo->length;
		if( max_width < pinfo->width )
			max_width = pinfo->width;
	}
	info.width = max_width;
	info.height = y-min_y;
	info.length = pstr - string;
	return &info;
}

STRING_INFO * mtextout16( u16 *buf, int bufwidth, int bufheight, const char * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
    return mtextout16_colormode( buf, bufwidth, bufheight, string, min_x, min_y, max_x, max_y, fontcolor, bgcolor, LCD_R5G6B5 );
}

STRING_INFO * mtextoutW16( u16 *buf, int bufwidth, int bufheight, const wchar_t * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
    return mtextoutW16_colormode( buf, bufwidth, bufheight, string, min_x, min_y, max_x, max_y, fontcolor, bgcolor, LCD_R5G6B5);
}

STRING_INFO * mtextout555( u16 *buf, int bufwidth, int bufheight, const char * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
    return mtextout16_colormode( buf, bufwidth, bufheight, string, min_x, min_y, max_x, max_y, fontcolor, bgcolor, LCD_A1R5G5B5 );
}

STRING_INFO * mtextoutW555( u16 *buf, int bufwidth, int bufheight, const wchar_t * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
    return mtextoutW16_colormode( buf, bufwidth, bufheight, string, min_x, min_y, max_x, max_y, fontcolor, bgcolor, LCD_A1R5G5B5 );
}

STRING_INFO * textout( const char * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
	return mtextout( lcd_bufferui(), screen_get_width(), screen_get_height(), string, min_x, min_y, max_x, max_y, fontcolor, bgcolor );
}

STRING_INFO * textoutW( const wchar_t * string, int min_x, int min_y, int max_x, int max_y, unsigned int fontcolor, unsigned int bgcolor )
{
	return mtextoutW( lcd_bufferui(), screen_get_width(), screen_get_height(), string, min_x, min_y, max_x, max_y, fontcolor, bgcolor );
}
