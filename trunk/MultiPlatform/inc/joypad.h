/***************************************************************************

		joypad.h

		No description.

		TIME LIST:
		CREATE	Rainy	2008-10-17 18:31:36

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _joypad_h_
#define _joypad_h_

#ifdef		__cplusplus
extern "C" {
#endif

typedef struct keyname{
	const char * name;
	u32 key;
} KEYNAME;

typedef struct keymap{
	u32 emukey;
	u32 phykey;
} KEYMAP;

#define MAX_BUTTONS 32

enum{
	SYSKEY_UP			= 0x00000001,
	SYSKEY_DOWN		= 0x00000002,
	SYSKEY_LEFT		= 0x00000004,
	SYSKEY_RIGHT	= 0x00000008,
	SYSKEY_A			= 0x00000010,
	SYSKEY_B			= 0x00000020,
	SYSKEY_C			= 0x00000040,
	SYSKEY_D			= 0x00000080,
	SYSKEY_SELECT	= 0x00000100,
	SYSKEY_START	= 0x00000200,
	SYSKEY_L			= 0x00000400,
	SYSKEY_R			= 0x00000800,
	SYSKEY_MENU		= 0x00001000,
	SYSKEY_VOLUP	= 0x00002000,
	SYSKEY_VOLDOWN= 0x00004000,
	SYSKEY_POWER	= 0x00008000,
	SYSKEY_ESCAPE	= 0x00010000,
	SYSKEY_E			= 0x00020000,
	SYSKEY_F			= 0x00040000,
	SYSKEY_SAVE		= 0x00080000,
	SYSKEY_LOAD		= 0x00100000
};

enum{
	FUNC_VOLUP		= 0x00000001,
	FUNC_VOLDOWN	= 0x00000002,
	FUNC_SAVE			= 0x00000004,
	FUNC_LOAD			= 0x00000008
};

u32 read_phykey( void );
u32 emu_read_phykey( void );
u32 getmenukey( void );

//等待按键清空
void menu_clear_key(void);

extern KEYNAME syskeyname[];		//SYS*系列按键名

const char * getkeyname( int key ,KEYNAME *kn );
u32 convert_key( u32 phykey , KEYMAP* key_map );
u32 reconvert_key( u32 emukey , KEYMAP* key_map );

//获取SYS按键名称
const char * GetSysKeyName( int syskey );

extern s32 autofire_interval;
extern u32 autofire_mask;

void set_softkey( u32 key );

//------------------------------------------------------------------------------
typedef enum {
	/* The keyboard syms have been cleverly chosen to map to ASCII */
	KSYM_UNKNOWN		= 0,
	KSYM_FIRST		= 0,
	KSYM_BACKSPACE		= 8,
	KSYM_TAB		= 9,
	KSYM_CLEAR		= 12,
	KSYM_RETURN		= 13,
	KSYM_PAUSE		= 19,
	KSYM_ESCAPE		= 27,
	KSYM_SPACE		= 32,
	KSYM_EXCLAIM		= 33,
	KSYM_QUOTEDBL		= 34,
	KSYM_HASH		= 35,
	KSYM_DOLLAR		= 36,
	KSYM_AMPERSAND		= 38,
	KSYM_QUOTE		= 39,
	KSYM_LEFTPAREN		= 40,
	KSYM_RIGHTPAREN		= 41,
	KSYM_ASTERISK		= 42,
	KSYM_PLUS		= 43,
	KSYM_COMMA		= 44,
	KSYM_MINUS		= 45,
	KSYM_PERIOD		= 46,
	KSYM_SLASH		= 47,
	KSYM_0			= 48,
	KSYM_1			= 49,
	KSYM_2			= 50,
	KSYM_3			= 51,
	KSYM_4			= 52,
	KSYM_5			= 53,
	KSYM_6			= 54,
	KSYM_7			= 55,
	KSYM_8			= 56,
	KSYM_9			= 57,
	KSYM_COLON		= 58,
	KSYM_SEMICOLON		= 59,
	KSYM_LESS		= 60,
	KSYM_EQUALS		= 61,
	KSYM_GREATER		= 62,
	KSYM_QUESTION		= 63,
	KSYM_AT			= 64,
	/* 
	   Skip uppercase letters
	 */
	KSYM_LEFTBRACKET	= 91,
	KSYM_BACKSLASH		= 92,
	KSYM_RIGHTBRACKET	= 93,
	KSYM_CARET		= 94,
	KSYM_UNDERSCORE		= 95,
	KSYM_BACKQUOTE		= 96,
	KSYM_a			= 97,
	KSYM_b			= 98,
	KSYM_c			= 99,
	KSYM_d			= 100,
	KSYM_e			= 101,
	KSYM_f			= 102,
	KSYM_g			= 103,
	KSYM_h			= 104,
	KSYM_i			= 105,
	KSYM_j			= 106,
	KSYM_k			= 107,
	KSYM_l			= 108,
	KSYM_m			= 109,
	KSYM_n			= 110,
	KSYM_o			= 111,
	KSYM_p			= 112,
	KSYM_q			= 113,
	KSYM_r			= 114,
	KSYM_s			= 115,
	KSYM_t			= 116,
	KSYM_u			= 117,
	KSYM_v			= 118,
	KSYM_w			= 119,
	KSYM_x			= 120,
	KSYM_y			= 121,
	KSYM_z			= 122,
	KSYM_DELETE		= 127,
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
	KSYM_WORLD_0		= 160,		/* 0xA0 */
	KSYM_WORLD_1		= 161,
	KSYM_WORLD_2		= 162,
	KSYM_WORLD_3		= 163,
	KSYM_WORLD_4		= 164,
	KSYM_WORLD_5		= 165,
	KSYM_WORLD_6		= 166,
	KSYM_WORLD_7		= 167,
	KSYM_WORLD_8		= 168,
	KSYM_WORLD_9		= 169,
	KSYM_WORLD_10		= 170,
	KSYM_WORLD_11		= 171,
	KSYM_WORLD_12		= 172,
	KSYM_WORLD_13		= 173,
	KSYM_WORLD_14		= 174,
	KSYM_WORLD_15		= 175,
	KSYM_WORLD_16		= 176,
	KSYM_WORLD_17		= 177,
	KSYM_WORLD_18		= 178,
	KSYM_WORLD_19		= 179,
	KSYM_WORLD_20		= 180,
	KSYM_WORLD_21		= 181,
	KSYM_WORLD_22		= 182,
	KSYM_WORLD_23		= 183,
	KSYM_WORLD_24		= 184,
	KSYM_WORLD_25		= 185,
	KSYM_WORLD_26		= 186,
	KSYM_WORLD_27		= 187,
	KSYM_WORLD_28		= 188,
	KSYM_WORLD_29		= 189,
	KSYM_WORLD_30		= 190,
	KSYM_WORLD_31		= 191,
	KSYM_WORLD_32		= 192,
	KSYM_WORLD_33		= 193,
	KSYM_WORLD_34		= 194,
	KSYM_WORLD_35		= 195,
	KSYM_WORLD_36		= 196,
	KSYM_WORLD_37		= 197,
	KSYM_WORLD_38		= 198,
	KSYM_WORLD_39		= 199,
	KSYM_WORLD_40		= 200,
	KSYM_WORLD_41		= 201,
	KSYM_WORLD_42		= 202,
	KSYM_WORLD_43		= 203,
	KSYM_WORLD_44		= 204,
	KSYM_WORLD_45		= 205,
	KSYM_WORLD_46		= 206,
	KSYM_WORLD_47		= 207,
	KSYM_WORLD_48		= 208,
	KSYM_WORLD_49		= 209,
	KSYM_WORLD_50		= 210,
	KSYM_WORLD_51		= 211,
	KSYM_WORLD_52		= 212,
	KSYM_WORLD_53		= 213,
	KSYM_WORLD_54		= 214,
	KSYM_WORLD_55		= 215,
	KSYM_WORLD_56		= 216,
	KSYM_WORLD_57		= 217,
	KSYM_WORLD_58		= 218,
	KSYM_WORLD_59		= 219,
	KSYM_WORLD_60		= 220,
	KSYM_WORLD_61		= 221,
	KSYM_WORLD_62		= 222,
	KSYM_WORLD_63		= 223,
	KSYM_WORLD_64		= 224,
	KSYM_WORLD_65		= 225,
	KSYM_WORLD_66		= 226,
	KSYM_WORLD_67		= 227,
	KSYM_WORLD_68		= 228,
	KSYM_WORLD_69		= 229,
	KSYM_WORLD_70		= 230,
	KSYM_WORLD_71		= 231,
	KSYM_WORLD_72		= 232,
	KSYM_WORLD_73		= 233,
	KSYM_WORLD_74		= 234,
	KSYM_WORLD_75		= 235,
	KSYM_WORLD_76		= 236,
	KSYM_WORLD_77		= 237,
	KSYM_WORLD_78		= 238,
	KSYM_WORLD_79		= 239,
	KSYM_WORLD_80		= 240,
	KSYM_WORLD_81		= 241,
	KSYM_WORLD_82		= 242,
	KSYM_WORLD_83		= 243,
	KSYM_WORLD_84		= 244,
	KSYM_WORLD_85		= 245,
	KSYM_WORLD_86		= 246,
	KSYM_WORLD_87		= 247,
	KSYM_WORLD_88		= 248,
	KSYM_WORLD_89		= 249,
	KSYM_WORLD_90		= 250,
	KSYM_WORLD_91		= 251,
	KSYM_WORLD_92		= 252,
	KSYM_WORLD_93		= 253,
	KSYM_WORLD_94		= 254,
	KSYM_WORLD_95		= 255,		/* 0xFF */

	/* Numeric keypad */
	KSYM_KP0		= 256,
	KSYM_KP1		= 257,
	KSYM_KP2		= 258,
	KSYM_KP3		= 259,
	KSYM_KP4		= 260,
	KSYM_KP5		= 261,
	KSYM_KP6		= 262,
	KSYM_KP7		= 263,
	KSYM_KP8		= 264,
	KSYM_KP9		= 265,
	KSYM_KP_PERIOD		= 266,
	KSYM_KP_DIVIDE		= 267,
	KSYM_KP_MULTIPLY	= 268,
	KSYM_KP_MINUS		= 269,
	KSYM_KP_PLUS		= 270,
	KSYM_KP_ENTER		= 271,
	KSYM_KP_EQUALS		= 272,

	/* Arrows + Home/End pad */
	KSYM_UP			= 273,
	KSYM_DOWN		= 274,
	KSYM_RIGHT		= 275,
	KSYM_LEFT		= 276,
	KSYM_INSERT		= 277,
	KSYM_HOME		= 278,
	KSYM_END		= 279,
	KSYM_PAGEUP		= 280,
	KSYM_PAGEDOWN		= 281,

	/* Function keys */
	KSYM_F1			= 282,
	KSYM_F2			= 283,
	KSYM_F3			= 284,
	KSYM_F4			= 285,
	KSYM_F5			= 286,
	KSYM_F6			= 287,
	KSYM_F7			= 288,
	KSYM_F8			= 289,
	KSYM_F9			= 290,
	KSYM_F10		= 291,
	KSYM_F11		= 292,
	KSYM_F12		= 293,
	KSYM_F13		= 294,
	KSYM_F14		= 295,
	KSYM_F15		= 296,

	/* Key state modifier keys */
	KSYM_NUMLOCK		= 300,
	KSYM_CAPSLOCK		= 301,
	KSYM_SCROLLOCK		= 302,
	KSYM_RSHIFT		= 303,
	KSYM_LSHIFT		= 304,
	KSYM_RCTRL		= 305,
	KSYM_LCTRL		= 306,
	KSYM_RALT		= 307,
	KSYM_LALT		= 308,
	KSYM_RMETA		= 309,
	KSYM_LMETA		= 310,
	KSYM_LSUPER		= 311,		/* Left "Windows" key */
	KSYM_RSUPER		= 312,		/* Right "Windows" key */
	KSYM_MODE		= 313,		/* "Alt Gr" key */
	KSYM_COMPOSE		= 314,		/* Multi-key compose key */

	/* Miscellaneous function keys */
	KSYM_HELP		= 315,
	KSYM_PRINT		= 316,
	KSYM_SYSREQ		= 317,
	KSYM_BREAK		= 318,
	KSYM_MENU		= 319,
	KSYM_POWER		= 320,		/* Power Macintosh power key */
	KSYM_EURO		= 321,		/* Some european keyboards */
	KSYM_UNDO		= 322,		/* Atari keyboard has Undo */

	/* Add any other keys here */

	KSYM_LAST
} SYSKey;
//------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif
