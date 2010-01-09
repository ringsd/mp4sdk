/***************************************************************************

		win32_config.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-05-02 13:35:01

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _win32_config_h_
#define _win32_config_h_

#ifdef	__cplusplus
extern "C" {
#endif

int win32_config_load( const char * ini_path );
int win32_config_save( const char * ini_path );

#ifdef __cplusplus
}
#endif

#endif
