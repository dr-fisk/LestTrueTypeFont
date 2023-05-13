all : main

objs = lestTtf.o endianGeneral.o cmap.o
CC = g++

CFLAGS = -g -std=c++17 -MMD -Wall -Werror
StateDeps := $(patsubst %.o, %.d, $(objs))
-include $(StateDeps)

%.o: %.c
	@echo "MAKE $<"
	$(Q)$(CC) $(CFLAGS) -c $<

main: $(objs) main.c
	@echo "MAKE $@"
	$(Q)$(CC) $(CFLAGS) main.c $(objs) -o $@

clean:
	rm *.o *.d
