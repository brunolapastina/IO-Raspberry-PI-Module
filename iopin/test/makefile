SRCS=main.c
OBJS=$(SRCS:.c=.o)

OUT=test

CFLAGS=-Wall -fpic -g
INCLUDES=-I../

ifeq ($(ARCH),ARM)
   CC=~/raspberry/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc
endif

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $^

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm *.o
	rm $(OUT)
