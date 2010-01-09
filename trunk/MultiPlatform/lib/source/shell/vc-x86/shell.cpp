// shell.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "MMSYSTEM.H"
#include <stdio.h>
#include <direct.h>
#include "types.h"
#include <joypad.h>
#include <system.h>
#include <Winbase.h>
#include <Windows.h>
#include <Windowsx.h>
#include "file_shell.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void Update_ClientRect16();
void Update_ClientRect32();
void (*Update_ClientRect)( void ) = Update_ClientRect32;


#define MAX_SCR_WIDTH 800
#define MAX_SCR_HEIGHT 600

u8 Video_Buffer[ MAX_SCR_WIDTH * MAX_SCR_HEIGHT * 4 ];
u8 Video_Buffer32[ MAX_SCR_WIDTH * MAX_SCR_HEIGHT * 4 ];

HWND Main_hWnd;

char ScreenBuffer[MAX_SCR_WIDTH*MAX_SCR_HEIGHT*4];
int cx,cy;
HBITMAP hScreenBuffer;
HDC hdcMem;
extern "C" void app_main( void );

void Set_Window_Size( u32 width, u32 height )
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(Main_hWnd, &ps);
	hdcMem=CreateCompatibleDC(hdc);
	hScreenBuffer=CreateCompatibleBitmap(hdc, width, height);
	SelectObject(hdcMem,hScreenBuffer);
	EndPaint(Main_hWnd, &ps);
	RECT wr;
	wr.bottom = height;
	wr.left = 0;
	wr.right = width;
	wr.top = 0;
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(
  Main_hWnd,             // handle to window
  HWND_TOP,  // placement-order handle
  0,                 // horizontal position
  0,                 // vertical position
  wr.right-wr.left,wr.bottom-wr.top,
  SWP_ASYNCWINDOWPOS | SWP_NOMOVE          // window-positioning options
);
}

