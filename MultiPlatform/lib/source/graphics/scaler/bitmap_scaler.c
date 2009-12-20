/***************************************************************************

		bitmap_scaler.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-11-29 17:51:02

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/

#include <global.h>
#include "bitmap_scaler.h"
#include "bitmap_scaler_2line.h"
#include "scale_soft.h"

struct BITMAP_SCALER{
    int sw;
    int sh;
    int dw;
    int dh;
    int divide;
    void * param;
    int (*readline)( int line, void * buffer, void * param );
    BITMAP_2LINE * bitmap_2line;
};

BITMAP_SCALER * bitmap_scaler_init
( int sw, int sh, int dw, int dh, 
int (*readline)( int line, void * buffer, void * param ), 
void * param )
{
    BITMAP_SCALER * scaler;
    scaler = malloc( sizeof(BITMAP_SCALER) );
    if( scaler == NULL )
        goto err;
    scaler->sw = sw;
    scaler->sh = sh;
    scaler->dw = dw;
    scaler->dh = dh;
    scaler->divide = sw/dw;
    if( scaler->divide < 1 )    // 预缩放<1, 则为放大模式
        scaler->divide = 1;
    scaler->param = param;
    scaler->readline = readline;
    scaler->bitmap_2line = bitmap_2line_init( sw, sh, scaler->divide, readline, param );
    return scaler;
 err:
    return NULL;
}

int bitmap_scaler_readline( BITMAP_SCALER * scaler, void * buf, int line )
{
    int prescl_w = scaler->sw/scaler->divide;
    int prescl_h = scaler->sh/scaler->divide;
    if( scaler->sw >= scaler->dw )
        rgb32_rgb32_scale_line( buf, bitmap_2line_getline( scaler->bitmap_2line, line * prescl_h / scaler->dh ),
		                    prescl_w, scaler->dw, prescl_h, scaler->dh, line );
    else
        rgb32_rgb32_blowup_line( buf, bitmap_2line_getline( scaler->bitmap_2line, line * prescl_h / scaler->dh ),
		                    prescl_w, scaler->dw, prescl_h, scaler->dh, line );
    return 0;
}

int bitmap_scaler_deinit( BITMAP_SCALER * scaler )
{
    bitmap_2line_deinit( scaler->bitmap_2line );
    if( scaler == NULL )
        goto err;
    free( scaler );
    return 0;
err:
    return -1;
}



