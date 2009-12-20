/***************************************************************************

		ccbmf.c

		ChinaChip(TM) BMF format.

		TIME LIST:
		CREATE	Rainy	2009-08-03 13:46:28

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>
#include "libbmf.h"
#include "bmfont.h"
#include "ccbmf.h"

struct CCBMF{
	FILE * fp;
	FILE * ascfp;
	u8 * data_buf;
	XFONT_NLS * nls;
	BMFONT hdrbmf;
	BMFONT aschdrbmf;
};


/*------------------------------------------------------------------------
 函数:  gbkpos
 功能:  通过 ASCII 码计算双字节内码
 参数:  unsigned char c1, c2: 国标码的两个字节
 输出:  双字节内码。字库顺序。
 作者:  vrix.yan
 日期:  2008-01-24 17:12
------------------------------------------------------------------------*/
static int gbkpos(unsigned char c1, unsigned char c2)
{
    int pos;

    pos = (c1 - GBK_FIRSTCHARMIN) * GBK_WEI;

    if (c2 >= GBK_LASTCHARMIN2)
        pos += c2 - GBK_LASTCHARMIN2 + GBK_LASTCHARMAX - GBK_LASTCHARMIN + 1;
    else
        pos += c2 - GBK_LASTCHARMIN;

    return pos;
}

/*------------------------------------------------------------------------
 函数:  gbpos
 功能:  unsigned char c1, c2: 国标码的两个字节
 参数:  c1,c2 是GB 内码
 输出:  双字节内码。字库顺序。
 作者:  vrix.yan
 日期:  2008-01-24 17:12
------------------------------------------------------------------------*/
static int gbpos(unsigned char c1, unsigned char c2)
{
    int pos;

    pos = (c1 - GB_FIRSTCHARMIN) * GB_WEI;
    pos += c2 - GB_LASTCHARMIN;

    return pos;
}

/*------------------------------------------------------------------------
 函数:  big5pos
 功能:  unsigned char c1, c2: big5本地码的两个字节
 参数:  c1,c2 是BIG5 内码
 输出:  双字节内码。字库顺序。
 作者:  vrix.yan
 日期:  2008-01-24 17:12
------------------------------------------------------------------------*/
static int big5pos(unsigned char c1, unsigned char c2)
{
    int pos;

    pos = (c1 - BIG5_FIRSTCHARMIN) * BIG5_WEI;
    if (c2 >= BIG5_LASTCHARMIN2)
        pos += c2 - BIG5_LASTCHARMIN2 + BIG5_LASTCHARMAX - BIG5_LASTCHARMIN + 1;
    else
        pos += c2 - BIG5_LASTCHARMIN;

    return pos;
}

/*------------------------------------------------------------------------
 函数:  cp949pos
 功能:  unsigned char c1, c2: hangul 本地码的两个字节
 参数:  c1,c2 是hangul 内码
 输出:  双字节内码。字库顺序。
 作者:  vrix.yan
 日期:  2008-01-28 17:25
------------------------------------------------------------------------*/
static int cp949pos(unsigned char c1, unsigned char c2)
{
    int pos;

    pos = (c1 - HANGUL_FIRSTCHARMIN) * HANGUL_WEI;
    pos += (c2 - HANGUL_LASTCHARMIN);

    return pos;
}

/*------------------------------------------------------------------------
 函数:  cp932pos
 功能:  unsigned char c1, c2: shiftjis 本地码的两个字节
 参数:  c1,c2 是 shiftjis 内码
 输出:  双字节内码。字库顺序。
 作者:  vrix.yan
 日期:  2008-01-28 17:25
------------------------------------------------------------------------*/
static int cp932pos(unsigned char c1, unsigned char c2)
{
    int pos;

    if (c1<=JIS_FIRSTCHARMAX)
    {
        pos = (c1 - JIS_FIRSTCHARMIN) * JIS_WEI;
    }
    if ((c1>=0xa1)&&(c1<=0xdf))
    {
        pos = (JIS_FIRSTCHARMAX-JIS_FIRSTCHARMIN+1) * JIS_WEI;
        pos+=(c1-0xa1);
        return pos;
    }
    else if (c1>=JIS_FIRSTCHARMIN2)
    {
        pos = (c1 - JIS_FIRSTCHARMIN2)+(JIS_FIRSTCHARMAX-JIS_FIRSTCHARMIN+1)*JIS_WEI+(0xdf-0xa1+1);
    }
    pos+=(c2-JIS_LASTCHARMIN);
    return pos;
}

