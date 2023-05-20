SHELL := /bin/bash

objs = lestTtf.o endianGeneral.o cmap.o head.o glyf.o loca.o maxp.o hhea.o hmtx.o name.o post.o
CC = g++

Q=@

CFLAGS = -g -std=c++17 -MMD -Wall -Werror
EXEC  = main

ifeq ($(BUILD), D)
	CFLAGS += -DDEBUG
endif

ifeq ($(TARGET), win)
	CC = x86_64-w64-mingw32-g++
	CFLAGS += -Wextra -static-libgcc -static-libstdc++
	EXEC = main.exe
	LDFLAGS = -lws2_32
endif

all: $(EXEC)

StateDeps := $(patsubst %.o, %.d, $(objs))
-include $(StateDeps)
export

%.o: %.c
	@echo "MAKE $<"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) -c $<

$(EXEC): $(objs) main.c
	@echo "MAKE $@"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) main.c $(objs) -o $@

clean:
	rm *.o *.d *.exe
