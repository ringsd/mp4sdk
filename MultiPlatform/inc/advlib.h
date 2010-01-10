/***************************************************************************

		regexp.h

 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.

		TIME LIST:
		CREATE	Rainy	2009-04-15 21:13:40

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _regexp_h_
#define _regexp_h_

#ifdef	__cplusplus
extern "C" {
#endif

#define NSUBEXP  10
typedef struct regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

extern regexp *regcomp(const char *re);
extern int regexec(regexp *rp, const char *s);
extern void regsub(const regexp *rp, const char *src, char *dst);
extern void regerror(char *message);
extern void regfree(regexp *rp);

#ifdef __cplusplus
}
#endif

#endif


/***************************************************************************

		advkey.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-15 21:16:20

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _advkey_h_
#define _advkey_h_

#ifdef	__cplusplus
extern "C" {
#endif

//菜单读键
//返回键值,特点是持续按下按键会自动连发
u32 menu_get_key();

#ifdef __cplusplus
}
#endif

#endif
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
/***************************************************************************

		file_explorer.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-14 21:08:19

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _file_explorer_h_
#define _file_explorer_h_

#ifdef	__cplusplus
extern "C" {
#endif

/*
file_explorer_openfile
一个简单的公共打开对话框,调用之后将打开一个打开文件的对话框,
让用户选择需要打开的文件,使用ANSI编码

char * path			初始地址
char * filter		正则表达式匹配文件(例如,所有文件使用".*")
char * return_path	返回的地址,请务必使用MAX_PATH大小的char型数组

如果path输入的是完整的文件路径,隐含的意思是先打开所在目录,再定位光标到文件上
*/
int file_explorer_openfile( char * path, char * filter,char * return_path, const char * custom_msg );

/*
用回调的方式打开文件,在用户选定文件后,回调函数就被调用,其他参数意义同file_explorer_openfile
int (*func)(char *path)
path是用户选定的文件路径
返回值0----继续浏览文件
      1----退出文件浏览
*/
int file_explorer_openfile_shell( char * path, char * filter,char * return_path, const char * custom_msg, int (*func)(char *path) );


#ifdef __cplusplus
}
#endif

#endif
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
/***************************************************************************

		platform_config.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-05-02 13:35:01

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _platform_config_h_
#define _platform_config_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum VALUE_TYPE {
	VALUE_TYPE_STRING,
	VALUE_TYPE_INT,
	VALUE_TYPE_UINT,
	VALUE_TYPE_DOUBLE,
	VALUE_TYPE_END
} VALUE_TYPE;

typedef struct CONFIG_LIST {
	char * section;
	char * key;
	int type;
	void * value;
} CONFIG_LIST;

typedef struct CONFIG_KEYS_LIST {
    VALUE_TYPE type;
    const char * key;
    void * value;
} CONFIG_KEYS_LIST;

int platform_config_load( const char * ini_path, CONFIG_LIST config_list[] );
int platform_config_save( const char * ini_path, CONFIG_LIST config_list[] );
int platform_load_keymap( const char * section, const char * ini_path, KEYMAP * keymap, KEYNAME * keyname );
int platform_save_keymap( const char * section, const char * ini_path, KEYMAP * keymap, KEYNAME * keyname );
int config_load_keys( INI_FILE * ini, const char * section, CONFIG_KEYS_LIST * config_list );
int config_save_keys( INI_FILE * ini, const char * section, CONFIG_KEYS_LIST * config_list );
int config_save_keymap( INI_FILE * ini, const char * section, KEYMAP * keymap, KEYNAME * keyname );
int config_load_keymap( INI_FILE * ini, const char * section, KEYMAP * keymap, KEYNAME * keyname );
int ini_config_save( INI_FILE * ini, CONFIG_LIST config_list[] );
int ini_config_load( INI_FILE * ini, CONFIG_LIST config_list[] );

#ifdef __cplusplus
}
#endif

#endif
