/***************************************************************************

		lseek.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-21 18:06:20

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <stdio.h>

long lseek(int handle, long offset, int fromwhere)
{
//	TRACE_LOG("lseek");
	fseek( (FILE *)handle, offset, fromwhere );
	return ftell( (FILE *)handle );
}
