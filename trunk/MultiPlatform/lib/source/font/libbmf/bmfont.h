/***************************************************************************

		bmfont.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-12-20 20:36:26

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/
#ifndef _bmfont_h_
#define _bmfont_h_

#ifdef	__cplusplus
extern "C" {
#endif

#define BMF_NONE    0x0        
#define BMF_ASCII   0x0101    
#define BMF_CP437   BMF_ASCII
#define BMF_GB2312  0x0201
#define BMF_GBK     0x0202
#define BMF_CP936   BMF_GBK   
#define BMF_GB18030 0x0203
#define BMF_BIG5    0x0301
#define BMF_CP950   BMF_BIG5  
#define BMF_JIS     0x0401
#define BMF_CP932   BMF_JIS   
#define BMF_KSC     0x0501
#define BMF_CP949   BMF_KSC   
#define BMF_CJK     0x0601
#define BMF_UNICODE 0x0701
#define BMF_CP1250  0x0702  
#define BMF_CP1251  0x0703  
#define BMF_CP1252  0x0704  
#define BMF_CP1253  0x0705  
#define BMF_CP1254  0x0706  
#define BMF_CP1255  0x0707  
#define BMF_CP1256  0x0708  
#define BMF_CP1257  0x0709  

#define BMF_NAMESIZE 31

typedef struct {
	unsigned short magic; //  'BF'
	unsigned short ver;	// 0x0101
	unsigned short type;
	unsigned short count; 
	unsigned short width; 
	unsigned short height;
	unsigned short bpline;
	unsigned short bpchar;

	char name[BMF_NAMESIZE + 1];
	union {
		unsigned char *pdata;
		unsigned long ofdata;
	}data;
	unsigned long size;
	union {
		unsigned char *pextra;
		unsigned long ofextra;
	}extra;
	unsigned long unused;
} BMFONT, *PBMFONT;

#ifdef __cplusplus
}
#endif

#endif
