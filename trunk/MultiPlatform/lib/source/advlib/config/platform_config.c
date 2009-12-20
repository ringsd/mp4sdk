/***************************************************************************

		platform_config.c

		通用配置保存/加载函数.
		需要libini的支持

		TIME LIST:
		CREATE	Rainy	2009-05-02 13:34:55

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <global.h>

int platform_config_load( const char * ini_path, CONFIG_LIST config_list[] )
{
	int i;
	INI_FILE * ini = ini_file_open( ini_path, INI_OPEN_MODE_READ );
	if( ini == NULL )
		goto err;
	i = 0;
	while( config_list[i].section )
	{
		int err;
		switch( config_list[i].type )
		{
			case VALUE_TYPE_STRING:
			err = ini_file_read_string( ini, config_list[i].section, config_list[i].key, config_list[i].value, MAX_PATH );
			break;
			case VALUE_TYPE_INT:
			err = ini_file_read_int( ini, config_list[i].section, config_list[i].key, config_list[i].value );
			break;
			case VALUE_TYPE_UINT:
			err = ini_file_read_uint( ini, config_list[i].section, config_list[i].key, config_list[i].value );
			break;
			case VALUE_TYPE_DOUBLE:
			err = ini_file_read_double( ini, config_list[i].section, config_list[i].key, config_list[i].value );
			break;
			default:
			goto err;
		}
		if( err )
			goto err;
		i++;
	}
	
	ini_file_close( ini );

	return 0;
err:
	return -1;
}

int platform_config_save( const char * ini_path, CONFIG_LIST config_list[] )
{
	int i;
	INI_FILE * ini = ini_file_open( ini_path, INI_OPEN_MODE_RW );
	if( ini == NULL )
		goto err;
	i = 0;
	while( config_list[i].section )
	{
		int err;
		switch( config_list[i].type )
		{
			case VALUE_TYPE_STRING:
			err = ini_file_write_string( ini, config_list[i].section, config_list[i].key, config_list[i].value );
			break;
			case VALUE_TYPE_INT:
			err = ini_file_write_int( ini, config_list[i].section, config_list[i].key, *(int*)config_list[i].value );
			break;
			case VALUE_TYPE_UINT:
			err = ini_file_write_uint( ini, config_list[i].section, config_list[i].key, *(unsigned int*)config_list[i].value );
			break;
			case VALUE_TYPE_DOUBLE:
			err = ini_file_write_double( ini, config_list[i].section, config_list[i].key, *(double *)config_list[i].value );
			break;
			default:
			goto err;
		}
		if( err )
			goto err;
		i++;
	}
	
	ini_file_close( ini );
	return 0;
err:
	if( ini )
		ini_file_close( ini );
	return -1;
}


int platform_save_keymap( const char * section, const char * ini_path, KEYMAP * keymap, KEYNAME * keyname )
{
	int i = 0;
	INI_FILE * ini = ini_file_open( ini_path, INI_OPEN_MODE_RW );
	if( ini == NULL )
		goto err;
	while( keymap[i].emukey )
	{
		ini_file_write_uint( ini, section, getkeyname(keymap[i].emukey, keyname), keymap[i].phykey );
		i++;
	}
	ini_file_close( ini );
	return 0;
err:
	TRACE_LOG("error!");
	return -1;
}

int platform_load_keymap( const char * section, const char * ini_path, KEYMAP * keymap, KEYNAME * keyname )
{
	int i = 0;
	INI_FILE * ini = ini_file_open( ini_path, INI_OPEN_MODE_READ );
	if( ini == NULL )
		goto err;
	while( keymap[i].emukey )
	{
		ini_file_read_uint( ini, section, getkeyname(keymap[i].emukey, keyname), &keymap[i].phykey );
		i++;
	}
	ini_file_close( ini );
	return 0;
err:
	TRACE_LOG("error!");
	return -1;
}

