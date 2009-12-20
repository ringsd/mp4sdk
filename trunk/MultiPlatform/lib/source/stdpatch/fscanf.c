/***************************************************************************

		fscanf.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-13 15:56:17

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <stdarg.h>
#include <stdio.h>

/* Read formatted input from STREAM according to the format string FORMAT.  */
/* VARARGS2 */
int
fscanf (FILE *stream, const char *format, ...)
{
  va_list arg;
  int done;
  char buf[1024];
  va_start (arg, format);
  int save_seek = ftell(save_seek);
  fread( buf, 1, sizeof(buf), stream );
  done = vsscanf (buf, format, arg);
  //done = __vfscanf (stream, format, arg);
  va_end (arg);
	fseek( stream, save_seek+done, SEEK_SET );
  return done;
}
