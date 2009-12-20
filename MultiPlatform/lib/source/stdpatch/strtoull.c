/***************************************************************************

		strtoull.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-05-14 12:53:11

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define LLONG_MAX    9223372036854775807LL
#define LLONG_MIN    (-LLONG_MAX - 1LL)
#define ULLONG_MAX (LLONG_MAX * 2ULL + 1ULL)
#define unconst(var, type) ((type)(var))

/*
 * Convert a string to an unsigned long long integer.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
unsigned long long int
strtoull(const char *nptr, char **endptr, int base)
{
  const unsigned char *s = (const unsigned char*) nptr;
  unsigned long long int acc;
  unsigned char c;
  unsigned long long int cutoff;
  int neg = 0, any, cutlim;

  /*
   * See strtol for comments as to the logic used.
   */
  do {
    c = *s++;
  } while (isspace(c));
  if (c == '-')
  {
    neg = 1;
    c = *s++;
  }
  else if (c == '+')
    c = *s++;
  if ((base == 0 || base == 16) &&
      c == '0' && (*s == 'x' || *s == 'X'))
  {
    c = s[1];
    s += 2;
    base = 16;
  }
  if (base == 0)
    base = c == '0' ? 8 : 10;
  cutoff = (unsigned long long int)ULLONG_MAX / base;
  cutlim = (unsigned long long int)ULLONG_MAX % base;
  for (acc = 0, any = 0; ; c = *s++)
  {
    if (isdigit(c))
      c -= '0';
    else if (isalpha(c))
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    else
      break;
    if (c >= base)
      break;
    if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
      any = -1;
    else {
      any = 1;
      acc *= base;
      acc += c;
    }
  }
  if (any < 0)
  {
    acc = ULLONG_MAX;
    //errno = ERANGE;
  }
  else if (neg)
    acc = -acc;
  if (endptr != 0)
    *endptr = any ? unconst(s, char *) - 1 : unconst(nptr, char *);
  return acc;
}
