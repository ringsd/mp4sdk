// shell.cpp : Defines the entry point for the application.
//
#include <vld.h>
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

#include "platform_keyboard.h"

int sys_get_keyboard( SYSVK * vkeys, int length )
{
    int i;
    for( i = 0; i < length; i++ )
    {
        if( KEY_DOWN(i) )
        {
            vkeys[i] = SYSVK_PRESS_MAX;
        }
        else
        {
            vkeys[i] = 0;
        }
    }
    return 0;
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
	free( pThread );
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
			if(Update_ClientRect) Update_ClientRect();
			break;
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
