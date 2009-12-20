/***************************************************************************

		_ctype.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-05-14 21:07:54

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <ctype.h>

#ifdef _CT_U
/*	MIPS SDE GCC 编译器的 ctype 数组存在严重的问题,
		导致所有的is****函数都不能严格正确得到结果
		而且他们是用(_ctype+1)[(unsigned)c]的方式访问数组
		所以在这种BT的访问情况下,必须对应的把 _ctype数组+1存储
*/
#define _U _CT_U
#define _L _CT_L
#define _D _CT_N
#define _S _CT_S
#define _P _CT_P
#define _C _CT_C
#define _X _CT_X
#define _SP _CT_SP

#endif

/* for ctype.h */
const char _ctype[] = {
	#ifdef _CT_U
	0,
	#endif
    _C,_C,_C,_C,_C,_C,_C,_C,                        /* 0-7 */
    _C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,         /* 8-15 */
    _C,_C,_C,_C,_C,_C,_C,_C,                        /* 16-23 */
    _C,_C,_C,_C,_C,_C,_C,_C,                        /* 24-31 */
    _S|_SP,_P,_P,_P,_P,_P,_P,_P,                    /* 32-39 */
    _P,_P,_P,_P,_P,_P,_P,_P,                        /* 40-47 */
    _D,_D,_D,_D,_D,_D,_D,_D,                        /* 48-55 */
    _D,_D,_P,_P,_P,_P,_P,_P,                        /* 56-63 */
    _P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,      /* 64-71 */
    _U,_U,_U,_U,_U,_U,_U,_U,                        /* 72-79 */
    _U,_U,_U,_U,_U,_U,_U,_U,                        /* 80-87 */
    _U,_U,_U,_P,_P,_P,_P,_P,                        /* 88-95 */
    _P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,      /* 96-103 */
    _L,_L,_L,_L,_L,_L,_L,_L,                        /* 104-111 */
    _L,_L,_L,_L,_L,_L,_L,_L,                        /* 112-119 */
    _L,_L,_L,_P,_P,_P,_P,_C,                        /* 120-127 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                /* 128-143 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                /* 144-159 */
    _S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,   /* 160-175 */
    _P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,       /* 176-191 */
    _U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,       /* 192-207 */
    _U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,       /* 208-223 */
    _L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,       /* 224-239 */
    _L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};      /* 240-255 */



