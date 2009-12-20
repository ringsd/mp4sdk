/***************************************************************************

		vfprintf.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-21 18:27:01

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <stdio.h>
#include <stdarg.h>

int
vfprintf(FILE *fp, const char *fmt0, va_list ap)
{
		char buf[0x1000];
		int n = vsprintf( buf, fmt0, ap );
		printf( "%s\n", buf );
		return fwrite( buf, n, 1, fp );
}

int
_IO_vfprintf(FILE *fp, const char *fmt0, va_list ap)
{
	printf( "%x %s\n", fp, fmt0 );
	vfprintf( fp, fmt0, ap );
}