static int strserch( const char * src, const char * str )
{
	while( *src )
	{
		const char * p = src;
		if( *p == *str )
		{
			const char *q = str+1;
			p++;
			while( *q )
			{
				if( *q != *p )
				{
					goto _skip;
				}
				q++;
				p++;
			}
			return 1;
		}
_skip:
		src++;
	}
	return 0;
}

/* 取出最合适的ascii字体 */
static const char * ascfiledir( const char * path )
{
	static char new_path[MAX_PATH];
	char * p;
	strcpy( new_path, path );
	p = strrchr( new_path, '\\' );
	if( !p )
		p = strrchr( new_path, '/' );
	if( !p )
		return "ASC_16x8.bmf";
	p[1] = 0;
	if( strserch( path, "_16" ) )
	{
		strcat( new_path, "ASC_16x8.bmf" );
	}
	else if( strserch( path, "_20" ) )
	{
		strcat( new_path, "ASC_20x10.bmf" );
	}
	return new_path;
}

static XFONT_CODE_PAGE get_xfont_copepage( int bmf_type )
{
	XFONT_CODE_PAGE codepage = XFONT_CP_DEFAULT;
	switch( bmf_type )
	{
		case BMF_CP437:
			codepage = XFONT_CP_437;
			break;
		case BMF_GB2312:
			codepage = XFONT_CP_936;
			break;
		case BMF_CP936:
		case BMF_GB18030:
			codepage = XFONT_CP_936;
			break;
		case BMF_CP950:
			codepage = XFONT_CP_950;
			break;
		case BMF_CP932:
			codepage = XFONT_CP_932;
			break;
		case BMF_CP949:
			codepage = XFONT_CP_949;
			break;
	}
	return codepage;
}

CCBMF * ccbmf_open( const char * path )
{
	CCBMF * bmf = malloc( sizeof(CCBMF) );
	if( !bmf )
		goto err;
	bmf->data_buf = NULL;
	bmf->ascfp = NULL;
	bmf->fp = fopen( path, "rb" );
	if( !bmf->fp )
		goto err;
	fread( &bmf->hdrbmf, sizeof(BMFONT), 1, bmf->fp );
	bmf->ascfp = fopen( ascfiledir(path), "rb" );
	if( !bmf->ascfp )
		goto err;
	fread( &bmf->aschdrbmf, sizeof(BMFONT), 1, bmf->ascfp );
	bmf->data_buf = malloc( max(bmf->hdrbmf.bpchar,bmf->aschdrbmf.bpchar) );
	if( !bmf->data_buf )
		goto err;
	bmf->nls = xfont_nls_open(get_xfont_copepage(bmf->hdrbmf.type));
	return bmf;
err:
	if( bmf )
	{
		if( bmf->fp )
			fclose( bmf->fp );
		if( bmf->ascfp )
			fclose( bmf->ascfp );
		if( bmf->data_buf )
			free( bmf->data_buf );
		free( bmf );
	}
	return NULL;
}

