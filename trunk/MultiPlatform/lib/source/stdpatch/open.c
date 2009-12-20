/***************************************************************************

		open.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-10 18:37:06

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
//#include <sys/io.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

int open(const char *pathname, int flags, ...)
{
	char openmode[8];
	FILE * fp;
	openmode[0] = 0;
	if( (flags & O_RDONLY) == O_RDONLY )
		strcat( openmode, "rb" );
	if( (flags & O_WRONLY) == O_WRONLY )
		strcat( openmode, "wb" );

	fp = fopen( pathname, openmode );
	printf( "open %s %s %x\n", pathname, openmode, fp );
	if( fp == NULL )
		return -1;
	return (int)fp;
}

