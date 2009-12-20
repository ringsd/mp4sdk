#*************************************************************************\
#      MIPS-GCC-LINUX   Makefile for X760P
#*************************************************************************/
TARGET    = stdpatch

CPP       = sde-g++
CC        = sde-gcc
LD        = sde-ld
OBJCOPY   = sde-objcopy
OBJDUMP   = sde-objdump
AR				=	sde-ar

SRCROOT   = ..

SRCDIR    = $(SRCROOT)

VPATH     = $(SRCDIR)

INCDIR    = ../../../../inc/ \
						$(SRCDIR) \

DEFINE =	__MULTIPLATFORM__           \
					__MULTIPLATFORM_X760P__    	\

#debug setting
ifeq ("$(debug)", "enable")
DEFINE += __DEBUG_TRACE
endif

INCLUDE   = $(addprefix -I,$(INCDIR))
_DEFINE   = $(addprefix -D,$(DEFINE))

W_OPTS    = -fno-pic -pipe -O2 -Wall -finline-functions -fomit-frame-pointer -msoft-float \
-fno-builtin -fsigned-char -mips32 -mno-abicalls -G 0 -EL

CC_OPTS   = -c -g $(INCLUDE) $(W_OPTS) $(_DEFINE)

CPP_OPTS  =	-c -fno-rtti $(INCLUDE) $(W_OPTS) $(_DEFINE)

CC_OPTS_A = $(CC_OPTS)

SRC_C   = _splitpath.c \
					strnicmp.c \
					_makepath.c \
					strcasecmp.c \
					sscanf.c \
					strtoll.c \
					strtoull.c \
					_ctype.c \
					_snprintf.c \
					_vsnprintf.c \
					isatty.c \
					signal.c \
					fscanf.c \
					open.c \
					read.c \
					write.c \
					close.c \
					strtod.c \
					strncasecmp.c \
					ferror.c \
					snprintf.c \
					
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
	cp $(TARGET).a ../../../gcc-mips/
	
$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<
	
.PHONY : clean depend

clean :
	rm -f $(IMAGE_LIB) $(OBJ) $(OBJ_MXU) $(MAP) *.S *.m *.dc *.dcpp
	