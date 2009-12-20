/***************************************************************************

		isnan.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-26 21:36:58

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

/* public domain rewrite of isnan(3) */

static int
double_ne(double n1, double n2)
{
    return n1 != n2;
}

int
isnan(double n)
{
    return double_ne(n, n);
}


