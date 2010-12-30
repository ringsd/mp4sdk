/***************************************************************************

		MM_jpg.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-28 14:11:02

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <global.h>
#include <jpeglib.h>
#include <setjmp.h>
#include "MM_jpg.h"

//#define RANDOM_READ_SUPPORTED

static jmp_buf mark;

METHODDEF(void)
MM_jpg_error_exit (j_common_ptr cinfo)
{
  /* Always display the message */
//  (*cinfo->err->output_message) (cinfo);	//这句话会让cc1600程序莫名奇妙的死掉

  /* Let the memory manager delete any temp files before we die */
//  jpeg_destroy(cinfo);
  longjmp( mark, -1 );
}

struct MM_IMAGE_JPG{
	FILE * fp;
	struct jpeg_decompress_struct * cinfo;
	struct jpeg_error_mgr * jerr;
#ifdef RANDOM_READ_SUPPORTED
	struct jpeg_decompress_struct * cinfos;
#endif
	JSAMPARRAY buffer;
	char filepath[MAX_PATH]; 
} ;

/* 检查jpeglib的初始化情况,根据需要初始化它
 * 返回0成功, -1失败
 */
static int jpeglib_check_init( MM_IMAGE_JPG * jpg )
{
	struct jpeg_decompress_struct * cinfo = jpg->cinfo;
	struct jpeg_error_mgr * jerr = jpg->jerr;
	
	if( cinfo == NULL && jerr == NULL )		//正常情况下这两个值一定是同时为NULL的
	{
		int jmpret;
		jmpret = setjmp( mark );
		if( jmpret == -1 )
			return -1;
		jpg->fp = fopen( jpg->filepath, "rb" );
		if( jpg->fp == NULL )
			return -1;
		jpg->cinfo = (cinfo = malloc( sizeof(struct jpeg_decompress_struct) ));
		jpg->jerr = (jerr = malloc( sizeof(struct jpeg_error_mgr) ));
		cinfo->err = jpeg_std_error (jerr);
		cinfo->err->error_exit = MM_jpg_error_exit;
		jpeg_create_decompress(cinfo);
		fseek( jpg->fp, 0, SEEK_SET );	//设置文件指针为0,否则可能会出错
		jpeg_stdio_src(cinfo, jpg->fp);
		(void) jpeg_read_header(cinfo, TRUE);
		(void) jpeg_start_decompress(cinfo);
#ifdef RANDOM_READ_SUPPORTED
		jpg->cinfos = malloc( cinfo->output_height * sizeof(struct jpeg_decompress_struct) );
#endif
	}
	return 0;
}


MM_IMAGE_JPG * MM_jpg_open( u8 *path )
{
	MM_IMAGE_JPG * jpg;
	jpg = malloc( sizeof(MM_IMAGE_JPG) );
	if( jpg == NULL )
		return NULL;
	strncpy( jpg->filepath, path, MAX_PATH );
	jpg->cinfo = NULL;
	jpg->jerr = NULL;
	jpg->buffer = NULL;
	jpg->fp = NULL;
	return jpg;
}

//获取图片大小
//返回非0--读取失败, 返回0--成功
int MM_jpg_size( MM_IMAGE_JPG * jpg, int * width, int * height  )
{
	if( jpeglib_check_init( jpg ) == -1 )
		return -1;
	*width = jpg->cinfo->output_width;
	*height = jpg->cinfo->output_height;
	return 0;
}

static void
transfer_rgb( u32 * out, JSAMPARRAY in, int width) {
  JSAMPROW inrow = *in;
  while (width--) {
    *out++ = inrow[2] | (inrow[1] << 8) | (inrow[0] << 16);
	inrow += 3;
  }
}

static void
transfer_gray( u32 * out, JSAMPARRAY in, int width) {
  JSAMPROW inrow = *in;
  while (width--) {
    *out = *inrow | (*inrow<<8) | (*inrow<<16);
    out++;
    inrow++;
  }
}

