/***************************************************************************

		scale_soft.c

		多格式软件缩放模块.

		TIME LIST:
		CREATE	Rainy	2009-07-24 12:16:42

		Copyright (c)  ChinaChip 2007-2009
***************************************************************************/

#include "global.h"

/* 合成一个颜色通道值
 */
#define MAKE_CHANNEL_TYPE(channel00, channel01, channel10, channel11, type ) \
		( 				((int)channel00)*h_weight*v_weight \
						+ ((int)channel10)*h_weight*(256-v_weight) \
		        + ((int)channel01)*(type-h_weight)*v_weight \
		        + ((int)channel11)*(type-h_weight)*(256-v_weight) \
		        ) / (type*256) \

#define MAKE_CHANNEL_SW(channel00, channel01, channel10, channel11) \
 MAKE_CHANNEL_TYPE(channel00, channel01, channel10, channel11, sw)

#define MAKE_CHANNEL_DW(channel00, channel01, channel10, channel11) \
 MAKE_CHANNEL_TYPE(channel00, channel01, channel10, channel11, dw)
 
/* RGB565格式的源图取色
 */
#define rgb565_R00 ((src[0]&0xF800)>>8)       
#define rgb565_G00 ((src[0]&0x07E0)>>3)    
#define rgb565_B00 ((src[0]&0x001F)<<3)        
                                    
#define rgb565_R01 ((src[1]&0xF800)>>8)   
#define rgb565_G01 ((src[1]&0x07E0)>>3)    
#define rgb565_B01 ((src[1]&0x001F)<<3)        
                                    
#define rgb565_R10 ((src[sw]&0xF800)>>8)  
#define rgb565_G10 ((src[sw]&0x07E0)>>3)   
#define rgb565_B10 ((src[sw]&0x001F)<<3)
                                    
#define rgb565_R11 ((src[sw+1]&0xF800)>>8)
#define rgb565_G11 ((src[sw+1]&0x07E0)>>3)
#define rgb565_B11 ((src[sw+1]&0x001F)<<3)

/* RGB32格式的源图取色
 */
#define rgb32_A00 ((src[0]&0xFF000000)>>24) 
#define rgb32_R00 ((src[0]&0xFF0000)>>16)       
#define rgb32_G00 ((src[0]&0xFF00)>>8)    
#define rgb32_B00 ((src[0]&0xFF))
                                    
#define rgb32_A01 ((src[1]&0xFF000000)>>24) 
#define rgb32_R01 ((src[1]&0xFF0000)>>16)   
#define rgb32_G01 ((src[1]&0xFF00)>>8)    
#define rgb32_B01 ((src[1]&0xFF))      

#define rgb32_A10 ((src[sw]&0xFF000000)>>24)                             
#define rgb32_R10 ((src[sw]&0xFF0000)>>16)  
#define rgb32_G10 ((src[sw]&0xFF00)>>8)   
#define rgb32_B10 ((src[sw]&0xFF))        
        
#define rgb32_A11 ((src[sw+1]&0xFF000000)>>24)                                
#define rgb32_R11 ((src[sw+1]&0xFF0000)>>16)
#define rgb32_G11 ((src[sw+1]&0xFF00)>>8)
#define rgb32_B11 ((src[sw+1]&0xFF))

/* 颜色生成 */
#define rgb565_make() ((*dst) = (((r&0xF8)<<8) | ((g&0xFC)<<3) | ((b&0xF8)>>3)))

#define rgb32_make() (*dst = ((a << 24) | (r << 16) | (g << 8) | b))

#define yuv422_make()											\
{                                                                \
	static int save_r, save_g, save_b;                             \
	if( i&1 )                                                      \
	{                                                              \
		s32 ty, u0, v0, u1, v1;                                      \
		                                                             \
		ty = (save_r * (s32)(RGB2YUV_RY*256)                              \
				+ save_g * (s32)(RGB2YUV_GY*256)                              \
				+ save_b * (s32)(RGB2YUV_BY*256))>>8;                         \
		v0 = (save_r-ty) * (s32)(RGB2YUV_RY_V*256);                         \
		u0 = (save_b-ty) * (s32)(RGB2YUV_BY_U*256);                         \
		                                                             \
		*y++ = ty;                                                   \
		ty = (r * (s32)(RGB2YUV_RY*256)                                   \
				+ g * (s32)(RGB2YUV_GY*256)                                   \
				+ b * (s32)(RGB2YUV_BY*256))>>8;                              \
		v1 = (r-ty) * (s32)(RGB2YUV_RY_V*256);                              \
		u1 = (b-ty) * (s32)(RGB2YUV_BY_U*256);                              \
		*y++ = ty;                                                   \
		                                                             \
		*u++ = ((u0+u1)>>9)+128;                                     \
		*v++ = ((v0+v1)>>9)+128;                                     \
	}                                                              \
	else                                                           \
	{                                                              \
		save_r = r;                                                  \
		save_g = g;                                                  \
		save_b = b;                                                  \
	}                                                              \
}                                                                \

