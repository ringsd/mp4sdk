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
	platform_config_save( root, conig_list );
	platform_save_keymap( "KEYMAP NORMAL", root, pskeymap_right0, ps_keyname );
	platform_save_keymap( "KEYMAP RIGHT90", root, pskeymap_right90, ps_keyname );
	platform_save_keymap( "KEYMAP RIGHT180", root, pskeymap_right180, ps_keyname );
	platform_save_keymap( "KEYMAP RIGHT270", root, pskeymap_right270, ps_keyname );
	return 0;
}

int ps_config_load( void )
{
	u8 root[ MAX_PATH ];
	u8 * point;
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
	platform_config_load( root, conig_list );
	platform_load_keymap( "KEYMAP NORMAL", root, pskeymap_right0, ps_keyname );
	platform_load_keymap( "KEYMAP RIGHT90", root, pskeymap_right90, ps_keyname );
	platform_load_keymap( "KEYMAP RIGHT180", root, pskeymap_right180, ps_keyname );
	platform_load_keymap( "KEYMAP RIGHT270", root, pskeymap_right270, ps_keyname );
	return 0;
}
