/***************************************************************************

		read.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-10 21:06:17

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <stdio.h>

int
read (int handle, void *buffer, unsigned int count)
{
	int c = fread (buffer, 1, count, (FILE *)handle);
//	printf( "read %d %d %d\n", count, c, ftell((FILE *)handle) );
  return c;
}

