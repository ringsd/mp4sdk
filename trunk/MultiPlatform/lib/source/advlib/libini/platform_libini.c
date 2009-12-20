/***************************************************************************

		platform_libini.c

		INI文件处理.

		TIME LIST:
		CREATE	Rainy	2009-05-01 21:07:00

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#define _platform_libini_h_
#include <global.h>
#undef _platform_libini_h_
#include "platform_libini.h"
#include <iniparser.h>

#define MAX_ASCIIS  1024

struct INI_FILE {
	dictionary * d;
	FILE * inifp;
	INI_OPEN_MODE mode;
};

INI_FILE * ini_file_open( const char * path, INI_OPEN_MODE mode )
{
	INI_FILE * ini_file;
	FILE * fp;
	
	if( (ini_file = malloc( sizeof(INI_FILE)) ) == NULL )
		goto err;
		
	memset( ini_file, 0, sizeof(INI_FILE) );
	ini_file->mode = mode;
	
	if( ini_file->mode == INI_OPEN_MODE_WRITE )
	{
		//不论文件是否存在,都创建
		fp = fopen( path, "wb" );
			if( fp == NULL )
				goto err;
			fclose( fp );
	}
	else if( ini_file->mode == INI_OPEN_MODE_RW )
	{	//判断文件是否存在,不存在则创建
		if( fp = fopen( path, "rb" ) )
		{
			fclose( fp );
		}
		else
		{
			fp = fopen( path, "wb" );
			if( fp == NULL )
				goto err;
			fclose( fp );
		}
	}

	if( (ini_file->d = iniparser_load( path )) == NULL )
		goto err;

	if( (ini_file->mode == INI_OPEN_MODE_WRITE) ||
		(ini_file->mode == INI_OPEN_MODE_RW) )
		if( (ini_file->inifp = fopen( path, "wb" )) == NULL )
			goto err;

	return ini_file;
	
err:

	if( ini_file )
	{
		if( ini_file->d )
			iniparser_freedict( ini_file->d );
		if( ini_file->inifp )
			fclose( ini_file->inifp );
		free( ini_file );
	}
	return NULL;
}

int ini_file_close( INI_FILE * ini_file )
{
	if( ini_file == NULL )
		goto err;
		
	if( ini_file->d && ini_file->inifp )
		iniparser_dump_ini( ini_file->d, ini_file->inifp );
		
	if( ini_file->d )
		iniparser_freedict( ini_file->d );
		
	if( ini_file->mode != INI_OPEN_MODE_READ )
		if( ini_file->inifp )
			fclose( ini_file->inifp );
			
	free( ini_file );
	return 0;
err:
	return -1;
}

int ini_file_read_string( INI_FILE * ini_file, const char * section, const char * key, char * val, int maxlen )
{
	char * string;
	char tmp[MAX_ASCIIS];
	sprintf(tmp, "%s:%s", section, key);
	if( ini_file == NULL )
		goto err;
	if( (string = iniparser_getstring( ini_file->d, tmp, NULL )) == NULL )
		goto err;
	strncpy( val, string, maxlen );
	return 0;
err:
	return -1;
}

int ini_file_read_int( INI_FILE * ini_file, const char * section, const char * key, int * val )
{
	char tmp[ MAX_ASCIIS ];
	if( ini_file_read_string( ini_file, section, key, tmp, MAX_ASCIIS ) != 0 )
		return -1;
	sscanf( tmp, "%d", val );
	return 0;
}

int ini_file_read_uint( INI_FILE * ini_file, const char * section, const char * key, int * val )
{
	char tmp[ MAX_ASCIIS ];
	if( ini_file_read_string( ini_file, section, key, tmp, MAX_ASCIIS ) != 0 )
		return -1;
	sscanf( tmp, "0x%X", val );
	return 0;
}

int ini_file_read_double( INI_FILE * ini_file, const char * section, const char * key, double * val )
{
	char tmp[ MAX_ASCIIS ];
	if( ini_file_read_string( ini_file, section, key, tmp, MAX_ASCIIS ) != 0 )
		return -1;
	*val = atof(tmp);
	return 0;
}

int ini_file_write_string( INI_FILE * ini_file, const char * section, const char * key, const char * val )
{
	char tmp			[MAX_ASCIIS];
	char tmp_section	[MAX_ASCIIS];
	char tmp_val		[MAX_ASCIIS];
	
	if( (ini_file->mode != INI_OPEN_MODE_WRITE) &&
		(ini_file->mode != INI_OPEN_MODE_RW) )
		goto err;
		
	strncpy( tmp_section, section, MAX_ASCIIS );
	strncpy( tmp_val, val, MAX_ASCIIS );

	if( ini_file == NULL )
		goto err;
	//查找section存在否
	if( iniparser_find_entry( ini_file->d, tmp_section ) == 0 )
	{
	//创建section
		if( iniparser_setstring( ini_file->d, tmp_section, NULL ) == -1 )
			goto err;
	}
	sprintf(tmp, "%s:%s", section, key);
	if( iniparser_setstring( ini_file->d, tmp, tmp_val ) == -1 )
		goto err;
	return 0;
err:
	return -1;
}

int ini_file_write_int( INI_FILE * ini_file, const char * section, const char * key, int val )
{
	char int_str[32];
	sprintf( int_str, "%d", val );
	return ini_file_write_string( ini_file, section, key, int_str );
}

int ini_file_write_uint( INI_FILE * ini_file, const char * section, const char * key, unsigned int val )
{
	char hex_str[32];
	sprintf( hex_str, "0x%X", val );
	return ini_file_write_string( ini_file, section, key, hex_str );
}

int ini_file_write_double( INI_FILE * ini_file, const char * section, const char * key, double val )
{
	char double_str[32];
	sprintf( double_str, "%f", val );
	return ini_file_write_string( ini_file, section, key, double_str );
}

#if 0
int ini_file_write_test( void )
{
	INI_FILE * ini = ini_file_open("test.ini");
	if( ini == NULL )
		goto err;
	ini_file_write_string( ini, "section1", "key1", "val1" );
	ini_file_write_int( ini, "section1", "int", 12345678 );
	ini_file_write_uint( ini, "section2", "uint", 43534543 );
	ini_file_write_double( ini, "section3", "double", 3.1415926 );
	ini_file_close( ini );
	return 0;
err:
	return -1;
}

int ini_file_read_test( void )
{
	INI_FILE * ini = ini_file_open("test.ini");
	char str[MAX_ASCIIS];
	int int_1, int_2;
	double double_1;
	
	if( ini == NULL )
		goto err;
		
	ini_file_read_string( ini, "section1", "key1", str, MAX_ASCIIS );
	ini_file_read_int( ini, "section1", "int", &int_1 );
	ini_file_read_uint( ini, "section2", "uint", &int_2 );
	ini_file_read_double( ini, "section3", "double", &double_1 );
	ini_file_close( ini );
	
	printf( "%s %d %x %f\n", str, int_1, int_2, double_1 );
	return 0;
err:
	return -1;
}

int ini_file_test( void )
{
	ini_file_write_test();
	ini_file_read_test();
	return 0;
}

#endif