static u8 * get_offset( CCBMF * bmf, int code, int * width, int * height, int * bpline )
{
	int offset;
	int c1 = (code)&0xFF;
	int c2 = (code>>8)&0xFF;
	FILE * fbmf;
	BMFONT * hdrbmf;
	if( c2 == 0 )
	{
		hdrbmf = &bmf->aschdrbmf;
		fbmf = bmf->ascfp;
	}
	else
	{
		hdrbmf = &bmf->hdrbmf;
		fbmf = bmf->fp;
	}
	switch( hdrbmf->type )
	{
		case BMF_CP437:
			offset = code;
			break;
		case BMF_GB2312:
			offset = gbpos(c1,c2);
			break;
		case BMF_CP936:
		case BMF_GB18030:
			offset = gbkpos(c1,c2);
			break;
		case BMF_CP950:
			offset = big5pos(c1,c2);
			break;
		case BMF_CP932:
			offset = cp932pos(c1,c2);
			break;
		case BMF_CP949:
			offset = cp949pos(c1,c2);
			break;
	}
	if( hdrbmf->width != 0 )
	{
		int foff = offset*hdrbmf->bpchar+sizeof(BMFONT);
		*width = hdrbmf->width;
		*height = hdrbmf->height;
		*bpline = hdrbmf->bpline;
		fseek( fbmf, foff, SEEK_SET );
		fread( bmf->data_buf, hdrbmf->bpchar, 1, fbmf );
		return bmf->data_buf;
	}
	else
	{
		int foff = offset*hdrbmf->bpchar+sizeof(BMFONT);
		fseek( fbmf, foff, SEEK_SET );
		fread( bmf->data_buf, hdrbmf->bpchar, 1, fbmf );
		*width = bmf->data_buf[0];
		*height = bmf->data_buf[1];
		*bpline = hdrbmf->bpline;
		return bmf->data_buf+2;
	}
}

int ccbmf_read( CCBMF * bmf, int code, int size, int style, u8 * buf, int limit, XFONT_INFO * info )
{
	int width, height, bpline;
	int x, y;
	u8 * bmf_buf = get_offset( bmf, 
		xfont_nls_u2a(bmf->nls,code), &width, &height, &bpline );
	u8 * dst = buf;
	for( y = 0; y < height; y++ )
	{
		u8 * src = bmf_buf + y * bpline;
		x = 0;
		while( x < width )
		{
			if( x + 8 < width )
			{
				u8 data = *src;
				dst[7] = data&1 ? 0xFF:0x00;
				dst[6] = (data>>=1)&1 ? 0xFF:0x00;
				dst[5] = (data>>=1)&1 ? 0xFF:0x00;
				dst[4] = (data>>=1)&1 ? 0xFF:0x00;
				dst[3] = (data>>=1)&1 ? 0xFF:0x00;
				dst[2] = (data>>=1)&1 ? 0xFF:0x00;
				dst[1] = (data>>=1)&1 ? 0xFF:0x00;
				dst[0] = (data>>=1)&1 ? 0xFF:0x00;
				dst += 8;
				src++;
				x += 8;
			}
			else
			{
				u8 data = *src;
				int i = 7;
				while( x < width )
				{
					*dst = (data >> i)&1 ? 0xFF:0x00;
					i--;
					dst ++;
					x ++; 
				}
			}
		}
	}
	info->font_width = width;
	info->bitmap_height = height;
	info->bitmap_top = height;
	info->bitmap_left = 0;
	info->bitmap_width = width;
	return 0;
}

int ccbmf_getsize( CCBMF * bmf, int code, int size, int style, XFONT_INFO * info )
{
	int width, height, bpline;
	get_offset( bmf, code, &width, &height, &bpline );
	info->font_width = width;
	info->bitmap_height = height;
	info->bitmap_top = height;
	info->bitmap_left = 0;
	info->bitmap_width = width;
	return 0;
}

const char * ccbmf_name( CCBMF * bmf )
{
	return bmf->hdrbmf.name;
}

void ccbmf_close( CCBMF * bmf )
{
	fclose( bmf->fp );
	fclose( bmf->ascfp );
	xfont_nls_close(bmf->nls);
	free( bmf->data_buf );
	free( bmf );
}


