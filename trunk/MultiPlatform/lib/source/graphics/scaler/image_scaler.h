/***************************************************************************

		image_scaler.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-11-30 20:36:43

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/
#ifndef _image_scaler_h_
#define _image_scaler_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct IMAGE_SCALER IMAGE_SCALER;

IMAGE_SCALER * image_scaler_init( const char * path, int dw, int dh );

int image_scaler_readline( IMAGE_SCALER * iscaler, void * buf, int scanline );

int image_scaler_deinit( IMAGE_SCALER * iscaler );

#ifdef __cplusplus
}
#endif

#endif
