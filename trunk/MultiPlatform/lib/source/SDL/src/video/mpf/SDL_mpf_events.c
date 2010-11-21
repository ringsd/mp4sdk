
#include "SDL_config.h"


#include "SDL.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"
#include "SDL_events.h"

#include "SDL_mpf_video.h"
#include "SDL_mpf_events_c.h"


typedef struct MPFKey{
	Uint32	MPF_key;		/* Length of wave data */
	SDLKey	SDL_key;			/* Pointer to wave data */
} MPFKey ;

static  MPFKey mpf_keymap[]={
{		SYSKEY_UP			,		SDLK_UP				},
{		SYSKEY_DOWN			,		SDLK_DOWN			},
{		SYSKEY_LEFT			,		SDLK_LEFT			},
{		SYSKEY_RIGHT		,		SDLK_RIGHT			},

{		SYSKEY_A			,		SDLK_a				},
{		SYSKEY_B			,		SDLK_b				},
{		SYSKEY_C			,		SDLK_c				},
{		SYSKEY_D			,		SDLK_d				},

{		SYSKEY_E			,		SDLK_e				},
{		SYSKEY_F			,		SDLK_f				},
{		SYSKEY_SELECT		,		SDLK_TAB			},
{		SYSKEY_START		,		SDLK_KP_ENTER		},

{		SYSKEY_L			,		SDLK_l				},
{		SYSKEY_R			,		SDLK_r				},
{		SYSKEY_MENU			,		SDLK_MENU			},
{		SYSKEY_VOLUP		,		SDLK_KP_MINUS		},

{		SYSKEY_VOLDOWN		,		SDLK_KP_MINUS		},
{		SYSKEY_SAVE			,		SDLK_s				},
{		SYSKEY_LOAD			,		SDLK_o				},
{		SYSKEY_POWER		,		SDLK_POWER			},

{		SYSKEY_ESCAPE		,		SDLK_ESCAPE			},
{		0					,		0					},
};

typedef struct KEY_MAPPER{
	Uint32	skey;   // 原按键
	Uint32	dkey;   // 转换按键
} KEY_MAPPER;