extern "C" {

static int Mouse_xPos = 0;
static int Mouse_yPos = 0;
static int Mouse_Press = 0;
int sys_get_pointer( int *x, int *y, int *press )
{
	*x = Mouse_xPos;
	*y = Mouse_yPos;
	*press = Mouse_Press;
	return 0;
}

static char shell_file[MAX_PATH];

void sys_get_arg( int *argc,char *argv[] , int maxargc )
{
#ifndef __MULTIPLATFORM_VC8_NOARG__
	s8 *find;
	s32 i;
	*argc = 1;
	find = GetCommandLine();
	
	//跳过第一个参数(程序本身路径)
	while( strchr( find , '\"' ) != NULL )
	{
		find = strchr( find , '\"' );
		if( find == NULL )
			break;
		find++;
	}
	
	if( *find )
	{
		//循环读取参数
		for( i = 0; i < maxargc; i++ )
		{
			find++;
			argv[ i ] = find;
			find = strchr( find , ' ' );
			if( find == NULL )
				break;
			*find = 0;
		}
		if( *argv[0] != 0 )
		{
			*argc = i+1;
			return;
		}
	}

	//缺少输入参数,使用打开文件对话框获取
	*argc = 1;
	argv[ 0 ] = shell_file;
	if( Win32_Get_File( shell_file, (char *)sys_reg_filetype() ) == 0 )
		*argc = 0;
	sys_delay(100);
#else
	*argc = 0;
#endif
}

u8 *sys_get_path( u8 *buf , int maxlen )
{
	char data[MAX_PATH];
	char * find = data;
	strcpy( (char *)find, GetCommandLine() );
	find = strchr( find , '\"' ) + 1;
	*strchr( find , '\"' ) = 0;
	strncpy( (char*)buf, find , maxlen );
	return buf;
}

static u32 scr_width = 800;
static u32 scr_height = 600;

static u32 color_mode = LCD_A8R8G8B8;

//设置屏幕模式
//返回0 --失败(不支持的色彩模式或者宽度高度)
//返回1 --设置成功
s32 screen_set_mode( u32 width, u32 height, u32 colormode )
{
	if( width > MAX_SCR_WIDTH )
		return 0;
	if( height > MAX_SCR_HEIGHT )
		return 0;
	scr_width = width;
	scr_height = height;
	Set_Window_Size( scr_width, scr_height );
	switch( colormode )
	{
	case LCD_A8R8G8B8:
		color_mode = LCD_A8R8G8B8;
		return 1;
	default:
		return 0;
	}

	
}

//返回屏幕宽度
u32 screen_get_width( void )
{
	return scr_width;
}

//返回屏幕高度
u32 screen_get_height( void )
{
	return scr_height;
}

u8	*lcd_get_frame(void)
{
	return Video_Buffer;
}

u8	*lcd_get_cframe(void)
{
	return Video_Buffer32;
}

void	lcd_flip(void)
{
	RECT rt;
	Update_ClientRect = Update_ClientRect32;
	GetClientRect(Main_hWnd, &rt);
	InvalidateRect(Main_hWnd,&rt,FALSE);
}

void	lcd_set_frame(void)
{
	RECT rt;
	Update_ClientRect = Update_ClientRect16;
	GetClientRect(Main_hWnd, &rt);
	InvalidateRect(Main_hWnd,&rt,FALSE);
}

#define   KEY_DOWN(vk_code)   ((GetAsyncKeyState(vk_code)   &   0x8000)   ?   1   :   0)

u32 kbd_get_key( void )
{
	u32 key;
	key=0;
	if(KEY_DOWN('W'))
		key|=SYSKEY_UP;
	if(KEY_DOWN('S'))
		key|=SYSKEY_DOWN;
	if(KEY_DOWN('A'))
		key|=SYSKEY_LEFT;
	if(KEY_DOWN('D'))
		key|=SYSKEY_RIGHT;
	if(KEY_DOWN('U'))
		key|=SYSKEY_D;
	if(KEY_DOWN('I'))
		key|=SYSKEY_C;
	if(KEY_DOWN('J'))
		key|=SYSKEY_B;
	if(KEY_DOWN('K'))
		key|=SYSKEY_A;
	if(KEY_DOWN('V'))
		key|=SYSKEY_SELECT;
	if(KEY_DOWN('B'))
		key|=SYSKEY_START;
	if(KEY_DOWN('C'))
		key|=SYSKEY_ESCAPE;
	if(KEY_DOWN('P'))
		key|=SYSKEY_POWER;
	return key;
}

u16 keymap[256]={
0,// VK_NONE		        0x00
0,// VK_LBUTTON        0x01
0,// VK_RBUTTON        0x02
0,// VK_CANCEL         0x03
0,// VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */
0,// VK_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
0,// VK_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */
0,// VK_NONE		        0x07
KSYM_BACKSPACE,// VK_BACK           0x08
0,// VK_TAB            0x09
0,// VK_NONE           0x0A
0,// VK_NONE           0x0B
0,// VK_CLEAR          0x0C
KSYM_RETURN,// VK_RETURN         0x0D
0,// VK_NONE		        0x0E
0,// VK_NONE		        0x0F
0,// VK_SHIFT          0x10
0,// VK_CONTROL        0x11
0,// VK_MENU           0x12
0,// VK_PAUSE          0x13
0,// VK_CAPITAL        0x14
0,// VK_HANGUL         0x15
0,// VK_NONE           0x16
0,// VK_JUNJA          0x17
0,// VK_FINAL          0x18
0,// VK_HANJA          0x19
0,// VK_KANJI          0x1A
KSYM_ESCAPE,// VK_ESCAPE         0x1B
0,// VK_CONVERT        0x1C
0,// VK_NONCONVERT     0x1D
0,// VK_ACCEPT         0x1E
0,// VK_MODECHANGE     0x1F
KSYM_SPACE,// VK_SPACE          0x20
0,// VK_PRIOR          0x21
0,// VK_NEXT           0x22
0,// VK_END            0x23
0,// VK_HOME           0x24
KSYM_LEFT,// VK_LEFT           0x25
KSYM_UP,// VK_UP             0x26
KSYM_RIGHT,// VK_RIGHT          0x27
KSYM_DOWN,// VK_DOWN           0x28
0,// VK_SELECT         0x29
0,// VK_PRINT          0x2A
0,// VK_EXECUTE        0x2B
0,// VK_SNAPSHOT       0x2C
0,// VK_INSERT         0x2D
0,// VK_DELETE         0x2E
0,// VK_HELP           0x2F
KSYM_0,// VK_NONE						0x30
KSYM_1,// VK_NONE           0x31
KSYM_2,// VK_NONE           0x32
KSYM_3,// VK_NONE           0x33
KSYM_4,// VK_NONE           0x34
KSYM_5,// VK_NONE           0x35
KSYM_6,// VK_NONE           0x36
KSYM_7,// VK_NONE           0x37
KSYM_8,// VK_NONE           0x38
KSYM_9,// VK_NONE           0x39
0,// VK_NONE           0x3A
0,// VK_NONE           0x3B
0,// VK_NONE           0x3C
0,// VK_NONE           0x3D
0,// VK_NONE           0x3E
0,// VK_NONE           0x3F
0,// VK_NONE						0x40
KSYM_a,// VK_NONE           0x41
KSYM_b,// VK_NONE           0x42
KSYM_c,// VK_NONE           0x43
KSYM_d,// VK_NONE           0x44
KSYM_e,// VK_NONE           0x45
KSYM_f,// VK_NONE           0x46
KSYM_g,// VK_NONE           0x47
KSYM_h,// VK_NONE           0x48
KSYM_i,// VK_NONE           0x49
KSYM_j,// VK_NONE           0x4A
KSYM_k,// VK_NONE           0x4B
KSYM_l,// VK_NONE           0x4C
KSYM_m,// VK_NONE           0x4D
KSYM_n,// VK_NONE           0x4E
KSYM_o,// VK_NONE           0x4F
KSYM_p,// VK_NONE						0x50
KSYM_q,// VK_NONE           0x51
KSYM_r,// VK_NONE           0x52
KSYM_s,// VK_NONE           0x53
KSYM_t,// VK_NONE           0x54
KSYM_u,// VK_NONE           0x55
KSYM_v,// VK_NONE           0x56
KSYM_w,// VK_NONE           0x57
KSYM_x,// VK_NONE           0x58
KSYM_y,// VK_NONE           0x59
KSYM_z,// VK_NONE           0x5A
0,// VK_LWIN           0x5B
0,// VK_RWIN           0x5C
0,// VK_APPS           0x5D
0,// VK_NONE           0x5E
0,// VK_SLEEP          0x5F
0,// VK_NUMPAD0        0x60
0,// VK_NUMPAD1        0x61
0,// VK_NUMPAD2        0x62
0,// VK_NUMPAD3        0x63
0,// VK_NUMPAD4        0x64
0,// VK_NUMPAD5        0x65
0,// VK_NUMPAD6        0x66
0,// VK_NUMPAD7        0x67
0,// VK_NUMPAD8        0x68
0,// VK_NUMPAD9        0x69
0,// VK_MULTIPLY       0x6A
0,// VK_ADD            0x6B
0,// VK_SEPARATOR      0x6C
0,// VK_SUBTRACT       0x6D
0,// VK_DECIMAL        0x6E
0,// VK_DIVIDE         0x6F
0,// VK_F1             0x70
0,// VK_F2             0x71
0,// VK_F3             0x72
0,// VK_F4             0x73
0,// VK_F5             0x74
0,// VK_F6             0x75
0,// VK_F7             0x76
0,// VK_F8             0x77
0,// VK_F9             0x78
0,// VK_F10            0x79
0,// VK_F11            0x7A
0,// VK_F12            0x7B
0,// VK_F13            0x7C
0,// VK_F14            0x7D
0,// VK_F15            0x7E
0,// VK_F16            0x7F
0,// VK_F17            0x80
0,// VK_F18            0x81
0,// VK_F19            0x82
0,// VK_F20            0x83
0,// VK_F21            0x84
0,// VK_F22            0x85
0,// VK_F23            0x86
0,// VK_F24            0x87
0,// VK_NONE           0x88
0,// VK_NONE           0x89
0,// VK_NONE           0x8A
0,// VK_NONE           0x8B
0,// VK_NONE           0x8C
0,// VK_NONE           0x8D
0,// VK_NONE           0x8E
0,// VK_NONE           0x8F
0,// VK_NUMLOCK        0x90
0,// VK_SCROLL         0x91
0,// VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad#define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
0,// VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
0,// VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
0,// VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
0,// VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key
0,// VK_NONE           0x97
0,// VK_NONE           0x98
0,// VK_NONE           0x99
0,// VK_NONE           0x9A
0,// VK_NONE           0x9B
0,// VK_NONE           0x9C
0,// VK_NONE           0x9D
0,// VK_NONE           0x9E
0,// VK_NONE           0x9F
KSYM_LSHIFT,// VK_LSHIFT         0xA0
KSYM_RSHIFT,// VK_RSHIFT         0xA1
0,// VK_LCONTROL       0xA2
0,// VK_RCONTROL       0xA3
0,// VK_LMENU          0xA4
0,// VK_RMENU          0xA5
0,// VK_BROWSER_BACK        0xA6
0,// VK_BROWSER_FORWARD     0xA7
0,// VK_BROWSER_REFRESH     0xA8
0,// VK_BROWSER_STOP        0xA9
0,// VK_BROWSER_SEARCH      0xAA
0,// VK_BROWSER_FAVORITES   0xAB
0,// VK_BROWSER_HOME        0xAC
0,// VK_VOLUME_MUTE         0xAD
0,// VK_VOLUME_DOWN         0xAE
0,// VK_VOLUME_UP           0xAF
0,// VK_MEDIA_NEXT_TRACK    0xB0
0,// VK_MEDIA_PREV_TRACK    0xB1
0,// VK_MEDIA_STOP          0xB2
0,// VK_MEDIA_PLAY_PAUSE    0xB3
0,// VK_LAUNCH_MAIL         0xB4
0,// VK_LAUNCH_MEDIA_SELECT 0xB5
0,// VK_LAUNCH_APP1         0xB6
0,// VK_LAUNCH_APP2         0xB7
0,// VK_NONE				         0xB8
0,// VK_NONE				         0xB9
KSYM_SEMICOLON,// VK_OEM_1          0xBA   // ';:' for US
0,// VK_OEM_PLUS       0xBB   // '+' any country
0,// VK_OEM_COMMA      0xBC   // ',' any country
0,// VK_OEM_MINUS      0xBD   // '-' any country
0,// VK_OEM_PERIOD     0xBE   // '.' any country
0,// VK_OEM_2          0xBF   // '/?' for US
0,// VK_OEM_3          0xC0   // '`~' for US
0,// VK_NONE           0xC1
0,// VK_NONE           0xC2
0,// VK_NONE           0xC3
0,// VK_NONE           0xC4
0,// VK_NONE           0xC5
0,// VK_NONE           0xC6
0,// VK_NONE           0xC7
0,// VK_NONE           0xC8
0,// VK_NONE           0xC9
0,// VK_NONE           0xCA
0,// VK_NONE           0xCB
0,// VK_NONE           0xCC
0,// VK_NONE           0xCD
0,// VK_NONE           0xCE
0,// VK_NONE           0xCF
0,// VK_NONE						0xD0
0,// VK_NONE           0xD1
0,// VK_NONE           0xD2
0,// VK_NONE           0xD3
0,// VK_NONE           0xD4
0,// VK_NONE           0xD5
0,// VK_NONE           0xD6
0,// VK_NONE           0xD7
0,// VK_NONE           0xD8
0,// VK_NONE           0xD9
0,// VK_NONE           0xDA
0,// VK_OEM_4          0xDB  //  '[{' for US
0,// VK_OEM_5          0xDC  //  '\|' for US
0,// VK_OEM_6          0xDD  //  ']}' for US
0,// VK_OEM_7          0xDE  //  ''"' for US
0,// VK_OEM_8          0xDF
0,// VK_NONE          	0xE0
0,// VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
0,// VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
0,// VK_ICO_HELP       0xE3  //  Help key on ICO
0,// VK_ICO_00         0xE4  //  00 key on ICO
0,// VK_PROCESSKEY     0xE5
0,// VK_ICO_CLEAR      0xE6
0,// VK_PACKET         0xE7
0,// VK_NONE           0xE8
0,// VK_OEM_RESET      0xE9
0,// VK_OEM_JUMP       0xEA
0,// VK_OEM_PA1        0xEB
0,// VK_OEM_PA2        0xEC
0,// VK_OEM_PA3        0xED
0,// VK_OEM_WSCTRL     0xEE
0,// VK_OEM_CUSEL      0xEF
0,// VK_OEM_ATTN       0xF0
0,// VK_OEM_FINISH     0xF1
0,// VK_OEM_COPY       0xF2
0,// VK_OEM_AUTO       0xF3
0,// VK_OEM_ENLW       0xF4
0,// VK_OEM_BACKTAB    0xF5
0,// VK_ATTN           0xF6
0,// VK_CRSEL          0xF7
0,// VK_EXSEL          0xF8
0,// VK_EREOF          0xF9
0,// VK_PLAY           0xFA
0,// VK_ZOOM           0xFB
0,// VK_NONAME         0xFC
0,// VK_PA1            0xFD
0,// VK_OEM_CLEAR      0xFE
0,// VK_OEM_CLEAR      0xFF
};

BYTE keydata[256];
//返回一串以0结束的按键码(每个码表示对应的按键被按下)
u16 *sys_key_state( void )
{
	static u16 vkey[256];
	int i = 0, j = 0;
	//if( GetKeyboardState(keydata) != 0 )	//获取按键状态
	{
	for( i = 0; i < 256; i++ )
	{
	//if( i >= '0' && i < '9' )
	//	printf( "%d ",keydata[i] );
		if( (keydata[i] & 0x80) /*&& keymap[i]*/ )
		{
			//printf( "%x ",i );
			vkey[j] = keymap[i];
			j++;
		}
	}
	//printf( "\n" );
	}
	vkey[j] = 0;
	return vkey;
}


u32 get_ticks_count( void )
{
	//LARGE_INTEGER CurrTicks, TicksCount;
	//QueryPerformanceFrequency(&TicksCount);
	//QueryPerformanceCounter(&CurrTicks);
	return GetTickCount();//CurrTicks.QuadPart / (TicksCount.QuadPart / 1000000i64);
}

void delay_ms( u32 time )
{
	Sleep( time );
}

void w32_event( void )
{
//	MSG msg;
//	GetMessage(&msg,Main_hWnd,0,0);
//	TranslateMessage(&msg);
//	DispatchMessage(&msg);
	Sleep(1);
}

HWAVEOUT hwo;

#define WAVEOUT_BUFFER_LENGTH (0x10000*4)

s16 waveout_buffer0[WAVEOUT_BUFFER_LENGTH/2];
s16 waveout_buffer1[WAVEOUT_BUFFER_LENGTH/2];

WAVEHDR wavwhdr0={
	(s8 *)waveout_buffer0,
	WAVEOUT_BUFFER_LENGTH,
	NULL,
	NULL,
	WHDR_DONE,
	0,
	NULL,
	NULL
};

WAVEHDR wavwhdr1={
	(s8 *)waveout_buffer1,
	WAVEOUT_BUFFER_LENGTH,
	NULL,
	NULL,
	WHDR_DONE,
	0,
	NULL,
	NULL
};

HANDLE bufempty0;
HANDLE bufempty1;

HANDLE bufwritting0;
HANDLE bufwritting1;

void CALLBACK waveOutProc(
  HWAVEOUT hwo,      
  UINT uMsg,         
  DWORD dwInstance,  
  DWORD dwParam1,    
  DWORD dwParam2     
)
{
	static u32 flip=1;
	if(uMsg != WOM_DONE) return;
	if( &wavwhdr0 == (LPWAVEHDR)dwParam1 )
	{
		WaitForSingleObject( bufempty0, INFINITE );
		waveOutWrite(hwo,  &wavwhdr0, sizeof(wavwhdr0));
		SetEvent( bufwritting0 );
	}
	else
	{
		WaitForSingleObject( bufempty1, INFINITE );
		waveOutWrite(hwo,  &wavwhdr1, sizeof(wavwhdr1));
		SetEvent( bufwritting1 );
	}
	flip^=1;
}

u32 w32waveout_write( s8 *buffer, u32 size )
{
	static u32 flip = 1;
	if( size > WAVEOUT_BUFFER_LENGTH )
		size = WAVEOUT_BUFFER_LENGTH;
	
	//memcpy( waveout_buffer0, buffer, size );
	
	if(flip)
	{
		WaitForSingleObject( bufwritting0, 1000 );
		memcpy( waveout_buffer0, buffer, size );
		wavwhdr0.dwBufferLength = size;
		SetEvent(bufempty0); 
	}
	else
	{
		WaitForSingleObject( bufwritting1, 1000 );
		memcpy( waveout_buffer1, buffer, size );
		wavwhdr1.dwBufferLength = size;
		SetEvent(bufempty1); 
	}
	
	flip^=1;
	return size;
}

s32 w32waveout_init( u16 freq , u16 bit , u16 channel )
{
	MMRESULT mmresult;

	WAVEFORMATEX waveformtex={
		WAVE_FORMAT_PCM,
		channel,
		freq,
		freq,
		bit*channel/8,
		bit,
		NULL
	};

	mmresult=waveOutOpen(
		&hwo,           
		WAVE_MAPPER,            
		&waveformtex,
		(DWORD)waveOutProc,         
		NULL,   
		CALLBACK_FUNCTION         
	);

	if( mmresult == MMSYSERR_NOERROR )
	{
		waveOutPrepareHeader(hwo,&wavwhdr0,sizeof(wavwhdr0));
		waveOutPrepareHeader(hwo,&wavwhdr1,sizeof(wavwhdr1));
		bufempty0 = CreateEvent( NULL,FALSE,FALSE,NULL );
		bufempty1 = CreateEvent( NULL,FALSE,FALSE,NULL );
		bufwritting0 = CreateEvent( NULL,FALSE,TRUE,NULL );
		bufwritting1 = CreateEvent( NULL,FALSE,TRUE,NULL );
		waveOutWrite(hwo,  &wavwhdr0, sizeof(wavwhdr0));
		waveOutWrite(hwo,  &wavwhdr1, sizeof(wavwhdr1));
		return 0;
	}
	else
	{
		return 1;
	}
}

void w32waveout_close( void )
{
	waveOutClose( hwo );
	CloseHandle( bufempty0 );
	CloseHandle( bufempty1 );
	CloseHandle( bufwritting0 );
	CloseHandle( bufwritting1 );
}

WAVEFORMATEX wavein_matex;
HWAVEIN hwi;
WAVEHDR wavein_hdr0;
WAVEHDR wavein_hdr1;
#define WAVEIN_BUF_LEN 0x10000
char wavein_buf0[WAVEIN_BUF_LEN];
char wavein_buf1[WAVEIN_BUF_LEN];
HANDLE wavein_empty0;
HANDLE wavein_empty1;
HANDLE wavein_writting0;
HANDLE wavein_writting1;

void CALLBACK waveInProc(
						 HWAVEIN hwi,       
						 UINT uMsg,         
						 DWORD dwInstance,  
						 DWORD dwParam1,    
						 DWORD dwParam2     
						 )
{
	static u32 flip=1;

	if( uMsg == WIM_DATA )
	{
		printf( "%d\n", flip );
		if(flip)
		{
			//WaitForSingleObject( wavein_buf0, INFINITE );
			//wavein_hdr0.dwBytesRecorded = 0;
			waveInAddBuffer( hwi, &wavein_hdr0, sizeof(wavein_hdr0) );
			SetEvent( wavein_writting0 );
		}
		else
		{
			//WaitForSingleObject( wavein_buf1, INFINITE );
			//wavein_hdr1.dwBytesRecorded = 0;
			waveInAddBuffer( hwi, &wavein_hdr1, sizeof(wavein_hdr1) );
			SetEvent( wavein_writting1 );
		}
		flip^=1;
		return;
	}
	
}

s32 sys_wavein_init( u32 freq , u32 bit , u32 channel )
{
	MMRESULT mmresult;
	wavein_matex.nChannels = channel;
	wavein_matex.nSamplesPerSec = freq;
	wavein_matex.nBlockAlign = channel*bit/8 ;
	wavein_matex.nAvgBytesPerSec = freq;
	wavein_matex.wBitsPerSample = bit;
	wavein_matex.wFormatTag = WAVE_FORMAT_PCM;
	wavein_matex.cbSize = 0;

	mmresult = waveInOpen(
		&hwi,            
		WAVE_MAPPER,            
		&wavein_matex,       
		(DWORD)waveInProc,          
		NULL,  
		CALLBACK_FUNCTION           );

	if( mmresult == MMSYSERR_NOERROR )
	{
		waveInPrepareHeader( hwi, &wavein_hdr0, sizeof(wavein_hdr0) );
		wavein_hdr0.lpData = wavein_buf0;
		wavein_hdr0.dwBufferLength = sizeof(wavein_buf0);
		wavein_hdr0.dwBytesRecorded = 0;
		wavein_hdr0.dwUser = 0;
		wavein_hdr0.dwFlags = WHDR_PREPARED;
		wavein_hdr0.dwLoops = 0;

		waveInPrepareHeader( hwi, &wavein_hdr1, sizeof(wavein_hdr1) );
		wavein_hdr1.lpData = wavein_buf1;
		wavein_hdr1.dwBufferLength = sizeof(wavein_buf1);
		wavein_hdr1.dwBytesRecorded = 1;
		wavein_hdr1.dwUser = 1;
		wavein_hdr1.dwFlags = WHDR_PREPARED;
		wavein_hdr1.dwLoops = 1;

		waveInAddBuffer( hwi, &wavein_hdr0, sizeof(wavein_hdr0) );
		
		wavein_empty0 = CreateEvent( NULL,FALSE,FALSE,NULL );
		wavein_empty1 = CreateEvent( NULL,FALSE,FALSE,NULL );
		wavein_writting0 = CreateEvent( NULL,FALSE,TRUE,NULL );
		wavein_writting1 = CreateEvent( NULL,FALSE,TRUE,NULL );

		waveInStart( hwi );
		return 0;
	}
	return 1;
}

void sys_wavein_deinit( void )
{
	waveInUnprepareHeader( hwi, &wavein_hdr0, sizeof(wavein_hdr0) );
	waveInUnprepareHeader( hwi, &wavein_hdr1, sizeof(wavein_hdr1) );
	waveInStop( hwi );
	waveInClose( hwi );
	CloseHandle( wavein_empty0 );
	CloseHandle( wavein_empty1 );
	CloseHandle( wavein_writting0 );
	CloseHandle( wavein_writting1 );
}

u32 sys_wavein_read( void *data, u32 length )
{
	static u32 flip=1;
	if( flip )
	{
		if( wavein_hdr0.dwUser >= wavein_hdr0.dwBufferLength )
		{
			wavein_hdr0.dwUser = 0;
			WaitForSingleObject( wavein_writting0, 1000 );
		}
		//WaitForSingleObject( wavein_writting0, 1000 );
		memcpy( data, wavein_buf0 + wavein_hdr0.dwUser, length );
		wavein_hdr0.dwUser += length;
		//SetEvent( wavein_empty0 ); 
	}
	else
	{
		//
		if( wavein_hdr1.dwUser >= wavein_hdr1.dwBufferLength )
		{
			wavein_hdr1.dwUser = 0;
			WaitForSingleObject( wavein_writting1, 1000 );
		}
		memcpy( data, wavein_buf1 + wavein_hdr1.dwUser, length );
		wavein_hdr1.dwUser += length;
		//SetEvent( wavein_empty1 ); 
		
	}
	
	printf( "%d %d %d%%\n", wavein_hdr0.dwBufferLength, wavein_hdr0.dwBytesRecorded, 
	wavein_hdr0.dwBytesRecorded*100/wavein_hdr0.dwBufferLength );
	
	return length;
}

}

