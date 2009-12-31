#***************************************************************************
#
#	Makefile
#
#	defalut.
#
#	TIME LIST:
#	CREATE	Rainy	2009-12-27 20:04:37
#
#	Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
#	All rights reserved.
#
#**************************************************************************/

TARGET    = libSDL
CPP       = arm-linux-g++
CC        = arm-linux-gcc
LD        = arm-linux-ld
OBJCOPY   = arm-linux-objcopy
OBJDUMP   = arm-linux-objdump
AR        = arm-linux-ar

# *************************************************************************
# Directories
# *************************************************************************
INC_ENV    = ../../../../inc
ROOTDIR    = ..

SRCDIR    = $(ROOTDIR) \
            $(ROOTDIR)/SDL_image \
            $(ROOTDIR)/SDL_ttf \
            $(ROOTDIR)/src \
            $(ROOTDIR)/src/audio \
            $(ROOTDIR)/src/audio/mpf \
            $(ROOTDIR)/src/cdrom \
            $(ROOTDIR)/src/cdrom/dummy \
            $(ROOTDIR)/src/cpuinfo \
            $(ROOTDIR)/src/events \
            $(ROOTDIR)/src/file \
            $(ROOTDIR)/src/joystick \
            $(ROOTDIR)/src/joystick/dummy \
            $(ROOTDIR)/src/loadso/dummy \
            $(ROOTDIR)/src/main/dummy \
            $(ROOTDIR)/src/stdlib \
            $(ROOTDIR)/src/thread \
            $(ROOTDIR)/src/thread/mpf \
            $(ROOTDIR)/src/timer \
            $(ROOTDIR)/src/timer/mpf \
            $(ROOTDIR)/src/video \
            $(ROOTDIR)/src/video/mpf \
           	$(ROOTDIR)/SDL_image/ \
	          $(ROOTDIR)/SDL_ttf/ \
	          $(ROOTDIR)/SDL_mixer/ \
	          $(ROOTDIR)/SDL_mixer/libmad/ \
	          $(ROOTDIR)/SDL_mixer/timidity/ \
	          $(ROOTDIR)/SDL_mixer/libmikmod/drivers \
	          $(ROOTDIR)/SDL_mixer/libmikmod/include \
	          $(ROOTDIR)/SDL_mixer/libmikmod/loaders \
	          $(ROOTDIR)/SDL_mixer/libmikmod/mmio \
	          $(ROOTDIR)/SDL_mixer/libmikmod/playercode \

INCDIR    =	$(SRCDIR) \
            $(INC_ENV) \
            $(INC_ENV)/freetype \
            $(INC_ENV)/libjpeg \
            $(INC_ENV)/libpng \
            $(INC_ENV)/zlib \
            $(ROOTDIR)/include \

DEFINE =	__MULTIPLATFORM__ \
            __MULTIPLATFORM_CC1600_GCC__ \
            __MULTIPLATFORM_CC1600__ \
            \
            FT2_BUILD_LIBRARY \
            FT_CONFIG_OPTION_NO_ASSEMBLER \

VPATH     = $(SRCDIR)

# *************************************************************************
# Compiler and linker options
# *************************************************************************

INCLUDE   = $(addprefix -I,$(INCDIR))

_DEFINE   = $(addprefix -D,$(DEFINE))

W_OPTS    = -fno-pic -pipe -O2 -Wall -finline-functions -fomit-frame-pointer -msoft-float \
-fno-builtin -mcpu=arm926ej-s -fsigned-char

CC_OPTS   = -c -g $(INCLUDE) $(W_OPTS) $(_DEFINE)

CPP_OPTS  =	-c -fno-exceptions -fno-rtti $(INCLUDE) $(W_OPTS) $(_DEFINE)

CC_OPTS_A = $(CC_OPTS)

# *************************************************************************
# Files to be compiled
# *************************************************************************

