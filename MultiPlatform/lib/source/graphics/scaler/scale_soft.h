/***************************************************************************

		scale_soft.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-07-24 12:16:45

		Copyright (c)  ChinaChip 2007-2009
***************************************************************************/
#ifndef _scale_soft_h_
#define _scale_soft_h_

#ifdef	__cplusplus
extern "C" {
#endif

/* RGB565模式高质量缩小图像一行(2x之内)
 * sw 源宽度 dw 目标宽度
 * sh 源高度 dh 目标高度
 * scanline  当前缩放的目标扫描线
 * dst 目标地址
 * src 源地址(缩放的当前行地址)
 * 源高度至少是两行
 * rgb32_rgb565_scale_line rgb32格式转rgb565缩放
 * rgb565_rgb565_scale_line rgb565格式转rgb565缩放
 */
// void rgb32_rgb565_scale_line( u16 * dst, u32 * src, int sw, int dw, int sh, int dh, int scanline );
void rgb32_rgb32_scale_line( u32 * dst, u32 * src, int sw, int dw, int sh, int dh, int scanline );
/*
void rgb565_rgb565_scale_line( u16 * dst, u16 * src, int sw, int dw, int sh, int dh, int scanline );
void rgb32_yuv422_scale_line( u8 * y, u8 * u, u8 *v, u32 * src, int sw, int dw, int sh, int dh, int scanline );
void rgb565_yuv422_scale_line( u8 * y, u8 * u, u8 *v, u16 * src, int sw, int dw, int sh, int dh, int scanline );
*/
/*
 * 放大图像一行, src必须有2行,dst输出一行, h_weight/dh == 第一行的权重
 */
/*
void rgb32_rgb565_normal_vertical_blowup( u32 * src, u16 * dst, int sw, int h_weight, int dh );
void rgb32_rgb565_rotate90_vertical_blowup( u32 * src, u16 * dst, int sw, int h_weight, int dh, int fbw );

void rgb32_rgb32_normal_horizontal_blowup( u32 * buf, u32 * src, int sw, int dw );
*/

void rgb32_rgb32_blowup_line( u32 * buf, u32 * src, int sw, int dw, int sh, int dh, int scanline );

#ifdef __cplusplus
}
#endif

#endif
