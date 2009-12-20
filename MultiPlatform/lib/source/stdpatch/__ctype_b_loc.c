/*
 *  Copyright (C) 2005, 2006, 2007 EMSCB Consortium
 *
 *  This file is part of the PERSEUS Security Framework
 *  (http://www.perseus-os.org). It was partly funded by the
 *  European Multilaterally Secure Computing Base (EMSCB) project
 *  (http://www.emscb.org).
 *
 *  PERSEUS is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *	当使用ADS1.2编译含GCC头文件的程序时,需要这个补丁
 */


/*!
 *  @file     ctype.cpp
 *  @brief    Implementation of missing libc function needed by libgmp
 *  @author   Marko Wolf (mwolf@crypto.rub.de)
 *  @version  $Revision: 2689 $
 *  @date     $Date: 2006-06-20 12:26:10 +0200 (Di, 20 Jun 2006) $
 *  @note     temporary solution!
 */
 
#include <ctype.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* self build ctype_b array based on sources from linux and uclibc */

#define _U      0x01    /* upper */
#define _L      0x02    /* lower */
#define _D      0x04    /* digit */
#define _C      0x08    /* cntrl */
#define _P      0x10    /* punct */
#define _S      0x20    /* white space (space/lf/tab) */
#define _X      0x40    /* hex digit */
#define _SP     0x80    /* hard space (0x20) */

// 384 bytes for access from -128 to 255
const unsigned short int _ctype[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                /* -128 (1)*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                /*    . (2)*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                /*    . (3)*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                /*    . (4)*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                /*    . (5)*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                /*    . (6)*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                /*    . (7)*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                /* -1   (8)*/
_C,_C,_C,_C,_C,_C,_C,_C,                                                                /* 0-7     */
_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,                                 /* 8-15    */
_C,_C,_C,_C,_C,_C,_C,_C,                                                                /* 16-23   */
_C,_C,_C,_C,_C,_C,_C,_C,                                                                /* 24-31   */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,                                                    /* 32-39   */
_P,_P,_P,_P,_P,_P,_P,_P,                                                                /* 40-47   */
_D,_D,_D,_D,_D,_D,_D,_D,                                                                /* 48-55   */
_D,_D,_P,_P,_P,_P,_P,_P,                                                                /* 56-63   */
_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,                              /* 64-71   */
_U,_U,_U,_U,_U,_U,_U,_U,                                                                /* 72-79   */
_U,_U,_U,_U,_U,_U,_U,_U,                                                                /* 80-87   */
_U,_U,_U,_P,_P,_P,_P,_P,                                                                /* 88-95   */
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,                              /* 96-103  */
_L,_L,_L,_L,_L,_L,_L,_L,                                                                /* 104-111 */
_L,_L,_L,_L,_L,_L,_L,_L,                                                                /* 112-119 */
_L,_L,_L,_P,_P,_P,_P,_C,                                                                /* 120-127 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                /* 128-143 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                /* 144-159 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,    /* 160-175 */
_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,        /* 176-191 */
_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,        /* 192-207 */
_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,        /* 208-223 */
_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,        /* 224-239 */
_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L         /* 240-255 */
};

// unsigned access offset = 1 ( signed access offset = 128 )
const unsigned short int *loc_ctype = _ctype + 1;

// return offset access on ctype_b array
const unsigned short int **__ctype_b_loc( void )
{
        return (const unsigned short int**) (&loc_ctype);
}


#ifdef __cplusplus
}
#endif
