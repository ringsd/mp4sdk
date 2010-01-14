/***************************************************************************

		MM_Image.c

		图像媒体总管理模块.
		提供对所有图像文件的统一访问接口.
		根据文件扩展名决定调用相应模块

		TIME LIST:
		CREATE	Rainy	2009-02-28 19:07:52

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <global.h>
#include "MM_Image.h"
#include "MM_bmp.h"
#include "MM_jpg.h"
#include "MM_ico.h"
#include "MM_png.h"

struct MM_IMAGE{
	void * 		image_spec;
	void *		(* MM_image_open)	( u8 *path );
	int			(* MM_image_read)	( void * image, void *buf, int width, int height );
	int			(* MM_image_write)	( void * image, void *buf, int width, int height );
	void		(* MM_image_close)	( void * image );
	int 		(* MM_image_size)	( void * image, int * width, int * height  );
	int 		(* MM_image_readline)	( void * image, void *buf, int scanline );
	int 		(* MM_image_is_random_read)	( void );
	char *		extname;
};

static int 		Enable	( void )
{
	return 1;
}

static int 		Disable	( void )
{
	return 0;
}

MM_IMAGE image_types[] = {
//		handle		open			read		write			close			size			readline		random		extname
	{	NULL,	MM_bmp_open,	NULL,	NULL,			MM_bmp_close,	MM_bmp_size,	MM_bmp_readline,	Enable,		"bmp"	},
	{	NULL,	MM_ico_open,	NULL,	NULL,			MM_ico_close,	NULL,			NULL,				Disable,	"ico"	},	
	{	NULL,	MM_jpg_open,	NULL,	MM_jpg_write,	MM_jpg_close,	MM_jpg_size,	MM_jpg_readline,	Disable,	"jpg"	},	
	{	NULL,	MM_png_open,	NULL,	MM_png_write,	MM_png_close,	MM_png_size,	MM_png_readline,	Disable,	"png"	},	
	{	NULL,	NULL,			NULL,			NULL,			NULL,			NULL,				NULL,		NULL	},//END Block
};

int 		MM_image_is_random_read	( MM_IMAGE * image )
{
	return image->MM_image_is_random_read();
}

MM_IMAGE *		MM_image_open	( const u8 *path )
{
	int i;
	u8 *dot = strrchr( path, '.' );
	if( dot == NULL )
		return NULL;
	dot++;
	i = 0;
	while( image_types[i].extname )
	{
		if( stricmp( dot, image_types[i].extname ) == 0 )
		{
			MM_IMAGE * image = malloc( sizeof(MM_IMAGE) );
			if( image )
			{
				image->MM_image_open	= image_types[i].MM_image_open;
				image->MM_image_read	= image_types[i].MM_image_read;
				image->MM_image_write	= image_types[i].MM_image_write;
				image->MM_image_close	= image_types[i].MM_image_close;
				image->MM_image_size	= image_types[i].MM_image_size;
				image->MM_image_readline= image_types[i].MM_image_readline;
				image->MM_image_is_random_read = image_types[i].MM_image_is_random_read;
				image->image_spec	= image->MM_image_open( path );
				if( image->image_spec == NULL )
				{
					free(image);
					image = NULL;
				}
			}
			return image;
		}
		i++;
	}
	return NULL;
}

int			MM_image_read	( MM_IMAGE * image, void *buf, int width, int height )
{
	if( image->MM_image_read == NULL )
	{
	    return image_scaler_read( image, buf, width, height );
    }
	return image->MM_image_read( image->image_spec, buf, width, height );
}

int			MM_image_write	( MM_IMAGE * image, void *buf, int width, int height )
{
	if( image->MM_image_write == NULL )
		return -1;
	return image->MM_image_write( image->image_spec, buf, width, height );
}

void		MM_image_close	( MM_IMAGE * image )
{
	image->MM_image_close( image->image_spec );
	free( image );
}

int 		MM_image_size	( MM_IMAGE * image, int * width, int * height  )
{
	if( image->MM_image_size == NULL )
		return -1;
	return image->MM_image_size( image->image_spec, width, height );
}

int 		MM_image_readline	( MM_IMAGE * image, void *buf, int scanline )
{
	if( image->MM_image_readline == NULL )
		return -1;
	return image->MM_image_readline	( image->image_spec, buf, scanline );
}



