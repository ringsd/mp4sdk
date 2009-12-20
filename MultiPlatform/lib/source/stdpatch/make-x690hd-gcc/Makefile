#*************************************************************************\
#      ARM-GCC-LINUX   Makefile for X690HD
#*************************************************************************/
TARGET    = stdpatch

CPP       = arm-linux-g++
CC        = arm-linux-gcc
LD        = arm-linux-ld
OBJCOPY   = arm-linux-objcopy
OBJDUMP   = arm-linux-objdump
AR				=	arm-linux-ar

SRCROOT   = ..

SRCDIR    = $(SRCROOT)

VPATH     = $(SRCDIR)

INCDIR    = ../../../../inc/ \
						$(SRCDIR) \

DEFINE =	__X690HD										\

#debug setting
ifeq ("$(debug)", "enable")
DEFINE += __DEBUG_TRACE
endif

INCLUDE   = $(addprefix -I,$(INCDIR))
_DEFINE   = $(addprefix -D,$(DEFINE))

W_OPTS    = -fno-pic -pipe -O2 -Wall -finline-functions -fomit-frame-pointer -msoft-float \
-fno-builtin -mcpu=arm926ej-s -fsigned-char

CC_OPTS   = -c -g $(INCLUDE) $(W_OPTS) $(_DEFINE)

CPP_OPTS  =	-c -fno-rtti $(INCLUDE) $(W_OPTS) $(_DEFINE)

CC_OPTS_A = $(CC_OPTS)

SRC_C   = _splitpath.c \
					cpp_patch_null.c \
					strnicmp.c \
					_makepath.c \
					qsort.c \
					_snprintf.c \
					isatty.c \
					signal.c \
					open.c \
					read.c \
					write.c \
					close.c \
					lseek.c \
					_div64.c \
					strlcpy.c \
					strlcat.c \
					vfprintf.c \
					vsprintf.c \
					sprintf.c \

SRC_CPP = 

SRC_S   = 

SRC     = $(SRC_C) $(SRC_CPP) $(SRC_S)

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

$(IMAGE_LIB) : $(OBJ) $(OBJ_MXU)
	$(AR) -rus $(TARGET).a $(OBJ) $(OBJ_MXU)
	cp $(TARGET).a ../../../x690hd_gcc/
	
$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<
	
.PHONY : clean depend

clean :
	rm -f $(IMAGE_LIB) $(OBJ) $(OBJ_MXU) $(MAP) *.S *.m *.dc *.dcpp
	