/***************************************************************************

		xgui.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-02-09 11:47:24

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>
#include "xgui.h"
//#include "bitmap.h"
#include "MM_Image.h"

enum{
	XGUI_STATUS_NORMAL = 0x0,			//正常,窗口无需重绘
	XGUI_STATUS_MOVE = 0x1,				//窗口即将移动
	XGUI_STATUS_HIDE = 0x2,				//窗口隐藏
	XGUI_STATUS_REFRESH = 0x4,		//全窗口需要刷新
	XGUI_STATUS_INIT = 0x8,				//窗口初始化中
	XGUI_STATUS_INVAL = 0x10,				//部分区域无效,需要重画
};



XGUI_WND *hRootWnd = NULL;
XGUI_WND *hLastWnd = NULL;

static int ScreenIsDirty = 0;

#define SCREEN_SET_DIRTY ScreenIsDirty = 1;
#define SCREEN_RESET_DIRTY ScreenIsDirty = 0;
#define SCREEN_IS_DIRTY (ScreenIsDirty==1)



ERRCODE XGUI_ShowWindow( XGUI_WND * hWnd )
{
	hWnd->inval.left		= hWnd->inval.left;
	hWnd->inval.top			= hWnd->inval.top;
	hWnd->inval.right		= hWnd->inval.right; 
	hWnd->inval.bottom	= hWnd->inval.bottom;
	return ERRCODE_NONE;
}

XGUI_WND * XGUI_CreateWindow( char *name ,
 int x, int y, int width, int height, XGUI_WND * parent,
 ERRCODE (* CallBack)( XGUI_WND * hWnd , XGUI_MSG * msg ), void *param  )
{
	XGUI_WND *hWnd = malloc( sizeof(XGUI_WND) );
	if( hWnd == NULL )
		return NULL;
	hWnd->rect.left = x;
	hWnd->rect.top = y;
	hWnd->rect.right = x + width-1;
	hWnd->rect.bottom = y + height-1;
	strncpy( hWnd->name, name, XGUI_MAXNAME );
	hWnd->CallBack = CallBack;
	hWnd->status = XGUI_STATUS_INIT|XGUI_STATUS_REFRESH;
	hWnd->param = param;
	if( parent == NULL )
		hWnd->parent = hRootWnd;
	else
	{
		hWnd->parent = parent;
	}
	hWnd->child = NULL;
	
	if( hWnd->parent->child == NULL )
	{
		hWnd->parent->child = hWnd;
		hWnd->prev = NULL;
		hWnd->next = NULL;
	}
	else
	{
		XGUI_WND *hWndBro = hWnd->parent->child;
		while( hWndBro->next )
		{
			hWndBro = hWndBro->next;
		}
		hWnd->prev = hWndBro;
		hWnd->next = NULL;
		hWndBro->next = hWnd;
	}
	/*
	hWnd->next = NULL;
	hWnd->prev = hLastWnd;
	hLastWnd->next = hWnd;
	hLastWnd = hWnd;
	*/
	SCREEN_SET_DIRTY;
	return hWnd;
}

static XGUI_WND * FocusWnd = NULL;

void XGUI_PrintWindowTree( XGUI_WND * hWnd )
{
	XGUI_WND * thWnd = hWnd;
	static int level = 0;
	level++;
	while( thWnd->prev )
		thWnd = thWnd->prev;
	while( thWnd )
	{
		int i = level;
		while( i-- )
			printf("\t");
		printf( "hWnd=%x\n", thWnd );
		if( thWnd->child )
			XGUI_PrintWindowTree( thWnd->child );
		thWnd = thWnd->next;
	}
	level--;
}

