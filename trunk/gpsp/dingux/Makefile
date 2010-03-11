# gpSP makefile
# Gilead Kutnick - Exophase
# GP2X port(ion) - Z

# Global definitions

PREFIX  = /Developer/dingux/mipsel-linux-uclibc
CC        = mipsel-linux-uclibc-gcc
STRIP     = mipsel-linux-uclibc-strip

OBJS      = main.o cpu.o memory.o video.o input.o sound.o gui.o \
            cheats.o zip.o cpu_threaded.o mips_stub.o
BIN       = gpsp.dge 

# Platform specific definitions 

VPATH      += ..
CFLAGS     += -DZAURUS
# NOTE: -funroll-loops will slow down compiling considerably
CFLAGS     += -mips32 -G0
CFLAGS     += -O2 -fno-builtin -fomit-frame-pointer
#CFLAGS     += -g

ASFLAGS    += -g
ASFLAGS    += -mips32

INCLUDES   = -I${PREFIX}/include -I${PREFIX}/include/SDL
LIBS       = -L${PREFIX}/lib -lSDL -lpthread -lz 

# Compilation:

.SUFFIXES: .c

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	$(CC) $(ASFLAGS) $(INCLUDES) -c -o $@ $<

all:	$(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(BIN)  
	$(STRIP) $(BIN)

clean:
	rm -f *.o *.u *.z $(BIN) 