static KEY_MAPPER mpf_sdl_keymap[] = {
{  SYSVK_BACK       ,  SDLK_BACKSPACE   },
{  SYSVK_TAB        ,  SDLK_TAB         },
{  SYSVK_RETURN     ,  SDLK_RETURN      },
{  SYSVK_LSHIFT     ,  SDLK_LSHIFT      },
{  SYSVK_RSHIFT     ,  SDLK_RSHIFT      },
{  SYSVK_LCONTROL   ,  SDLK_LCTRL       },
{  SYSVK_RCONTROL   ,  SDLK_RCTRL       },
{  SYSVK_LMENU      ,  SDLK_LALT        },
{  SYSVK_RMENU      ,  SDLK_RALT        },
{  SYSVK_SPACE      ,  SDLK_SPACE       },
{  SYSVK_CAPITAL    ,  SDLK_CAPSLOCK    },

{  '0'              ,  SDLK_0           },
{  '1'              ,  SDLK_1           },
{  '2'              ,  SDLK_2           },
{  '3'              ,  SDLK_3           },
{  '4'              ,  SDLK_4           },
{  '5'              ,  SDLK_5           },
{  '6'              ,  SDLK_6           },
{  '7'              ,  SDLK_7           },
{  '8'              ,  SDLK_8           },
{  '9'              ,  SDLK_9           },
{  'A'              ,  SDLK_a           },
{  'B'              ,  SDLK_b           },
{  'C'              ,  SDLK_c           },
{  'D'              ,  SDLK_d           },
{  'E'              ,  SDLK_e           },
{  'F'              ,  SDLK_f           },
{  'G'              ,  SDLK_g           },
{  'H'              ,  SDLK_h           },
{  'I'              ,  SDLK_i           },
{  'J'              ,  SDLK_j           },
{  'K'              ,  SDLK_k           },
{  'L'              ,  SDLK_l           },
{  'M'              ,  SDLK_m           },
{  'N'              ,  SDLK_n           },
{  'O'              ,  SDLK_o           },
{  'P'              ,  SDLK_p           },
{  'Q'              ,  SDLK_q           },
{  'R'              ,  SDLK_r           },
{  'S'              ,  SDLK_s           },
{  'T'              ,  SDLK_t           },
{  'U'              ,  SDLK_u           },
{  'V'              ,  SDLK_v           },
{  'W'              ,  SDLK_w           },
{  'X'              ,  SDLK_x           },
{  'Y'              ,  SDLK_y           },
{  'Z'              ,  SDLK_z           },

{  SYSVK_OEM_MINUS  ,  SDLK_MINUS       },
{  SYSVK_OEM_PLUS   ,  SDLK_EQUALS      },
{  SYSVK_OEM_4      ,  SDLK_LEFTBRACKET },  //  '[{' for US
{  SYSVK_OEM_6      ,  SDLK_RIGHTBRACKET},  //  ']}' for US
{  SYSVK_OEM_5      ,  SDLK_BACKSLASH   },  //  '\|' for US
{  SYSVK_OEM_1      ,  SDLK_SEMICOLON   },  // ';:' for US
{  SYSVK_OEM_7      ,  SDLK_QUOTE       },  //  ''"' for US
{  SYSVK_OEM_COMMA  ,  SDLK_COMMA       },  // ',' any country
{  SYSVK_OEM_PERIOD ,  SDLK_PERIOD      },  // '.' any country
{  SYSVK_OEM_2      ,  SDLK_SLASH       },  // '/?' for US
{  SYSVK_OEM_3      ,  SDLK_BACKQUOTE   },  // '`~' for US

{  SYSVK_LEFT       ,  SDLK_LEFT        },
{  SYSVK_UP         ,  SDLK_UP          },
{  SYSVK_RIGHT      ,  SDLK_RIGHT       },
{  SYSVK_DOWN       ,  SDLK_DOWN        },

{  SYSVK_ESCAPE     ,  SDLK_ESCAPE      },
{  SYSVK_F1         ,  SDLK_F1          },
{  SYSVK_F2         ,  SDLK_F2          },
{  SYSVK_F3         ,  SDLK_F3          },
{  SYSVK_F4         ,  SDLK_F4          },
{  SYSVK_F5         ,  SDLK_F5          },
{  SYSVK_F6         ,  SDLK_F6          },
{  SYSVK_F7         ,  SDLK_F7          },
{  SYSVK_F8         ,  SDLK_F8          },
{  SYSVK_F9         ,  SDLK_F9          },
{  SYSVK_F10        ,  SDLK_F10         },
{  SYSVK_F11        ,  SDLK_F11         },
{  SYSVK_F12        ,  SDLK_F12         },

{  SYSVK_SNAPSHOT   ,  SDLK_SYSREQ      },
{  SYSVK_SCROLL     ,  SDLK_SCROLLOCK   },
{  SYSVK_PAUSE      ,  SDLK_BREAK       },

{  SYSVK_INSERT     ,  SDLK_INSERT      },
{  SYSVK_DELETE     ,  SDLK_DELETE      },
{  SYSVK_HOME       ,  SDLK_HOME        },
{  SYSVK_END        ,  SDLK_END         },
{  SYSVK_PRIOR      ,  SDLK_PAGEUP      },
{  SYSVK_NEXT       ,  SDLK_PAGEDOWN    },

{  SYSVK_NUMLOCK    ,  SDLK_NUMLOCK     },
{  SYSVK_DIVIDE     ,  SDLK_KP_DIVIDE   },
{  SYSVK_MULTIPLY   ,  SDLK_KP_MULTIPLY },
{  SYSVK_SUBTRACT   ,  SDLK_KP_MINUS    },
{  SYSVK_ADD        ,  SDLK_KP_PLUS     },
{  SYSVK_SEPARATOR  ,  SDLK_PERIOD      },

{  SYSVK_NUMPAD0    ,  SDLK_KP0         },
{  SYSVK_NUMPAD1    ,  SDLK_KP1         },
{  SYSVK_NUMPAD2    ,  SDLK_KP2         },
{  SYSVK_NUMPAD3    ,  SDLK_KP3         },
{  SYSVK_NUMPAD4    ,  SDLK_KP4         },
{  SYSVK_NUMPAD5    ,  SDLK_KP5         },
{  SYSVK_NUMPAD6    ,  SDLK_KP6         },
{  SYSVK_NUMPAD7    ,  SDLK_KP7         },
{  SYSVK_NUMPAD8    ,  SDLK_KP8         },
{  SYSVK_NUMPAD9    ,  SDLK_KP9         },

{  0                ,  0                }
};

static int convert_keyboard( SYSVK key, KEY_MAPPER * keymap, SDL_keysym * sym )
{
    int i           = 0;
    sym->sym        = SDLK_UNKNOWN;
    sym->scancode   = 0;
    sym->mod        = KMOD_NONE;
    sym->unicode    = 0;
    
    if( key == 0 )
        return 0;
        
    while( keymap[i].skey )
    {
        if( key == keymap[i].skey )
        {
            sym->sym        = keymap[i].dkey;
            sym->scancode   = keymap[i].skey;
            return 0;
        }
        i++;
    }
    
    return -1;
}

static SYSVK        sys_keys[2][256];
static int          sys_keys_flip;
static SDL_keysym   sym;
static SDLMod       mod;

