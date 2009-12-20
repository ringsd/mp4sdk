/***************************************************************************

		sprintf.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-21 19:23:26

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <stdio.h>
#include <stdarg.h>

int
sprintf (char *buf, const char *fmt,...)
{
  va_list args;
  int i;

  va_start (args, fmt);
  i = vsprintf (buf, fmt, args);
  va_end (args);
  return i;
}