/* 移动到下一个像素 */
// 正常
#define rgb565_normal_movedst() (dst++)
#define rgb32_normal_movedst() (dst++)
#define yuv422_normal_movedst()
// 顺时针旋转90度
#define rgb565_rotate90_movedst() (dst+=dw)
#define rgb32_rotate90_movedst() (dst+=dw)
#define yuv422_rotate90_movedst()

/* RGB数据类型
 */
#define rgb565_type u16
#define rgb32_type u32

/* 输出参数定义
 */
#define rgb565_dparam u16 * dst
#define rgb32_dparam u32 * dst
#define yuv422_dparam u8 * y, u8 * u, u8 * v
 
/* 高质量RGB32,RGB565->RGB565缩小显示函数模板
 * STYPE 可选 rgb565, rgb32
 * DTYPE 可选 rgb565, yuv422
 */
#define SCALE_LINE_BUILDER( STYPE, DTYPE )							\
void STYPE##_##DTYPE##_scale_line											\
( DTYPE##_dparam, STYPE##_type * src, int sw, int dw, int sh, int dh, int scanline )	\
{                                                                           \
	int i;                                                                    \
	int v_weight = (dh-(scanline*sh%dh))*256/sh;                         \
	int h_weight = dw;                                            \
	for( i = 0; i < dw; i++ )                                                 \
	{                                                                         \
		int r = MAKE_CHANNEL_SW(STYPE##_R00, STYPE##_R01, STYPE##_R10, STYPE##_R11);    \
		int g = MAKE_CHANNEL_SW(STYPE##_G00, STYPE##_G01, STYPE##_G10, STYPE##_G11);    \
		int b = MAKE_CHANNEL_SW(STYPE##_B00, STYPE##_B01, STYPE##_B10, STYPE##_B11);    \
		int a = MAKE_CHANNEL_SW(STYPE##_A00, STYPE##_A01, STYPE##_A10, STYPE##_A11);    \
		                                                                        \
		DTYPE##_make();															\
		DTYPE##_normal_movedst();												\
		h_weight += dw;															\
        if( h_weight <= sw )													\
        {																		\
			src++;																\
			h_weight += (dw-sw);												\
		}																	    \
		else																	\
		{																		\
			h_weight -= sw;														\
		}																		\
		src++;                                                                  \
	}                                                                         \
}                                                                           \

// SCALE_LINE_BUILDER(rgb565,rgb565)
// SCALE_LINE_BUILDER(rgb32,rgb565)
// SCALE_LINE_BUILDER(rgb565,yuv422)
// SCALE_LINE_BUILDER(rgb32,yuv422)
SCALE_LINE_BUILDER(rgb32,rgb32)

/*
 * 转换格式,并水平方向放大一行, dw > sw
 */
#define BLOWUP_LINE_BUILDER( STYPE, DTYPE )																				\
void STYPE##_##DTYPE##_blowup_line( DTYPE##_dparam, STYPE##_type * src, int sw, int dw, int sh, int dh, int scanline ) \
{                                                                                 \
	int i;                                                                          \
	int h_weight = dw;                                                            \
	int v_weight = 255-(scanline*sh*256/dh)%256;                         \
	for( i = 0; i < dw; i++ )                                                       \
	{                                                                               \
		int r = MAKE_CHANNEL_DW(STYPE##_R00, STYPE##_R01, STYPE##_R10, STYPE##_R11);    \
		int g = MAKE_CHANNEL_DW(STYPE##_G00, STYPE##_G01, STYPE##_G10, STYPE##_G11);    \
		int b = MAKE_CHANNEL_DW(STYPE##_B00, STYPE##_B01, STYPE##_B10, STYPE##_B11);    \
		int a = MAKE_CHANNEL_DW(STYPE##_A00, STYPE##_A01, STYPE##_A10, STYPE##_A11);    \
		DTYPE##_make();                                                               \
		DTYPE##_normal_movedst();                                               \
		h_weight -= sw;                                                               \
		if( h_weight <= 0 )                                                           \
		{                                                                             \
			src++;                                                                      \
			h_weight += dw;                                                             \
		}                                                                             \
	}                                                                               \
}                                                                                 \

BLOWUP_LINE_BUILDER( rgb32, rgb32 )

// BLOWUP_LINE_BUILDER( rgb32, rgb565, normal );
// BLOWUP_LINE_BUILDER( rgb32, yuv422, normal );
// BLOWUP_LINE_BUILDER( rgb32, rgb32, rotate90 );
// BLOWUP_LINE_BUILDER( rgb32, rgb565, rotate90 );








