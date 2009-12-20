/***************************************************************************

		write.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-10 21:08:01

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <stdio.h>

int
write (int handle, void *buffer, unsigned int count)
{
  return fwrite (buffer, count, 1, (FILE *)handle);
}

