/** This file is part of the Mingw32 package.
* unistd.h maps     (roughly) to io.h
*/

#ifndef _UNISTD_H
#define _UNISTD_H

#include <io.h>
#include <process.h>

#ifndef _XBOX				// chdir and getcwd not supported on Xbox hardware
#include <direct.h>
#define chdir _chdir
#define getcwd _getcwd
#endif
#define FREEZEFOLDER	GUI.FreezeFileDir
//zinx suggested this, for *nix compatibility
#define MAX_PATH 1024
#define PATH_MAX	MAX_PATH

#define W_OK 02

#define strncasecmp strnicmp
#define strcasecmp stricmp

#endif /* _UNISTD_H */ 
