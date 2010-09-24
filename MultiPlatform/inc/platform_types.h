/***************************************************************************

		types.h

		No description.

		TIME LIST:
		CREATE	Rainy	2007-12-18 10:03:07

		Copyright (c)  2007-2007
***************************************************************************/
#ifndef _types_
#define _types_

#ifdef		__cplusplus
extern "C" {
#endif

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef long s32;
typedef unsigned long u32;
typedef long long s64;
typedef unsigned long long u64;

#define UTYPES_DEFINED

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
