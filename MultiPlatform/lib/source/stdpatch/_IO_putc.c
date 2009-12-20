/***************************************************************************

		_IO_putc.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-26 21:05:08

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <stdio.h>

int _IO_putc(int c, FILE *f)
{
	return fputc(f,c);
}