ERRCODE XGUI_DestroySingleWindow( XGUI_WND * hWnd )
{
	XGUI_MSG msg;
	msg.data = NULL;
	msg.type = XGUI_MSG_DESTROY;
	if( hWnd->CallBack )
		hWnd->CallBack( hWnd, &msg );
	if( hWnd->prev )
		hWnd->prev->next = hWnd->next;
	else if( hWnd->parent )
	{
		hWnd->parent->child = hWnd->next;
	}
		
	if( hWnd->next )
		hWnd->next->prev = hWnd->prev;
	else if( hWnd->prev )
		FocusWnd = hWnd->prev;
	free( hWnd );
//	printf( "hWnd=%x\n", hWnd );
	return ERRCODE_NONE;
}

void XGUI_DestroyLinkWindows( XGUI_WND * hWnd )
{
	XGUI_WND * thWnd = hWnd;
	while( thWnd->prev )
		thWnd = thWnd->prev;
	while( thWnd )
	{
		XGUI_WND * tmphWnd;
		if( thWnd->child )
			XGUI_DestroyLinkWindows( thWnd->child );
		tmphWnd = thWnd->next;
		XGUI_DestroySingleWindow( thWnd );
		thWnd = tmphWnd;
	}
}

ERRCODE XGUI_DestroyWindow( XGUI_WND * hWnd )
{
	if( hWnd->child )
		XGUI_DestroyLinkWindows( hWnd->child );
	XGUI_DestroySingleWindow( hWnd );
	return ERRCODE_NONE;
}

ERRCODE XGUI_SetFocusWnd( XGUI_WND * hWnd )
{
	XGUI_WND * hLastWnd = hWnd;
	printf( "SetFocus %s\n", hWnd->name );
	while( hLastWnd->next )
		hLastWnd = hLastWnd->next;
	if( hWnd->next )
		hWnd->next->prev = hWnd->prev;
	else
	{
		FocusWnd = hWnd;
		return ERRCODE_NONE;
	}
	if( hWnd->prev )
		hWnd->prev->next = hWnd->next;
	else
		hWnd->parent->child = hWnd->next;
	hWnd->prev = hLastWnd;
	hWnd->next = NULL;
	hLastWnd->next = hWnd;
	FocusWnd = hWnd;
	return ERRCODE_NONE;
}

static XGUI_WND * XGUI_GetLastBroWnd( XGUI_WND * hStartWnd )
{
	XGUI_WND *hWnd;
	hWnd = hStartWnd;
	while( hWnd->next )
		hWnd = hWnd->next;
	if( hWnd->child )
		hWnd = XGUI_GetLastBroWnd( hWnd->child );
	//printf( "%s\n", hWnd->name );
	XGUI_PrintWindowTree( hRootWnd );
	return hWnd;
}

XGUI_WND * XGUI_GetFrontWnd( void )
{
	return XGUI_GetLastBroWnd( hRootWnd );
}

XGUI_WND * XGUI_GetFocusWnd( void )
{
	//XGUI_WND * hWnd = XGUI_GetLastBroWnd( hRootWnd );
	//printf( "GetFocus %s\n" , FocusWnd->name );
	return FocusWnd;
}

ERRCODE XGUI_SetGlobalRectInval( XGUI_WND * hStartWnd, XGUI_RECT * rect )
{
	XGUI_WND *thWnd = hStartWnd;
	while( thWnd->prev )
		thWnd = thWnd->prev;
	while( thWnd )
	{
		if( rect->left <= thWnd->rect.right &&
				rect->right >= thWnd->rect.left && 
				rect->top <= thWnd->rect.bottom && 
				rect->bottom >= thWnd->rect.top
				)
		{
			printf( "%s ", thWnd->name );
			thWnd->status |= XGUI_STATUS_INVAL;
			thWnd->inval.left = max(max( rect->left, thWnd->rect.left ), 0 );
			thWnd->inval.right = min( rect->right, thWnd->rect.right );
			thWnd->inval.top = max(max( rect->top, thWnd->rect.top ), 0 );
			thWnd->inval.bottom = min( rect->bottom, thWnd->rect.bottom );
		}
		if( thWnd->child )
			XGUI_SetGlobalRectInval( thWnd->child, rect );
		thWnd = thWnd->next;
	}
	return ERRCODE_NONE;
}

