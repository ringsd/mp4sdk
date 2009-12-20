/***************************************************************************

		ccbmf.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-08-03 14:00:01

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _ccbmf_h_
#define _ccbmf_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct CCBMF CCBMF;

CCBMF * ccbmf_open( const char * path );
int ccbmf_read( CCBMF * bmf, int code, int size, int style, u8 * buf, int limit, XFONT_INFO * info );
int ccbmf_getsize( CCBMF * bmf, int code, int size, int style, XFONT_INFO * info );
const char * ccbmf_name( CCBMF * bmf );
void ccbmf_close( CCBMF * bmf );

#ifdef __cplusplus
}
#endif

#endif
