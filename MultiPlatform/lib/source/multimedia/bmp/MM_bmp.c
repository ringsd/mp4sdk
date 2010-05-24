/***************************************************************************

		MM_bmp.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-25 20:52:13

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <global.h>
#include "MM_bmp.h"

enum{
BI_RGB = 0,
BI_RLE8 = 1,
BI_RLE4 = 2,
BI_BITFIELDS = 3,
};
    
struct MM_IMAGE_BMP{
	FILE * bmp_file;
	int width;
	int height;
	int bit_count;
	int data_offset;
	int line_invert;	//行序翻转 0 - 不翻转 , 1-翻转
	u32 r_field;		//色彩掩码
	u32 g_field;
	u32 b_field;
	u32 * palette;
	void * line_buffer;
};

typedef struct xtagBITMAPINFOHEADER{
	s32  biSize;
	s32  biWidth;
	s32  biHeight;
	s16  biPlanes;
	s16  biBitCount;
	s32  biCompression;
	s32  biSizeImage;
	s32  biXPelsPerMeter;
	s32  biYPelsPerMeter;
	s32  biClrUsed;
	s32  biClrImportant;
} xBITMAPINFOHEADER;

typedef struct xtagBITMAPFILEHEADER{
  s16 Align;	//对齐需要
  s16 bfType;
  s32 bfSize;
  s16 bfReserved1;
  s16 bfReserved2;
  s32 bfOffBits;
} xBITMAPFILEHEADER;

typedef struct xtagBITMAPINFO { 
  xBITMAPINFOHEADER bmiHeader; 
  u32          bmiColors[256]; 
} xBITMAPINFO;

MM_IMAGE_BMP * MM_bmp_open( u8 *path )
{
	MM_IMAGE_BMP * bmp = NULL;
	xBITMAPINFO tbinfo;
	xBITMAPFILEHEADER tbfh;
	
	#define MAX_HEAD 0x100
	u8 * head = malloc( MAX_HEAD );
	
	if( (bmp = malloc( sizeof( MM_IMAGE_BMP ) )) == NULL )
		goto err;
	if( (bmp->bmp_file = fopen( path, "rb" )) == NULL )
		goto err;
		
	fread( ((char *)&tbfh)+2, sizeof(xBITMAPFILEHEADER)-2, 1, bmp->bmp_file );
	fread( &tbinfo, sizeof(xBITMAPINFO), 1, bmp->bmp_file );
	
	printf( "%d %d %d\n", tbinfo.bmiHeader.biWidth, tbinfo.bmiHeader.biHeight, tbinfo.bmiHeader.biBitCount );
	
	bmp->width = tbinfo.bmiHeader.biWidth;
	if( tbinfo.bmiHeader.biHeight > 0 )
	{
		bmp->line_invert = 0;
		bmp->height = tbinfo.bmiHeader.biHeight;
	}
	else
	{
		bmp->line_invert = 1;
		bmp->height = -tbinfo.bmiHeader.biHeight;
	}
	
	bmp->bit_count = tbinfo.bmiHeader.biBitCount;
	bmp->data_offset = tbfh.bfOffBits;
	bmp->line_buffer = malloc( bmp->bit_count * bmp->width / 8 );
	bmp->palette = NULL;
	
	switch( bmp->bit_count )
	{
		case 1:
			bmp->palette = malloc( 2 * sizeof( u32 ) );
			fseek( bmp->bmp_file, sizeof(xBITMAPFILEHEADER)+sizeof(xBITMAPINFOHEADER)-2, SEEK_SET );
			fread( bmp->palette, 1, 2 * sizeof( u32 ), bmp->bmp_file );
			break;
		case 4:
			bmp->palette = malloc( 16 * sizeof( u32 ) );
			fseek( bmp->bmp_file, sizeof(xBITMAPFILEHEADER)+sizeof(xBITMAPINFOHEADER)-2, SEEK_SET );
			fread( bmp->palette, 1, 16 * sizeof( u32 ), bmp->bmp_file );
			break;
		case 8:
			bmp->palette = malloc( 256 * sizeof( u32 ) );
			fseek( bmp->bmp_file, sizeof(xBITMAPFILEHEADER)+sizeof(xBITMAPINFOHEADER)-2, SEEK_SET );
			fread( bmp->palette, 1, 256 * sizeof( u32 ), bmp->bmp_file );
			break;
		case 16:
			if( tbinfo.bmiHeader.biCompression == BI_RGB )
			{
				bmp->r_field = 0x7C00;
				bmp->g_field = 0x03E0;
				bmp->b_field = 0x001F;
			}
			else if( tbinfo.bmiHeader.biCompression == BI_BITFIELDS )
			{
				bmp->r_field = tbinfo.bmiColors[0];
				bmp->g_field = tbinfo.bmiColors[1];
				bmp->b_field = tbinfo.bmiColors[2];
			}
			break;
		case 24:
			bmp->r_field = 0xFF0000;
			bmp->g_field = 0x00FF00;
			bmp->b_field = 0x0000FF;
			break;
		case 32:
			if( tbinfo.bmiHeader.biCompression == BI_RGB )
			{
				bmp->r_field = 0xFF0000;
				bmp->g_field = 0x00FF00;
				bmp->b_field = 0x0000FF;
			}
			else if( tbinfo.bmiHeader.biCompression == BI_BITFIELDS )
			{
				bmp->r_field = *(u32 *)&tbinfo.bmiColors[0];
				bmp->g_field = *(u32 *)&tbinfo.bmiColors[1];
				bmp->b_field = *(u32 *)&tbinfo.bmiColors[2];
			}
			break;
	}
	free( head );
	return bmp;
err:
	if( bmp )
		free( bmp );
	return NULL;
}

//获取图片大小
//返回非0--读取失败, 返回0--成功
int MM_bmp_size( MM_IMAGE_BMP * bmp, int * width, int * height  )
{
	*width = bmp->width;
	*height = bmp->height;
	return 0;
}

static int calc_offset( u32 dst_field, u32 src_field )
{
	int i = 0, j = 0;
	while( (dst_field >> i) != 0 ) i++;
	while( (src_field >> j) != 0 ) j++;
	return i - j;
}

static void read_32bit( MM_IMAGE_BMP * bmp, u32 * buf, int scanline )
{
	int i;
	FILE *fp = bmp->bmp_file;
	u32 * src = bmp->line_buffer;
	int offset;
	u32 mask[3];
	int move[3];
	
	mask[2] = bmp->r_field;
	mask[1] = bmp->g_field;
	mask[0] = bmp->b_field;
	
	move[2] = calc_offset( 0xFF0000, bmp->r_field );
	move[1] = calc_offset( 0x00FF00, bmp->g_field );
	move[0] = calc_offset( 0x0000FF, bmp->b_field );
	
	if( bmp->line_invert )
		offset = bmp->data_offset + scanline*((bmp->width*4+3)&~0x3);
	else
		offset = bmp->data_offset + (bmp->height-1-scanline)*((bmp->width*4+3)&~0x3);
	
	fseek( fp, offset, SEEK_SET );
	fread( src, 1, bmp->width*4, fp );
	
	for( i = 0; i < bmp->width; i++ )
	{
		u32 pal = *src;
		u32 color = 0;
		int j;
		for( j = 0; j < 3; j++ )
		{
			if( move[j] >= 0 )
				color |= ((pal&mask[j]) << move[j] );
			else
				color |= ((pal&mask[j]) >> (-move[j]) );
		}
		*buf = color;
		buf++;
		src++;
	}
}

static void read_24bit( MM_IMAGE_BMP * bmp, u32 * buf, int scanline )
{
	int i;
	FILE *fp = bmp->bmp_file;
	u8 * src = bmp->line_buffer;
	int offset;
	
	if( bmp->line_invert )
		offset = bmp->data_offset + (scanline*((bmp->width*3+3)&~0x3) );
	else
		offset = bmp->data_offset + ((bmp->height-1-scanline)*((bmp->width*3+3)&~0x3) );
	
	fseek( fp, offset, SEEK_SET );
	fread( src, 1, bmp->width*3, fp );
	for( i = 0; i < bmp->width; i++ )
	{
		*buf = src[0] | (src[1]<<8) | (src[2]<<16);
		buf++;
		src+=3;
	}
}

static void read_16bit( MM_IMAGE_BMP * bmp, u32 * buf, int scanline )
{
	int i;
	FILE *fp = bmp->bmp_file;
	u16 * src = bmp->line_buffer;
	int offset;
	u32 mask[3];
	int move[3];
	
	mask[2] = bmp->r_field;
	mask[1] = bmp->g_field;
	mask[0] = bmp->b_field;
	
	move[2] = calc_offset( 0xFF0000, bmp->r_field );
	move[1] = calc_offset( 0x00FF00, bmp->g_field );
	move[0] = calc_offset( 0x0000FF, bmp->b_field );
	
	if( bmp->line_invert )
		offset = bmp->data_offset + scanline*((bmp->width*2+3)&~0x3);
	else
		offset = bmp->data_offset + (bmp->height-1-scanline)*((bmp->width*2+3)&~0x3);
	
	fseek( fp, offset, SEEK_SET );
	fread( src, 1, bmp->width*2, fp );
	
	for( i = 0; i < bmp->width; i++ )
	{
		u32 pal = *src;
		u32 color = 0;
		int j;
		for( j = 0; j < 3; j++ )
		{
			if( move[j] >= 0 )
				color |= ((pal&mask[j]) << move[j] );
			else
				color |= ((pal&mask[j]) >> (-move[j]) );
		}
		*buf = color;
		buf++;
		src++;
	}
}

static void read_8bit( MM_IMAGE_BMP * bmp, u32 * buf, int scanline )
{
	int i;
	FILE *fp = bmp->bmp_file;
	u32 * src = bmp->line_buffer;
	int offset;
	u32 * palette = bmp->palette;
	
	if( bmp->line_invert )
		offset = bmp->data_offset + (scanline*((bmp->width+3)&~0x3) );
	else
		offset = bmp->data_offset + ((bmp->height-1-scanline)*((bmp->width+3)&~0x3) );
	
	fseek( fp, offset, SEEK_SET );
	fread( src, 1, bmp->width, fp );
	
	for( i = 0; i < bmp->width; )
	{
		int j;
		u32 col32 = *src;
		for( j = 0; j < 4; j++ )
		{
			*buf = palette[ col32 & 0xFF ];
			buf++;
			i++;
			if( i >= bmp->width )
				break;
			col32 >>= 8;
		}
		src++;
	}
	
}

static void read_4bit( MM_IMAGE_BMP * bmp, u32 * buf, int scanline )
{
	int i;
	FILE *fp = bmp->bmp_file;
	u32 * src = bmp->line_buffer;
	int offset;
	u32 * palette = bmp->palette;
	
	if( bmp->line_invert )
		offset = bmp->data_offset + scanline*((bmp->width+7)&~0x7)/2;
	else
		offset = bmp->data_offset + (bmp->height-1-scanline)*((bmp->width+7)&~0x7)/2;

	fseek( fp, offset, SEEK_SET );
	fread( src, 1, bmp->width/2, fp );
	
	for( i = 0; i < bmp->width; )
	{
		int j;
		u32 col32 = *src;
		for( j = 0; j < 8; j+=2 )
		{
			buf[0] = palette[ (col32>>4) & 0xF ];
			i++;
			if( i >= bmp->width )
				break;
			
			buf[1] = palette[ col32 & 0xF ];
			i++;
			if( i >= bmp->width )
				break;
				
			buf += 2;
			col32 >>= 8;
		}
		src++;
	}
	
}

static void read_1bit( MM_IMAGE_BMP * bmp, u32 * buf, int scanline )
{
	int i;
	FILE *fp = bmp->bmp_file;
	u32 * src = bmp->line_buffer;
	int offset;
	u32 * palette = bmp->palette;
	
	if( bmp->line_invert )
		offset = bmp->data_offset + scanline*((bmp->width+31)&~0x1F)/8;
	else
		offset = bmp->data_offset + (bmp->height-1-scanline)*((bmp->width+31)&~0x1F)/8;
	
	fseek( fp, offset, SEEK_SET );
	fread( src, 1, bmp->width/8, fp );
	
	for( i = 0; i < bmp->width; )
	{
		int j;
		u32 col32 = *src;
		for( j = 0; j < 32; j++ )
		{
			*buf = palette[ (col32>>((j&0x18)+7-(j&0x7))) & 1 ];
			buf++;
			i++;
			if( i >= bmp->width )
				break;
		}
		src++;
	}
	
}

int 		MM_bmp_readline	( MM_IMAGE_BMP * bmp, void *buf, int scanline )
{
	if( scanline >= bmp->height )
		return -1;
	switch( bmp->bit_count )
	{
		case 1:
			read_1bit( bmp, buf, scanline );
			break;
		case 4:
			read_4bit( bmp, buf, scanline );
			break;
		case 8:
			read_8bit( bmp, buf, scanline );
			break;
		case 16:
			read_16bit( bmp, buf, scanline );
			break;
		case 24:
			read_24bit( bmp, buf, scanline );
			break;
		case 32:
			read_32bit( bmp, buf, scanline );
			break;
	}
	return 0;
}

//返回-1--读取失败, 返回0--成功
int MM_bmp_read( MM_IMAGE_BMP * bmp, void *buf, int width, int height )
{
	return 0;
}

//关闭bmp文件,返回值0:成功 -1失败
void MM_bmp_close( MM_IMAGE_BMP *bmp )
{
	if( bmp->palette )
		free( bmp->palette );
	fclose( bmp->bmp_file );
	free( bmp->line_buffer );
	free( bmp );
}

