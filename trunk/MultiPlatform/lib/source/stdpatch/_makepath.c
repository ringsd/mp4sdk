/***************************************************************************

		_makepath.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-11 20:40:15

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>

void _makepath (char *path, const char *drive, const char *dir,
                const char *fname, const char *ext)
{
    if (dir && *dir)
    {
        strcpy (path, dir);
        strcat (path, "/");
    }
    else
        *path = 0;
    strcat (path, fname);
    if (ext && *ext)
    {
        strcat (path, ".");
        strcat (path, ext);
    }
}