ERRCODE XGUI_SetWndInval( XGUI_WND * hWnd )
{
	if( hWnd == NULL )
		return ERRCODE_PARAM;
	XGUI_SetGlobalRectInval( hWnd->parent, &hWnd->rect );
	SCREEN_SET_DIRTY;
	return ERRCODE_NONE;
}

ERRCODE XGUI_SetPos( XGUI_WND * hWnd, int x, int y )
{
	XGUI_WND *thWnd;
	int x_add, y_add;
	SCREEN_SET_DIRTY;
	x_add = x - hWnd->rect.left;
	y_add = y - hWnd->rect.top;
	printf( "XGUI_SetPos..." );
	XGUI_SetGlobalRectInval( hWnd->parent, &hWnd->rect );
	/*
	thWnd = hWnd;
	while( thWnd->prev )
		thWnd = thWnd->prev;
	while( thWnd != NULL )
	{
		if( hWnd->rect.left < thWnd->rect.right &&
				hWnd->rect.right > thWnd->rect.left && 
				hWnd->rect.top < thWnd->rect.bottom && 
				hWnd->rect.bottom > thWnd->rect.top &&
				hWnd != thWnd
				)
		{
			printf( "%s ", thWnd->name );
			thWnd->status |= XGUI_STATUS_INVAL;
			thWnd->inval.left = max( hWnd->rect.left, thWnd->rect.left );
			thWnd->inval.right = min( hWnd->rect.right, thWnd->rect.right );
			thWnd->inval.top = max( hWnd->rect.top, thWnd->rect.top );
			thWnd->inval.bottom = min( hWnd->rect.bottom, thWnd->rect.bottom );
		}
		if( thWnd == hWnd->parent )
			break;
		if( thWnd->next )
			thWnd = thWnd->next;
		else
			thWnd = hWnd->parent;
	}
	*/
	hWnd->status &= ~XGUI_STATUS_INVAL;

	hWnd->rect.left += x_add;
	hWnd->rect.right += x_add;
	hWnd->rect.top += y_add;
	hWnd->rect.bottom += y_add;
	
	hWnd->status |= XGUI_STATUS_REFRESH;
	
	printf( "%d %d\n", x, y );
	return ERRCODE_NONE;
}

static KEYMAP phy_keymap[MAX_BUTTONS]={
{		XGUI_KEY_FUNC_UP,			SYSKEY_UP			},
{		XGUI_KEY_FUNC_DOWN,		SYSKEY_DOWN		},
{		XGUI_KEY_FUNC_LEFT,		SYSKEY_LEFT		},
{		XGUI_KEY_FUNC_RIGHT,	SYSKEY_RIGHT	},
{		XGUI_KEY_FUNC_ENTER,	SYSKEY_A		},
{		XGUI_KEY_FUNC_ESC,		SYSKEY_B		},
{		0,	0	},
};

//返回值	-- 0 没有按键被按下
//				!= 0 有按键被按下
int XGUI_GetKeyDown( XGUI_KEYDATA *keydata )
{
	static u32 bak_func = 0;
	u32 key = sys_get_key();
	u32 tkey = convert_key( key, phy_keymap );
	keydata->numsym = 0;
	keydata->word = 0;
	keydata->func = (bak_func^tkey) & tkey;
	keydata->other = 0;
	bak_func = tkey;
	return (keydata->numsym|keydata->word|keydata->func|keydata->other);
}

