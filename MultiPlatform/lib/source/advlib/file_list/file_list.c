/***************************************************************************

		file_list.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-18 21:40:19

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#define _file_list_h_
#include <global.h>
#undef _file_list_h_
#include "file_list.h"

static void file_list_print( FILE_LIST * hlist );

typedef struct FILE_LIST_ITEM{
	FILE_LIST_DATA data;
	struct FILE_LIST_ITEM *next;
	struct FILE_LIST_ITEM *prev;
} FILE_LIST_ITEM;

struct FILE_LIST{
	FILE_LIST_ITEM * first;
	FILE_LIST_ITEM * last;
	FILE_LIST_ITEM * now;
	int total;
	int index;
};

static void * regexp_test_internal( void )
{
	regexp *rp;
	int ret;
	rp = regcomp( "a" );
	printf( "%x\n", rp );
	ret = regexec( rp , "a.txt" );
	regfree( rp );
	return rp;
}

void regexp_test( void )
{
	if( regexp_test_internal() != regexp_test_internal() )
		printf( "regexp_test head error!\n" );
	else
		printf( "regexp_test head ok!\n" );
}

/*
pattern使用正则表达式匹配文件名
attrib 匹配文件属性
*/
FILE_LIST * file_list_open( const char * path, const char * pattern, FILE_ATTRIB attrib )
{
	FILE_LIST_ITEM * rootlist = NULL;
	FILE_LIST_ITEM * list = NULL;
	FILE_FIND_BLOCK * find = find_open( path );
	FILE_LIST * hlist = NULL;
	int total = 0;
	regexp * rp = regcomp( pattern );
	if( find == NULL )
	{
		printf( "find_open %s %s error!\n", path, pattern );
		return NULL;
	}
	while( find_next( find ) )
	{
		if( (find->attrib & attrib) != find->attrib && attrib != FFD_ALL )
			continue;
		if( (find->attrib & (FFD_DIR|FFD_VOLUME)) == 0 )
			if( regexec( rp , find->name ) == 0 )
				continue;
			
		if( list == NULL )
		{
			list = (rootlist = malloc( sizeof(FILE_LIST_ITEM) ));
			list->prev = NULL;
			list->next = NULL;
		}
		else
		{
			list->next = malloc( sizeof(FILE_LIST_ITEM) );
			list->next->prev = list;
			list->next->next = NULL;
			list = list->next;
		}
		
		strcpy( list->data.fname, find->name );
		list->data.attrib = find->attrib;
		list->data.size = find->size;
		list->data.time_access = find->time_access;
		list->data.time_create = find->time_create;
		list->data.time_write = find->time_write;
		total++;
	}
	
	regfree( rp );
	find_close( find );
	
	if( rootlist )
	{
		hlist = malloc( sizeof(FILE_LIST) );
		hlist->first = rootlist;
		hlist->index = 0;
		hlist->now = rootlist;
		hlist->last = list;
		hlist->total = total;
	}
	//file_list_print( hlist );
	return hlist;
}

/*排序两个字符串
if (return == 0)  str1 == str2
if (return < 0)  str1 < str2
if (return > 0)  str1 > str2
*/
static int StringCompare( char * str1, char * str2 )
{
	while( *str1 || *str2 )
	{
		if( *str1 != *str2 )
			return *str1 - *str2;
		str1++;
		str2++;
	}
	return 0;
}

static int FileNameCompare( FILE_LIST_DATA * data1, FILE_LIST_DATA * data2 )
{
	return StringCompare( data1->fname, data2->fname );
}

static int FileSizeCompare( FILE_LIST_DATA * data1, FILE_LIST_DATA * data2 )
{
	return data1->size-data2->size;
}

static int FileTimeAccessCompare( FILE_LIST_DATA * data1, FILE_LIST_DATA * data2 )
{
	if( data1->time_access > data2->time_access )
		return 1;
	return -1;
}

static int FileTimeCreateCompare( FILE_LIST_DATA * data1, FILE_LIST_DATA * data2 )
{
	if( data1->time_create > data2->time_create )
		return 1;
	return -1;
}

static int FileTimeWriteCompare( FILE_LIST_DATA * data1, FILE_LIST_DATA * data2 )
{
	if( data1->time_write > data2->time_write )
		return 1;
	return -1;
}

static int FileDirCompare( FILE_LIST_DATA * data1, FILE_LIST_DATA * data2 )
{
	if( (data1->attrib & FFD_DIR) < (data2->attrib & FFD_DIR) ||
	    (data1->attrib & FFD_DIR) == (data2->attrib & FFD_DIR) )
		return 1;
	return -1;
}

static void file_list_sort_name_quick( FILE_LIST_ITEM * strlist, FILE_LIST_ITEM * endlist, int (*compare)( FILE_LIST_DATA * data1, FILE_LIST_DATA * data2 ) )
{
	FILE_LIST_ITEM * list = strlist->next;
	FILE_LIST_ITEM * prevlist = NULL;
	while( list && list != endlist )
	{
		if( compare( &list->data, &strlist->data ) < 0 )
		{
			if( list->next )
				list->next->prev = list->prev;
			if( list->prev )
				list->prev->next = list->next;
			list->prev = strlist->prev;
			list->next = strlist;
			
			if( strlist->prev )
			{
				strlist->prev = (strlist->prev->next = list);
			}
			else
			{
				strlist->prev = list;
			}
			if( prevlist == NULL )
				prevlist = list;
			list = list->next;
		}
		list = list->next;
	}

	if( prevlist )
	{
		if( prevlist != strlist->prev )
			file_list_sort_name_quick( prevlist, strlist, compare );
	}

	if( endlist != strlist->next )
	{
		file_list_sort_name_quick( strlist->next, endlist, compare );
	}
}