void delay_us(__int64 Us) {
	LARGE_INTEGER CurrTicks, TicksCount;
	QueryPerformanceFrequency(&TicksCount);
	QueryPerformanceCounter(&CurrTicks);
	TicksCount.QuadPart = TicksCount.QuadPart * Us / 1000000i64;
	TicksCount.QuadPart += CurrTicks.QuadPart;
	while(CurrTicks.QuadPart<TicksCount.QuadPart)
		QueryPerformanceCounter(&CurrTicks);
}

struct SYS_THREAD {
	HANDLE hThread;
	int (*sys_thread_proc)( void * param , struct SYS_THREAD * thread );
	void * param;
};

static DWORD WINAPI SysThreadEntry(
  LPVOID lpParameter   // thread data
)
{
	SYS_THREAD * pThread = (SYS_THREAD *)lpParameter;
	pThread->sys_thread_proc( pThread->param, pThread );
	return 0;
}

SYS_THREAD * sys_create_thread( int stack_size, int (*sys_thread_proc)( void * param, SYS_THREAD * thread ), void * param, SYS_THREAD_STATUS status )
{
	SYS_THREAD * pThread;
	DWORD dwCreationFlags;
	if( ( pThread = (SYS_THREAD * )malloc( sizeof(SYS_THREAD) ) ) == NULL )
		goto err;
	switch( status )
	{
		case SYS_THREAD_STATUS_STANDBY:
			dwCreationFlags = CREATE_SUSPENDED;
			break;
		case SYS_THREAD_STATUS_RUN:
			dwCreationFlags = NULL;
			break;
		default:
			goto err;
	} 
	pThread->param = param;
	pThread->sys_thread_proc = sys_thread_proc;
	pThread->hThread = CreateThread( NULL, stack_size, SysThreadEntry, pThread, dwCreationFlags, NULL );
	return pThread;
err:
	if( pThread )
		free( pThread );
	return NULL;
}

