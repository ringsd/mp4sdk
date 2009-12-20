/***************************************************************************

		libbmf.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-12-20 20:30:37

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/
#ifndef _ANSI_BMF_H
#define _ANSI_BMF_H

#define FONTNAME_LEN 50         
#define FONTNAME_MAXLEN 200
#define DISPSTRING_MAXLEN 200

#define GB_FIRSTCHARMIN 0xA1
#define GB_FIRSTCHARMAX 0xF7
#define GB_LASTCHARMIN 0xA1
#define GB_LASTCHARMAX 0xFE
#define GB_QU (GB_FIRSTCHARMAX - GB_FIRSTCHARMIN + 1)
#define GB_WEI (GB_LASTCHARMAX - GB_LASTCHARMIN + 1)
#define GB_NUM (GB_QU * GB_WEI)

#define GBK_FIRSTCHARMIN 0x81
#define GBK_FIRSTCHARMAX 0xFE
#define GBK_LASTCHARMIN 0x40
#define GBK_LASTCHARMAX 0x7E
#define GBK_LASTCHARMIN2 0x80
#define GBK_LASTCHARMAX2 0xFE
#define GBK_QU (GBK_FIRSTCHARMAX - GBK_FIRSTCHARMIN + 1)
#define GBK_WEI (GBK_LASTCHARMAX - GBK_LASTCHARMIN + 1 + GBK_LASTCHARMAX2 - GBK_LASTCHARMIN2 + 1)
#define GBK_NUM (GBK_QU * GBK_WEI)

#define BIG5_FIRSTCHARMIN 0xA1
#define BIG5_FIRSTCHARMAX 0xF9
#define BIG5_LASTCHARMIN 0x40
#define BIG5_LASTCHARMAX 0x7E
#define BIG5_LASTCHARMIN2 0xA1
#define BIG5_LASTCHARMAX2 0xFE
#define BIG5_QU (BIG5_FIRSTCHARMAX - BIG5_FIRSTCHARMIN + 1)
#define BIG5_WEI (BIG5_LASTCHARMAX - BIG5_LASTCHARMIN + 1 + BIG5_LASTCHARMAX2 - BIG5_LASTCHARMIN2 + 1)
#define BIG5_NUM (BIG5_QU * BIG5_WEI)

#define HANGUL_FIRSTCHARMIN 0x81
#define HANGUL_FIRSTCHARMAX 0xFE
#define HANGUL_LASTCHARMIN 0x40
#define HANGUL_LASTCHARMAX 0xFF
#define HANGUL_QU (HANGUL_FIRSTCHARMAX - HANGUL_FIRSTCHARMIN + 1)
#define HANGUL_WEI (HANGUL_LASTCHARMAX - HANGUL_LASTCHARMIN + 1)
#define HANGUL_NUM (HANGUL_QU * HANGUL_WEI)

#define JIS_FIRSTCHARMIN 0x81
#define JIS_FIRSTCHARMAX 0x9F
#define JIS_FIRSTCHARMIN2 0xE0
#define JIS_FIRSTCHARMAX2 0xFC
#define JIS_LASTCHARMIN 0x40
#define JIS_LASTCHARMAX 0xff
#define JIS_QU (JIS_FIRSTCHARMAX-JIS_FIRSTCHARMIN+1+JIS_FIRSTCHARMAX2-JIS_FIRSTCHARMIN2+1)
#define JIS_WEI (JIS_LASTCHARMAX-JIS_LASTCHARMIN+1)
#define JIS_NUM (JIS_QU * JIS_WEI)

#ifndef U8
#define U8 unsigned char
#endif

#ifndef U16
#define U16 unsigned short
#endif

#ifndef U32
#define U32 unsigned long
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef NULL
#define NULL (void *)0
#endif


#endif

