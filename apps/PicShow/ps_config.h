/***************************************************************************

		ps_config.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-12 20:43:03

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _ps_config_h_
#define _ps_config_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum ZOOM_MODE{
	ZOOM_MODE_WIDTH,
	ZOOM_MODE_HEIGHT,
	ZOOM_MODE_ORIGIN,
	ZOOM_MODE_50,
	ZOOM_MODE_25,
	ZOOM_MODE_12_5,
	ZOOM_MODE_MAX,
} ZOOM_MODE;

extern ZOOM_MODE default_zoom_mode;
extern ZOOM_MODE current_zoom_mode;

extern int use_slip_key;
//显示方向设置
extern BITMAP_ZOOM_DIRECTION PicShow_direction;
//自动方向设置
typedef enum AUTO_DIRECTION{
	AUTO_DIRECTION_SAME,
	AUTO_DIRECTION_90,
	AUTO_DIRECTION_270,
	AUTO_DIRECTION_END
} AUTO_DIRECTION;

extern AUTO_DIRECTION PicShow_auto_direction;
extern int load_preview;

int ps_config_save( void );
int ps_config_load( void );

//图片目录
extern char PicDir[];

#ifdef __cplusplus
}
#endif

#endif
