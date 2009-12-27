/***************************************************************************

		system.h

		外壳接口定义文件.
		上层:外壳主体
		下层:平台接口层

		TIME LIST:
		CREATE	Rainy	2008-10-17 22:06:47

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _system_h_
#define _system_h_

#ifdef		__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "platform_types.h"
#include <time.h>
//----------------------------------------------------------------------------
//设置屏幕模式
//返回0 --失败(不支持的色彩模式或者宽度高度)
//返回1 --设置成功
//屏幕色彩选项
typedef enum COLOR_MODE{
	LCD_A8R8G8B8,
	LCD_R5G6B5,
	LCD_A1R5G5B5,
	COLOR_MODE_MAX
} COLOR_MODE;
s32 screen_set_mode( u32 width, u32 height, u32 colormode );
//返回屏幕宽度
u32 screen_get_width( void );
//返回屏幕高度
u32 screen_get_height( void );

//设置屏幕亮度(返回0--成功(范围0~100))
int screen_set_backlight( int value );

//更新屏幕
void lcd_updateui( void );
//清空缓存
void lcd_clearui( u32 color );
//返回32位缓存
u32* lcd_bufferui( void );
//----------------------------------------------------------------------------
//系统延时(毫秒)
void sys_delay( u32 time );
//执行系统任务
void sys_event( void );
//----------------------------------------------------------------------------
//读取按键
u32 sys_get_key( void );
//读取定位设备
//返回0--成功 其他--失败
#define POINTER_PRESS_MAX 255
#define POINTER_PRESS_MIN 0
int sys_get_pointer( int *x, int *y, int *press );
//返回一串以0结束的按键码(每个码表示对应的按键被按下)
u16 *sys_key_state( void );
//----------------------------------------------------------------------------
//调整系统音量
void sys_volume( int volume );
//获取系统周期(毫秒)
u32 sys_ticks( void );
//声音设备初始化
s32 sys_sound_init( u32 freq , u32 bit , u32 channel );
//声音设备卸载
void sys_sound_deinit( void );
//声音输入设备初始化
s32 sys_wavein_init( u32 freq , u32 bit , u32 channel );
//声音输入设备卸载
void sys_wavein_deinit( void );
//写声音数据
u32 sys_sound_write( void *data , u32 length );
//读声音数据
u32 sys_wavein_read( void *data, u32 length );
//检查声音设备是否可写
s32 sys_sound_canwrite( void );
//系统初始化
s32 sys_init( void );
//创建目录,返回0--成功 -1--失败
int sys_mkdir( char *path );
//-------------------------------------------------------------------------
//File Find Functions(文件查找结构)
typedef struct FILE_FIND_BLOCK{
	u32 attrib;
	u8* name;
	u32 size;
	time_t time_create;
	time_t time_write;
	time_t time_access;
	void *usrdat1;
	void *usrdat2;
	void *usrdat3;
	void *usrdat4;
} FILE_FIND_BLOCK;

typedef enum {
	FFD_READONLY = 0x1,
	FFD_WRITE = 0x2,
	FFD_SYSTEM = 0x4,
	FFD_HIDDEN = 0x8,
	FFD_VOLUME = 0x10,
	FFD_DIR = 0x20,
	FFD_ARCHIVE = 0x40,
	FFD_DIR_OR = 0x80,
	FFD_UNKNOW = 0x40000000,
	FFD_ALL = 0x4FFFFFFF,
	FFD_MAX
} FILE_ATTRIB;

//打开一个查找文件的过程
FILE_FIND_BLOCK *find_open( const u8 *path );
//关闭查找文件过程
void find_close( FILE_FIND_BLOCK *block );
//查找下一个文件
int find_next( FILE_FIND_BLOCK *block );
//-------------------------------------------------------------------------
#ifndef MAX_PATH
#define MAX_PATH 1024
#endif
//----------------------------路径与参数接口-------------------------------
//获取命令行参数( argc -- 参数个数, argv -- 参数指针数组 ,maxargc--最大接收参数个数)
//argc == 0 -- 无参数
void sys_get_arg( int *argc,char *argv[] , int maxargc );
//获取当前目录
//返回buf--成功,路径存储在buf中,返回NULL--失败
u8 *sys_get_dir( u8 *buf , int maxlen );
//获取应用程序本身路径
//返回buf--成功,路径存储在buf中,返回NULL--失败
u8 *sys_get_path( u8 *buf , int maxlen );
//注册支持的文件类型
u8 * sys_reg_filetype( void );

//---------------------------线程接口---------------------------------------

typedef enum SYS_THREAD_STATUS {	//线程状态
	SYS_THREAD_STATUS_STANDBY,		//挂起状态
	SYS_THREAD_STATUS_RUN,			//运行状态
	SYS_THREAD_STATUS_MAX
} SYS_THREAD_STATUS;

typedef struct SYS_THREAD SYS_THREAD;			//线程对象
extern int sys_thread_proc( void * param, SYS_THREAD * thread );	//线程原型

//创建线程(stack_size 栈容量, sys_thread_proc 线程函数, param 线程参数, status 线程初始状态
SYS_THREAD * sys_create_thread( int stack_size, int (*sys_thread_proc)( void * param, SYS_THREAD * thread ), void * param, SYS_THREAD_STATUS status );
//关闭线程(返回0成功)
int sys_delete_thread( SYS_THREAD * pThread );
//挂起线程
int sys_suspend_thread( SYS_THREAD * pThread );
//恢复线程
int sys_resume_thread( SYS_THREAD * pThread );

/* 设置线程优先级
 */
typedef enum{
	PRIO_TYPE_IDLE,
	PRIO_TYPE_LOWEST,
	PRIO_TYPE_BELOW_NORMAL,
	PRIO_TYPE_NORMAL,
	PRIO_TYPE_ABOVE_NORMAL,
	PRIO_TYPE_HIGHEST,
	PRIO_TYPE_REALTIME,
	PRIO_TYPE_MAX
} PRIO_TYPE;

int sys_priority_thread( SYS_THREAD * pThread, PRIO_TYPE prio );

//----------------------------------------------------------------------------------
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 

#ifdef __cplusplus
}
#endif

#endif

