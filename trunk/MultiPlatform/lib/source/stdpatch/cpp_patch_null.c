/***************************************************************************

		cpp_patch_null.c

		cpp的异常机制函数,空模拟.

		TIME LIST:
		CREATE	Rainy	2009-04-10 22:57:01

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

//This file is used with projects that use -nostdlib option
//These are some symbols that libstdc++ and libsupc++ require but I can't include libgcc
//  because it wants symbols from Win32 API.
//makefile:b_dfgcc = TRUE
#include <global.h>

void _Unwind_SjLj_Register() { TRACE_LOG(""); }
void _Unwind_SjLj_Unregister() { TRACE_LOG("");}
void _Unwind_SjLj_Resume() { TRACE_LOG("");}
void _Unwind_SjLj_Resume_or_Rethrow() { TRACE_LOG("");}
void _Unwind_SjLj_RaiseException() { TRACE_LOG("");}
void _Unwind_DeleteException() { TRACE_LOG("");}
void _Unwind_SetIP() { TRACE_LOG("");}
void _Unwind_SetGR() { TRACE_LOG("");}
void _Unwind_GetIP() { TRACE_LOG("");}
void _Unwind_GetRegionStart() { TRACE_LOG("");}
void _Unwind_GetDataRelBase() { TRACE_LOG("");}
void _Unwind_GetTextRelBase() { TRACE_LOG("");}
void _Unwind_GetLanguageSpecificData() { TRACE_LOG("");}
/*
int _CRT_MT = 0;

void __gthr_win32_setspecific() { TRACE_LOG("");}
void __gthr_win32_getspecific() { TRACE_LOG("");}
void __gthr_win32_once() { TRACE_LOG("");}
void __gthr_win32_mutex_lock() { TRACE_LOG("");}
void __gthr_win32_mutex_unlock() { TRACE_LOG("");}
void __gthr_win32_key_create() { TRACE_LOG("");}
void __gthr_win32_mutex_init_function() { TRACE_LOG("");}
void __w32_sharedptr_terminate() { TRACE_LOG("");}
void __w32_sharedptr_unexpected() { TRACE_LOG("");}
*/


//void __udivdi3() { }

