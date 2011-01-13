/***************************************************************************

		ps_config.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-12 20:43:00

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include "ps_common.h"

ZOOM_MODE default_zoom_mode = ZOOM_MODE_HEIGHT;
ZOOM_MODE current_zoom_mode = ZOOM_MODE_HEIGHT;

AUTO_DIRECTION PicShow_auto_direction = AUTO_DIRECTION_SAME;
//使用滑动效果
int use_slip_key = 0;
//加载时显示预览
int load_preview = 1;
//默认的图片目录
char PicDir[MAX_PATH] = "";

typedef struct PS_COMFIG{
	ZOOM_MODE default_zoom_mode;
	BITMAP_ZOOM_DIRECTION direction;
} PS_COMFIG;

PS_COMFIG ps_config;

CONFIG_LIST conig_list[] = {
{	"VIEW",			"Default Zoom Mode",	VALUE_TYPE_INT,		&default_zoom_mode		},
{	"VIEW",			"Auto Direction Mode",	VALUE_TYPE_INT,		&PicShow_auto_direction	},
{	"VIEW",			"Current Direction",	VALUE_TYPE_INT,		&PicShow_direction		},
{	"VIEW",			"Loading Preview",		VALUE_TYPE_INT,		&load_preview			},
{	"CONTROL",		"Slip",					VALUE_TYPE_INT,		&use_slip_key			},
{	"PATH",			"Startup Image Path",	VALUE_TYPE_STRING,	PicDir					},
{	NULL,			NULL,					0,					NULL					}
};

int ps_config_save( void )
{
	u8 root[ MAX_PATH ];
	u8 * point;
	INI_FILE * ini;

	sys_get_path( root, MAX_PATH );	//获取应用程序路径
	point = strrchr( root, '\\' );	//寻找路径分隔符
	if( point == NULL )
	{	
		point = strrchr( root, '/' );	//尝试寻找linux分隔符
	}
	if( point )
	{
		*point = 0;
	}
	strcat( root, "\\PicShow.ini" );
	
	ini = ini_file_open( root, INI_OPEN_MODE_RW );
	if( ini == NULL )
		goto err;
		
	ini_config_save( ini, conig_list );
	config_save_keymap( ini, "KEYMAP NORMAL", pskeymap_right0, ps_keyname );
	config_save_keymap( ini, "KEYMAP RIGHT90",  pskeymap_right90, ps_keyname );
	config_save_keymap( ini, "KEYMAP RIGHT180", pskeymap_right180, ps_keyname );
	config_save_keymap( ini, "KEYMAP RIGHT270", pskeymap_right270, ps_keyname );

	ini_file_close( ini );
	return 0;
err:
	return -1;
}

int ps_config_load( void )
{
	u8 root[ MAX_PATH ];
	u8 * point;
	INI_FILE * ini;
	
	sys_get_path( root, MAX_PATH );	//获取应用程序路径
	point = strrchr( root, '\\' );	//寻找路径分隔符
	if( point == NULL )
	{	
		point = strrchr( root, '/' );	//尝试寻找linux分隔符
	}
	if( point )
	{
		*point = 0;
	}
	strcat( root, "\\PicShow.ini" );
	
	ini = ini_file_open( root, INI_OPEN_MODE_READ );
	if( ini == NULL )
		goto err;
		
	ini_config_load( ini, conig_list );
	config_load_keymap( ini, "KEYMAP NORMAL", pskeymap_right0, ps_keyname );
	config_load_keymap( ini, "KEYMAP RIGHT90",  pskeymap_right90, ps_keyname );
	config_load_keymap( ini, "KEYMAP RIGHT180", pskeymap_right180, ps_keyname );
	config_load_keymap( ini, "KEYMAP RIGHT270", pskeymap_right270, ps_keyname );

	ini_file_close( ini );
	
	return 0;
err:
	return -1;
}
