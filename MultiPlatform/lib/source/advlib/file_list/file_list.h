/***************************************************************************

		file_list.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-18 21:40:27

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _file_list_h_
#define _file_list_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct FILE_LIST FILE_LIST;

typedef enum {
	FILE_LIST_SORT_NAME,
	FILE_LIST_SORT_SIZE,
	FILE_LIST_SORT_TIME_CREATE,
	FILE_LIST_SORT_TIME_ACCESS,
	FILE_LIST_SORT_TIME_WRITE,
	FILE_LIST_SORT_DIR,
	FILE_LIST_SORT_MAX
} FILE_LIST_SORT;

typedef struct FILE_LIST_DATA{
	char fname[MAX_PATH];
	FILE_ATTRIB attrib;
	u32 size;
	time_t time_create;
	time_t time_write;
	time_t time_access;
} FILE_LIST_DATA;

void file_list_test( void );
FILE_LIST * file_list_open( const char * path, const char * pattern, FILE_ATTRIB attrib );
void file_list_sort( FILE_LIST * hlist, FILE_LIST_SORT sort );
void file_list_close( FILE_LIST * hlist );
FILE_LIST_DATA * file_list_read( FILE_LIST * hlist, int index );
int file_list_count( FILE_LIST * hlist );
int file_list_index( FILE_LIST * hlist, const char * fname );

#ifdef __cplusplus
}
#endif

#endif
