/***************************************************************************

		MM_imager.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-26 12:58:17

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <stdio.h>
#include "msicon.h"

ssize_t  img_fread(io_glue *ig, void *buf, size_t count)
{
	return fread( buf, 1, count, ig->exdata );
}

ssize_t img_fwrite(io_glue *ig, const void *buf, size_t count)
{
	return fwrite( buf, 1, count, ig->exdata );
}

off_t  img_fseek(io_glue *ig, off_t offset, int whence)
{
	fseek( ig->exdata, offset, whence );
	return ftell( ig->exdata );
}

int    img_fclose(io_glue *ig)
{
	return fclose( ig->exdata );
}

ssize_t img_size (io_glue *ig)
{
	int seek = ftell( ig->exdata );
	int length;
	fseek( ig->exdata, 0, SEEK_END );
	length = ftell( ig->exdata );
	fseek( ig->exdata, seek, SEEK_SET );
	return length;
}

