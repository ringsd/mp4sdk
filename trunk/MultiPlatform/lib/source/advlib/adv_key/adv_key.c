/***************************************************************************

		adv_key.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-01-31 20:17:47

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include "global.h"

//菜单读键
//返回键值,特点是持续按下按键会自动连发
u32 menu_get_key()
{
	u32 key;
	while( 1 )
	{
		key = sys_get_key();
		if( key )
		{
			sys_delay(200);
			return key;
		}
		sys_delay(10);
	}
}

//等待按键清空
void menu_clear_key()
{
	while( sys_get_key() )
		sys_delay(10);
}

//按键转换,提供需转换的按键和转换表即可实现最多32bit的键值转换
u32 convert_key( u32 phykey , KEYMAP* key_map )
{
	int emukey = 0;
	int i = 0;
	while( key_map[i].emukey )
	{
		if( key_map[i].phykey )
		{
			if( (phykey & key_map[i].phykey) == key_map[i].phykey )
			{
				emukey |= key_map[i].emukey;
			}
		}
		i++;
	}
	return emukey;
}

KEYNAME syskeyname[] = {
{	"SYSKEY_UP"	,			SYSKEY_UP				},
{	"SYSKEY_DOWN"	,		SYSKEY_DOWN		  },
{	"SYSKEY_LEFT"	,		SYSKEY_LEFT		  },
{	"SYSKEY_RIGHT",		SYSKEY_RIGHT	  },
{	"SYSKEY_A",				SYSKEY_A			  },
{	"SYSKEY_B",				SYSKEY_B			  },
{	"SYSKEY_C",				SYSKEY_C			  },
{	"SYSKEY_D",				SYSKEY_D			  },
{	"SYSKEY_SELECT"	,	SYSKEY_SELECT	  },
{	"SYSKEY_START",		SYSKEY_START	  },
{	"SYSKEY_L",				SYSKEY_L			  },
{	"SYSKEY_R",				SYSKEY_R			  },
{	"SYSKEY_MENU"	,		SYSKEY_MENU		  },
{	"SYSKEY_VOLUP",		SYSKEY_VOLUP	  },
{	"SYSKEY_VOLDOWN",	SYSKEY_VOLDOWN  },
{	"SYSKEY_POWER",		SYSKEY_POWER	  },
{	"SYSKEY_ESCAPE"	,	SYSKEY_ESCAPE	  },
{	"SYSKEY_E",				SYSKEY_E			  },
{	"SYSKEY_F",				SYSKEY_F			  },
{	"SYSKEY_SAVE"	,		SYSKEY_SAVE		  },
{	"SYSKEY_LOAD"	,		SYSKEY_LOAD		  },
{	NULL,							0							  }
};


const char * getkeyname( int key ,KEYNAME *kn )
{
	int i = 0;
	static char buf[512];
	buf[0] = 0;
	while( kn[i].key )
	{
		if( kn[i].key & key )
		{
			strcat( buf , kn[i].name );
			strcat( buf , "+" );
		}
		i++;
	}
	if( strrchr( buf , '+' ) )
		*(char *)strrchr( buf , '+' ) = 0;
	return buf;
}

const char * GetSysKeyName( int syskey )
{
	return getkeyname( syskey, syskeyname );
}
