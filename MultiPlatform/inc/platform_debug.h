/***************************************************************************

		debug.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-02 21:51:50

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _debug_h_
#define _debug_h_

#ifdef	__cplusplus
extern "C" {
#endif

//ต๗สิบ๊
#if defined(__DEBUG_TRACE) || defined(__DEBUG_TRACE_LOG__)
#define TRACE_LOG(comment)  printf( "FILE:%s LINE:%d COMMEMT:%s\n", __FILE__, __LINE__, comment )
#else
#define TRACE_LOG(comment)
#endif

#ifdef __cplusplus
}
#endif

#endif