int sys_delete_thread( SYS_THREAD * pThread )
{
	TerminateThread( pThread->hThread, 0 );
	CloseHandle( pThread->hThread );
	free( pThread );
	return 0;
}

int sys_suspend_thread( SYS_THREAD * pThread )
{
	if( SuspendThread( pThread->hThread ) == -1 )
		goto err;
	return 0;
err:
	return -1;
}

int sys_resume_thread( SYS_THREAD * pThread )
{
	if( ResumeThread( pThread->hThread ) == -1 )
		goto err;
	return 0;
err:
	return -1;
}

typedef struct PRIO_MAP {
	int nPriority;
	PRIO_TYPE prio;
} PRIO_MAP;

PRIO_MAP prio_map[] = {
{	THREAD_PRIORITY_ABOVE_NORMAL,	PRIO_TYPE_ABOVE_NORMAL	},
{	THREAD_PRIORITY_BELOW_NORMAL,	PRIO_TYPE_BELOW_NORMAL	},
{	THREAD_PRIORITY_HIGHEST,		PRIO_TYPE_HIGHEST		},
{	THREAD_PRIORITY_IDLE,			PRIO_TYPE_IDLE			},
{	THREAD_PRIORITY_LOWEST,			PRIO_TYPE_LOWEST		},
{	THREAD_PRIORITY_NORMAL,			PRIO_TYPE_NORMAL		},
{	THREAD_PRIORITY_TIME_CRITICAL,	PRIO_TYPE_REALTIME		},
{	0,								PRIO_TYPE_MAX			},
};

