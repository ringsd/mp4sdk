#/***************************************************************************
#
#		Makefile
#
#		defalut.
#
#		TIME LIST:
#		CREATE	Rainy	2009-12-20 13:12:41
#
#		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
#		All rights reserved.
#
#***************************************************************************/

TARGET    = libSDL
CPP       = armcc
CC        = armcc
AR        =	armar -r

# *************************************************************************
# Directories
# *************************************************************************

SRCDIR    = ../ \
	../src/ \
	../src/audio/ \
	../src/cdrom/ \
	../src/cpuinfo/ \
	../src/events/ \
	../src/file/ \
	../src/joystick/ \
	../src/stdlib/ \
	../src/thread/ \
	../src/timer/ \
	../src/video/ \
	../src/joystick/dummy/ \
	../src/cdrom/dummy/ \
	../src/loadso/dummy/ \
	../src/audio/mpf/ \
	../src/thread/mpf/ \
	../src/timer/mpf/ \
	../src/video/mpf/ \
            

INCDIR    =	$(SRCDIR) \
			../../../../inc \
			../include \
            D:/cygwin/arm-linux/sysroot/usr/include \

#include include "-I"
INCCMD		= 

#defined include "-D"
DEFCMD		=	-D"__align=arm_align" \
                -D"inline=__inline" \
                -D"__gnuc_va_list=va_list" \
						
VPATH     = $(SRCDIR)

# *************************************************************************
# Compiler and linker options
# *************************************************************************

INCLUDE   = $(addprefix -I,$(INCDIR)) $(INCCMD)
_DEFINE   = $(addprefix -D,$(DEFINE)) $(DEFCMD)

CC_OPTS   = -c -W --gnu --enum_is_int --cpu=ARM926EJ-S --fpu=SoftVFP -O3 -Otime $(INCLUDE) $(_DEFINE)

CPP_OPTS  =	-c -W --gnu --enum_is_int --cpu=ARM926EJ-S --fpu=SoftVFP -O3 -Otime $(INCLUDE) $(_DEFINE)

CC_OPTS_A = --cpu=ARM926EJ-S --fpu=SoftVFP

# *************************************************************************
# Files to be compiled
# *************************************************************************

SRC_C   = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
SRC_CPP = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.cpp))
SRC_S   = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.S))
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
	cp  $(TARGET).a ../../../cc1600-rv40/

$(IMAGE_LIB) : $(OBJ)
	$(AR) $(TARGET).a $(OBJ)
	$(STR) $(TARGET).a

$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $@.S

.PHONY : clean depend

clean :
	rm -f $(IMAGE_LIB) $(OBJ) $(MAP) *.S *.m *.dc *.dcpp

# *************************************************************************
#                              END  OF  FILE
# *************************************************************************

