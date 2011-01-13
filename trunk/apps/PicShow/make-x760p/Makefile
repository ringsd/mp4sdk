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

CPP       = sde-g++
CC        = sde-gcc
LD        = sde-ld
OBJCOPY   = sde-objcopy
OBJDUMP   = sde-objdump

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
TEXT_BASE = 0x80800000

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
IMAGE_BIN = $(TARGET).bin
MAP       = $(TARGET).map

# **********************************************
# Compiler and linker options
# **********************************************

INCLUDE   = $(addprefix -I,$(INCDIR))

W_OPTS    = -Wall -finline-functions -fomit-frame-pointer -msoft-float \
-fno-builtin

CC_OPTS   = -g -c -O2 $(INCLUDE) -mips32 $(W_OPTS) -mno-abicalls -fno-pic \
-pipe -G 0  -$(ENDIAN) $(_DEFINE)

CC_OPTS_A = $(CC_OPTS) -D_ASSEMBLER_ -$(ENDIAN)

CPP_OPTS  = -g -c -O2 $(INCLUDE) -mips32 $(W_OPTS) -mno-abicalls -fno-pic \
-pipe -G 0  -$(ENDIAN) $(_DEFINE)

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
					../../lib/x760p/shell.a \
					../../lib/gcc-mips/zlib.a \
					../../lib/gcc-mips/stdpatch.a \
					
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
	$(LD) $(OBJ) $(LIBS) $(LD_OPTS) $(LIBS) $(LIBS)

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
