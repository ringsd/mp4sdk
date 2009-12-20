/***************************************************************************

		GA_2D.c

		2D图形加速器纯软件渲染引擎.

		TIME LIST:
		CREATE	Rainy	2009-02-17 19:06:06

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>
#include "GA_2D.h"

/*缩写说明
RectCopy 对矩形做直接拷贝
RectAlpha 对矩形做Alpha拷贝

色彩格式(从高到低位的色彩排列及所占的位数)
A Alpha R Red G Green B Blue
A8R8G8B8	32位色
R8G8B8		24位色
Bit4			4位的索引色

镜像方式
HM		水平镜像
VM		垂直镜像

掩码模式
MASK	使用掩码(0为不透明,1为透明)



*/

//平面图形复制
//ARGB888模式

#define GA_RectCopy_Head	\
	int x, y;                                                                                       \
	int maxline = dstblock->bottom - dstblock->top + 1;                                                 \
	int maxcol = dstblock->right - dstblock->left + 1;                                                  \
	int src_height = (srcblock->bottom - srcblock->top) + 1;                                            \
	int maxsrccol = srcblock->right - srcblock->left + 1;                                               \
	int limit_width = min( dstblock->right, dstwnd->right ) + 1;                                    \
	int limit_dststartx = max( dstblock->left, dstwnd->left );                                      \
	int limit_srcstartx = max( (dstwnd->left - dstblock->left) * maxsrccol / maxcol, 0 );           \
	int srcwidth = srcwnd->right - srcwnd->left + 1;                                                \
	int dstwidth = dstwnd->right - dstwnd->left + 1;																								\
	int line = max( srcblock->top, dstwnd->top - dstblock->top);																		\
	int starty = max(dstwnd->top,dstblock->top);																										\
	int endy = dstblock->bottom;																																		\
	int startx = max( dstblock->left, dstwnd->left );																									\
	int endx = min( dstblock->right, dstwnd->right );																								\

#define GA_RectCopy_End

#define GA_RectCopy_GetSrcOff( SrcStrX, SrcStrY )													\
(( src_height * (SrcStrY) / maxline ) * srcwidth + (SrcStrX))							\

#define GA_RectCopy_GetDstOff( DstStrX, DstStrY )													\
(( dstwidth * (DstStrY) ) + (DstStrX))																		\

void GA_RectCopy_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u32 *src = (u32 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line );
		
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			u32 pal = src[ col * maxsrccol / maxcol ];
			*dst = pal;
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}

void GA_RectAlpha_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u32 *src = (u32 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line );
		
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			u32 pal = src[ col * maxsrccol / maxcol ];
			u8 alpha = (u8 )(pal >> 24);
			if( alpha == 255 )
				*dst = pal;
			else if( alpha > 0 )
			{
				u32 bg = *dst;
				u32 r0 = (pal >> 16)&0xFF;
				u32 g0 = (pal >> 8)&0xFF;
				u32 b0 = pal&0xFF;
				u32 r1 = (bg >> 16)&0xFF;
				u32 g1 = (bg >> 8)&0xFF;
				u32 b1 = bg&0xFF;
				r1  = r0 * alpha / 255 + r1 * (255 - alpha) / 255;
				g1  = g0 * alpha / 255 + g1 * (255 - alpha) / 255;
				b1  = b0 * alpha / 255 + b1 * (255 - alpha) / 255;
				*dst = (r1 << 16) | (g1 << 8) | b1;
			}
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}

//使用外部Alpha表进行Alpha操作(Alpha表与源图像数据一一对应)
void GA_RectExtraAlpha_A8R8G8B8_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf, u8* alpha )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u32 *src = (u32 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line );
		u8 *alp = (u8 *)alpha + GA_RectCopy_GetSrcOff( srcblock->left, line );
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			u32 pal = src[ col * maxsrccol / maxcol ];
			u8 alpha = alp[ col * maxsrccol / maxcol ];
			if( alpha == 255 )
				*dst = pal;
			else if( alpha > 0 )
			{
				u32 bg = *dst;
				u32 r0 = (pal >> 16)&0xFF;
				u32 g0 = (pal >> 8)&0xFF;
				u32 b0 = pal&0xFF;
				u32 r1 = (bg >> 16)&0xFF;
				u32 g1 = (bg >> 8)&0xFF;
				u32 b1 = bg&0xFF;
				r1  = r0 * alpha / 255 + r1 * (255 - alpha) / 255;
				g1  = g0 * alpha / 255 + g1 * (255 - alpha) / 255;
				b1  = b0 * alpha / 255 + b1 * (255 - alpha) / 255;
				*dst = (r1 << 16) | (g1 << 8) | b1;
			}
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}

