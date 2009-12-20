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
