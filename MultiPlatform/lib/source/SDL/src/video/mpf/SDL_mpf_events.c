
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

}

/* end of SDL_nullevents.c ... */

