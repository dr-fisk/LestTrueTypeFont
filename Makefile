all : main

objs = lestTtf.o endianGeneral.o cmap.o head.o glyf.o loca.o maxp.o hhea.o hmtx.o name.o
CC = g++

ifneq ($(DEBUG), 1)
  Q=@
  DEBUG = 0
endif

CFLAGS = -g -std=c++17 -MMD -Wall -Werror
StateDeps := $(patsubst %.o, %.d, $(objs))
-include $(StateDeps)
export

%.o: %.c
	@echo "MAKE $<"
	$(Q)$(CC) $(CFLAGS) -c $<

main: $(objs) main.c
	@echo "MAKE $@"
	$(Q)$(CC) $(CFLAGS) main.c $(objs) -o $@

clean:
	rm *.o *.d
