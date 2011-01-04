/***************************************************************************

		miniio.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-01-28 12:31:05

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _miniio_h_
#define _miniio_h_

#ifdef	__cplusplus
extern "C" {
#endif

//类printf函数,输出到ui窗口
void ui_printf( const char *format ,...);
//可定位坐标的dprintf
void lprintf( u32 x, u32 y ,const char *format ,...);
//可定位坐标的printf, 会自动填充背景色
void loc_printf( u32 x, u32 y ,const char *format ,...);
//清空debug屏幕,并将printf坐标复位
void DbgCon_Cls( void );
//将printf坐标复位
void DbgCon_Reset( void );
//设置debug界面色彩
void DbgCon_SetColor( u32 font, u32 bg );

#ifdef __cplusplus
}
#endif

#endif

