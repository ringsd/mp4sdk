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

typedef struct CONFIG_KEYS_LIST {
    VALUE_TYPE type;
    const char * key;
    void * value;
} CONFIG_KEYS_LIST;

int platform_config_load( const char * ini_path, CONFIG_LIST config_list[] );
int platform_config_save( const char * ini_path, CONFIG_LIST config_list[] );
int platform_load_keymap( const char * section, const char * ini_path, KEYMAP * keymap, KEYNAME * keyname );
int platform_save_keymap( const char * section, const char * ini_path, KEYMAP * keymap, KEYNAME * keyname );
int config_load_keys( INI_FILE * ini, const char * section, CONFIG_KEYS_LIST * config_list );
int config_save_keys( INI_FILE * ini, const char * section, CONFIG_KEYS_LIST * config_list );
int config_save_keymap( INI_FILE * ini, const char * section, KEYMAP * keymap, KEYNAME * keyname );
int config_load_keymap( INI_FILE * ini, const char * section, KEYMAP * keymap, KEYNAME * keyname );
int ini_config_save( INI_FILE * ini, CONFIG_LIST config_list[] );
int ini_config_load( INI_FILE * ini, CONFIG_LIST config_list[] );

#ifdef __cplusplus
}
#endif

#endif
