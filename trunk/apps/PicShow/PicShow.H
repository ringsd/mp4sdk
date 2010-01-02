/***************************************************************************

		PicShow.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-05-07 14:28:25

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _PicShow_h_
#define _PicShow_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum RUN_FLAG{
	RUN,
	EXIT,
} RUN_FLAG;

extern RUN_FLAG run_flag;
extern KEYMAP pskeymap_right0[];
extern KEYMAP pskeymap_right90[];
extern KEYMAP pskeymap_right180[];
extern KEYMAP pskeymap_right270[];

typedef enum PICSHOW_KEY{
	PSKEY_UP = 0x1,
	PSKEY_DOWN = 0x2,
	PSKEY_LEFT = 0x4,
	PSKEY_RIGHT = 0x8,
	PSKEY_ZOOMIN = 0x10,
	PSKEY_ZOOMOUT = 0x20,
	PSKEY_PREV = 0x40,
	PSKEY_NEXT = 0x80,
	PSKEY_EXIT = 0x100,
	PSKEY_ZOOM_STEP = 0x200,
	PSKEY_ZOOM_TYPE_WIDTH = 0x400,
	PSKEY_ZOOM_TYPE_HEIGHT = 0x800,
	PSKEY_ZOOM_TYPE_ORIGIN = 0x1000,
	PSKEY_MENU = 0x2000,
	//-------------virtual key---------------
	PSKEY_ZOOMIN_SLIP = 0x02000000,
	PSKEY_ZOOMOUT_SLIP = 0x04000000,
	PSKEY_UP_SLIP = 0x08000000,
	PSKEY_DOWN_SLIP = 0x10000000,
	PSKEY_LEFT_SLIP = 0x20000000,
	PSKEY_RIGHT_SLIP = 0x40000000,
	PSKEY_END
} PICSHOW_KEY;

extern char CurrDir[MAX_PATH];

#ifdef __cplusplus
}
#endif

#endif
