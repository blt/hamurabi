CC=/usr/bin/c99-gcc
CFLAGS=-Wall -Werror -pedantic -O2 -g
OBJS=city.o main.o

all: hamurabi

hamurabi: $(OBJS)
	$(CC) $(CFLAGS) -o hamurabi $(OBJS)

main.o: main.c
city.o: city.h city.c

clean:
	$(RM) $(OBJS) hamurabi