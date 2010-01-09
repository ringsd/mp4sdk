/***************************************************************************

		vc2005_fix.cpp

		VC2005可能会找不到VC6的一些全局数据.

		TIME LIST:
		CREATE	Rainy	2009-03-19 21:19:13

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/


extern "C" 
{
    int __mb_cur_max;
    //unsigned short* _pctype;
    int errno;
}
