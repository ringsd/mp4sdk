/***************************************************************************

		bitmap_scaler.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-11-29 17:52:28

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/
#ifndef _bitmap_scaler_h_
#define _bitmap_scaler_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct BITMAP_SCALER BITMAP_SCALER;

BITMAP_SCALER * bitmap_scaler_init
( int sw, int sh, int dw, int dh, 
int (*readline)( int line, void * buffer, void * param ), 
void * param );

int bitmap_scaler_readline( BITMAP_SCALER * scaler, void * buf, int line );

int bitmap_scaler_deinit( BITMAP_SCALER * scaler );

#ifdef __cplusplus
}
#endif

#endif