static int sdl_update_keyboard( void )
{
    if( sys_get_keyboard( sys_keys[sys_keys_flip], 256 ) == 0 )
    {
        int i;
        for( i = 0; i < 256; i++ )
        {
            if( sys_keys[sys_keys_flip  ][i] == SYSVK_PRESS_MIN &&
                sys_keys[sys_keys_flip^1][i] >  SYSVK_PRESS_MIN )
            {
                if( convert_keyboard( i, mpf_sdl_keymap, &sym ) == 0 )
                {
                    switch( sym.sym )
                    {
                        case SDLK_CAPSLOCK:  
                            break;
                        case SDLK_NUMLOCK:
                            break;
                        default:
                            SDL_PrivateKeyboard( SDL_RELEASED, &sym );
                    }
                }
            }
            else
            if( sys_keys[sys_keys_flip  ][i] >  SYSVK_PRESS_MIN &&
                sys_keys[sys_keys_flip^1][i] == SYSVK_PRESS_MIN )
            {
                Uint8 state;
                if( convert_keyboard( i, mpf_sdl_keymap, &sym ) == 0 )
                {
                    if( sym.sym == SDLK_CAPSLOCK )
                    {
                        if( mod & KMOD_CAPS )
                        {
                            state = SDL_RELEASED;
                        }
                        else
                        {
                            state = SDL_PRESSED;
                        }
                        mod ^= KMOD_CAPS;
                    }
                    else
                    if( sym.sym == SDLK_NUMLOCK )
                    {
                        if( mod & KMOD_NUM )
                        {
                            state = SDL_RELEASED;
                        }
                        else
                        {
                            state = SDL_PRESSED;
                        }
                        mod ^= KMOD_NUM;
                    }
                    else
                    {
                        state = SDL_PRESSED;
                    }
                    
                    SDL_PrivateKeyboard( state, &sym );
                }
            }
        }
        sys_keys_flip ^= 1;
        return 0;
    }
    return -1;
}

static Uint32 SDL_lastkeys;

static Uint32 mousemove_sta=0,mousebtn_sta=0;

void MPF_PumpEvents(_THIS)
{	
	Uint32 keys=sys_get_key();
	Uint32 keysd = ~SDL_lastkeys&keys;//按下的键
	Uint32 keysu = SDL_lastkeys&~keys;//弹起的键
	SDL_keysym sym;
	Uint32 i;
	Uint32 mouse_x,mouse_y,mouse_sta;


	SDL_lastkeys=keys;
    
    if( sdl_update_keyboard() == -1 )
    {
    	for (i=0;i<21 ;i++ )
    	{
    		if (keysd&mpf_keymap[i].MPF_key)
    		{
    			sym.sym=mpf_keymap[i].SDL_key;
    			sym.scancode=mpf_keymap[i].MPF_key;
    			SDL_PrivateKeyboard(SDL_PRESSED,&sym);
    		}		
    		if (keysu&mpf_keymap[i].MPF_key)
    		{
    			sym.sym=mpf_keymap[i].SDL_key;
    			sym.scancode=mpf_keymap[i].MPF_key;
    			SDL_PrivateKeyboard(SDL_RELEASED,&sym);
    		}
    	}
    }
    
#if 0 //按键版
	mouse_x=0;mouse_y=0;
	if (keys&SYSKEY_UP)
	{
		mouse_y-=3;
	}
	if (keys&SYSKEY_DOWN)
	{
		mouse_y+=3;
	}
	if (keys&SYSKEY_RIGHT)
	{
		mouse_x+=3;
	}
	if (keys&SYSKEY_LEFT)
	{
		mouse_x-=3;
	}
	if (mouse_x!=0||mouse_y!=0)
	{
		SDL_PrivateMouseMotion(0, 1, mouse_x, mouse_y);
	}
	if (keysd&SYSKEY_A)
	{
		SDL_PrivateMouseButton(SDL_PRESSED, 1, 0, 0);
	}	
	if (keysu&SYSKEY_A)
	{
		SDL_PrivateMouseButton(SDL_RELEASED, 1, 0, 0);
	}
#endif

	if( sys_get_pointer(&mouse_x,&mouse_y,&mouse_sta) == 0 ){
		if (mouse_sta>0)
		{
			if (mousemove_sta==0||mousebtn_sta!=0)
			{
				SDL_PrivateMouseMotion(0, 0, mouse_x, mouse_y);mousemove_sta=1;
			}
			else
			{
				if (mousebtn_sta==0)
				{
					SDL_PrivateMouseButton(SDL_PRESSED, 1, mouse_x, mouse_y);mousebtn_sta=mouse_sta;
				}
			}
		}
		else
		{
			if (mousebtn_sta!=0)
			{
				SDL_PrivateMouseButton(SDL_RELEASED, 1,0, 0);mousemove_sta=0;mousebtn_sta=0;
			}
		}
	}

}


void MPF_InitOSKeymap(_THIS)
{
    memset( sys_keys, 0, sizeof(sys_keys) );
    sys_keys_flip = 0;
    sym.scancode  = 0;
    sym.sym       = SDLK_UNKNOWN;
    sym.mod       = KMOD_NONE;
    mod           = KMOD_NONE;
}

/* end of SDL_nullevents.c ... */