static ERRCODE XGUI_WndMgrPointer( void )
{
	int pointer_x, pointer_y, pointer_press;
	if( sys_get_pointer( &pointer_x, &pointer_y, &pointer_press ) == 0 )
	{
		static int holdwnd_x = -1, holdwnd_y = -1;
		if( pointer_press == POINTER_PRESS_MAX )
		{
			XGUI_WND * hLastWnd;
			if( holdwnd_x == -1 || holdwnd_y == -1 )
			{
				XGUI_WND * thWnd = XGUI_GetFrontWnd();
				while( thWnd != hRootWnd )
				{
					if( thWnd->rect.left < pointer_x &&
							thWnd->rect.top < pointer_y &&
							thWnd->rect.right > pointer_x &&
							thWnd->rect.bottom > pointer_y )
					{
						holdwnd_x = pointer_x - thWnd->rect.left;
						holdwnd_y = pointer_y - thWnd->rect.top;
						XGUI_SetFocusWnd( thWnd );
						break;
					}
					if( thWnd->prev )
					{
						thWnd = thWnd->prev;
						while( thWnd->child )
							thWnd = XGUI_GetLastBroWnd(thWnd->child);
					}
					else if( thWnd->parent )
						thWnd = thWnd->parent;
				}
			}
			hLastWnd = XGUI_GetFocusWnd();
			if( hLastWnd != hRootWnd && holdwnd_x >= 0 && holdwnd_y >= 0 )
				XGUI_SetPos( hLastWnd, pointer_x - holdwnd_x, pointer_y - holdwnd_y );
			printf( "%d %d %d\n", pointer_x, pointer_y, pointer_press );
		}
		else
		{
			holdwnd_x = -1;
			holdwnd_y = -1;
		}
	}
	return ERRCODE_NONE;
}

ERRCODE XGUI_WndMgrDraw( XGUI_WND * hParentWnd )
{
	XGUI_WND * hWnd;
	XGUI_MSG msg;
		
	hWnd = hParentWnd;
	if( !SCREEN_IS_DIRTY )
		return ERRCODE_NONE;
	while( hWnd )
	{
		#define STATUS( a ) if( hWnd->status & a )
		
		STATUS( XGUI_STATUS_INIT )
		{
			msg.data = hWnd->param;
			msg.type = XGUI_MSG_INIT;
			if( hWnd->CallBack )
				hWnd->CallBack( hWnd, &msg );
			hWnd->status &= ~XGUI_STATUS_INIT;
		}
		
		STATUS( XGUI_STATUS_REFRESH )
		{
			XGUI_RECT inval;
			inval.left		= hWnd->rect.left;
			inval.top			= hWnd->rect.top;
			inval.right		= hWnd->rect.right; 
			inval.bottom	= hWnd->rect.bottom;
			msg.data = &inval;
			msg.type = XGUI_MSG_DRAW;
			if( hWnd->CallBack )
				hWnd->CallBack( hWnd, &msg );
			hWnd->status &= ~XGUI_STATUS_REFRESH;
		}
		
		STATUS( XGUI_STATUS_INVAL )
		{
			XGUI_RECT inval;
			inval.left		= hWnd->inval.left;
			inval.top			= hWnd->inval.top;
			inval.right		= hWnd->inval.right; 
			inval.bottom	= hWnd->inval.bottom;
			msg.data = &inval;
			msg.type = XGUI_MSG_DRAW;
			if( hWnd->CallBack )
				hWnd->CallBack( hWnd, &msg );
			hWnd->status &= ~XGUI_STATUS_INVAL;
		}
		
		if( hWnd->child )
			XGUI_WndMgrDraw( hWnd->child );
			
		hWnd = hWnd->next;
	}
	
	return ERRCODE_NONE;
}

static int xgui_run = 0;

//离开GUI循环,但并不释放资源
ERRCODE XGUI_Leave( void )
{
	xgui_run = 0;
	return ERRCODE_NONE;
}

