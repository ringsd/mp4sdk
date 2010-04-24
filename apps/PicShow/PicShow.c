/***************************************************************************

		PicShow.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-03-02 16:52:58

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include "ps_common.h"

//#define printf ui_printf

static void PicShow_ListNext( void );
static void PicShow_ListPrev( void );

static u32 test_malloc( int size, char *msg )
{
	u8 *data = malloc( size );
	printf( "%s heap:%x size:%d\n", msg, data, size );
	free( data );
	return (u32)data;
}


RUN_FLAG run_flag;

u8 * sys_reg_filetype( void )
{
	return "JPG|BMP";
}

KEYMAP pskeymap_right0[] = {
{	PSKEY_UP,	SYSKEY_UP	},
{	PSKEY_DOWN,	SYSKEY_DOWN	},
{	PSKEY_LEFT,	SYSKEY_LEFT	},
{	PSKEY_RIGHT,	SYSKEY_RIGHT	},
{	PSKEY_PAGE_UP,	0	},
{	PSKEY_PAGE_DOWN,	0	},
{	PSKEY_PAGE_LEFT,	0	},
{	PSKEY_PAGE_RIGHT,	0	},
{	PSKEY_ZOOMIN,	SYSKEY_A	},
{	PSKEY_ZOOMOUT,	SYSKEY_B	},
{	PSKEY_PREV,	SYSKEY_C	},
{	PSKEY_NEXT,	SYSKEY_D	},
{	PSKEY_EXIT,	SYSKEY_MENU	},
{	PSKEY_MENU, SYSKEY_POWER	},
{	PSKEY_ZOOM_TYPE_WIDTH,	SYSKEY_MENU | SYSKEY_LEFT	},
{	PSKEY_ZOOM_TYPE_HEIGHT,	SYSKEY_MENU | SYSKEY_UP	},
{	PSKEY_ZOOM_TYPE_ORIGIN,	SYSKEY_MENU | SYSKEY_RIGHT	},
{	PSKEY_ZOOM_STEP,	SYSKEY_B | SYSKEY_A	},
{	0,	0	},
};

KEYMAP pskeymap_right90[] = {
{	PSKEY_UP,	SYSKEY_UP	},
{	PSKEY_DOWN,	SYSKEY_DOWN	},
{	PSKEY_LEFT,	SYSKEY_LEFT	},
{	PSKEY_RIGHT,	SYSKEY_RIGHT	},
{	PSKEY_PAGE_UP,	0	},
{	PSKEY_PAGE_DOWN,	0	},
{	PSKEY_PAGE_LEFT,	0	},
{	PSKEY_PAGE_RIGHT,	0	},
{	PSKEY_ZOOMIN,	SYSKEY_A	},
{	PSKEY_ZOOMOUT,	SYSKEY_B	},
{	PSKEY_PREV,	SYSKEY_C	},
{	PSKEY_NEXT,	SYSKEY_D	},
{	PSKEY_EXIT,	SYSKEY_MENU	},
{	PSKEY_MENU, SYSKEY_POWER	},
{	PSKEY_ZOOM_TYPE_WIDTH,	SYSKEY_MENU | SYSKEY_LEFT	},
{	PSKEY_ZOOM_TYPE_HEIGHT,	SYSKEY_MENU | SYSKEY_UP	},
{	PSKEY_ZOOM_TYPE_ORIGIN,	SYSKEY_MENU | SYSKEY_RIGHT	},
{	PSKEY_ZOOM_STEP,	SYSKEY_B | SYSKEY_A	},
{	0,	0	},
};

KEYMAP pskeymap_right180[] = {
{	PSKEY_UP,	SYSKEY_UP	},
{	PSKEY_DOWN,	SYSKEY_DOWN	},
{	PSKEY_LEFT,	SYSKEY_LEFT	},
{	PSKEY_RIGHT,	SYSKEY_RIGHT	},
{	PSKEY_PAGE_UP,	0	},
{	PSKEY_PAGE_DOWN,	0	},
{	PSKEY_PAGE_LEFT,	0	},
{	PSKEY_PAGE_RIGHT,	0	},
{	PSKEY_ZOOMIN,	SYSKEY_A	},
{	PSKEY_ZOOMOUT,	SYSKEY_B	},
{	PSKEY_PREV,	SYSKEY_C	},
{	PSKEY_NEXT,	SYSKEY_D	},
{	PSKEY_EXIT,	SYSKEY_MENU	},
{	PSKEY_MENU, SYSKEY_POWER	},
{	PSKEY_ZOOM_TYPE_WIDTH,	SYSKEY_MENU | SYSKEY_LEFT	},
{	PSKEY_ZOOM_TYPE_HEIGHT,	SYSKEY_MENU | SYSKEY_UP	},
{	PSKEY_ZOOM_TYPE_ORIGIN,	SYSKEY_MENU | SYSKEY_RIGHT	},
{	PSKEY_ZOOM_STEP,	SYSKEY_B | SYSKEY_A	},
{	0,	0	},
};

KEYMAP pskeymap_right270[] = {
{	PSKEY_UP,	SYSKEY_UP	},
{	PSKEY_DOWN,	SYSKEY_DOWN	},
{	PSKEY_LEFT,	SYSKEY_LEFT	},
{	PSKEY_RIGHT,	SYSKEY_RIGHT	},
{	PSKEY_PAGE_UP,	0	},
{	PSKEY_PAGE_DOWN,	0	},
{	PSKEY_PAGE_LEFT,	0	},
{	PSKEY_PAGE_RIGHT,	0	},
{	PSKEY_ZOOMIN,	SYSKEY_A	},
{	PSKEY_ZOOMOUT,	SYSKEY_B	},
{	PSKEY_PREV,	SYSKEY_C	},
{	PSKEY_NEXT,	SYSKEY_D	},
{	PSKEY_EXIT,	SYSKEY_MENU	},
{	PSKEY_MENU, SYSKEY_POWER	},
{	PSKEY_ZOOM_TYPE_WIDTH,	SYSKEY_MENU | SYSKEY_LEFT	},
{	PSKEY_ZOOM_TYPE_HEIGHT,	SYSKEY_MENU | SYSKEY_UP	},
{	PSKEY_ZOOM_TYPE_ORIGIN,	SYSKEY_MENU | SYSKEY_RIGHT	},
{	PSKEY_ZOOM_STEP,	SYSKEY_B | SYSKEY_A	},
{	0,	0	},
};

typedef struct SLIP_KEY{
u32 normalkey;
u32 slipkey;
int loop;
int limit;
} SLIP_KEY;

SLIP_KEY slipkey[] = {
{	PSKEY_ZOOMIN,	PSKEY_ZOOMIN_SLIP,	0,	0,	},
{	PSKEY_ZOOMOUT,	PSKEY_ZOOMOUT_SLIP,	0,	0,	},
{	PSKEY_UP,		PSKEY_UP_SLIP,		0,	0,	},
{	PSKEY_DOWN,		PSKEY_DOWN_SLIP,	0,	0,	},
{	PSKEY_LEFT,		PSKEY_LEFT_SLIP,	0,	0,	},
{	PSKEY_RIGHT,	PSKEY_RIGHT_SLIP,	0,	0,	},
{	0,			0,	0,	0,	},
};

static u32 PS_slipkey( u32 key )
{
	static u32 last_key = 0;
	int i = 0;
	u32 release, press, hold, out = 0;
	
	release = (last_key ^ key) & ~key;
	hold = (last_key & key);
	press = (last_key ^ key) & key;
	last_key = key;
	
	while( slipkey[i].normalkey )
	{
		if( slipkey[i].normalkey & release )
		{
			slipkey[i].loop = slipkey[i].limit;
			slipkey[i].limit = 0;
		}
		if( slipkey[i].normalkey & hold )
			slipkey[i].limit++;
		if( slipkey[i].loop > 0 )
		{
			slipkey[i].loop--;
			out |= slipkey[i].slipkey;
		}
		i++;
	}
	return out;
}

static u32 PS_slipkey_clear( u32 key )
{
	int i = 0;
	while( slipkey[i].normalkey )
	{
		if( slipkey[i].slipkey & key )
			slipkey[i].loop = 0;
		i++;
	}
	return 0;
}

static u32 PS_readkey( void )
{
	u32 key = sys_get_key();
	KEYMAP * pskeymap;
	
	switch( PicShow_direction )
	{
		case BITMAP_ZOOM_RIGHT0:
			pskeymap = pskeymap_right0;
			break;
		case BITMAP_ZOOM_RIGHT90:
			pskeymap = pskeymap_right90;
			break;
		case BITMAP_ZOOM_RIGHT180:
			pskeymap = pskeymap_right180;
			break;
		case BITMAP_ZOOM_RIGHT270:
			pskeymap = pskeymap_right270;
			break;
		default:		//实际上不可能到这里
			return 0;
	}
	
	key = convert_key( key, pskeymap );

	if( use_slip_key )
		key |= PS_slipkey( key );

	return key;
}

static void PS_waitkeyclear( void )
{
	while( PS_readkey() )
		sys_delay( 10 );
}

void set_zoom_mode( IMAGE_ZOOM * image_zoom, int * width, int * height, int * x, int * y, ZOOM_MODE type )
{
	int w = *width, h = *height;
	switch( type )
	{
		case ZOOM_MODE_HEIGHT:
			if( PicShow_direction == BITMAP_ZOOM_RIGHT0 || PicShow_direction == BITMAP_ZOOM_RIGHT180 )
				h = screen_get_height();
			else
				h = screen_get_width();
			w = image_zoom->bitmap_zoom->width * h / image_zoom->bitmap_zoom->height;
			lprintf( 0, 16, "fit height" );
			break;
		case ZOOM_MODE_ORIGIN:
			w = image_zoom->real_width;
			h = image_zoom->real_height;
			lprintf( 0, 16, "origin" );
			break;
		case ZOOM_MODE_WIDTH:
			if( PicShow_direction == BITMAP_ZOOM_RIGHT0 || PicShow_direction == BITMAP_ZOOM_RIGHT180 )
				w = screen_get_width();
			else
				w = screen_get_height();
			h = w * image_zoom->bitmap_zoom->height / image_zoom->bitmap_zoom->width;
			lprintf( 0, 16, "fit width" );
			break;
		case ZOOM_MODE_50:
			w = image_zoom->real_width / 2;
			h = image_zoom->real_height / 2;
			break;
		case ZOOM_MODE_25:
			w = image_zoom->real_width / 4;
			h = image_zoom->real_height / 4;
			break;
		case ZOOM_MODE_12_5:
			w = image_zoom->real_width / 8;
			h = image_zoom->real_height / 8;
			break;
	}
	*width = w;
	*height = h;

	lprintf( 0, 0,"Width:%d Height:%d Zoom: %d%%\n", 
	image_zoom->real_width, image_zoom->real_height, (w+1)*100/image_zoom->real_width );
}

//显示方向设置
BITMAP_ZOOM_DIRECTION PicShow_direction = BITMAP_ZOOM_RIGHT0;

static void PicShow_SetDirection( int w, int h )
{
	
	switch( PicShow_auto_direction )
	{
		case AUTO_DIRECTION_SAME:
			break;
		case AUTO_DIRECTION_90:
			if( h > w )
				PicShow_direction = BITMAP_ZOOM_RIGHT90;
			else
				PicShow_direction = BITMAP_ZOOM_RIGHT0;
			break;
		case AUTO_DIRECTION_270:
			if( h > w )
				PicShow_direction = BITMAP_ZOOM_RIGHT270;
			else
				PicShow_direction = BITMAP_ZOOM_RIGHT0;
			break;
		default:
			break;
	}
}

//平移步长(像素单位)
#define PIXEL_PITCH 4
//缩放步长 1/x
#define ZOOM_RATE_BASE 200
static void PicShow_Zoom( IMAGE_ZOOM * image_zoom )
{
	int x, y, w, h, sw, sh;
	int room_rate, pixel_pitch_slip;
	int screen_width, screen_height, zoom_state = 0;
	int zoom_start = 0, move_start = 0;
	
	if( use_slip_key )
		room_rate = ZOOM_RATE_BASE, pixel_pitch_slip = 2;			//滑动
	else
		room_rate = ZOOM_RATE_BASE/8, pixel_pitch_slip = PIXEL_PITCH*16;	//普通

	sw = (w = image_zoom->real_width);
	sh = (h = image_zoom->real_height);
	
	PicShow_SetDirection( w, h );
	set_zoom_mode( image_zoom, &w, &h, &x, &y, default_zoom_mode );
	
	screen_width = screen_get_width();
	screen_height = screen_get_height();
	
	//设置居中
	switch( PicShow_direction )
	{
		case BITMAP_ZOOM_RIGHT0:
		case BITMAP_ZOOM_RIGHT180:
			x = -(screen_width*sw/w - sw)/2;
			y = -(screen_height*sh/h - sh)/2;
			break;
		case BITMAP_ZOOM_RIGHT90:
		case BITMAP_ZOOM_RIGHT270:
			y = -(screen_height*sw/w - sw)/2;
			x = -(screen_width*sh/h - sh)/2;
			break;
	}
	
	image_zoom_read( image_zoom, PicShow_direction, lcd_bufferui(), x, y, screen_width, screen_height, w, h, 0 );
	lcd_updateui();
	
	while( 1 )
	{
		u32 key = PS_readkey();
		
		int pixel_pitch = pixel_pitch_slip;
		int y_off = max( pixel_pitch * sh / h, 1 );
		int x_off = max( pixel_pitch * sw / w, 1 );
		int zoom_wp = max( w/room_rate, 1 );
		//printf( "%d\n", x_off );
		if( key == 0 )
		{
			if( room_rate )
			{
				pixel_pitch_slip = PIXEL_PITCH;
				room_rate = ZOOM_RATE_BASE;
			}
			else
			{
				pixel_pitch_slip = PIXEL_PITCH;
				room_rate = ZOOM_RATE_BASE/8;
			}
			sys_delay( 10 );
			if( zoom_state == 0 )
				continue;
		}
		
		if( key & (PSKEY_UP|PSKEY_UP_SLIP) )
		{
			if( y - y_off > -sh*screen_height/h + pixel_pitch )
				y -= y_off;
			PS_slipkey_clear( PSKEY_DOWN_SLIP );
		}
		if( key & (PSKEY_DOWN|PSKEY_DOWN_SLIP) )
		{
			if( y + y_off < sh - pixel_pitch )
				y += y_off;
			PS_slipkey_clear( PSKEY_UP_SLIP );
		}
		if( key & (PSKEY_LEFT|PSKEY_LEFT_SLIP) )
		{
			if( x - x_off > -sw*screen_width/w + pixel_pitch )
				x -= x_off;
			PS_slipkey_clear( PSKEY_RIGHT_SLIP );
		}
		if( key & (PSKEY_RIGHT|PSKEY_RIGHT_SLIP) )
		{
			if( x + x_off < sw - pixel_pitch )
				x += x_off;
			PS_slipkey_clear( PSKEY_LEFT_SLIP );
		}
		
		if( key & (PSKEY_UP_SLIP | PSKEY_DOWN_SLIP | PSKEY_LEFT_SLIP | PSKEY_RIGHT_SLIP) )
		{
			pixel_pitch_slip-=3;
			if( pixel_pitch_slip < 0 )
			{
				pixel_pitch_slip = 0;
				PS_slipkey_clear( (PSKEY_UP_SLIP | PSKEY_DOWN_SLIP | PSKEY_LEFT_SLIP | PSKEY_RIGHT_SLIP) );
			}
		}
		if( key & (PSKEY_UP | PSKEY_DOWN | PSKEY_LEFT | PSKEY_RIGHT) )
		{
			static u32 start_time;
			u32 now_time = sys_ticks();
			u32 time;
			
			if( move_start == 0 )		//第一次按下
			{
				start_time = sys_ticks();
				move_start = 1;
			}
			else
			{
				if( now_time > start_time )
					time = now_time - start_time;
				else
					time = (0xFFFFFFFF - start_time)+now_time+1;
				if( time > 100 )
					pixel_pitch_slip = PIXEL_PITCH * (time)/100;
				//printf( "%d\n", pixel_pitch );
				//room_rate-=2;
				//if( pixel_pitch < PIXEL_PITCH * 4 )
				//	pixel_pitch_slip+=2;
			}
		}
		else
		{
			move_start = 0;			//弹起
		}
		
		if( key & (PSKEY_ZOOMIN | PSKEY_ZOOMIN_SLIP) )
		{
			w += zoom_wp;
			h = w * image_zoom->bitmap_zoom->height / image_zoom->bitmap_zoom->width;
			x += ( sw * screen_width ) / ( 2 * w * (room_rate+1));
			y += ( sh * screen_height ) / ( 2 * h * (room_rate+1));
			PS_slipkey_clear( PSKEY_ZOOMOUT_SLIP );
		}

		if( key & (PSKEY_ZOOMOUT | PSKEY_ZOOMOUT_SLIP) )
		{
			if( w -zoom_wp > 32 )
			{
				w -= zoom_wp;
				h = w * image_zoom->bitmap_zoom->height / image_zoom->bitmap_zoom->width;
				x -= ( sw * screen_width ) / ( 2 * w * (room_rate+1));
				y -= ( sh * screen_height ) / ( 2 * h * (room_rate+1));
			}
			PS_slipkey_clear( PSKEY_ZOOMIN_SLIP );
		}

		if( key & (PSKEY_ZOOMOUT_SLIP | PSKEY_ZOOMIN_SLIP) )
		{
			room_rate +=2;
			if( room_rate > ZOOM_RATE_BASE )
				PS_slipkey_clear( (PSKEY_ZOOMOUT_SLIP | PSKEY_ZOOMIN_SLIP) );
		}
		
		if( key & (PSKEY_ZOOMOUT | PSKEY_ZOOMIN) )
		{
			static u32 start_time;
			u32 now_time = sys_ticks();
			u32 time;
			
			if( zoom_start == 0 )		//第一次按下
			{
				start_time = sys_ticks();
				zoom_start = 1;
			}
			else
			{
				if( now_time > start_time )
					time = now_time - start_time;
				else
					time = (0xFFFFFFFF - start_time)+now_time+1;
				if( time > 100 )
					room_rate = ZOOM_RATE_BASE *100/(time);
				
				//room_rate-=2;
				if( room_rate < ZOOM_RATE_BASE/32 )
					room_rate = ZOOM_RATE_BASE/32;
			}
		}
		else
		{
			zoom_start = 0;			//弹起
		}
		
		if( key & PSKEY_EXIT )
		{
			run_flag = EXIT;
			return;
		}
		
		if( key & PSKEY_MENU )
		{
			switch( PicShow_Menu() )
			{
			case MENU_CMD_EXIT:
				return;
			case MENU_CMD_REZOOM:
				set_zoom_mode( image_zoom, &w, &h, &x, &y, current_zoom_mode );
				break;
			}
		}
		
		if( key & PSKEY_ZOOM_TYPE_WIDTH )
		{
			set_zoom_mode( image_zoom, &w, &h, &x, &y, default_zoom_mode = ZOOM_MODE_WIDTH );
			PS_waitkeyclear();
			key = 0;
		}
		if( key & PSKEY_ZOOM_TYPE_HEIGHT )
		{
			set_zoom_mode( image_zoom, &w, &h, &x, &y, default_zoom_mode = ZOOM_MODE_HEIGHT );
			PS_waitkeyclear();
			key = 0;
		}
		if( key & PSKEY_ZOOM_TYPE_ORIGIN )
		{
			set_zoom_mode( image_zoom, &w, &h, &x, &y, default_zoom_mode = ZOOM_MODE_ORIGIN );
			PS_waitkeyclear();
			key = 0;
		}
		if( key & PSKEY_ZOOM_STEP )
		{
			set_zoom_mode( image_zoom, &w, &h, &x, &y, default_zoom_mode = (default_zoom_mode+1)%ZOOM_MODE_MAX );
			PS_waitkeyclear();
			key = 0;
		}
		
		if( key & PSKEY_PAGE_UP )
		{
		    y -= screen_get_height() * sh / h;
		    menu_get_key();
		}
		
		if( key & PSKEY_PAGE_DOWN )
		{
		    y += screen_get_height() * sh / h;
		    menu_get_key();
		}
		
		if( key & PSKEY_PAGE_LEFT )
		{
		    x -= screen_get_width() * sw / w;
		    menu_get_key();
		}
		
		if( key & PSKEY_PAGE_RIGHT )
		{
		    x += screen_get_width() * sw / w;
		    menu_get_key();
		}
		    
		if( key & PSKEY_NEXT )
		{
			PicShow_ListNext();
			return;
		}
		if( key & PSKEY_PREV )
		{
			PicShow_ListPrev();
			return;
		}
		
		// 限定范围
		if( (int)screen_get_width() * sw / w < sw )
		{
		    if( x + (int)screen_get_width() * sw / w > sw )
		        x = sw - (int)screen_get_width() * sw / w;
		    if( x < 0 )
		        x = 0;
		}
		else
		{
		    if( x + (int)screen_get_width() * sw / w < sw )
		        x = sw - (int)screen_get_width() * sw / w;
		    if( x > 0 )
		        x = 0;
		}
		if( (int)screen_get_height() * sh / h < sh )
		{
		    if( y + (int)screen_get_height() * sh / h > sh )
		        y = sh - (int)screen_get_height() * sh / h;
		    if( y < 0 )
		        y = 0;
		}
		else
		{
		    if( y + (int)screen_get_height() * sh / h < sh )
		        y = sh - (int)screen_get_height() * sh / h;
		    if( y > 0 )
		        y = 0;
		}
		 
		if( key &(PSKEY_LEFT_SLIP | PSKEY_RIGHT_SLIP | PSKEY_UP_SLIP | PSKEY_DOWN_SLIP | 
					PSKEY_LEFT | PSKEY_RIGHT | PSKEY_UP | PSKEY_DOWN ) )
		{
			if( image_zoom->cache_line->blocks < screen_height )
				zoom_state = 1;
			else
				zoom_state = 0;
			image_zoom_read( image_zoom, PicShow_direction, lcd_bufferui(), x, y, screen_width, screen_height, w, h, zoom_state );
			lcd_updateui();
		}
		else if( key &(PSKEY_ZOOMOUT_SLIP | PSKEY_ZOOMIN_SLIP | PSKEY_ZOOMIN | PSKEY_ZOOMOUT) )
		{
			zoom_state = 1;
			image_zoom_read( image_zoom, PicShow_direction, lcd_bufferui(), x, y, screen_width, screen_height, w, h, 1 );
			lprintf( 0, 0,"Width:%d Height:%d Zoom: %d%%\n", sw, sh, (w+1)*100/sw );
		}
		else
		{
			zoom_state = 0;
			image_zoom_read( image_zoom, PicShow_direction, lcd_bufferui(), x, y, screen_width, screen_height, w, h, 0 );
			lcd_updateui();
		}
		sys_delay( 10 );
	}
}

static FILE_LIST * list;
static int file_index;
static char path_startup[MAX_PATH];
static char path_image[MAX_PATH];

static void PicShow_ListOpen( char * file_path )
{
	char fname[MAX_PATH];
	char * p;
	strcpy( path_image, file_path );
	strcpy( path_startup, file_path );
	p = strrchr( path_startup, '\\' );
	if( p == NULL )
		p = strrchr( path_startup, '/' );
	if( p == NULL )
	{
		strcpy( path_startup, ".\\" );
		strcpy( fname ,file_path );
	}
	else
	{
		*p = 0; p++;
		strcpy( fname, p );
		strcat( path_startup, "\\" );
	}
	list = file_list_open( path_startup, "jpg|bmp|JPG|BMP", ~FFD_DIR );
	if( list == NULL ) goto err;
	file_list_sort( list, FILE_LIST_SORT_NAME );
	file_index = file_list_index( list, fname );
	return;
err:
	printf( "list err\n" );
}

static void PicShow_ListClose( void )
{
	if( list )
		file_list_close( list );
}

static void PicShow_ListNext( void )
{
	TRACE_LOG("PicShow_ListNext");
	file_index++;
	if( file_index >= file_list_count( list ) )
		file_index = 0;
	strcpy( path_image, path_startup );
	strcat( path_image, file_list_read( list, file_index )->fname );
}

static void PicShow_ListPrev( void )
{
	char * path;
	TRACE_LOG("PicShow_ListPrev");
	file_index--;
	if( file_index < 0 )
		file_index = file_list_count( list ) - 1;
	TRACE_LOG("");
	strcpy( path_image, path_startup );
	TRACE_LOG("");
	path = file_list_read( list, file_index )->fname;
	TRACE_LOG("");
	strcat( path_image, path );
	TRACE_LOG(path_image);
}

static void PicShow_SingleView( char * fname )
{
	int width, height;
	MM_IMAGE *	image = NULL;
	BITMAP_ZOOM * zoom = NULL;
	IMAGE_ZOOM * image_zoom = NULL;
	
	ui_printf( "Loading: \"%s\" ...\n", fname );
	
	image = MM_image_open( fname );
	if( image == NULL )
		goto err;
	MM_image_size	( image, &width, &height  );
	ui_printf( "Size: %d x %d\n", width, height );

	printf( "Reading...\n" );

	image_zoom = image_zoom_open( image );

	PicShow_Zoom( image_zoom );

	image_zoom_close( image_zoom );
	
	MM_image_close	( image );
	image = NULL;
	
	return;
err:
	if( zoom )
		bitmap_zoom_close( zoom );
	if( image )
		MM_image_close	( image );
	printf( "error: memory not enough.\n" );
	printf( "Press any key to exit.\n" );
	run_flag = EXIT;
	menu_get_key();
	return;
}

char CurrDir[MAX_PATH];

static void save_current_dir( const char * path )
{
	char * point;
	strcpy( CurrDir, path );
	
	point = strrchr( CurrDir, '\\' );	//寻找路径分隔符
	if( point == NULL )
	{	
		point = strrchr( CurrDir, '/' );	//尝试寻找linux分隔符
	}
	if( point )
	{
		*point = 0;
	}
	strcat( CurrDir, "\\" );
	
}

#define MAX_ARGC 256
#define COLOR_BG 0x00000000
#define COLOR_FT 0x00FFFFFF

void picshow_main( char * img_path )
{
	
	u32 key;
	
	save_current_dir( img_path );
	
	DbgCon_SetColor( COLOR_FT, COLOR_BG );
	
	ui_printf( "PicShow V0.33\n" );
	
	PicShow_ListOpen( img_path );
	run_flag = RUN;
	while( run_flag == RUN )
	{
		test_malloc( 0x100, "PicShow_SingleView In" );
		PicShow_SingleView( path_image );
		test_malloc( 0x100, "PicShow_SingleView Out" );
		DbgCon_Reset();
	}
	
	PicShow_ListClose();
}

static void platform_init()
{
	xfont_cache_init();
	textout_init( xfont_default_font, gui_default_font_size, gui_default_line_height, 0, XFONT_STYLE_NORMAL, xfont_default_codepage );
}

static void platform_deinit()
{
	textout_deinit();
	xfont_cache_deinit();
}

static void test_hw_lcd_resizer( void )
{
    int scr_w = screen_get_width();
    int scr_h = screen_get_height();
    
    scr_w = scr_w / 8;
    scr_h = scr_h / 8;
    
    printf( "test_hw_lcd_resizer\n" );
    
    while( scr_w < screen_get_width() )
    {
        screen_set_mode( scr_w, scr_h, LCD_A8R8G8B8 );
        ui_printf( "%d %d %d\n", scr_w, scr_h );
        scr_w += 8;
        scr_h += 4;
        sys_delay(100);
    }
}

void app_main( void )
{
	int argc;
	char *argv[MAX_ARGC];
	char img_path[MAX_PATH];
//	test_hw_lcd_resizer();
//	return;
	
	sys_get_arg( &argc, argv, MAX_ARGC );
	platform_init();
	ps_config_load();
	if( argc == 0 )
	{
		char start_path[MAX_PATH];
		strcpy( start_path, PicDir );
		while( file_explorer_openfile( start_path, "\\.(JPG|jpg|BMP|bmp|PNG|png)$", img_path, "PicShow v0.33" ) == 0 )
		{
			char * p;
			picshow_main( img_path );
			strcpy( start_path, img_path );
			p = strrchr( start_path, '\\' );
			if( p == NULL )
				p = strrchr( start_path, '/' );
			if( p == NULL )
				strcpy( start_path, "" );
			else
				p[1] = 0;
		}
	}
	else
	{
		strcpy( img_path, argv[0] );
		picshow_main( img_path );
	}
	ps_config_save();
	platform_deinit();
}
