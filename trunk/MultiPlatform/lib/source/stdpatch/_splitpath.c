/***************************************************************************

		_splitpath.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-10 22:35:52

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>

#define MAX_PATH 1024

void _splitpath (const char *path, char *drive, char *dir, char *fname,
                 char *ext)
{
	char *slash;
	char *dot;
	
    *drive = 0;

    slash = strrchr (path, '/');
    if (!slash)
        slash = strrchr (path, '\\');

    dot = strrchr (path, '.');

    if (dot && slash && dot < slash)
        dot = NULL;

    if (!slash)
    {
        strcpy (dir, "");
        strcpy (fname, path);
        if (dot)
        {
            *(fname + (dot - path)) = 0;
            strcpy (ext, dot + 1);
        }
        else
            strcpy (ext, "");
    }
    else
    {
        strcpy (dir, path);
        *(dir + (slash - path)) = 0;
        strcpy (fname, slash + 1);
        if (dot)
        {
            *(fname + (dot - slash) - 1) = 0;
            strcpy (ext, dot + 1);
        }
        else
            strcpy (ext, "");
    }
}