static int find_mapper_prio( PRIO_TYPE prio )
{
	int i = 0;
	while( prio_map[i].prio != PRIO_TYPE_MAX )
	{
		if( prio_map[i].prio == prio )
			return prio_map[i].nPriority;
		i++;
	}
	return THREAD_PRIORITY_NORMAL;
}

int sys_priority_thread( SYS_THREAD * pThread, PRIO_TYPE prio )
{
	SetThreadPriority( pThread->hThread, find_mapper_prio(prio) );
	return 0;
}

extern "C" s32 sys_deinit( void );
DWORD WINAPI Kernel_ThreadProc(
  LPVOID lpParameter   // thread data
)
{
//	u8 buf[MAX_PATH];
	printf( "kernel thread start.\n" );
	delay_ms(100);
//	sys_get_dir( buf, MAX_PATH );
//	_chdir( (const char *)buf );
	sys_init();
	app_main();
	sys_deinit();
	printf( "kernel thread exit.\n" );
	#ifndef _DEBUG
	exit(1);
	#endif
	return TRUE;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	
	#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$","w+t",stdout); freopen("CONIN$","r+t",stdin);
	freopen("CONIN$","r+t",stderr);
	#endif
	
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SHELL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SHELL);
	//app_main();
	CreateThread( NULL, 0x100000, Kernel_ThreadProc, NULL, NULL, NULL );

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	#ifdef _DEBUG
	FreeConsole();
	#endif
	
	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_SMALL);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_SHELL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
