/***************************************************************************

		strcasecmp.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-29 10:43:35

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <global.h>
#include <ctype.h>

#define CTYPE_NUM_CHARS       256

#ifndef UCHAR_MAX
#define UCHAR_MAX     0xff      /* maximum unsigned char value */
#endif
#ifndef INT_MAX
#define INT_MAX       2147483647    /* maximum (signed) int value */
#endif

int strcasecmp (const char *s1, const char *s2)
{
  if (s1 == s2)
    return 0;
    {
      const unsigned char *p1 = (const unsigned char *) s1;
      const unsigned char *p2 = (const unsigned char *) s2;
      unsigned char c1, c2;

      do
        {
          c1 = tolower (*p1);
          c2 = tolower (*p2);

          if (c1 == '\0')
            break;

          ++p1;
          ++p2;
        }
      while (c1 == c2);

      if (UCHAR_MAX <= INT_MAX)
        return c1 - c2;
      else
        /* On machines where 'char' and 'int' are types of the same size, the
           difference of two 'unsigned char' values - including the sign bit -
           doesn't fit in an 'int'.  */
        return (c1 > c2 ? 1 : c1 < c2 ? -1 : 0);
    }
}


