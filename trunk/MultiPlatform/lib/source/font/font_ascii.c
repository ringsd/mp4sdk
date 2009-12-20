/***************************************************************************

		font_ascii.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-05 22:41:24

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>
#include "font_ascii_data.h"

void display_byte_rgb32(char code,u32 *dst,u32 font_color,u32 bg_color, int width )
{
	const u8 *src;
	u32 i;
	src = &font_ascii[code*16];
	
	for(i=0;i<16;i++)
	{
		u8 data = *src;
		dst[7]=data&0x1?font_color:bg_color;data>>=1;
		dst[6]=data&0x1?font_color:bg_color;data>>=1;
		dst[5]=data&0x1?font_color:bg_color;data>>=1;
		dst[4]=data&0x1?font_color:bg_color;data>>=1;
		dst[3]=data&0x1?font_color:bg_color;data>>=1;
		dst[2]=data&0x1?font_color:bg_color;data>>=1;
		dst[1]=data&0x1?font_color:bg_color;data>>=1;
		dst[0]=data&0x1?font_color:bg_color;data>>=1;

		src++;
		dst+=width;
	}
}

void display_byte_gray8(char code,u8 *dst, int width )
{
	const u8 *src;
	u8 font_color = 0xFF;
	u8 bg_color = 0x0;
	u32 i;
	src = &font_ascii[code*16];
	
	for(i=0;i<16;i++)
	{
		u8 data = *src;
		dst[7]=data&0x1?font_color:bg_color;data>>=1;
		dst[6]=data&0x1?font_color:bg_color;data>>=1;
		dst[5]=data&0x1?font_color:bg_color;data>>=1;
		dst[4]=data&0x1?font_color:bg_color;data>>=1;
		dst[3]=data&0x1?font_color:bg_color;data>>=1;
		dst[2]=data&0x1?font_color:bg_color;data>>=1;
		dst[1]=data&0x1?font_color:bg_color;data>>=1;
		dst[0]=data&0x1?font_color:bg_color;data>>=1;
		src++;
		dst+=width;
	}
}