void file_list_sort( FILE_LIST * hlist, FILE_LIST_SORT sort )
{
	FILE_LIST_ITEM * list = hlist->first;
	switch( sort )
	{
	case FILE_LIST_SORT_NAME:
		file_list_sort_name_quick( list, NULL, FileNameCompare );
		break;
	case FILE_LIST_SORT_SIZE:
		file_list_sort_name_quick( list, NULL, FileSizeCompare );
		break;
	case FILE_LIST_SORT_TIME_CREATE:
		file_list_sort_name_quick( list, NULL, FileTimeCreateCompare );
		break;
	case FILE_LIST_SORT_TIME_ACCESS:
		file_list_sort_name_quick( list, NULL, FileTimeAccessCompare );
		break;
	case FILE_LIST_SORT_TIME_WRITE:
		file_list_sort_name_quick( list, NULL, FileTimeWriteCompare );
		break;
	case FILE_LIST_SORT_DIR:
	    file_list_sort_name_quick( list, NULL, FileDirCompare );
	    break;
	}
	
	while( list->prev )
		list = list->prev;
	hlist->first = list;
	while( list->next )
		list = list->next;
	hlist->last = list;

	hlist->index = 0;
	hlist->now = hlist->first;
}

void file_list_close( FILE_LIST * hlist )
{
	FILE_LIST_ITEM * list = hlist->first;
	
	while( list->prev )
		list = list->prev;
	while( list )
	{
		FILE_LIST_ITEM * tmp = list->next;
		free(list);
		list = tmp;
	}
	free( hlist );
}

static void file_list_print( FILE_LIST * hlist )
{
	FILE_LIST_ITEM * list = hlist->first;
	int i = 0;
	printf( "file_list_print\n" ); 
	while( list )
	{
		printf( "%d,%x,%s\n", i, list, list->data.fname ); 
		list = list->next;
		i++;
	}
}

int file_list_count( FILE_LIST * hlist )
{
	return hlist->total;
}

FILE_LIST_DATA * file_list_read( FILE_LIST * hlist, int index )
{
	FILE_LIST_ITEM * list = NULL;
	int tindex = index;
	if( tindex < 0 )
	    return NULL;
	if( index >= hlist->total )
		return NULL;
	if( (hlist->index - index) >= hlist->index/2 && (hlist->index - index) >= 0 )
	{
		list = hlist->first;
		while( tindex-- )
		{
			if( list->next == NULL )
				return NULL;
			list = list->next;
		}
	}
	else if( (hlist->index - index) < hlist->index/2 && (hlist->index - index) >= 0 )
	{
		list = hlist->now;
		tindex = hlist->index - index;
		while( tindex-- )
		{
			if( list->prev == NULL )
				return NULL;
			list = list->prev;
		}
	}
	else if( (index - hlist->index) <= (hlist->total - hlist->index)/2 && (index - hlist->index) >= 0 )
	{
		list = hlist->now;
		tindex = index - hlist->index;
		while( tindex-- )
		{
			if( list->next == NULL )
				return NULL;
			list = list->next;
		}
	}
	else if( (index - hlist->index) > (hlist->total - hlist->index)/2 && (index - hlist->index) >= 0 )
	{
		list = hlist->last;
		tindex = hlist->total - index - 1;
		while( tindex-- )
		{
			if( list->prev == NULL )
				return NULL;
			list = list->prev;
		}
	}
	hlist->now = list;
	hlist->index = index;
	return &list->data;
}

int file_list_index( FILE_LIST * hlist, const char * fname )
{
	FILE_LIST_DATA * data = NULL;
	int i = 0;
	while( data = file_list_read( hlist, i ) )
	{
		if( !strcmp( data->fname, fname ) )
			break;
		i++;
	}
	return i;
}

#define MAX_FILES 100
static void * file_list_test_internal( void )
{
	FILE_LIST * list = file_list_open( ".\\testdir\\", ".*", FFD_ALL );
	FILE_LIST_DATA * data[MAX_FILES];
	int i, total;
	printf( "list=%x\n", list );
	//file_list_print( list );
	//menu_get_key();
	file_list_sort( list, FILE_LIST_SORT_NAME );
	file_list_print( list );
	//menu_get_key();
	total = file_list_count( list );
	for( i = 0; i < MAX_FILES; i++ )
	{
		data[i] = file_list_read( list, i );
	}
	for( i = 0; i < MAX_FILES; i++ )
	{
		int index = rand() % MAX_FILES;
		if( data[index] != file_list_read( list, index )  )
		{
			printf( "in index %d error.\n", index );
			break;
		}
	}
	if( i == MAX_FILES )
		printf( "filelist random read ok!\n" );
		
	file_list_close( list );
	return list;
}

void file_list_test( void )
{
	if( file_list_test_internal() != file_list_test_internal() )
		printf( "file_list_test heap error!\n" );
	else
		printf( "file_list_test heap ok!\n" );
	regexp_test();
}

