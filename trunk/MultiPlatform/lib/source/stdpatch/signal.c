/***************************************************************************

		signal.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-10 18:27:07

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <signal.h>

#ifdef __sighandler_t
__sighandler_t signal (int __sig, __sighandler_t __handler)
{
    return 0;
}
#else
void	(*signal (int __sig, void (*__handler)(int)  )) (int)
{
	return 0;
}
#endif
