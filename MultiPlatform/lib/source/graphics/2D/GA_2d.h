/***************************************************************************

		GA_2d.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-17 19:06:00

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _GA_2d_h_
#define _GA_2d_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct GA_RECT{
  int left; 
  int top; 
  int right; 
  int bottom; 
} GA_RECT;

void GA_RectCopy_A8R8G8B8( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf );
void GA_RectCopy_R8G8B8_A8R8G8B8( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf );
void GA_RectCopy_R5G6B5_A8R8G8B8( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf );
void GA_RectCopy_A8R5G6B5_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf );
void GA_RectCopy_A9R5G5B5_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf );
void GA_RectCopy_R5G5B5A9_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf );
void GA_RectCopy_B8G8R8_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf );
void GA_RectCopy_R8G8B8_A8R8G8B8_VM( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf );
void GA_RectAlpha_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf );
void GA_RectCopy_Bit8_A8R8G8B8_MASK( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf,u32 *palette, u8 *mask_data );
void GA_RectCopy_Bit4_A8R8G8B8_MASK( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf,u32 *palette, u8 *mask_data );
void GA_RectCopy_Bit8_A8B8G8R8_MASK( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf,u32 *palette, u8 *mask_data );
void GA_RectCopy_Bit4_A8B8G8R8_A8R8G8B8( GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf, GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf,u32 *palette );

//使用外部Alpha表进行Alpha操作(Alpha表与源图像数据一一对应)
void GA_RectExtraAlpha_A8R8G8B8_A8R8G8B8(	GA_RECT *srcblock, GA_RECT *srcwnd, u8 *srcbuf,
														GA_RECT *dstblock, GA_RECT *dstwnd, u8 *dstbuf, u8* alpha );

#ifdef __cplusplus
}
#endif

#endif
