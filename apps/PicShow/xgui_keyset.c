/***************************************************************************

		xgui_keyset.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-05-07 19:12:40

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>

#define COLOR_FORM 0x00FFFFFF
#define COLOR_FONT 0x00000000
#define COLOR_FONT_BG 0x00000000
#define COLOR_SELECT 0x000000FF

#define KEYSET_HEIGHT gui_default_line_height		//菜单行高
#define KEYSET_WIDTH (gui_default_font_size)		//菜单名字节宽度

typedef struct XGUI_KEYSET {
	KEYMAP * keymap;
	KEYNAME * srcname;
	KEYNAME * dstname;
	const char * title;
	int maxkey;
	int select;
	int start;
	int max_disp;
	int userkey_x;
} XGUI_KEYSET;


static int XGUI_KeySet_Set( XGUI_KEYSET * hKeySet, XGUI_WND * hWnd, XGUI_RECT * rect )
{
	u32 key;
	int i;
	XGUI_RECT this_rect;
	this_rect.left = rect->left+KEYSET_WIDTH;
	this_rect.right = rect->right-KEYSET_WIDTH;
	this_rect.top = (rect->top + rect->bottom)/3;
	this_rect.bottom = this_rect.top + KEYSET_HEIGHT*3;
	
	menu_clear_key();
	
	XGUI_RectFill( hWnd, &this_rect, COLOR_SELECT );
	textout( "等待按键中...",
						this_rect.left, this_rect.top, 
						this_rect.right, this_rect.bottom, 
						COLOR_FONT, COLOR_FONT_BG );
	lcd_updateui();
	
	while( sys_get_key() == 0 )
		sys_delay(200);
	while( key = sys_get_key() )
	{
		XGUI_RectFill( hWnd, &this_rect, COLOR_SELECT );
	
		textout( getkeyname( key, hKeySet->srcname ), 
						this_rect.left, this_rect.top, 
						this_rect.right, this_rect.bottom, 
						COLOR_FONT, COLOR_FONT_BG );
		
		i = 0;
		while( hKeySet->keymap[i].phykey )
		{
		    if( hKeySet->keymap[i].phykey == key )
		    {
		        hKeySet->keymap[i].phykey = 0;
		    }
		    i++;
		}
		
		hKeySet->keymap[hKeySet->select].phykey = key;
		
		lcd_updateui();
		sys_delay(200);
	}
	return 0;
}

static ERRCODE KeySet_CallBack( XGUI_WND * hWnd , XGUI_MSG * msg )
{
	XGUI_KEYSET * hKeySet = XGUI_GetWndUsrParam( hWnd );
	switch( msg->type )
	{
		case XGUI_MSG_DRAW:
		{
			int i;
			int select = hKeySet->select;
			int start = hKeySet->start;
			XGUI_RECT light_rect;
			KEYMAP * keymap = hKeySet->keymap;
			KEYNAME * srcname = hKeySet->srcname;
			KEYNAME * dstname = hKeySet->dstname;
			XGUI_RECT * rect = XGUI_GetWndRect( hWnd );
			XGUI_RectFill( hWnd, rect, COLOR_FORM );
			
			light_rect.left = rect->left;
			light_rect.right = rect->right;
			light_rect.top = rect->top + (select-start) * KEYSET_HEIGHT;
			light_rect.bottom = light_rect.top + KEYSET_HEIGHT - 1;
			XGUI_RectFill( hWnd, &light_rect, COLOR_SELECT );
			
			for( i = 0; i < hKeySet->maxkey; i++ )
			{
				if( rect->top + i * KEYSET_HEIGHT >= rect->bottom )
					break;
				
				
				//显示功能键
				textout( getkeyname( keymap[i+start].emukey, dstname ), 
						rect->left, rect->top + i * KEYSET_HEIGHT, 
						rect->right, min(rect->top + (i+1) * KEYSET_HEIGHT, rect->bottom),
						COLOR_FONT, COLOR_FONT_BG );
						
				//显示对应按键
				textout( getkeyname( keymap[i+start].phykey, srcname ), 
						rect->left + hKeySet->userkey_x, rect->top + i * KEYSET_HEIGHT, 
						rect->right, min(rect->top + (i+1) * KEYSET_HEIGHT, rect->bottom),
						COLOR_FONT, COLOR_FONT_BG );
				
			}
			break;
		}
		case XGUI_MSG_KEYDOWN:
		{
			XGUI_KEYDATA * keydata = (XGUI_KEYDATA * )msg->data;
			if( keydata->func & XGUI_KEY_FUNC_DOWN )
			{
				hKeySet->select++;
				if( hKeySet->select >= hKeySet->maxkey )
				{
					hKeySet->select = 0;
					hKeySet->start = 0;
				}
				else if( hKeySet->select - hKeySet->start >= hKeySet->max_disp )
					hKeySet->start++;
			}
			if( keydata->func & XGUI_KEY_FUNC_UP )
			{
				hKeySet->select--;
				if( hKeySet->select < 0 )
				{
					hKeySet->select = hKeySet->maxkey - 1;
					hKeySet->start = max(hKeySet->maxkey - hKeySet->max_disp, 0);
				}
				else
				if( hKeySet->select < hKeySet->start )
					hKeySet->start--;
			}
			if( keydata->func & XGUI_KEY_FUNC_ENTER )
			{
				XGUI_KeySet_Set( hKeySet, hWnd, XGUI_GetWndRect( hWnd ) );
			}
			if( keydata->func & XGUI_KEY_FUNC_ESC ) //销毁本级菜单
			{
				XGUI_SetFocusWnd( hWnd->parent );
				XGUI_SetWndInval( hWnd );
				XGUI_DestroyWindow( hWnd );
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
			free( hKeySet );
			break;
		}
	}
	return ERRCODE_NONE;
}

int XGUI_KeySet( KEYMAP * keymap, KEYNAME * srcname, KEYNAME * dstname, const char * title )
{
	XGUI_WND * hWnd;
	XGUI_KEYSET * hKeySet;
	int maxkey = 0;
	int width = 0;
	int height = 0;
	int userkey_x = 0;
	
	if( (hKeySet = malloc(sizeof(XGUI_KEYSET))) == NULL )
		goto err;
	
	//计算最大的按键数,需要窗口宽度,高度
	while( keymap[maxkey].emukey )
	{
		int dstname_width = strlen( getkeyname( keymap[maxkey].emukey, dstname ) ) * KEYSET_WIDTH;
		
		int string_width = dstname_width + strlen( getkeyname( keymap[maxkey].phykey, srcname ) )*KEYSET_WIDTH;
		
		if( userkey_x < dstname_width )
			userkey_x = dstname_width;
			
		if( width < string_width )
			width = string_width;

		height += KEYSET_HEIGHT;
		maxkey++;
	}
	
	height = min( height, screen_get_height() );
	
	hKeySet->keymap = keymap;
	hKeySet->title = title;
	hKeySet->maxkey = maxkey;
	hKeySet->srcname = srcname;
	hKeySet->dstname = dstname;
	hKeySet->select = 0;
	hKeySet->start = 0;
	hKeySet->max_disp = height/KEYSET_HEIGHT;
	hKeySet->userkey_x = userkey_x;
	
	if( (hWnd = XGUI_CreateWindow( "Key Setting" ,
						 (screen_get_width()-width)/2, (screen_get_height()-height)/2,
						 width, height, NULL,
						 KeySet_CallBack, hKeySet )) == NULL )
						 goto err;
	
	return 0;
err:
	return -1;
}