//内存节省模式,直接解压读取一行,返回非0--读取失败, 返回0--成功
int MM_jpg_readline( MM_IMAGE_JPG * jpg, void *buf, int scanline )
{
	struct jpeg_decompress_struct * cinfo;
	struct jpeg_error_mgr * jerr;
	int row_stride;
	
	if( jpeglib_check_init( jpg ) == -1 )
		return -1;
	
	if( scanline >= (int)jpg->cinfo->output_height )
		return -1;
	
	cinfo = jpg->cinfo;
	jerr = jpg->jerr;
	
	row_stride = cinfo->output_width * cinfo->output_components;
	if( jpg->buffer == NULL )
		jpg->buffer = (*(cinfo->mem)->alloc_sarray) ((j_common_ptr) cinfo, JPOOL_IMAGE, row_stride, 1);
#ifdef RANDOM_READ_SUPPORTED
	memcpy( &jpg->cinfos[scanline] ,cinfo, sizeof(struct jpeg_decompress_struct) );
#endif
	(void) jpeg_read_scanlines(cinfo, jpg->buffer, 1);
	switch (cinfo->out_color_space)
	{
	case JCS_RGB:
		transfer_rgb( buf, jpg->buffer, cinfo->output_width );
		break;
	case JCS_GRAYSCALE:
		transfer_gray( buf, jpg->buffer, cinfo->output_width );
		break;
	}
	return 0;
}

//返回非0--读取失败, 返回0--成功
int MM_jpg_read( MM_IMAGE_JPG * jpg, void *buf, int width, int height )
{
	int i;
	u32 * linebuf = buf;
	if( width != jpg->cinfo->output_width || 
		height != jpg->cinfo->output_height )
		return -1;
	for( i = 0; i < height; i++ )
	{
		if( MM_jpg_readline( jpg, linebuf, i ) != 0 )
			return -1;
		linebuf += width;
	}
	return 0;
}

static void RGB32toRGB24( char * dst, char * src, int width )
{
	while( width-- )
	{
		dst[0] = src[2];
		dst[1] = src[1];
		dst[2] = src[0];
		src += 4;
		dst += 3;
	}
}

/* 写BITMAP数据到JPG文件
 * 返回0 -- 成功, -1 -- 失败
 */
int MM_jpg_write( MM_IMAGE_JPG * jpg, void *buf, int width, int height )
{
	char * line_buffer;
	char * image_buffer = buf;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	/* More stuff */
	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);
	if ((outfile = fopen(jpg->filepath, "wb")) == NULL) {
		printf("can't open %s\n", jpg->filepath);
		return -1;
	}
	jpeg_stdio_dest(&cinfo, outfile);
	/* First we supply a description of the input image.
	 * Four fields of the cinfo struct must be filled in:
	 */
	cinfo.image_width = width; 	/* image width and height, in pixels */
	cinfo.image_height = height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 90, TRUE /* limit to baseline-JPEG values */);
	jpeg_start_compress(&cinfo, TRUE);
	row_stride = width * 3;	/* JSAMPLEs per row in image_buffer */
	line_buffer = malloc( width * 3 );
	while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
		RGB32toRGB24( line_buffer, image_buffer, width );
		row_pointer[0] = line_buffer;
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
		image_buffer += width * 4;
	}
	free( line_buffer );
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
	return 0;
}

//关闭jpg文件,返回值0:成功 -1失败
void MM_jpg_close( MM_IMAGE_JPG *jpg )
{
	if( jpg->cinfo )
	{
		jpeg_destroy_decompress(jpg->cinfo); 
		free( jpg->cinfo );
	}
	if( jpg->jerr )
	{
		free( jpg->jerr );
	}
#ifdef RANDOM_READ_SUPPORTED
	if( jpg->cinfos )
	{
		free( jpg->cinfos );
	}
#endif
	if( jpg->fp )
		fclose( jpg->fp );
	free( jpg );
}

