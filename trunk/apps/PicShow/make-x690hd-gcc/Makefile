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
#TEXT_BASE = 0x80800000

# **********************************************
# Directories
# **********************************************

ROOT_DIR  = ../../../
ROOT      = ..
SRCROOT   = ../ \

SRCDIR    = $(SRCROOT)
MXUDIR    = $(ROOT)/lib/gba_mxu
VPATH     = $(SRCDIR) $(MXUDIR)
INCDIR    = $(ROOT_DIR)/MultiPlatform/inc/ \
	$(SRCROOT) \

LIBROOT   = $(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc

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
DEFINE =	__X690HD

#debug setting
ifeq ("$(debug)", "enable")
DEFINE += __DEBUG_TRACE
endif

INCLUDE   = $(addprefix -I,$(INCDIR))
_DEFINE   = $(addprefix -D,$(DEFINE))

W_OPTS    = -fno-pic -pipe -O2 -Wall -finline-functions -fomit-frame-pointer -msoft-float \
-fno-builtin -mcpu=arm9
# `-fno-builtin -mcpu=arm926ej-s

CC_OPTS   = -c -g $(INCLUDE) $(W_OPTS) $(_DEFINE)

CPP_OPTS  =	-c -fno-exceptions -fno-rtti $(INCLUDE) $(W_OPTS) $(_DEFINE)

CC_OPTS_A = $(CC_OPTS)

LD_OPTS   = -Bstatic -g -T $(LD_SCRIPT) -o $(IMAGE_ELF) -Map $(MAP) \
-L$(LIBROOT) -lgcc -lm
# -lgcc -lmem -lm -lstdc++ 

# **********************************************
# Files to be compiled
# **********************************************

SRC_C   = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
SRC_CPP = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.cpp))
SRC_S   = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.S))
SRC     = $(SRC_C) $(SRC_CPP) $(SRC_S)

MXU_SRC = $(foreach dir, $(MXUDIR), $(wildcard $(dir)/*.c))
MXU_MID = $(notdir $(patsubst %.c, %.m, $(MXU_SRC)))
MXU_ASM = $(notdir $(patsubst %.m, %.S, $(MXU_MID)))
MXU_OBJ = $(notdir $(patsubst %.S, %.o, $(MXU_ASM)))

OBJ_C   = $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJ_CPP = $(notdir $(patsubst %.cpp, %.o, $(SRC_CPP)))
OBJ_S   = $(notdir $(patsubst %.S, %.o, $(SRC_S)))
OBJ     = $(OBJ_C) $(OBJ_CPP) $(OBJ_S)

LIBS    = \
	$(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc/miniio.a \
	$(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc/font.a \
	$(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc/MultiMedia.a \
	$(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc/Graphics.a \
	$(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc/xgui.a \
	$(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc/advlib.a \
	$(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc/shell_app.a \
	$(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc/zlib.a \
	$(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc/libc.a \
	$(ROOT_DIR)/MultiPlatform/lib/x690hd_gcc/stdpatch.a \
					
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

$(IMAGE_ELF) : $(OBJ) $(LD_SCRIPT) ./Makefile $(LIBS)
	$(LD) $(OBJ) $(LIBS) $(LD_OPTS)

$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<

$(MXU_OBJ) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<
$(MXU_ASM) : %.S : %.m	
	mxu_as $< > $@
$(MXU_MID) : %.m : %.c
	$(CC) -S $(CC_OPTS) -o $@ $<

.PHONY : clean

clean :
	rm -f $(IMAGE_BIN) $(IMAGE_ELF) $(OBJ) $(MAP) *.dl *.tab *.str *.dsc err.txt

#**************************************************************************
#                              END  OF  FILE
#**************************************************************************
