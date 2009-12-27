/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#include "SDL_thread.h"
#include "../SDL_systhread.h"
#include "../SDL_thread_c.h"

#define OS_STACK_SIZE 1024*4 //4K     /* Stack size */
typedef struct {
	int (SDLCALL *func)(void *);
	void *data;
	SDL_Thread *info;
	SDL_sem *wait;
} thread_args;

static int ThreadEntry( void * argp, SYS_THREAD * thread )
{
	SDL_RunThread( argp);
	//sys_delete_thread(thread);
	return 0;
}

int SDL_SYS_CreateThread(SDL_Thread *thread, void *args)
{

	thread->handle =sys_create_thread( OS_STACK_SIZE, 
		ThreadEntry,
		args,
		SYS_THREAD_STATUS_RUN); 

	if(!thread->handle) {
		SDL_SetError("SDL_SYS_CreateThread() failed");
		return -1;
	}
	thread->threadid=(Uint32)thread->handle;
	return 0;
}

void SDL_SYS_SetupThread(void)
{
}

Uint32 SDL_ThreadID(void)
{
	/* 不能工作 */
	return 0;
}

void SDL_SYS_WaitThread(SDL_Thread *thread)
{
	sys_delete_thread(thread->handle);
}

void SDL_SYS_KillThread(SDL_Thread *thread)
{ 
	sys_delete_thread(thread->handle);
}
