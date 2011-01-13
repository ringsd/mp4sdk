/***************************************************************************

		ps_menu.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-12 20:26:04

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _ps_menu_h_
#define _ps_menu_h_

#ifdef	__cplusplus
extern "C" {
#endif

//菜单返回的附加命令
typedef enum MENU_CMD {
	MENU_CMD_NORMAL,		//正常返回,继续PicShow
	MENU_CMD_EXIT,			//退出程序,return.
	MENU_CMD_REZOOM,		//重缩放
	MENU_CMD_END
} MENU_CMD;

MENU_CMD PicShow_Menu( void );
extern KEYNAME ps_keyname[];

#ifdef __cplusplus
}
#endif

#endif
