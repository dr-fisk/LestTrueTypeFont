SHELL := /bin/bash

objs = lestTtf.o endianGeneral.o cmap.o head.o glyf.o loca.o maxp.o hhea.o hmtx.o name.o post.o
CC = g++

Q=@

CFLAGS = -g -O2 -std=c++17 -MMD -Wall -Werror -Wextra
EXEC  = main
MAINEXEC = main.c

ifeq ($(BUILD), D)
	CFLAGS += -DDEBUG
endif

ifneq ($(TYPE), lib)
	ifeq ($(TARGET), win)
		CC = x86_64-w64-mingw32-g++
		CFLAGS += -g -Wextra -static-libgcc -static-libstdc++
		EXEC = main.exe
		LDFLAGS = -lws2_32
	endif
else
	ifeq ($(TARGET), win)
		CC = x86_64-w64-mingw32-g++
		CFLAGS += -Wextra -static-libgcc -static-libstdc++
		EXEC = lestttf.dll
		LDFLAGS = -lws2_32 -shared
	endif
endif



all: $(EXEC)

StateDeps := $(patsubst %.o, %.d, $(objs))
-include $(StateDeps)
export

%.o: %.c
	@echo "MAKE $<"
	$(Q)$(CC) $(CFLAGS) $< $(LDFLAGS) -fPIC -c

help:
	@echo "Available commands are:"
	@echo "make TYPE=lib TARGET=win"
	@echo "make TARGET=win"
	@echo "make BUILD=D"

ifneq ($(TYPE), lib)
$(EXEC): $(objs) main.c
	@echo "MAKE $@"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) main.c $(objs) -o $@
else
$(EXEC): $(objs)
	@echo "MAKE $@"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(objs)
endif

clean:
	rm *.o *.d *.exe
