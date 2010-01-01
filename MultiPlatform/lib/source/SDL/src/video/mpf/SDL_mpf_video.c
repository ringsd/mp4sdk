
#include "SDL_config.h"

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_mpf_video.h"
#include "SDL_mpf_events_c.h"
#include "SDL_mpf_mouse_c.h"

#define MPFVID_DRIVER_NAME "mpf"

/* Initialization/Query functions */
static int MPF_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **MPF_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *MPF_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int MPF_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void MPF_VideoQuit(_THIS);

/* Hardware surface functions */
static int MPF_AllocHWSurface(_THIS, SDL_Surface *surface);
static int MPF_LockHWSurface(_THIS, SDL_Surface *surface);
static void MPF_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void MPF_FreeHWSurface(_THIS, SDL_Surface *surface);

/* etc. */
static void MPF_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

/* MPF driver bootstrap functions */

static int MPF_Available(void)
{

	return(1);
}

static void MPF_DeleteDevice(SDL_VideoDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_VideoDevice *MPF_CreateDevice(int devindex)
{ 
	SDL_VideoDevice *device;
	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
	}
	if ( (device == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			SDL_free(device);
		}
		return(0);
	}

	/* Set the function pointers */
	device->VideoInit = MPF_VideoInit;
	device->ListModes = MPF_ListModes;
	device->SetVideoMode = MPF_SetVideoMode;
	device->CreateYUVOverlay = NULL;
	device->SetColors = MPF_SetColors;
	device->UpdateRects = MPF_UpdateRects;
	device->VideoQuit = MPF_VideoQuit;
	device->AllocHWSurface = MPF_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = MPF_LockHWSurface;
	device->UnlockHWSurface = MPF_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = MPF_FreeHWSurface;
	device->SetCaption = NULL;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->GetWMInfo = NULL;
	device->InitOSKeymap = MPF_InitOSKeymap;
	device->PumpEvents = MPF_PumpEvents;

	device->free = MPF_DeleteDevice;

	return device;
}

VideoBootStrap MPF_bootstrap = {
	"mpf", "SDL mpf video driver",
	MPF_Available, MPF_CreateDevice
};
	
int MPF_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
  vformat->BitsPerPixel = 32;	// mode 3
  vformat->BytesPerPixel = 4;
  vformat->Rmask = 0x00ff0000;
  vformat->Gmask = 0x0000ff00;
  vformat->Bmask = 0x000000ff; 
	
  return(0); 
}

SDL_Rect **MPF_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
   	 return (SDL_Rect **) -1;
}

SDL_Surface *MPF_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	bpp=32;
	screen_set_mode(width,height,LCD_A8R8G8B8);
	#if 0
	if (width ! = screen_get_width()||height != screen_get_height())
	{
		SDL_SetError("unsupported resolution.");
		return(NULL);
	}
	#else

	width = screen_get_width();
	height = screen_get_height();

	#endif

	current->flags = flags & SDL_FULLSCREEN;
	current->w = width;
	current->h = height;
	current->pitch = width  * (bpp / 8);
	current->pixels = lcd_bufferui();
	//SDL_ShowCursor(0);// Òþ²ØÊó±ê
	return(current);
}

static int MPF_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	return(-1);
}
static void MPF_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

/* We need to wait for vertical retrace on page flipped displays */
static int MPF_LockHWSurface(_THIS, SDL_Surface *surface)
{
	return(0);
}

static void MPF_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static void MPF_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	lcd_updateui();

}

int MPF_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	/* do nothing of note. */
	return(1);
}


void MPF_VideoQuit(_THIS)
{
	if (this->screen->pixels != NULL)
	{
		this->screen->pixels = NULL;
	}
}
