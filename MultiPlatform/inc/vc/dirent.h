/***************************************************************************

		dirent.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2010-01-17 22:29:41

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/
#ifndef _dirent_h_
#define _dirent_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct DIR DIR;
#define mkdir( a, b ) mkdir(a)

#ifndef F_OK
#define F_OK 0
#endif
#ifndef X_OK
#define X_OK 4
#endif
#ifndef W_OK
#define W_OK 2
#endif
#ifndef R_OK
#define R_OK 4
#endif

#ifdef __cplusplus
}
#endif

#endif
