#*************************************************************************\
#
#	Makefile
#
#	defalut.
#
#	TIME LIST:
#	CREATE	Rainy	2009-09-27 10:20:22
#
#	Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
#	All rights reserved.
#*************************************************************************/

MIPSTOOLS = D:/cygwin/mipseltools/include

CPP       = mipsel-linux-g++
CC        = mipsel-linux-gcc
LD        = mipsel-linux-ld
OBJCOPY   = mipsel-linux-objcopy
OBJDUMP   = mipsel-linux-objdump

# **********************************************
# Endianness
# **********************************************

ENDIAN  = EL
OFORMAT = elf32-littlemips

# **********************************************
# Name of application
# **********************************************

TARGET = txtreader
LD_SCRIPT = link.xn
TEXT_BASE = 0x80b80000
# **********************************************
# Directories
# **********************************************

ROOT      = ..
SRCROOT   = ../ \
						../regex/ \

SRCDIR    = $(SRCROOT)
MXUDIR    = $(ROOT)/lib/gba_mxu
VPATH     = $(SRCDIR) $(MXUDIR)
INCDIR    = ../../inc/ \
						$(SRCROOT) \
						
LIBROOT   = ../../lib/gcc-mips

# **********************************************
# Image file names and map file
# **********************************************

IMAGE_ELF = $(TARGET).elf
IMAGE_BIN = $(TARGET).emu
MAP       = $(TARGET).map

# **********************************************
# Compiler and linker options
# **********************************************

INCLUDE   = $(addprefix -I,$(INCDIR))

W_OPTS    = -Wall -finline-functions -fomit-frame-pointer -msoft-float \
-fno-builtin -fno-exceptions

CC_OPTS   = -g -c -O2 $(INCLUDE) -mips32 $(W_OPTS) -mno-abicalls -fno-pic \
-pipe -G 0

CC_OPTS_A = $(CC_OPTS) -D_ASSEMBLER_

CPP_OPTS  = -g -c -O2 $(INCLUDE) -mips32 $(W_OPTS) -mno-abicalls -fno-pic \
-pipe -G 0

LD_OPTS   = -Bstatic -g -T $(LD_SCRIPT) -o $(IMAGE_ELF) -Map $(MAP) \
-Ttext $(TEXT_BASE) -L$(LIBROOT)  -lm -lstdc++ -lc -lgcc -lmem

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

LIBS    = ../../lib/gcc-mips/miniio.a \
					../../lib/gcc-mips/font.a \
					../../lib/gcc-mips/advlib.a \
					../../lib/gcc-mips/wchar.a \
					../../lib/gcc-mips/MultiMedia.a \
					../../lib/gcc-mips/Graphics.a \
					../../lib/gcc-mips/xgui.a \
					../../lib/jzemu/shell.a \
					../../lib/gcc-mips/zlib.a \
					../../lib/gcc-mips/stdpatch.a \
					
# **********************************************
# Rules
# **********************************************

.PHONY : all

all : $(IMAGE_BIN)

$(IMAGE_BIN) : $(IMAGE_ELF)
	$(OBJCOPY) -O binary  $(IMAGE_ELF) $(IMAGE_BIN) 


$(IMAGE_ELF) : $(OBJ) $(LD_SCRIPT) ./Makefile $(LIBS)
	$(LD) $(OBJ) $(LIBS) $(LIBS) $(LD_OPTS) $(LIBS)

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
