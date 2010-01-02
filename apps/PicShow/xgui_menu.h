/***************************************************************************

		xgui_menu.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-12 20:26:25

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _xgui_menu_h_
#define _xgui_menu_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum{
MENU_STATE_CONTINUE,
MENU_STATE_UPLEVEL,
MENU_STATE_EXIT,
} MENU_STATE;

typedef struct PICSHOW_MENU_HANDLE PICSHOW_MENU_HANDLE;
typedef struct PICSHOW_MENU PICSHOW_MENU;

struct PICSHOW_MENU {
	char * name;
	MENU_STATE (*picshow_menu_callback)( void * param );
	struct PICSHOW_MENU * child;
};

PICSHOW_MENU_HANDLE * PicShow_CreateMenu( PICSHOW_MENU * menu, char * name, int x, int y, XGUI_WND * mount );
void PicShow_DestroyMenu( PICSHOW_MENU_HANDLE * handle );

#ifdef __cplusplus
}
#endif

#endif

