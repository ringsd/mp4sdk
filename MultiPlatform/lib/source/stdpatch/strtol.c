/***************************************************************************

		strtol.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-26 20:48:16

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <stdlib.h>

#define LONG_MIN                0x80000000
#define LONG_MAX                0x7fffffff
#define INT_MIN                 0x80000000
#define INT_MAX                 0x7fffffff

#define weak_function

#define ULONG_MAX       ((unsigned long)(~0L))          /* 0xFFFFFFFF */
#define LONG_MAX        ((long)(ULONG_MAX >> 1))        /* 0x7FFFFFFF */
#define LONG_MIN        ((long)(~LONG_MAX))             /* 0x80000000 */

extern int errno;

long
__strtol_internal(s, ptr, base)
     const char *s; char **ptr; int base;
{
  extern unsigned long  strtoul();
  int minus=0;
  unsigned long tmp;
  const char *start=s, *eptr;

  if (s==NULL)
    {
      // errno = ERANGE;
      if (!ptr)
        *ptr = (char *)start;
      return 0L;
    }
  while (isspace(*s))
        s++;
  if (*s == '-') {
        s++;
        minus = 1;
      }
  else if (*s == '+')
    s++;

  /*
   * let strtoul do the hard work.
   */
  tmp = strtoul(s, &eptr, base);
  if (ptr != NULL)
    *ptr = (char *)((eptr==s) ? (char *)start : eptr);
  if (tmp > (minus ? - (unsigned long) LONG_MIN : (unsigned long) LONG_MAX))
    {
      // errno = -1;
      return (minus ? LONG_MIN : LONG_MAX);
    }
  return (minus ? (long) -tmp : (long) tmp);
}
