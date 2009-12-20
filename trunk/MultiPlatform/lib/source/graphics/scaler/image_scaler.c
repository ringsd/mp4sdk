/***************************************************************************

		image_scaler.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-11-30 20:36:39

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/

#include <global.h>
#include "image_scaler.h"
#include "bitmap_scaler.h"

#define ERRLOG printf

struct IMAGE_SCALER{
    MM_IMAGE * image;
    BITMAP_SCALER * bscaler;
};

static int readline( int line, void * buffer, IMAGE_SCALER * iscaler )
{
    MM_image_readline( iscaler->image, buffer, line );
//    printf( "%d\n", line );
    return 0;
}

IMAGE_SCALER * image_scaler_init( const char * path, int dw, int dh )
{
    IMAGE_SCALER * iscaler;
    BITMAP_SCALER * bscaler;
    MM_IMAGE * image;
    int width, height;
    
    iscaler = malloc( sizeof(IMAGE_SCALER) );
    if( iscaler == NULL )
        goto err1;
    
    image = MM_image_open( path );
    
    if( image == NULL )
        goto err2;
        
    MM_image_size( image, &width, &height );
    
    bscaler = bitmap_scaler_init
        ( width, height, dw, dh, 
        readline, 
        iscaler );
    
    if( bscaler == NULL )
        goto err3;
    
    iscaler->image = image;
    iscaler->bscaler = bscaler;
    return iscaler;
    
err3:
    ERRLOG( "bitmap scaler init error.\n" );
    MM_image_close( image );
err2:
    ERRLOG( "image open error.\n" );
    free( iscaler );
err1:
    return NULL;
}

int image_scaler_readline( IMAGE_SCALER * iscaler, void * buf, int scanline )
{
    return bitmap_scaler_readline( iscaler->bscaler, buf, scanline );
}

int image_scaler_deinit( IMAGE_SCALER * iscaler )
{
    bitmap_scaler_deinit( iscaler->bscaler );
    MM_image_close( iscaler->image );
    free( iscaler );
    return 0;
}



