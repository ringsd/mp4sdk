/***************************************************************************

		file_shell.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-04-05 18:20:13

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>
#include <windows.h>

int file_dialog(HWND hwnd, LPCSTR filter, char *fname, u32 flags)
{
	OPENFILENAME OFN;

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner   = hwnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile   = fname;
	OFN.nMaxFile    = MAX_PATH*2;
	OFN.Flags       = flags;
	OFN.lpstrTitle  = "Select a file.";

	return GetOpenFileName(&OFN);
}

int Win32_Get_File( char * fname, char * filter )
{
	return file_dialog( NULL, "*", fname, NULL );
}