//窗口管理器
ERRCODE XGUI_WindowsManager( void )
{
	xgui_run = 1;
	while( xgui_run )
	{
		static XGUI_KEYDATA keydata;
		XGUI_MSG msg;
		
		if( XGUI_GetKeyDown( &keydata ) )
		{
			hLastWnd = XGUI_GetFocusWnd();
			msg.data = &keydata;
			msg.type = XGUI_MSG_KEYDOWN;
			if( hLastWnd )
				if( hLastWnd->CallBack )
				hLastWnd->CallBack( hLastWnd, &msg );
		}
		
		XGUI_WndMgrPointer();
		XGUI_WndMgrDraw( hRootWnd );
		lcd_updateui();
		SCREEN_RESET_DIRTY;
		sys_delay(10);
	}
	return ERRCODE_NONE;
}

//填充矩形(支持Alpha操作)
void XGUI_RectFill( XGUI_WND * hWnd, XGUI_RECT *rect, XGUI_COLOR color )
{
	int x , y;
	int sx, sy, dx, dy;
	int width = screen_get_width();
	u32 tcolor = color;
	u32 *base;
	u32 alpha = 255-(tcolor >> 24);
	u32 r0 = (tcolor >> 16)&0xFF;
	u32 g0 = (tcolor >> 8)&0xFF;
	u32 b0 = tcolor&0xFF;
	r0 = r0 * alpha / 255;
	g0 = g0 * alpha / 255;
	b0 = b0 * alpha / 255;
	
	if( hWnd->parent )
	{
		sx = max( rect->left, hWnd->parent->rect.left );
		sy = max( rect->top, hWnd->parent->rect.top );
		dx = min( rect->right, hWnd->parent->rect.right );
		dy = min( rect->bottom, hWnd->parent->rect.bottom );
	}
	else
	{
		sx = max( rect->left, 0 );
		sy = max( rect->top, 0 );
		dx = min( rect->right, screen_get_width()-1 );
		dy = min( rect->bottom, screen_get_height()-1 );
	}
	base = lcd_bufferui() + sx + sy*width;
	for( y = sy ; y <= dy ; y++ )
	{
		u32 *buf = base;
		for( x = sx ; x <= dx ; x++ )
		{
			u32 bg = *buf;
			u32 r1 = (bg >> 16)&0xFF;
			u32 g1 = (bg >> 8)&0xFF;
			u32 b1 = bg&0xFF;
			r1  = r0 + r1 * (255 - alpha) / 255;
			g1  = g0 + g1 * (255 - alpha) / 255;
			b1  = b0 + b1 * (255 - alpha) / 255;
			*buf = (r1 << 16) | (g1 << 8) | b1;
			buf ++ ;
		}
		base += width;
	}
}

static ERRCODE XGUI_DesktopCallBack( XGUI_WND * hWnd , XGUI_MSG * msg )
{
	static u8 *bgdata;
	switch( msg->type )
	{
		case XGUI_MSG_DRAW:
		{
			//GA_RECT srcblock, srcwnd, dstblock, dstwnd;
			XGUI_RECT *rect = (XGUI_RECT *)msg->data;
			XGUI_RectFill( hWnd, rect, 0x000000 );
			//XGUI_COLOR color = 0xFFFFFFFF;
			//GA_RectCopy_A8R8G8B8( (GA_RECT*)rect, (GA_RECT*)&hWnd->rect, bgdata, (GA_RECT*)rect, (GA_RECT*)&hWnd->rect, lcd_bufferui() );
			//printf( "DESKTOP_DRAW %d,%d,%d,%d\n", rect->left, rect->top, rect->right, rect->bottom );
			break;
		}
		case XGUI_MSG_INIT:
		{
			//bgdata = malloc( screen_get_width() * screen_get_height() * 4 );
			//if( bgdata == NULL )
			//	return ERRCODE_MALLOC;
			//memcpy( bgdata, lcd_bufferui(), screen_get_width() * screen_get_height() * 4 );
			break;
		}
		case XGUI_MSG_DESTROY:
		{
			//free( bgdata );
			break;
		}
	}
	return ERRCODE_NONE;
}

