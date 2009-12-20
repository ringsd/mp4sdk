/***************************************************************************

		platform_config.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-05-02 13:35:01

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _platform_config_h_
#define _platform_config_h_

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum VALUE_TYPE {
	VALUE_TYPE_STRING,
	VALUE_TYPE_INT,
	VALUE_TYPE_UINT,
	VALUE_TYPE_DOUBLE,
	VALUE_TYPE_END
} VALUE_TYPE;

typedef struct CONFIG_LIST {
	char * section;
	char * key;
	int type;
	void * value;
} CONFIG_LIST;

int platform_config_load( const char * ini_path, CONFIG_LIST config_list[] );
int platform_config_save( const char * ini_path, CONFIG_LIST config_list[] );
int platform_load_keymap( const char * section, const char * ini_path, KEYMAP * keymap, KEYNAME * keyname );
int platform_save_keymap( const char * section, const char * ini_path, KEYMAP * keymap, KEYNAME * keyname );

#ifdef __cplusplus
}
#endif

#endif
