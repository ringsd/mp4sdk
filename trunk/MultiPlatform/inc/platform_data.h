/***************************************************************************

		platform_data.h

		平台公共数据声明.

		TIME LIST:
		CREATE	Rainy	2009-05-02 17:07:14

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _platform_data_h_
#define _platform_data_h_

#ifdef	__cplusplus
extern "C" {
#endif

//Font相关配置
extern char xfont_nls_path[ MAX_PATH ];		//多国语言转换表目录
extern char xfont_fonts_path[ MAX_PATH ];		//字体目录
extern char xfont_default_font[ MAX_PATH ];		//默认GUI字体文件路径
extern XFONT_CODE_PAGE xfont_default_codepage;	//默认GUI字体代码页
extern int xfont_cache_memory_size;			//xfont内置的cache占用内存大小
extern int xfont_cache_font_max_size;					//xfont内置的cache最大字体大小
extern NLS_FILE_NAME * nls_fname;   //全局NLS文件名列表

//GUI相关配置
extern int gui_default_line_height;		//默认的行高
extern int gui_default_font_size;		//默认的字体大小

#ifdef __cplusplus
}
#endif

#endif
