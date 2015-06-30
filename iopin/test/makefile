SRCS=main.c
OBJS=$(SRCS:.c=.o)

OUT=test

CFLAGS=-Wall -fpic -g
INCLUDES=-I../

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $^

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm *.o
	rm $(OUT)
