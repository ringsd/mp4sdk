/***************************************************************************

		strnlen.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-27 15:19:24

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <stdio.h>

size_t
strnlen(const char *s, size_t len)
{
    size_t i;

    for(i = 0; i < len && s[i]; i++)
        ;
    return i;
}

