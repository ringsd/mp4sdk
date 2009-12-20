/***************************************************************************

		_snprintf.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-10 17:51:28

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <stdarg.h>

int
_snprintf(char *buf, unsigned int buflen, char *fmt, ...)
{
  int rv;
  va_list ap;

  va_start(ap, fmt);
  rv = vsnprintf (buf, buflen, fmt, ap);
  va_end(ap);

  return (0);
}

