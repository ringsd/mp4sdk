#/***************************************************************************
#
#		Makefile
#
#		defalut.
#
#		TIME LIST:
#		CREATE	Rainy	2009-12-11 08:26:29
#
#		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
#		All rights reserved.
#
#***************************************************************************/

CPP       = armcc
CC        = armcc
LD        = armlink
OBJCOPY   = fromelf
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
LD_SCRIPT = $(LinkScript)

# **********************************************
# Directories
# **********************************************

ROOT      = ..
SRCROOT   = ..

SRCDIR    = $(SRCROOT) \
            
VPATH     = $(SRCDIR)

INCDIR    = ../../inc \
            ../../inc/sdl \
            ../include \
            $(SRCDIR) \

INCCMD		= -I"E:/Program Files/ARM/RVCT/Data/4.0/400/include/windows"

DEFCMD		=	-D"__align=arm_align" \
                -D"inline=__inline" \
                -D"__gnuc_va_list=va_list" \

# **********************************************
# Image file names and map file
# **********************************************

IMAGE_AXF = $(TARGET).axf
IMAGE_SEC = $(TARGET)
MAP       = $(TARGET).map

# **********************************************
# Compiler and linker options
# **********************************************

# machine defined
DEFINE =	__MULTIPLATFORM__ \
            __MULTIPLATFORM_HD8800__ \
            
LIBROOT   = ../../lib/cc1600-rv40

RV40LIB   =  --libpath "e:\Program Files\ARM\RVCT\Data\4.0\400\lib"

#debug setting
ifeq ("$(debug)", "enable")
DEFINE += __DEBUG_TRACE
DEBUG_OPT = -O2 --debug --dwarf2
else
DEBUG_OPT = -O2 -Otime -W
endif

INCLUDE   = $(addprefix -I,$(INCDIR)) $(INCCMD)
_DEFINE   = $(addprefix -D,$(DEFINE)) $(DEFCMD)

CC_OPTS   = $(DEBUG_OPT) --gnu --arm_only --enum_is_int --cpu=ARM926EJ-S --fpu=SoftVFP $(INCLUDE) $(_DEFINE)

CPP_OPTS  =	$(DEBUG_OPT) --gnu --arm_only --enum_is_int --cpu=ARM926EJ-S --fpu=SoftVFP --exceptions $(INCLUDE) $(_DEFINE)

CC_OPTS_A = --cpu=ARM926EJ-S --fpu=SoftVFP

LD_OPTS   = --libpath $(LIBROOT) $(RV40LIB) --debug --verbose \
            --arm_only --no_strict_enum_size --muldefweak \
            --map --symbols --xref --no_remove --diag_suppress=6238,6373,6433 \
            --cpu=ARM926EJ-S --fpu=SoftVFP --feedback=fb.txt --list=DLL.LST \
            --scatter=$(LD_SCRIPT)

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

PREC_C   = $(notdir $(patsubst %.c, %.prec, $(SRC_C)))
PREC_CPP = $(notdir $(patsubst %.cpp, %.prec, $(SRC_CPP)))
PREC     = $(PREC_C) $(PREC_CPP)

LIBS    =   libSDL.a stdpatch.a miniio.a font.a MultiMedia.a Graphics.a xgui.a advlib.a zlib.a \
            $(shell).a

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
	./dlmake.exe $(dllcfg)

$(IMAGE_AXF) : $(OBJ) $(LD_SCRIPT) ./Makefile $(_LIBS)
	$(LD) $(OBJ) $(_LIBS) $(LD_OPTS) -o $@

$(OBJ_C) : %.o : %.c
	$(CC) -c $(CC_OPTS) -o $@ $<

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) -c $(CPP_OPTS) -o $@ $<

$(PREC_C) : %.prec : %.c
	$(CC) -E $(CC_OPTS) -o $@ $<

$(PREC_CPP) : %.prec : %.cc
	$(CPP) -E $(CPP_OPTS) -o $@ $<
	
$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<

.PHONY : clean

clean :
	rm -rf $(IMAGE_BIN) $(IMAGE_AXF) $(OBJ) $(MAP) *.dl *.tab *.str *.dsc \
	*.prec err.txt *.s fb.txt downenv.bin *.cc DLL.LST downenv
    
#**************************************************************************
#                              END  OF  FILE
#**************************************************************************
