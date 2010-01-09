/***************************************************************************

		win32_config.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-05-02 13:34:55

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>

//Font相关配置
char xfont_nls_path[ MAX_PATH ];		//多国语言转换表目录
char xfont_fonts_path[ MAX_PATH ];		//字体目录
char xfont_default_font[ MAX_PATH ];		//默认GUI字体文件路径
XFONT_CODE_PAGE xfont_default_codepage = 936;	//默认GUI字体代码页
int xfont_cache_memory_size = 1000000;			//xfont内置的cache占用内存大小
int xfont_cache_font_max_size = 32;					//xfont内置的cache最大字体大小

//GUI相关配置
int gui_default_line_height = 16;		//默认的行高
int gui_default_font_size = 16;			//默认的字体大小

NLS_FILE_NAME nls_fname_win32[] = {
{		XFONT_CP_437,				"cp_437.nls"			},
{		XFONT_CP_874,				"cp_874.nls"			},
{		XFONT_CP_936,				"cp_936.nls"			},
{		XFONT_CP_949,				"cp_949.nls"			},
{		XFONT_CP_950,				"cp_950.nls"			},
{		XFONT_CP_1250,				"cp_1250.nls"			},
{		XFONT_CP_1251,				"cp_1251.nls"			},
{		XFONT_CP_1253,				"cp_1253.nls"			},
{		XFONT_CP_1254,				"cp_1254.nls"			},
{		XFONT_CP_1256,				"cp_1256.nls"			},
{		(XFONT_CODE_PAGE)NULL,		(char *)NULL			}
};

NLS_FILE_NAME * nls_fname = nls_fname_win32;

//Screen相关配置
static u32 screen_width = 800;
static u32 screen_height = 600;
static u32 color_mode = LCD_A8R8G8B8;


static int path2dir( char * path )
{
	char * p;
	if( p = strrchr( path, '\\' ) )
	{
		p[1] = 0;
		return 0;
	}
	else if( p = strrchr( path, '/' ) )
	{
		p[1] = 0;
		return 0;
	}
	return -1;
}

static void platform_config_loaddefault( void )
{
	char path[ MAX_PATH ];
	
	sys_get_dir( path, MAX_PATH );
	
	strcpy( xfont_nls_path, path );
	path2dir( xfont_nls_path );
	
	strcat( xfont_nls_path, "platform\\nls\\" );
	
	strcpy( xfont_fonts_path, path );
	path2dir( xfont_fonts_path );
	strcat( xfont_fonts_path, "platform\\fonts\\" );
	
	strcpy( xfont_default_font, "Default.TTF" );
	
	screen_set_mode( screen_width, screen_height, LCD_A8R8G8B8 );
}

CONFIG_LIST config_list[] = {
{	"Font",			"NLS Path",				VALUE_TYPE_STRING,		xfont_nls_path				},
{	"Font",			"Fonts Path",			VALUE_TYPE_STRING,		xfont_fonts_path			},
{	"Font",			"Default Font",			VALUE_TYPE_STRING,		xfont_default_font			},
{	"Font",			"Default Code Page",	VALUE_TYPE_INT,			&xfont_default_codepage		},
{	"Font",			"Cache Memory Size",	VALUE_TYPE_INT,			&xfont_cache_memory_size	},
{	"Font",			"Cache Font Size",		VALUE_TYPE_INT,			&xfont_cache_font_max_size	},
{	"GUI",			"Default Line Height",	VALUE_TYPE_INT,			&gui_default_line_height	},
{	"GUI",			"Default Font Size",	VALUE_TYPE_INT,			&gui_default_font_size		},
{	"Screen",		"Screen Width",			VALUE_TYPE_INT,			&screen_width				},
{	"Screen",		"Screen Height",		VALUE_TYPE_INT,			&screen_height				},
{	"Screen",		"Color Mode",			VALUE_TYPE_INT,			&color_mode					},
{	NULL,			NULL,					0,						NULL						}
};

int win32_config_load( const char * ini_path )
{
	if( platform_config_load( ini_path, config_list ) != 0 )
		goto err;
	//应用设置
	screen_set_mode( screen_width, screen_height, color_mode );
	
	return 0;
err:
	platform_config_loaddefault();
	win32_config_save( ini_path );
	return 0;
}

int win32_config_save( const char * ini_path )
{
	return platform_config_save( ini_path, config_list );
}

	