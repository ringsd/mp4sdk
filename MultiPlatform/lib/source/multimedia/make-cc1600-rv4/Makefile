#*************************************************************************\
#  																lzysoft                                 *
#                                                                         *
#   PROJECT     : ccpmp                                                   *
#                                                                         *
#   MODULE      : MakeFile                                                *
#                                                                         *
#   PROCESSOR   : arm926 x690hd															              *
#                                                                         *
#   Tool-chain  : arm-linux-gcc                                           *
#                                                                         *
#   DESCRIPTION :                                                         *
#   Makefile used for building application.                               *
#                                                                         *
#   The default target (all) builds application in two formats :          *
#   *.a : The file is archives for ccpmp.                                 *
#                                                                         *
#   Other targets are :                                                   *
#   clean :    Deletes all files generated by makefile.                   *
#                                                                         *
#  History :                                                              *
#    Version      Date      Author     Memo                               *
#     1.0     2008-1-30      rainy    Created.                            *
#*************************************************************************/

TARGET		= MultiMedia

CPP       = armcc
CC        = armcc
LD        = armlink
AR				=	ar rus

# *************************************************************************
# Directories
# *************************************************************************
ROOTDIR    = ..

SRCDIR    = $(ROOTDIR) \
						$(ROOTDIR)/bmp \
						$(ROOTDIR)/ico \
						$(ROOTDIR)/jpg \
						$(ROOTDIR)/png \
						$(ROOTDIR)/png/libpng-1.2.35-src \
						$(ROOTDIR)/png/zlib-1.2.3 \
						$(ROOTDIR)/common/imager \
						$(ROOTDIR)/jpg/jpeg-6b \
						
INCDIR    =	$(SRCDIR) \
						../../../../inc \
						D:/cygwin/arm-linux/sysroot/usr/include \
						
#include include "-I"
INCCMD		= 

#defined include "-D"
DEFCMD		=	-D"__align=arm_align" \
						-D"inline=__inline" \
						-D"__gnuc_va_list=va_list" \

# defined
DEFINE =	__MULTIPLATFORM__ \
					__MULTIPLATFORM_CC1600_GCC__ \
					__MULTIPLATFORM_CC1600__ \
					\
					
VPATH     = $(SRCDIR)

# *************************************************************************
# Compiler and linker options
# *************************************************************************

INCLUDE   = $(addprefix -I,$(INCDIR))

INCLUDE   = $(addprefix -I,$(INCDIR)) $(INCCMD)
_DEFINE   = $(addprefix -D,$(DEFINE)) $(DEFCMD)

CC_OPTS   = -c -W --gnu --enum_is_int --cpu=ARM926EJ-S --fpu=SoftVFP -O3 -Otime $(INCLUDE) $(_DEFINE)

CPP_OPTS  =	-c -W --gnu --enum_is_int --cpu=ARM926EJ-S --fpu=SoftVFP -O3 -Otime $(INCLUDE) $(_DEFINE)

CC_OPTS_A = --cpu=ARM926EJ-S --fpu=SoftVFP

# *************************************************************************
# Files to be compiled
# *************************************************************************

SRC_IMAGER = 

SRC_LIBJPEG = jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c \
              jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c \
              jcphuff.c jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c \
              jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c \
              jdinput.c jdmainct.c jdmarker.c jdmaster.c jdmerge.c jdphuff.c \
              jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c \
              jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jmemnobs.c \
              jmemmgr.c jquant1.c jquant2.c jutils.c rdbmp.c rdcolmap.c rdppm.c \
              rdrle.c rdswitch.c rdtarga.c transupp.c wrbmp.c wrgif.c wrppm.c \
              wrrle.c wrtarga.c \

SRC_LIBPNG =	png.c pngerror.c pnggccrd.c pngget.c pngmem.c pngpread.c pngread.c \
							pngrio.c pngrtran.c pngrutil.c pngset.c pngtrans.c pngvcrd.c pngwio.c \
							pngwrite.c pngwtran.c pngwutil.c \
          
SRC_C    = MM_Image.c \
					 MM_imager.c \
           MM_ico.c \
           msicon.c \
           MM_bmp.c \
           MM_jpg.c \
           MM_png.c \
           $(SRC_IMAGER) \
           $(SRC_LIBJPEG) \
           $(SRC_LIBPNG) \
           
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

$(IMAGE_LIB) : $(OBJ)
	$(AR) $(TARGET).a $(OBJ)
	cp $(TARGET).a ../../../cc1600-rv40/

$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $@.S

.PHONY : clean depend

clean :
	rm -f $(IMAGE_LIB) $(OBJ) $(MAP)

# *************************************************************************
#                              END  OF  FILE
# *************************************************************************