#define STYLE_WND WS_OVERLAPPEDWINDOW

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable
	RECT wr = {0,0,320,240};
	AdjustWindowRect(&wr, STYLE_WND, true);

   hWnd = CreateWindow(szWindowClass, szTitle, STYLE_WND,
      CW_USEDEFAULT, 0,wr.right-wr.left,wr.bottom-wr.top, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	   u32 err = GetLastError();
      return FALSE;
   }

   HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
	hdcMem=CreateCompatibleDC(hdc);
	hScreenBuffer=CreateCompatibleBitmap(hdc,320,240);
	SelectObject(hdcMem,hScreenBuffer);
	EndPaint(hWnd, &ps);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

  // SetTimer(hWnd,1,100,NULL);
  // SetTimer(hWnd,2,10,NULL);


   return TRUE;
}


#define   KEY_DOWN(vk_code)   ((GetAsyncKeyState(vk_code)   &   0x8000)   ?   1   :   0)

u8 *get_video( void )
{
	return Video_Buffer;
}



void Update_ClientRect16()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(Main_hWnd, &ps);
	u16 *src;
	src = (u16 *)get_video();
	u32 i;
	for(i=0;i<320*240*4;i+=4)
	{
		u16 color = *src;
		ScreenBuffer[i] = (color << 3) & 0xFF ;
		ScreenBuffer[i+1] = (color >> 2) & 0xFF ;
		ScreenBuffer[i+2] = (color >> 8) & 0xFF ;
		ScreenBuffer[i+3] = 0 ;
		src ++;
	}
	SetBitmapBits(hScreenBuffer,320*240*4,ScreenBuffer);
	StretchBlt(hdc,0,0,cx,cy,hdcMem,0,0,320,240,SRCCOPY);
	EndPaint(Main_hWnd, &ps);
}

