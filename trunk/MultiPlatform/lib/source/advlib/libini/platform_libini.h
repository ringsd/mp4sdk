/***************************************************************************

		platform_libini.h

		INI文件读写.

		TIME LIST:
		CREATE	Rainy	2009-05-01 21:07:04

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _platform_libini_h_
#define _platform_libini_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct INI_FILE INI_FILE;

int ini_file_test( void );

//INI文件打开方式
typedef enum INI_OPEN_MODE{	
	INI_OPEN_MODE_READ,			//只读打开,文件不存在会返回错误码
	INI_OPEN_MODE_WRITE,		//覆写打开,文件不存在会自动创建,且原内容被情空
	INI_OPEN_MODE_RW,			//读写打开,文件不存在会自动创建,保留原内容
	INI_OPEN_MODE_END
}INI_OPEN_MODE;

//以下的INI函数,返回值为0表示成功,其他值为失败
//section:         INI段名称
//key:             INI键名称
//val:             INI键值

//打开一个INI文件
INI_FILE * ini_file_open( const char * path, INI_OPEN_MODE mode );
//关闭一个INI文件
int ini_file_close( INI_FILE * ini_file );
//读取INI文件中的一个字符串
int ini_file_read_string( INI_FILE * ini_file, const char * section, const char * key, char * val, int maxlen );
//读取INI文件中的int数据
int ini_file_read_int( INI_FILE * ini_file, const char * section, const char * key, int * val );
//读取INI文件中的unsigned int数据
int ini_file_read_uint( INI_FILE * ini_file, const char * section, const char * key, int * val );
//读取INI文件中的double数据
int ini_file_read_double( INI_FILE * ini_file, const char * section, const char * key, double * val );
//写INI文件字符串数据
int ini_file_write_string( INI_FILE * ini_file, const char * section, const char * key, const char * val );
//写INI文件int数据
int ini_file_write_int( INI_FILE * ini_file, const char * section, const char * key, int val );
//写INI文件unsigned int数据
int ini_file_write_uint( INI_FILE * ini_file, const char * section, const char * key, unsigned int val );
//写INI文件double数据
int ini_file_write_double( INI_FILE * ini_file, const char * section, const char * key, double val );

#ifdef __cplusplus
}
#endif

#endif