void GA_RectCopy_R5G6B5_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u16 *src = (u16 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line );
		
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			u32 pal = src[ col * maxsrccol / maxcol ];
			*dst = (( pal << 8 ) & 0xFF0000) | (( pal << 5 ) & 0xFF00) | (( pal << 3 ) & 0xFF);
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}

/*
void GA_RectCopy_A8R8G8B8( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	int x, y;
	int maxline = dstblock->bottom - dstblock->top;
	int maxcol = dstblock->right - dstblock->left;
	int line, col;
	int src_height = (srcblock->bottom - srcblock->top);
	int maxsrccol = srcblock->right - srcblock->left;
	int limit_width = min( dstblock->right, dstwnd->right ) + 1;
	int limit_dststartx = max( dstblock->left, dstwnd->left );
	int limit_srcstartx = max( (dstwnd->left - dstblock->left) * maxsrccol / maxcol, 0 );
	int srcwidth = srcwnd->right - srcwnd->left + 1;
	int dstwidth = dstwnd->right - dstwnd->left + 1;
	line = max( srcblock->top, dstwnd->top - dstblock->top);
	
	for( y = max(dstwnd->top,dstblock->top); y <= dstblock->bottom; y++ )
	{
		u32 *dst = (u32 *)dstbuf + ( dstwidth * y ) + limit_dststartx;
		u32 *src = (u32 *)srcbuf + ( src_height * line / maxline ) * srcwidth + srcblock->left;
		
		col = limit_srcstartx;
		for( x = dstblock->left + limit_srcstartx; x < limit_width; x++ )
		{
			u32 pal = src[ col * maxsrccol / maxcol ];
			*dst = pal;
			dst++;
			col++;
		}
		line++;
	}
}
*/

void GA_RectCopy_R8G8B8_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line ) * 3;
		
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			u8 *pixel = src + ( col * maxsrccol / maxcol ) * 3;
			*dst = pixel[0] | (pixel[1] << 8) | (pixel[2] << 16);
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}

void GA_RectCopy_B8G8R8_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line ) * 3;
		
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			u8 *pixel = src + ( col * maxsrccol / maxcol ) * 3;
			*dst = (pixel[0] << 16) | (pixel[1] << 8) | (pixel[2]);
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}

void GA_RectCopy_A8R5G6B5_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line ) * 3;
		
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			u8 *pixel = src + ( col * maxsrccol / maxcol ) * 3;
			u32 pal = pixel[0] | (pixel[1] << 8);
			*dst = (( pal << 8 ) & 0xFF0000) | (( pal << 5 ) & 0xFF00) | (( pal << 3 ) & 0xFF);
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}

void GA_RectCopy_A9R5G5B5_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line ) * 3;
		
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			u8 *pixel = src + ( col * maxsrccol / maxcol ) * 3;
			u32 pal = (u32)pixel[0] | ((u32)pixel[1] << 8);
			*dst = ( (pal&0x7C00) << 10 ) | ( (pal&0x3E0) << 6 ) | ( (pal&0x1F) << 3  );
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}

void GA_RectCopy_R5G5B5A9_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line ) * 3;
		
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			u8 *pixel = src + ( col * maxsrccol / maxcol ) * 3;
			u32 pal = (u32)pixel[1] | ((u32)pixel[2] << 8);
			//*dst = ( (pal&0x7C00) << 10 ) | ( (pal&0x3E0) << 6 ) | ( (pal&0x1F) << 3  );
			*dst = ( (pal&0x3E0) << 6 ) ; 
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}


void GA_RectCopy_R8G8B8_A8R8G8B8_VM(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	GA_RectCopy_Head;
	for( y = endy; y >= starty; y-- )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line ) * 3;
		
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			u8 *pixel = src + ( col * maxsrccol / maxcol ) * 3;
			*dst = pixel[0] | (pixel[1] << 8) | (pixel[2] << 16);
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}