void Update_ClientRect32()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(Main_hWnd, &ps);
	SetBitmapBits(hScreenBuffer,scr_width*scr_height*4,Video_Buffer32);
	StretchBlt(hdc,0,0,cx,cy,hdcMem,0,0,scr_width,scr_height,SRCCOPY);
	EndPaint(Main_hWnd, &ps);
}



//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	Main_hWnd = hWnd;
	GetKeyboardState(keydata);
	switch (message) 
	{
		case WM_LBUTTONDOWN:
			Mouse_xPos = GET_X_LPARAM(lParam); 
			Mouse_yPos = GET_Y_LPARAM(lParam);
			Mouse_Press = POINTER_PRESS_MAX;
			break;
		case WM_LBUTTONUP:
			Mouse_xPos = GET_X_LPARAM(lParam); 
			Mouse_yPos = GET_Y_LPARAM(lParam);
			Mouse_Press = POINTER_PRESS_MIN;
			break;
		case WM_MOUSEMOVE:
		{
			Mouse_xPos = GET_X_LPARAM(lParam); 
			Mouse_yPos = GET_Y_LPARAM(lParam);
			Mouse_Press = POINTER_PRESS_MIN;
			if( wParam & MK_LBUTTON )
				Mouse_Press = POINTER_PRESS_MAX;
			break;
		}

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_SIZE:
			cx=LOWORD(lParam);
			cy=HIWORD(lParam);
			break;
		case WM_PAINT:
			/*
			hdc = BeginPaint(hWnd, &ps);
			u16 *src;
			src = (u16 *)get_video();
			u32 i;
			for(i=0;i<320*240*4;i+=4)
			{
				u16 color = *src;
				ScreenBuffer[i] = (color << 3) & 0xFF ;
				ScreenBuffer[i+1] = (color >> 2) & 0xFF ;
				ScreenBuffer[i+2] = (color >> 8) & 0xFF ;
				ScreenBuffer[i+3] = 0 ;
				src ++;
			}
			SetBitmapBits(hScreenBuffer,320*240*4,ScreenBuffer);
			StretchBlt(hdc,0,0,cx,cy,hdcMem,0,0,320,240,SRCCOPY);
			EndPaint(hWnd, &ps);*/
			if(Update_ClientRect) Update_ClientRect();
			break;
		/*
		case WM_TIMER:
			switch (wParam)
			{
				case 1:
				RECT rt;
				GetClientRect(hWnd, &rt);
				InvalidateRect(hWnd,&rt,FALSE);
				break;
			}
			break;
		*/
		case MM_WOM_DONE:
			printf("MM_WOM_DONE\n");
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
