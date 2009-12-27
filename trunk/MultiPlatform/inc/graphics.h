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
/***************************************************************************

		bitmap_scaler.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-11-29 17:52:28

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/
#ifndef _bitmap_scaler_h_
#define _bitmap_scaler_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct BITMAP_SCALER BITMAP_SCALER;

BITMAP_SCALER * bitmap_scaler_init
( int sw, int sh, int dw, int dh, 
int (*readline)( int line, void * buffer, void * param ), 
void * param );

int bitmap_scaler_readline( BITMAP_SCALER * scaler, void * buf, int line );

int bitmap_scaler_deinit( BITMAP_SCALER * scaler );

#ifdef __cplusplus
}
#endif

#endif
/***************************************************************************

		image_scaler.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-11-30 20:36:43

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/
#ifndef _image_scaler_h_
#define _image_scaler_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct IMAGE_SCALER IMAGE_SCALER;

IMAGE_SCALER * image_scaler_init( const char * path, int dw, int dh );

int image_scaler_readline( IMAGE_SCALER * iscaler, void * buf, int scanline );

int image_scaler_deinit( IMAGE_SCALER * iscaler );

#ifdef __cplusplus
}
#endif

#endif