/*
void GA_RectCopy_R8G8B8_A8R8G8B8( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	int x, y;
	int maxline = dstblock->bottom - dstblock->top;
	int maxcol = dstblock->right - dstblock->left;
	int line, col;
	int maxsrccol = srcblock->right - srcblock->left;
	line = srcblock->top;
	
	for( y = dstblock->top; y < dstblock->bottom; y++ )
	{
		u32 *dst = (u32 *)dstbuf + ( dstwnd->right * y ) + dstblock->left;
		u8 *src = srcbuf + ( ( srcwnd->right * (line *srcblock->bottom/maxline) ) + srcblock->left ) * 3;
		col = 0;
		for( x = dstblock->left; x < dstblock->right; x++ )
		{
			u8 *pixel = src + ( col * maxsrccol / maxcol ) * 3;
			*dst = pixel[0] | (pixel[1] << 8) | (pixel[2] << 16);
			dst++;
			col++;
		}
		line++;
	}
}

void GA_RectCopy_R8G8B8_A8R8G8B8_VM( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf )
{
	int x, y;
	int maxline = dstblock->bottom - dstblock->top;
	int maxcol = dstblock->right - dstblock->left;
	int line, col;
	int maxsrccol = srcblock->right - srcblock->left;
	line = srcblock->top;
	
	for( y = dstblock->bottom - 1; y >= dstblock->top; y-- )
	{
		u32 *dst = (u32 *)dstbuf + ( dstwnd->right * y ) + dstblock->left;
		u8 *src = srcbuf + ( ( srcwnd->right * (line *srcblock->bottom/maxline) ) + srcblock->left ) * 3;
		col = 0;
		for( x = dstblock->left; x < dstblock->right; x++ )
		{
			u8 *pixel = src + ( col * maxsrccol / maxcol ) * 3;
			*dst = pixel[0] | (pixel[1] << 8) | (pixel[2] << 16);
			dst++;
			col++;
		}
		line++;
	}
}
*/

void GA_RectCopy_Bit8_A8R8G8B8_MASK( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf,u32 *palette, u8 *mask_data )
{
	GA_RectCopy_Head;
	if( mask_data )
	{
		for( y = starty; y <= endy; y++ )
		{
			u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
			u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line );
			u8 *msk = (u8 *)mask_data + GA_RectCopy_GetSrcOff( srcblock->left, line );
			int col = limit_srcstartx;
			for( x = startx; x <= endx; x++ )
			{
				int off = col * maxsrccol / maxcol;
				if( msk[ off ] == 0 )
					*dst = palette[src[ off ]];
				else
					*dst = 0;
				dst++;
				col++;
			}
			line++;
		}
	}
	else
	{
		for( y = starty; y <= endy; y++ )
		{
			u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
			u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line );
			int col = limit_srcstartx;
			for( x = startx; x <= endx; x++ )
			{
				int off = col * maxsrccol / maxcol;
				*dst = palette[src[ off ]];
				dst++;
				col++;
			}
			line++;
		}
	}
	GA_RectCopy_End;
}

void GA_RectCopy_Bit8_A8B8G8R8_MASK( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf,u32 *palette, u8 *mask_data )
{
	u32 new_pal[256];
	GA_RectCopy_Head;
	{
		int i;
		for( i = 0; i < 256; i++ )
		{
			u32 color = palette[i];
			new_pal[i] = ( ( color >> 16 ) & 0xFF ) | 
						 ( ( color << 16 ) & 0xFF0000 ) | 
						 (   color & 0xFF00FF00 );
		}
	}
	if( mask_data )
	{
		for( y = starty; y <= endy; y++ )
		{
			u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
			u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line );
			u8 *msk = (u8 *)mask_data + GA_RectCopy_GetSrcOff( srcblock->left, line );
			int col = limit_srcstartx;
			for( x = startx; x <= endx; x++ )
			{
				int off = col * maxsrccol / maxcol;
				if( msk[ off ] == 0 )
					*dst = new_pal[src[ off ]];
				else
					*dst = 0;
				dst++;
				col++;
			}
			line++;
		}
	}
	else
	{
		for( y = starty; y <= endy; y++ )
		{
			u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
			u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line );
			int col = limit_srcstartx;
			for( x = startx; x <= endx; x++ )
			{
				int off = col * maxsrccol / maxcol;
				*dst = new_pal[src[ off ]];
				dst++;
				col++;
			}
			line++;
		}
	}
	GA_RectCopy_End;
}


void GA_RectCopy_Bit4_A8R8G8B8_MASK( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf,u32 *palette, u8 *mask_data )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line )/2;
		u8 *msk = (u8 *)mask_data + GA_RectCopy_GetSrcOff( srcblock->left, line );
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			int off = col * maxsrccol / maxcol;
			if( msk[ off ] == 0 )
				*dst = palette[ src[ off ] ];
			else
				*dst = 0;
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}

void GA_RectCopy_Bit4_A8B8G8R8_A8R8G8B8( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf,u32 *palette )
{
	GA_RectCopy_Head;
	for( y = starty; y <= endy; y++ )
	{
		u32 *dst = (u32 *)dstbuf + GA_RectCopy_GetDstOff( limit_dststartx, y );
		u8 *src = (u8 *)srcbuf + GA_RectCopy_GetSrcOff( srcblock->left, line );
		int col = limit_srcstartx;
		for( x = startx; x <= endx; x++ )
		{
			int off = ( col * maxsrccol / maxcol );
			*dst = palette[ src[ off ] & 0xF ];
			dst++;
			col++;
		}
		line++;
	}
	GA_RectCopy_End;
}
