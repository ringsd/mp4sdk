/***************************************************************************

		miniio.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-01-28 12:31:08

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include "global.h"

static u32 dp_x = 0, dp_y = 0, move_line = 0;
static u32 color_font = 0x00FFFFFF, color_bg = 0x00000000;
static u32 move_height = 0;

static void fbfill( u32 * buf, u32 color, int length )
{
	while( --length )
	{
		*buf++ = color;
	}
}

void dprint_line( u8 *string )
{
	u32 width = screen_get_width();
	u32 height = screen_get_height();
	
	STRING_INFO *info;

	while( *string )
	{
		
		if( move_line == 1 )
		{
			memcpy( lcd_bufferui(), lcd_bufferui()+width*move_height, width*(height-move_height)*4 );
			//memset( lcd_bufferui()+width*(height-16), 0 , width*16*4 );
			fbfill( lcd_bufferui()+width*(height-move_height), color_bg, width*move_height );
		}
		
		info = textout_line( string, dp_x, dp_y, width, height, color_font, color_bg );
		
		if( dp_x + info->width >= width - 16 )
		{
			dp_x = 0;
			dp_y += info->height;
		}
		else
		{
			if( *(string + info->length - 1) == '\n' )
			{
				dp_x = 0;
				dp_y += info->height;
			}
			else
				dp_x += info->width;
		}
		
		if( dp_y >= height - info->height )
		{
			dp_y -= info->height;
			dp_x = 0;
			move_line = 1;
			move_height = info->height;
		}
		string += info->length;
	}
}

void ui_printf( const char *format ,...)
{
	u8 buffer[1024];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buffer,format,argptr);
	va_end(argptr);
	dprint_line(buffer);
	lcd_updateui();
}

void lprintf( u32 x, u32 y ,const char *format ,...)
{
	u8 buffer[1024];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buffer,format,argptr);
	va_end(argptr);
	textout( buffer, x, y, screen_get_width(), screen_get_height(), color_font, color_bg );
	lcd_updateui();
}

void loc_printf( u32 x, u32 y ,const char *format ,...)
{
    int width = screen_get_width();
	u8 buffer[1024];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buffer,format,argptr);
	va_end(argptr);
	if( move_height == 0 )
	    move_height = gui_default_line_height;
	fbfill( lcd_bufferui() + x + y * width, color_bg, move_height * width );
	textout( buffer, x, y, screen_get_width(), screen_get_height(), color_font, color_bg );
	lcd_updateui();
}

void DbgCon_Cls( void )
{
	dp_x = 0;
	dp_y = 0;
	move_line = 0;
	memset( lcd_bufferui(), 0, screen_get_width()*screen_get_height()*4 );
}

void DbgCon_Reset( void )
{
	dp_x = 0;
	dp_y = 0;
	move_line = 0;
}

void DbgCon_SetColor( u32 font, u32 bg )
{
	color_font = font;
	color_bg = bg;
}

