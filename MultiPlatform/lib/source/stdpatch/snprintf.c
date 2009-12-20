/***************************************************************************

		snprintf.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-09-20 18:30:12

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <stdio.h>
#include <stdarg.h>

int
snprintf (char *str, size_t sz, const char *format, ...)
{
  va_list args;
  int ret;

  va_start(args, format);
  ret = vsnprintf (str, sz, format, args);

  va_end(args);
  return ret;
}

