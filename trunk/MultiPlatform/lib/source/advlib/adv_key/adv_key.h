/***************************************************************************

		advkey.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-15 21:16:20

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _advkey_h_
#define _advkey_h_

#ifdef	__cplusplus
extern "C" {
#endif

//菜单读键
//返回键值,特点是持续按下按键会自动连发
u32 menu_get_key();

#ifdef __cplusplus
}
#endif

#endif
