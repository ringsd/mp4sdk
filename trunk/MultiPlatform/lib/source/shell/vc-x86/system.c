/***************************************************************************

		system.c

		平台相关的接口集合.

		TIME LIST:
		CREATE	Rainy	2008-10-17 22:06:53

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include <global.h>
#include <direct.h>
#include <Windows.h>

#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 320

u16* lcd_buffer16( void )
{
	return lcd_get_frame();
}

u32* lcd_buffer24( void )
{
	return lcd_get_cframe();
}

u32* lcd_bufferui( void )
{
	return lcd_buffer24();
}

void lcd_update24( void )
{
	lcd_flip();
}

void lcd_update16( void )
{
	lcd_set_frame();
}

void lcd_update16_sync( void )
{
	lcd_set_frame();
}

void lcd_updateui( void )
{
	lcd_flip();
}

void lcd_clearui( u32 color )
{
	u32 x, y;
	u32 height = screen_get_height();
	u32	width = screen_get_width();
	u32 *dst = lcd_bufferui();
	for( y = 0; y < height; y++ )
	{
		for( x = 0; x < width; x+=8 )
		{
			dst[0] = color;
			dst[1] = color;
			dst[2] = color;
			dst[3] = color;
			dst[4] = color;
			dst[5] = color;
			dst[6] = color;
			dst[7] = color;
			dst += 8;
		}
	}
}

void lcd_clear32( u32 color )
{
	memset( lcd_get_cframe() , color , screen_get_width() * screen_get_height()* 4 );
}

u32 lcd_get_mode( void )
{
	return 32;
}

void sys_volume( int volume )
{
}

u32 sys_ticks( void )
{
	return get_ticks_count();
}

u32 sys_get_key( void )
{
	return kbd_get_key();
}

void sys_delay( u32 time )
{
	delay_ms(time);
}

void sys_event( void )
{
	w32_event();
}

s32 sys_sound_init( u32 freq , u32 bit , u32 channel )
{
	return w32waveout_init( freq , bit , channel );
}

void sys_sound_deinit( void )
{
	w32waveout_close();
}

u32 sys_sound_write( void *data , u32 length )
{
	w32waveout_write( data, length );
	return length;
}

s32 sys_sound_canwrite( void )
{
	return 1;
}

s32 mkdir( u8 *path )
{
	return _mkdir( path );
}

u8 *sys_get_dir( u8 *buf , int maxlen )
{
	_getcwd( buf, maxlen );
	strcat( buf, "\\" );
	return buf;
}

#define FILE_NAME_CONFIG  "win32.ini"
char config_path[ MAX_PATH ];

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

int sys_get_startupdir( char * path, int max_len )
{
	sys_get_path( path, MAX_PATH );
	path2dir( path );
}

s32 sys_init( void )
{
	char path[MAX_PATH];
	sys_get_startupdir( path, MAX_PATH );
	//chdir( path );
	win32_config_load(FILE_NAME_CONFIG);
	return 0;
}

s32 sys_deinit( void )
{
	char path[MAX_PATH];
	sys_get_startupdir( path, MAX_PATH );
	chdir( path );
	return 0;
}

int screen_set_backlight( int value )
{
	return 0;
}

void sys_get_time( SYS_TIME * mtime )
{
    time_t atime;
    struct tm *tblock;
    
    time( &atime );
    tblock = localtime(&atime);
    
    mtime->year = tblock->tm_year + 1900;
    mtime->month = tblock->tm_mon;
    mtime->day = tblock->tm_mday;
    mtime->hour = tblock->tm_hour;
    mtime->minute = tblock->tm_min;
    mtime->second = tblock->tm_sec;
}
