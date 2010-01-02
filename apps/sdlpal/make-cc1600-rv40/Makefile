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

CPP       = armcc
CC        = armcc
LD        = armlink
OBJCOPY   = fromelf

# **********************************************
# Name of application
# **********************************************

TARGET = downenv
LD_SCRIPT = $(LinkScript)

# **********************************************
# Directories
# **********************************************

ROOT      = ..
SRCROOT   = ../ \
			../adplug/ \

SRCDIR    = $(SRCROOT)
VPATH     = $(SRCDIR)
INCDIR    = ../../inc/ \
            ../../inc/SDL/ \
            $(SRCROOT) \
           D:/cygwin/arm-linux/sysroot/usr/include \
#           D:/cygwin/arm-linux/sysroot/usr/include/linux \
#           D:/cygwin/arm-linux/sys-include/gcc/include \
#           D:/cygwin/arm-linux/include/c++/4.0.2 \
#           D:/cygwin/arm-linux/include/c++/4.0.2/arm-linux \
           
INCCMD  = -I"E:/Program Files/ARM/RVCT/Data/4.0/400/include/windows"

DEFCMD  =   -D"__align=arm_align" \
            -D"inline=__inline" \
            -D"__gnuc_va_list=va_list" \

LIBROOT   = ../../lib/cc1600-rv40

RV40LIB   =  --libpath "e:\Program Files\ARM\RVCT\Data\4.0\400\lib"

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

IMAGE_AXF = $(TARGET).axf
IMAGE_SEC = $(TARGET)
MAP       = $(TARGET).map

INCLUDE   = $(INCCMD) $(addprefix -I,$(INCDIR)) 
_DEFINE   = $(DEFCMD) $(addprefix -D,$(DEFINE)) 

CC_OPTS   = -c -W --gnu --arm_only --enum_is_int --cpu=ARM926EJ-S --fpu=SoftVFP -O3 -Otime $(INCLUDE) $(_DEFINE)

CPP_OPTS  =	-c -W --gnu --arm_only --enum_is_int --cpu=ARM926EJ-S --fpu=SoftVFP -O3 -Otime $(INCLUDE) $(_DEFINE)

CC_OPTS_A = --cpu=ARM926EJ-S --fpu=SoftVFP

LD_OPTS   = $(RV40LIB) \
            --arm_only --no_strict_enum_size \
            --map --symbols --xref --no_remove --diag_suppress=6238,6433 \
            --cpu=ARM926EJ-S --fpu=SoftVFP --feedback=fb.txt --list=DLL.LST \
            --scatter=$(LD_SCRIPT)

# **********************************************
# Files to be compiled
# **********************************************

SRC_C   = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
SRC_CPP = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.cpp))
SRC_S   = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.S))
SRC     = $(SRC_C) $(SRC_CPP) $(SRC_S)

OBJ_C   = $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJ_CPP = $(notdir $(patsubst %.cpp, %.o, $(SRC_CPP)))
OBJ_S   = $(notdir $(patsubst %.S, %.o, $(SRC_S)))
OBJ     = $(OBJ_C) $(OBJ_CPP) $(OBJ_S)

LIBS    =   stdpatch.a miniio.a font.a MultiMedia.a Graphics.a xgui.a advlib.a zlib.a \
            $(shell).a libSDL.a

_LIBS		= $(addprefix $(LIBROOT)/,$(LIBS))
					
# **********************************************
# Rules
# **********************************************

.PHONY : all

all : $(IMAGE_SEC)

$(IMAGE_SEC) : $(IMAGE_AXF)
	$(OBJCOPY) $(IMAGE_AXF) -c --output $(IMAGE_SEC) --bin
	cp downenv/IMPORT_STRING_EXEC import.str
	cp downenv/EXPORT_STRING_EXEC export.str
	cp downenv/IMPORT_TABLE_EXEC import.tab
	cp downenv/EXPORT_TABLE_EXEC export.tab
	cp downenv/SECTION_INFO_EXEC section.dsc
	cp downenv/RO_RW_EXEC downenv.bin
	dlmake $(dllcfg)

$(IMAGE_AXF) : $(OBJ) $(LD_SCRIPT) ./Makefile $(_LIBS)
	$(LD) $(OBJ) $(_LIBS) $(LD_OPTS) -o $@

$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<

.PHONY : clean

clean :
	rm -f $(IMAGE_BIN) $(IMAGE_AXF) $(OBJ) $(MAP) *.dl *.tab *.str *.dsc err.txt

#**************************************************************************
#                              END  OF  FILE
#**************************************************************************



