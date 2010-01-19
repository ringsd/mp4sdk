/***************************************************************************

		platform_keyboard.h

		defalut.

		TIME LIST:
		CREATE	Rainy	2010-01-16 13:13:44

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.

***************************************************************************/
#ifndef _platform_keyboard_h_
#define _platform_keyboard_h_

#ifdef	__cplusplus
extern "C" {
#endif


/*
 * 0x00 - 0x07 : reserved
 */

#define SYSVK_BACK           0x08
#define SYSVK_TAB            0x09

/*
 * 0x0A - 0x0B : reserved
 */

#define SYSVK_CLEAR          0x0C
#define SYSVK_RETURN         0x0D

#define SYSVK_SHIFT          0x10
#define SYSVK_CONTROL        0x11
#define SYSVK_MENU           0x12
#define SYSVK_PAUSE          0x13
#define SYSVK_CAPITAL        0x14

#define SYSVK_KANA           0x15
#define SYSVK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define SYSVK_HANGUL         0x15
#define SYSVK_JUNJA          0x17
#define SYSVK_FINAL          0x18
#define SYSVK_HANJA          0x19
#define SYSVK_KANJI          0x19

#define SYSVK_ESCAPE         0x1B

#define SYSVK_CONVERT        0x1C
#define SYSVK_NONCONVERT     0x1D
#define SYSVK_ACCEPT         0x1E
#define SYSVK_MODECHANGE     0x1F

#define SYSVK_SPACE          0x20
#define SYSVK_PRIOR          0x21
#define SYSVK_NEXT           0x22
#define SYSVK_END            0x23
#define SYSVK_HOME           0x24
#define SYSVK_LEFT           0x25
#define SYSVK_UP             0x26
#define SYSVK_RIGHT          0x27
#define SYSVK_DOWN           0x28
#define SYSVK_SELECT         0x29
#define SYSVK_PRINT          0x2A
#define SYSVK_EXECUTE        0x2B
#define SYSVK_SNAPSHOT       0x2C
#define SYSVK_INSERT         0x2D
#define SYSVK_DELETE         0x2E
#define SYSVK_HELP           0x2F

/*
 * SYSVK_0 - SYSVK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x40 : unassigned
 * SYSVK_A - SYSVK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */

#define SYSVK_LWIN           0x5B
#define SYSVK_RWIN           0x5C
#define SYSVK_APPS           0x5D

/*
 * 0x5E : reserved
 */

#define SYSVK_SLEEP          0x5F

#define SYSVK_NUMPAD0        0x60
#define SYSVK_NUMPAD1        0x61
#define SYSVK_NUMPAD2        0x62
#define SYSVK_NUMPAD3        0x63
#define SYSVK_NUMPAD4        0x64
#define SYSVK_NUMPAD5        0x65
#define SYSVK_NUMPAD6        0x66
#define SYSVK_NUMPAD7        0x67
#define SYSVK_NUMPAD8        0x68
#define SYSVK_NUMPAD9        0x69
#define SYSVK_MULTIPLY       0x6A
#define SYSVK_ADD            0x6B
#define SYSVK_SEPARATOR      0x6C
#define SYSVK_SUBTRACT       0x6D
#define SYSVK_DECIMAL        0x6E
#define SYSVK_DIVIDE         0x6F
#define SYSVK_F1             0x70
#define SYSVK_F2             0x71
#define SYSVK_F3             0x72
#define SYSVK_F4             0x73
#define SYSVK_F5             0x74
#define SYSVK_F6             0x75
#define SYSVK_F7             0x76
#define SYSVK_F8             0x77
#define SYSVK_F9             0x78
#define SYSVK_F10            0x79
#define SYSVK_F11            0x7A
#define SYSVK_F12            0x7B
#define SYSVK_F13            0x7C
#define SYSVK_F14            0x7D
#define SYSVK_F15            0x7E
#define SYSVK_F16            0x7F
#define SYSVK_F17            0x80
#define SYSVK_F18            0x81
#define SYSVK_F19            0x82
#define SYSVK_F20            0x83
#define SYSVK_F21            0x84
#define SYSVK_F22            0x85
#define SYSVK_F23            0x86
#define SYSVK_F24            0x87

/*
 * 0x88 - 0x8F : unassigned
 */

#define SYSVK_NUMLOCK        0x90
#define SYSVK_SCROLL         0x91

/*
 * 0x97 - 0x9F : unassigned
 */

/*
 * SYSVK_L* & SYSVK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define SYSVK_LSHIFT         0xA0
#define SYSVK_RSHIFT         0xA1
#define SYSVK_LCONTROL       0xA2
#define SYSVK_RCONTROL       0xA3
#define SYSVK_LMENU          0xA4
#define SYSVK_RMENU          0xA5

/*
 * 0xB8 - 0xB9 : reserved
 */

#define SYSVK_OEM_1          0xBA   // ';:' for US
#define SYSVK_OEM_PLUS       0xBB   // '+' any country
#define SYSVK_OEM_COMMA      0xBC   // ',' any country
#define SYSVK_OEM_MINUS      0xBD   // '-' any country
#define SYSVK_OEM_PERIOD     0xBE   // '.' any country
#define SYSVK_OEM_2          0xBF   // '/?' for US
#define SYSVK_OEM_3          0xC0   // '`~' for US

/*
 * 0xC1 - 0xD7 : reserved
 */

/*
 * 0xD8 - 0xDA : unassigned
 */

#define SYSVK_OEM_4          0xDB  //  '[{' for US
#define SYSVK_OEM_5          0xDC  //  '\|' for US
#define SYSVK_OEM_6          0xDD  //  ']}' for US
#define SYSVK_OEM_7          0xDE  //  ''"' for US
#define SYSVK_OEM_8          0xDF

/*
 * 0xE0 : reserved
 */
 
/*
 * 0xE8 : unassigned
 */

/*
 * 0xFF : reserved
 */

/*
 * rename
 */
#define     SYSVK_MINUS             SYSVK_OEM_MINUS     
#define     SYSVK_EQUALS            SYSVK_OEM_PLUS      
#define     SYSVK_LEFTBRACKET       SYSVK_OEM_4         //  '[{' for US
#define     SYSVK_RIGHTBRACKET      SYSVK_OEM_6         //  ']}' for US
#define     SYSVK_BACKSLASH         SYSVK_OEM_5         //  '\|' for US
#define     SYSVK_SEMICOLON         SYSVK_OEM_1         // ';:' for US
#define     SYSVK_QUOTE             SYSVK_OEM_7         //  ''"' for US
#define     SYSVK_COMMA             SYSVK_OEM_COMMA     // ',' any country
#define     SYSVK_PERIOD            SYSVK_OEM_PERIOD    // '.' any country
#define     SYSVK_SLASH             SYSVK_OEM_2         // '/?' for US
#define     SYSVK_BACKQUOTE         SYSVK_OEM_3         // '`~' for US

#define SYSVK_PRESS_MAX 255
#define SYSVK_PRESS_MIN 0

typedef u8 SYSVK;

/*
 * 读取键盘所有键的状态
 * 函数复制一份最大长度为length的数组到vkeys, 
 * 该数组的第i项代表编号为i的按键压力值
 * 该值为 SYSVK_PRESS_MIN 时, 对应的按键为弹起状态
 * 该值为 > SYSVK_PRESS_MIN 时, 对应的按键为按下状态
 * 
 * SYSVK  * vkeys    用来存储键盘状态的缓存
 * int      length   缓存的大小(sizeof(SYSVK)字节单位)
 * 返回值:
 * 0    -- 成功
 * -1   -- 失败
 * 
 * 典型调用方法:
 * SYSVK vkeys[256];
 * sys_get_keyboard( vkeys, 256 );
 * // 此时vkeys中存贮了编号为0~255的按键的键值
 */
int sys_get_keyboard( SYSVK * vkeys, int length );

#ifdef __cplusplus
}
#endif

#endif
