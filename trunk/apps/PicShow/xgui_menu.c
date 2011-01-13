/***************************************************************************

		menu.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-11 10:23:25

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>
#include "xgui_menu.h"

#define COLOR_FORM 0x00FFFFFF
#define COLOR_FONT 0x00000000
#define COLOR_SELECT 0x000000FF
#define COLOR_FORM_SELECT 0x000000FF
#define COLOR_FONT_SELECT 0x00000000
#define COLOR_FONT_BG  0x00000000
#define MENU_ITEM_HEIGHT gui_default_line_height		//菜单行高
#define MENU_ITEM_WIDTH (gui_default_font_size/2)		//菜单名字节宽度
#define MENU_ITEM_LEFT MENU_ITEM_WIDTH		//菜单字距菜单左边距离
#define MENU_ITEM_TOP MENU_ITEM_WIDTH		//菜单字距菜单顶边距离
#define MENU_ITEM_BOTTOM MENU_ITEM_WIDTH		//菜单字距菜单下边距离
#define MENU_ITEM_RIGHT MENU_ITEM_WIDTH		//菜单字距菜单右边距离
#define MENU_LIGHT_EDGE (MENU_ITEM_WIDTH/4) //菜单高亮区域距菜单边缘

#define MAX_CHARS 32

struct PICSHOW_MENU_HANDLE{
	PICSHOW_MENU * menu;
	XGUI_WND * hWnd;
	XGUI_WND * mount;		//挂接的父窗口
	char name[ MAX_CHARS ];
	int x;
	int y;
	int select;
	int num;
};

static ERRCODE MainMenu_CallBack( XGUI_WND * hWnd , XGUI_MSG * msg )
{
	PICSHOW_MENU_HANDLE * hMenu = XGUI_GetWndUsrParam( hWnd );
	PICSHOW_MENU * menu_data = hMenu->menu;
	int select = hMenu->select;
	int max_select = hMenu->num;
	
	switch( msg->type )
	{
		case XGUI_MSG_DRAW:
		{
			int i = 0;
			XGUI_RECT light_rect;
			XGUI_RECT * rect = XGUI_GetWndRect( hWnd );//(XGUI_RECT *)msg->data;
			XGUI_RectFill( hWnd, rect, COLOR_FORM );
			
			light_rect.left = rect->left + MENU_LIGHT_EDGE;
			light_rect.right = rect->right - MENU_LIGHT_EDGE;
			light_rect.top = rect->top + MENU_ITEM_TOP + select * MENU_ITEM_HEIGHT;
			light_rect.bottom = light_rect.top + MENU_ITEM_HEIGHT;
			XGUI_RectFill( hWnd, &light_rect, COLOR_SELECT );
			
			while( menu_data[i].name )
			{/*
				if( i == select )
				{
					textout( menu_data[i].name, rect->left+MENU_ITEM_LEFT, rect->top + MENU_ITEM_TOP + i * MENU_ITEM_HEIGHT, rect->right, rect->bottom,
						 COLOR_FONT_SELECT, COLOR_FORM_SELECT );
				}
				else*/
				{
					textout( menu_data[i].name, rect->left+MENU_ITEM_LEFT, rect->top + MENU_ITEM_TOP + i * MENU_ITEM_HEIGHT, rect->right, rect->bottom,
						 COLOR_FONT, COLOR_FONT_BG );
				}
				i++;
			}
			break;
		}
		case XGUI_MSG_KEYDOWN:
		{
			XGUI_KEYDATA * keydata = (XGUI_KEYDATA * )msg->data;
			
			if( keydata->func & XGUI_KEY_FUNC_DOWN )
			{
				select++;
				if( select >= max_select )
					select = 0;
			}
			if( keydata->func & XGUI_KEY_FUNC_UP )
			{
				select--;
				if( select < 0 )
					select = max_select - 1;
			}
			if( keydata->func & (XGUI_KEY_FUNC_ENTER|XGUI_KEY_FUNC_RIGHT) )
			{
				if( menu_data[select].picshow_menu_callback )
				{
					switch( menu_data[select].picshow_menu_callback( NULL ) )
					{
						case MENU_STATE_EXIT:				//退出整个菜单循环
							XGUI_Leave();
							return ERRCODE_NONE;
						case MENU_STATE_UPLEVEL:			//退到上级菜单
							XGUI_SetFocusWnd( hWnd->parent );
							PicShow_DestroyMenu( hMenu );
							return ERRCODE_NONE;
					}
				}
				else if( menu_data[select].child )
				{
					XGUI_RECT * rect = XGUI_GetWndRect( hWnd );
					
					PicShow_CreateMenu( menu_data[select].child, menu_data[select].name,
										rect->right + 1, rect->top + select * MENU_ITEM_HEIGHT, NULL );
										
				}
			}
			if( keydata->func & (XGUI_KEY_FUNC_LEFT|XGUI_KEY_FUNC_ESC) ) //销毁本级菜单
			{
				if( hMenu->mount == NULL )				//只有子菜单才能销毁自身,子菜单的mount项为0
				{
					XGUI_SetFocusWnd( hWnd->parent );
					XGUI_SetWndInval( hWnd );
					PicShow_DestroyMenu( hMenu );
				}
				else
				{
					XGUI_Leave();
					return ERRCODE_NONE;
				}
				return ERRCODE_NONE;
			}
			XGUI_SetWndInval( hWnd );
			break;
		}
		case XGUI_MSG_INIT:
		{
			XGUI_SetFocusWnd( hWnd );
			break;
		}
		case XGUI_MSG_DESTROY:
		{
			break;
		}
	}
	hMenu->select = select;
	return ERRCODE_NONE;
}

static void Menu_Get_Size( PICSHOW_MENU * menu, int * width, int * height, int * num )
{
	int i = 0;
	int max_height = MENU_ITEM_TOP, max_width = 0;
	while( menu[i].name )
	{
		int width = MENU_ITEM_WIDTH * strlen( menu[i].name );
		if( width > max_width )
			max_width = width;
		max_height += MENU_ITEM_HEIGHT;
		i++;
	}
	*width = max_width + MENU_ITEM_RIGHT + MENU_ITEM_LEFT;
	*height = max_height + MENU_ITEM_BOTTOM;
	*num = i;
}

PICSHOW_MENU_HANDLE * PicShow_CreateMenu( PICSHOW_MENU * menu, char * name, int x, int y, XGUI_WND * mount )
{
	XGUI_WND * hWnd = NULL;
	int width, height, num;
	PICSHOW_MENU_HANDLE * handle = malloc( sizeof(PICSHOW_MENU_HANDLE) );
	if( handle == NULL )
	{
		TRACE_LOG("malloc handle error");
		goto err;
	}
	Menu_Get_Size( menu, &width, &height, &num );
	
	handle->menu = menu;
	strncpy( handle->name, name, MAX_CHARS );
	handle->x = x;
	handle->y = y;
	
	handle->select = 0;
	handle->num = num;
	
	hWnd = XGUI_CreateWindow( name ,
						 x, y, width, height, NULL,
						 MainMenu_CallBack, handle );
	
	handle->hWnd = hWnd;
	handle->mount = mount;
	
	if( hWnd == NULL )
	{
		TRACE_LOG("CreateWindow error");
		goto err;
	}
	printf( "handle = %x\n", handle );
	return handle;
err:
	if( handle )
		free( handle );
	if( hWnd )
		XGUI_DestroyWindow( hWnd );
	return NULL;
}

void PicShow_DestroyMenu( PICSHOW_MENU_HANDLE * handle )
{
	XGUI_DestroyWindow( handle->hWnd );
	free( handle );
}