SRC_C    =	SDL.c SDL_error.c SDL_fatal.c \
            IMG.c IMG_bmp.c IMG_gif.c IMG_ImageIO.c IMG_jpg.c IMG_lbm.c IMG_pcx.c IMG_png.c IMG_pnm.c IMG_tga.c IMG_tif.c IMG_xcf.c IMG_xpm.c IMG_xv.c IMG_xxx.c \
            SDL_ttf.c \
            SDL_audio.c SDL_audiocvt.c SDL_audiodev.c SDL_mixer.c SDL_mixer_m68k.c SDL_mixer_MMX.c SDL_mixer_MMX_VC.c SDL_wave.c \
            SDL_mpf_audio.c \
            SDL_cdrom.c \
            SDL_syscdrom.c \
            SDL_cpuinfo.c \
            SDL_active.c SDL_events.c SDL_expose.c SDL_keyboard.c SDL_mouse.c SDL_quit.c SDL_resize.c \
            SDL_rwops.c \
            SDL_joystick.c \
            SDL_sysjoystick.c \
            SDL_sysloadso.c \
            SDL_dummy_main.c \
            SDL_getenv.c SDL_iconv.c SDL_malloc.c SDL_qsort.c SDL_stdlib.c SDL_string.c \
            SDL_thread.c \
            SDL_sysmutex.c SDL_syssem.c SDL_systhread.c \
            SDL_timer.c \
            SDL_systimer.c \
            SDL_blit.c SDL_blit_0.c SDL_blit_1.c SDL_blit_A.c SDL_blit_N.c SDL_bmp.c SDL_cursor.c SDL_gamma.c SDL_pixels.c SDL_RLEaccel.c SDL_stretch.c SDL_surface.c SDL_video.c SDL_yuv.c SDL_yuv_mmx.c SDL_yuv_sw.c \
            SDL_mpf_events.c SDL_mpf_mouse.c SDL_mpf_video.c \
            IMG.c IMG_bmp.c IMG_gif.c IMG_ImageIO.c IMG_jpg.c IMG_lbm.c IMG_pcx.c IMG_png.c IMG_pnm.c IMG_tga.c IMG_tif.c IMG_xcf.c IMG_xpm.c IMG_xv.c IMG_xxx.c \
            SDL_ttf.c \
            dynamic_flac.c dynamic_mod.c dynamic_mp3.c dynamic_ogg.c effect_position.c effect_stereoreverse.c effects_internal.c load_aiff.c load_flac.c load_ogg.c load_voc.c mixer.c music.c music_cmd.c music_flac.c music_mad.c music_mod.c music_ogg.c wavestream.c \
            common.c ctrlmode.c filter.c instrum.c mix.c output.c playmidi.c readmidi.c resample.c sdl_a.c sdl_c.c tables.c timidity.c \
            drv_nos.c \
            load_mod.c \
            mmalloc.c mmerror.c mmio.c \
            mdreg.c mdriver.c mdulaw.c mloader.c mlreg.c mlutil.c mplayer.c munitrk.c mwav.c npertab.c sloader.c virtch2.c virtch.c virtch_common.c
						
           
SRC_CPP  =
SRC_S    =
SRC     = $(SRC_C) $(SRC_CPP) $(SRC_S)

#OBJ_C   = $(patsubst %.c, %.o, $(SRC_C))

OBJ_C   = $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJ_CPP = $(notdir $(patsubst %.cpp, %.o, $(SRC_CPP)))
OBJ_S   = $(notdir $(patsubst %.S, %.o, $(SRC_S)))

OBJ     = $(OBJ_C) $(OBJ_CPP) $(OBJ_S)

# *************************************************************************
# Image file names and map file
# *************************************************************************

IMAGE_LIB = $(TARGET).a

# *************************************************************************
# Rules
# *************************************************************************

.PHONY : all

all : $(IMAGE_LIB)

$(IMAGE_LIB) : $(OBJ)
	$(AR) -r $(TARGET).a $(OBJ)

$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<

.PHONY : clean depend

clean :
	rm -f $(IMAGE_LIB) $(OBJ) $(OBJ_MXU) $(MAP) *.S *.m *.dc *.dcpp

# *************************************************************************
#                              END  OF  FILE
# *************************************************************************

