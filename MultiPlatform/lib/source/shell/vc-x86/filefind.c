/***************************************************************************

		filefind.c

		文件查找函数.

		TIME LIST:
		CREATE	Rainy	2009-01-29 16:36:56

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#include "global.h"
#include <io.h>
#include <Windows.h>

typedef struct ATTRIB_MAP{
	u32 s_attrib;
	u32 d_attrib;
} ATTRIB_MAP;
 
ATTRIB_MAP attrib_map[] = {
	{	_A_RDONLY, FFD_READONLY	},
	{	_A_HIDDEN, FFD_HIDDEN	},
	{	_A_SYSTEM, FFD_SYSTEM	},
	{	_A_SUBDIR, FFD_DIR	},
	{	_A_ARCH, FFD_ARCHIVE	},
	{	0, 0	},
};

u32 translate_attrib( u32 s_attrib )
{
	u32 d_attrib = 0;
	int i;
	for( i = 0; i < 32 ; i++ )
	{
		if( attrib_map[i].s_attrib == 0 )
			break;
		if( s_attrib & attrib_map[i].s_attrib )
		{
			d_attrib |= attrib_map[i].d_attrib;
		}
	}
	return d_attrib;
}

FILE_FIND_BLOCK *find_open( const u8 *path )
{
	FILE_FIND_BLOCK *block = malloc(sizeof(FILE_FIND_BLOCK));
	u8 truepath[MAX_PATH];
	struct _finddata_t *finddata;
	int i;

	if( block == NULL )
	{
		return NULL;
	}
	
	if( *path == '\0' )			//空字符串则隐含搜索逻辑驱动器
	{
		block->usrdat1 = NULL;
		block->usrdat2 = NULL;
		block->usrdat3 = GetLogicalDrives();	//存储了逻辑驱动器信息
		block->usrdat4 = 0;				//存储了当前移位次数(驱动器编号)
		return block;
	}

	finddata = malloc(sizeof(struct _finddata_t));

	if( finddata == NULL )
	{
		free( block );
		return NULL;
	}
	sprintf( truepath, "%s\*", path ); 
	if( (block->usrdat1 = _findfirst( truepath, finddata ) ) == -1L )
	{
		free( block );
		free( finddata );
		return NULL;
	}
	
	block->usrdat2 = finddata;
	
	return block;
}

void find_close( FILE_FIND_BLOCK *block )
{
	if( block->usrdat1 != NULL )		//查找的是逻辑驱动器则usrdat1,usrdat2都未使用,不需要释放
	{
		_findclose( block->usrdat1 );
		free( block->usrdat2 );
	}
	free( block );
}

int find_next( FILE_FIND_BLOCK *block )
{
	struct _finddata_t *finddata = block->usrdat2;

	if( block->usrdat1 == NULL )		//查找的是逻辑驱动器
	{
		static u8 name[32];
		while( ((u32)block->usrdat3 & 1) == 0 )
		{
			((u32)block->usrdat3) >>= 1;
			((u32)block->usrdat4)++;
			if( (u32)block->usrdat3 == 0 )
				return 0;
		}
		sprintf( name, "%c:", (u32)block->usrdat4 + 'A' );
		block->attrib = FFD_VOLUME;
		block->name = name;
		block->size = 0;
		block->time_create = 0;
		block->time_access = 0;
		block->time_write = 0;

		((u32)block->usrdat3) >>= 1;
		((u32)block->usrdat4)++;
		return 1;
	}

	if( _findnext( block->usrdat1, block->usrdat2 ) != 0 )
		return 0;
	block->attrib = translate_attrib( finddata->attrib );
	block->name = finddata->name;
	block->size = finddata->size;
	block->time_create = finddata->time_create;
	block->time_access = finddata->time_access;
	block->time_write = finddata->time_write;
	return 1;
}



