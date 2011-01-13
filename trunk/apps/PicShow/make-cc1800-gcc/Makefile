#*************************************************************************\
#
#	Makefile
#
#	defalut.
#
#	TIME LIST:
#	CREATE	Rainy	2009-09-27 10:21:17
#
#	Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
#	All rights reserved.
#
#*************************************************************************/

CPP       = arm-linux-g++
CC        = arm-linux-gcc
LD        = arm-linux-ld
OBJCOPY   = arm-linux-objcopy
OBJDUMP   = arm-linux-objdump

# **********************************************
# Endianness
# **********************************************

ENDIAN  = EL
OFORMAT = elf32-littlemips

# **********************************************
# Name of application
# **********************************************

TARGET = downenv
LD_SCRIPT = link.xn

# **********************************************
# Directories
# **********************************************

ROOT      = ..
SRCROOT   = ../ \

SRCDIR    = $(SRCROOT)
            
VPATH     = $(SRCDIR)
INCDIR    = ../../inc/ \
						$(SRCROOT) \
						
LIBROOT   = ../../lib/x690hd_gcc

# **********************************************
# Image file names and map file
# **********************************************

IMAGE_ELF = $(TARGET).elf
IMAGE_BIN = $(TARGET).bin
MAP       = $(TARGET).map

# **********************************************
# Compiler and linker options
# **********************************************

# machine defined
DEFINE =	__MULTIPLATFORM__ \
            __MULTIPLATFORM_HD8800__ \


#debug setting
ifeq ("$(debug)", "enable")
DEFINE += __DEBUG_TRACE
endif

INCLUDE   = $(addprefix -I,$(INCDIR))
_DEFINE   = $(addprefix -D,$(DEFINE))

W_OPTS    = -fno-pic -pipe -O2 -Wall -finline-functions -fomit-frame-pointer -msoft-float \
-fno-builtin -mcpu=arm926ej-s

CC_OPTS   = -c -g $(INCLUDE) $(W_OPTS) $(_DEFINE)

CPP_OPTS  =	-c -fno-exceptions -fno-rtti $(INCLUDE) $(W_OPTS) $(_DEFINE)

CC_OPTS_A = $(CC_OPTS)

LD_OPTS   = -Bstatic -g -T $(LD_SCRIPT) -o $(IMAGE_ELF) -Map $(MAP) \
-L$(LIBROOT) -lgcc -lm
# -lgcc -lmem -lm -lstdc++ 

# **********************************************
# Files to be compiled
# **********************************************

SRC_C   =   bitmap_zoom.c image_cache.c image_zoom.c PicShow.c ps_config.c ps_menu.c \
            xgui_keyset.c xgui_menu.c

SRC_CPP =   

SRC_S   = 
SRC     = $(SRC_C) $(SRC_CPP) $(SRC_S)

OBJ_C   = $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJ_CPP = $(notdir $(patsubst %.cpp, %.o, $(SRC_CPP)))
OBJ_S   = $(notdir $(patsubst %.S, %.o, $(SRC_S)))
OBJ     = $(OBJ_C) $(OBJ_CPP) $(OBJ_S)

LIBS    = \
					../../lib/x690hd_gcc/miniio.a \
					../../lib/x690hd_gcc/font.a \
					../../lib/x690hd_gcc/MultiMedia.a \
					../../lib/x690hd_gcc/Graphics.a \
					../../lib/x690hd_gcc/xgui.a \
					../../lib/x690hd_gcc/advlib.a \
					../../lib/cc1800_gcc/$(shell).a \
					../../lib/x690hd_gcc/zlib.a \
					../../lib/x690hd_gcc/libc.a \
					../../lib/x690hd_gcc/stdpatch.a \
					
# **********************************************
# Rules
# **********************************************

.PHONY : all

all : $(IMAGE_BIN)

$(IMAGE_BIN) : $(IMAGE_ELF)
	$(OBJCOPY) -O binary -R .export_table -R .export_string -R \
	.import_table -R .import_string -R .section_info $(IMAGE_ELF) $(IMAGE_BIN)
	$(OBJCOPY) -O binary -j .export_table $(IMAGE_ELF) export.tab
	$(OBJCOPY) -O binary -j .export_string $(IMAGE_ELF) export.str
	$(OBJCOPY) -O binary -j .import_table $(IMAGE_ELF) import.tab
	$(OBJCOPY) -O binary -j .import_string $(IMAGE_ELF) import.str
	$(OBJCOPY) -O binary -j .section_info $(IMAGE_ELF) section.dsc
	./dlmake.exe $(dllcfg)

$(IMAGE_ELF) : $(OBJ) $(LD_SCRIPT) ./Makefile $(LIBS)
	$(LD) $(OBJ) $(LIBS) $(LD_OPTS)

$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<

.PHONY : clean

clean :
	rm -f $(IMAGE_BIN) $(IMAGE_ELF) $(OBJ) $(MAP) *.dl *.tab *.str *.dsc err.txt

#**************************************************************************
#                              END  OF  FILE
#**************************************************************************