ERRCODE XGUI_Init( ERRCODE (* RootCallBack)( struct XGUI_WND * hWnd , XGUI_MSG * msg ) )
{
	//Init Root Window
	if( hRootWnd != NULL )
		return ERRCODE_RECALL;
	
	hRootWnd = malloc( sizeof(XGUI_WND) );
	if( hRootWnd == NULL )
		return ERRCODE_MALLOC;
	hRootWnd->rect.left = 0;
	hRootWnd->rect.top = 0;
	hRootWnd->rect.right = screen_get_width() - 1;
	hRootWnd->rect.bottom = screen_get_height() - 1;
	strncpy( hRootWnd->name, "ROOT\0", XGUI_MAXNAME );
	hRootWnd->CallBack = RootCallBack;
	hRootWnd->next = NULL;
	hRootWnd->prev = NULL;
	hRootWnd->parent = NULL;
	hRootWnd->child = NULL;
	hRootWnd->status = XGUI_STATUS_INIT|XGUI_STATUS_REFRESH;
	//hRootWnd = XGUI_CreateWindow( "ROOT", 0, 0, screen_get_width(), screen_get_height() , NULL ,XGUI_DesktopCallBack );
	hLastWnd = hRootWnd;
	
	FocusWnd = NULL;
	
	return ERRCODE_NONE;
}

ERRCODE XGUI_DeInit( void )
{
	if( hRootWnd == NULL )
		return ERRCODE_NOTPREPARE;
	if( hLastWnd == NULL )
		return ERRCODE_NOTPREPARE;
	XGUI_DestroyWindow( hRootWnd );
	hRootWnd = NULL;
	hLastWnd = NULL;
	return ERRCODE_NONE;
}

//转换绝对坐标到窗口坐标
void XGUI_RectConv_Abs2Wnd( XGUI_RECT * abs, XGUI_RECT * wnd, XGUI_WND * hWnd )
{
	wnd->left = abs->left - hWnd->rect.left;
	wnd->right = abs->right - hWnd->rect.left;
	wnd->bottom = abs->bottom - hWnd->rect.top;
	wnd->top = abs->top - hWnd->rect.top;
}

//获取窗口矩形
XGUI_RECT * XGUI_GetWndRect( XGUI_WND * hWnd )
{
	if( hWnd == NULL )		//NULL指窗口
	{
		hRootWnd->rect.left = 0;
		hRootWnd->rect.top = 0;
		hRootWnd->rect.right = screen_get_width() - 1;
		hRootWnd->rect.bottom = screen_get_height() - 1;
		return &hRootWnd->rect;
	}
	else
		return &hWnd->rect;
}

//获取窗口宽度
int XGUI_GetWndWidth( XGUI_WND * hWnd )
{
	return (hWnd->rect.right-hWnd->rect.left+1);
}

//获取窗口高度
int XGUI_GetWndHeight( XGUI_WND * hWnd )
{
	return (hWnd->rect.bottom-hWnd->rect.top+1);
}

//获取窗口自定义参数
void * XGUI_GetWndUsrParam( XGUI_WND * hWnd )
{
	return hWnd->param;
}

//获取桌面窗口真实句柄
XGUI_WND * XGUI_GetDesktop( void )
{
	return hRootWnd;
}

//设置窗口自定义参数
void XGUI_SetWndUsrParam( XGUI_WND * hWnd, void *param )
{
	hWnd->param = param;
}

//设置桌面显示模式
ERRCODE XGUI_SetScreenMode( int width, int height, int mode )
{
	if( screen_set_mode( width, height, mode ) == 0 )
	{
		return ERRCODE_PARAM;
	}
	//改变桌面矩形
	hRootWnd->rect.left = 0;
	hRootWnd->rect.top = 0;
	hRootWnd->rect.right = screen_get_width() - 1;
	hRootWnd->rect.bottom = screen_get_height() - 1;
	//所有窗口标记为"需重画"
	XGUI_SetGlobalRectInval( hRootWnd, &hRootWnd->rect );
	return ERRCODE_NONE;
}
