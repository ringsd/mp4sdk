/***************************************************************************

		close.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-10 21:09:11

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <stdio.h>

int
close (int handle)
{
  return fclose ((FILE *)handle);
}
