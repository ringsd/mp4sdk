/***************************************************************************

		file_explorer.c

		一个简单的界面.

		TIME LIST:
		CREATE	Rainy	2009-04-14 21:08:13
		加入触摸支持
		
		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#define _file_explorer_h_
#include <global.h>
#undef _file_explorer_h_
#include "file_explorer.h"


#define LINE_HEIGHT gui_default_line_height             //行高
#define FONT_WIDTH (gui_default_font_size/2)             //字节宽
#define COLOR_ADDRBAR_FONT  0x00FFFF
#define COLOR_ADDRBAR_BG    0x000000
#define COLOR_CUSTOMMSG_FONT  0xFFFFFF
#define COLOR_CUSTOMMSG_BG    0xFF000000
#define COLOR_FONT  0xFFFFFF
#define COLOR_BG    0x000000
#define COLOR_DIR    0x00FF00
#define LOCY_ADDRBAR LINE_HEIGHT				//地址栏Y坐标
#define LOCY_CUSTOMMSG    0              //用户自定义消息栏Y坐标
#define LOCY_FILES   (gui_default_line_height+4+LINE_HEIGHT)				//文件栏Y坐标
#define LOCX_LEFT    0              //整个界面的起始x坐标
#define FILE_SIZE_DIV    2/3        //文件尺寸在列表水平比例

//#define CUSTOMMSG_BOTTOM    LINE_HEIGHT              //用户自定义消息栏距离屏幕底部的距离

static void rect_highlight( int sx, int sy, int w, int h )
{
	int width = screen_get_width();
	u32 * screen = lcd_bufferui()+width*sy+sx;
	int x, y;
	for( y = 0; y < h; y++ )
	{
		for( x = 0; x < w; x++ )
		{
			*screen |= 0x003f3f3f;
			screen++;
		}
		screen += width-w;
	}
}

static void draw_title_bar( void )
{
	int x, y;
	int width = screen_get_width();
	u32 * dst = lcd_bufferui() + LOCY_CUSTOMMSG * width;
	for( y = 0; y < LINE_HEIGHT; y++ )
	{
		for( x = 0; x < width; x++ )
		{
			*dst++ = 0x0000FF * (width-x) / width;
		}
	}
}

//计算指定项目行的Y坐标
static int ListGetYPos( int num )
{
	return LOCY_FILES+num*LINE_HEIGHT;
}

//计算Y坐标对应的项目行
static int ListGetNum( int ypos )
{
	return (ypos - LOCY_FILES)/LINE_HEIGHT;
}

//计算拖动距离对应的项目数
static int DistGetNum( int dist )
{
	return dist/LINE_HEIGHT;
}

//判断指定Y坐标是否是路径栏
static int ListIsPathBar( int ypos )
{
	if( ypos >= LOCY_ADDRBAR && ypos < LOCY_ADDRBAR+LINE_HEIGHT )
		return 1;
	return 0;
}

static void file_explorer_list_draw( FILE_LIST * flist, const char * path, int select, int start, int max_items, const char * custom_msg )
{
	int i;
	int width = screen_get_width();
	int height = screen_get_height();
	int path_str_offset;		//路径显示的偏移
	FILE_LIST_DATA * fdata;
	path_str_offset = strlen( path )-width/FONT_WIDTH;
	if( path_str_offset < 0 )
		path_str_offset = 0;
	lcd_clearui(COLOR_BG);
	textout( path+path_str_offset, LOCX_LEFT, LOCY_ADDRBAR, width, height, COLOR_ADDRBAR_FONT, COLOR_ADDRBAR_BG );
	for( i = 0; i < max_items; i++ )
	{
		fdata = file_list_read( flist, i+start );
		if( fdata == NULL )
			break;
		//printf( "%d %s\n", i, fdata->fname );
		if( fdata->attrib & (FFD_DIR|FFD_VOLUME) )
			textout_line( fdata->fname, LOCX_LEFT, ListGetYPos(i), width, height, COLOR_DIR, COLOR_BG );
		else
		{
			char buf[32];
			textout_line( fdata->fname, LOCX_LEFT, ListGetYPos(i), LOCX_LEFT+width*FILE_SIZE_DIV, height, COLOR_FONT, COLOR_BG );		//文件名
			sprintf( buf, "%d Bytes", fdata->size );
			textout_line( buf, LOCX_LEFT+width*FILE_SIZE_DIV, ListGetYPos(i), width, height, COLOR_FONT, COLOR_BG );		//大小
		}
	}
	rect_highlight( LOCX_LEFT, LOCY_FILES+(select-start)*LINE_HEIGHT, width-LOCX_LEFT, LINE_HEIGHT );
	draw_title_bar();
	textout( custom_msg, LOCX_LEFT, LOCY_CUSTOMMSG, width, height, COLOR_CUSTOMMSG_FONT, COLOR_CUSTOMMSG_BG );
	lcd_updateui();
}

typedef enum {
EXPLORER_TYPE_NEWDIR,
EXPLORER_TYPE_SELECT,
EXPLORER_TYPE_ERROR,
} EXPLORER_TYPE;

static EXPLORER_TYPE path_up_dir( char * path, char * return_path )
{
			char * t;
			strcpy( return_path, path );
			t = strrchr( return_path, '\\' );
			if( t == NULL )
				t = strrchr( return_path, '/' );
			if( t == NULL )
			{
				return EXPLORER_TYPE_ERROR;
			}
			if( t == strchr( return_path, '\\' ) || t == strchr( return_path, '/' ) )
			{
				*return_path = 0;
				return EXPLORER_TYPE_NEWDIR;
			}
			*t = 0;
			t = strrchr( return_path, '\\' );
			if( t == NULL )
				t = strrchr( return_path, '/' );
			if( t )
				*(t+1) = 0;
			return EXPLORER_TYPE_NEWDIR;
}

//判断路径是否目录,返回1是成功
static int IsDir( const char * path )
{
	const char * p;
	
	if( *path == '\0' )					//空字串也是合法的路径
		return 1;
		
	p = path + strlen(path) - 1;		//取最后一个字符
	
	if( *p == '\\' || *p == '/' || *p == ':' )
	{
		return 1;
	}
	return 0;
}

//从一个url中切取路径部分和文件部分
//返回0成功
//path和fname的区域必须足够放下数据
static int CutPath( const char * url, char * path, char * fname )
{
	char * p;
	strcpy( path, url );
	p = strrchr( path, '\\' );
	if( p == NULL )
	{
		p = strrchr( url, '/' );
		if( p == NULL )
			goto err;
	}
	strcpy( fname, p+1 );
	p[1] = 0;
	if( p[2] == 0 )
		goto err;
	return 0;
err:
	return -1;
}

//资源管理器触摸状态

typedef enum TOUCH_STATE {
	TOUCH_STATE_NONE,	//无触摸事件
	TOUCH_STATE_DOWN,	//触摸按下
	TOUCH_STATE_SELECT,	//选定坐标
	TOUCH_STATE_VDRAG, //垂直拖动
	TOUCH_STATE_RMOVE, //向右拖动手势
	TOUCH_STATE_MAX
} TOUCH_STATE;

#define TOUCH_CLICK_RANGE 4 //单击判断范围
#define TOUCH_MOVE_RANGE 32 //方向手势判断范围

static TOUCH_STATE GetTouchState( int * data )
{
	int pointer_x, pointer_y, pointer_press;
	static int	pointer_lastx = 0, pointer_lasty = 0,
				pointer_downx = 0, pointer_downy = 0;
	static int isdown = 0, isdrag = 0;
	int null_data;
	TOUCH_STATE state = TOUCH_STATE_NONE;
	
	if( data == NULL )
		data = &null_data;
		
	if( sys_get_pointer( &pointer_x, &pointer_y, &pointer_press ) == 0 )
	{
		if( pointer_press > POINTER_PRESS_MIN )
		{
			if( isdown == 0 )
			{
				pointer_downx = pointer_x;
				pointer_downy = pointer_y;
				*data = pointer_y;
				state = TOUCH_STATE_DOWN;
				isdrag = 0;
				isdown = 1;
			}
			
			else if( pointer_lasty > 0 && pointer_lastx > 0 )
			{
				if( abs(pointer_downy - pointer_y) > TOUCH_CLICK_RANGE )
				{
					*data = pointer_y - pointer_downy;
					state = TOUCH_STATE_VDRAG;
					isdrag = 1;
				}
			}
			
			pointer_lastx = pointer_x;
			pointer_lasty = pointer_y;
		}
		else
		{
			if( isdown == 1 )
			{
				isdown = 0;
				if( abs(pointer_downy - pointer_lasty) <= TOUCH_CLICK_RANGE &&
					abs(pointer_downx - pointer_x) <= TOUCH_CLICK_RANGE && isdrag == 0 )
				{
					*data = pointer_downy; 
					state = TOUCH_STATE_SELECT;
				}
				else if( abs(pointer_downy - pointer_lasty) <= TOUCH_MOVE_RANGE &&
						 pointer_x - pointer_downx > TOUCH_MOVE_RANGE )
				{
					*data = pointer_x;
					state = TOUCH_STATE_RMOVE;
				}
			}
		}
		
	}
	return state;
}

//等待按键与触摸事件
static void touch_get_key( u32 * key, TOUCH_STATE * touch_state, int * touch_data )
{
	int nul_key, nul_touch_data;
	TOUCH_STATE nul_touch_state;
	if( key == NULL )
		key = &nul_key;
	if( touch_state == NULL )
		touch_state = &nul_touch_state;
	if( touch_data == NULL )
		touch_data = &nul_touch_data;
	if( sys_get_key() )
		sys_delay(200);
	while( 1 )
	{
		*key = sys_get_key();
		*touch_state = GetTouchState(touch_data);
		if( *key || *touch_state != TOUCH_STATE_NONE )
		{
			return;
		}
		sys_delay(10);
	}
}

static EXPLORER_TYPE file_explorer_list( char * path, char * filter, char * return_path, const char * custom_msg, int (*func)(char *path) )
{
	FILE_LIST * flist;
	EXPLORER_TYPE type;
	int max_items = (screen_get_height()-LOCY_FILES-LOCY_CUSTOMMSG)/LINE_HEIGHT;
	
	u32 key = 0;
	int touch_data = 0;
	TOUCH_STATE touch_state = TOUCH_STATE_NONE;
	int hold_start = 0;
	
	int select = 0;
	int start = 0;
	int max_select;
	int init_draw = 0;
	
	if( IsDir( path ) )
	{
		flist = file_list_open( path, filter, FFD_ALL );
		if( flist )
			file_list_sort( flist, FILE_LIST_SORT_NAME );
	}
	else		//path输入的是完整的文件路径,隐含的意思是先打开路径,再定位光标到文件上
	{
		char dir[MAX_PATH];
		char fname[MAX_PATH];
		if( CutPath( path, dir, fname ) != 0 )
			return EXPLORER_TYPE_ERROR;
		flist = file_list_open( dir, filter, FFD_ALL );
		file_list_sort( flist, FILE_LIST_SORT_NAME );
		select = file_list_index( flist, fname );
		start = select;
		strcpy( path, dir );
	}
	
	if( flist == NULL )
	{
		file_explorer_list_draw( NULL, path, 0, 0, 0, custom_msg );
		key = 0;
		while( 1 )
		{
			touch_get_key(&key,&touch_state,NULL);
			if( (key & (SYSKEY_A|SYSKEY_B)) ||
				touch_state == TOUCH_STATE_SELECT ||
				touch_state == TOUCH_STATE_RMOVE
			)
				 break;
			sys_delay(20);
		}
		return path_up_dir( path, return_path );
	}
	max_select = file_list_count( flist );
	
	while( 1 )
	{
		switch( touch_state )
		{
			case TOUCH_STATE_SELECT:
				if( ListIsPathBar(touch_data) )
					key |= SYSKEY_B;
				else if( start + ListGetNum(touch_data) < max_select )
					key |= SYSKEY_A;
				break;
			case TOUCH_STATE_RMOVE:
				key |= SYSKEY_B;
				break;
			case TOUCH_STATE_DOWN:
				if( start + ListGetNum(touch_data) < max_select &&
					start + ListGetNum(touch_data) >= 0 )
				{
					select = start + ListGetNum(touch_data);
				}
				hold_start = start;
				break;
			case TOUCH_STATE_VDRAG:
				start = hold_start - DistGetNum(touch_data);
				if( start < 0 )
					start = 0;
				if( start >= max_select )
					start = max_select-1;
				if( select < start )
					select = start;
				if( select - start >= max_items )
					select = start + max_items - 1;
				break;
		}
		//printf( "%d\n", touch_data );
		if( key & SYSKEY_UP )
		{
			select--;
			if( select < 0 )
			{
				select = max_select-1;
				if( select >= max_items )
					start = max_select-max_items;
				else
					start = 0;
			}
			if( select < start )
				start--;
		}
		if( key & SYSKEY_DOWN )
		{
			select++;
			if( select >= max_select )
			{
				select = 0;
				start = 0;
			}
			if( select-start >= max_items )
				start++;
		}
		if( key & SYSKEY_LEFT )
		{
			select-=max_items;
			if( select < 0 )
			{
				select = max_select-1;
				if( select >= max_items )
					start = max_select-max_items;
				else
					start = 0;
			}
			if( select < start )
				start=select;
			
		}
		if( key & SYSKEY_RIGHT )
		{
			select+=max_items;
			if( select >= max_select )
			{
				select = 0;
				start = 0;
			}
			if( select-start >= max_items )
				start=select-max_items+1;
		}
		if( key & SYSKEY_A )
		{
			FILE_LIST_DATA * fdata = file_list_read( flist, select );
			
			if( fdata->attrib & (FFD_DIR | FFD_VOLUME) )
			{
				type = EXPLORER_TYPE_NEWDIR;
				sprintf( return_path, "%s%s\\", path, fdata->fname );
			}
			else
			{
				type = EXPLORER_TYPE_SELECT;
				sprintf( return_path, "%s%s", path, fdata->fname );
			}
			if( func == NULL || type == EXPLORER_TYPE_NEWDIR )	//没有回调函数,直接返回
				break;
			if( func( return_path ) == -1 )		//使用回调
				break;
		}
		if( key & SYSKEY_B )
		{
			char * t;
			strcpy( return_path, path );
			t = strrchr( return_path, '\\' );
			if( t == NULL )
				t = strrchr( return_path, '/' );
			if( t == NULL )
			{
				type = EXPLORER_TYPE_ERROR;
				break;
			}
			if( t == strchr( return_path, '\\' ) || t == strchr( return_path, '/' ) )
			{
				*return_path = 0;
				type = EXPLORER_TYPE_NEWDIR;
				break;
			}
			*t = 0;
			t = strrchr( return_path, '\\' );
			if( t == NULL )
				t = strrchr( return_path, '/' );
			if( t )
				*(t+1) = 0;
			type = EXPLORER_TYPE_NEWDIR;
			break;
		}
		
		if( key != 0 || touch_state != TOUCH_STATE_NONE || init_draw == 0 )
		{
			init_draw = 1;
			file_explorer_list_draw( flist, path, select, start, max_items, custom_msg );
		}
		
		touch_get_key( &key, &touch_state, &touch_data );
	}
	file_list_close( flist );
	return type;
}

int file_explorer_openfile( char * path, char * filter,char * return_path, const char * custom_msg )
{
	char next_path[MAX_PATH];
	char now_path[MAX_PATH];
	EXPLORER_TYPE type;
	strcpy( now_path, path );
	while( (type = file_explorer_list( now_path, filter, next_path, custom_msg, NULL )) == EXPLORER_TYPE_NEWDIR )
	{
		strcpy( now_path, next_path );
	}
	if( type == EXPLORER_TYPE_SELECT )
	{
		strcpy( return_path, next_path );
		return 0;
	}
	return -1;
}


//用回调的方式打开文件,在用户选定文件后,回调函数就被调用
//int (*func)(char *path)
//path是用户选定的文件路径
//返回值0----继续浏览文件
//		1----退出文件浏览
int file_explorer_openfile_shell( char * path, char * filter,char * return_path, const char * custom_msg, int (*func)(char *path) )
{
	char next_path[MAX_PATH];
	char now_path[MAX_PATH];
	EXPLORER_TYPE type;
	strcpy( now_path, path );
	while( (type = file_explorer_list( now_path, filter, next_path, custom_msg, func )) == EXPLORER_TYPE_NEWDIR )
	{
		strcpy( now_path, next_path );
	}
	if( type == EXPLORER_TYPE_SELECT )
	{
		strcpy( return_path, next_path );
		return 0;
	}
	return -1;
}

#if 0
void app_main( void )
{
	char return_path[MAX_PATH];
	screen_set_mode( 480, 272, LCD_A8R8G8B8 );
	file_explorer_openfile( "c:\\", ".*", return_path );
	printf( "get: %s\n", return_path );
}
#endif



