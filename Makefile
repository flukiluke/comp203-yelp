CC = gcc
CFLAGS = -g -Wall

all: yelp1 yelp2

clean:
	rm -f yelp1 yelp2 *.o

yelp1: main.o btree-std.o safealloc.o io.o
	$(CC) $(CFLAGS) -o yelp1 $^

yelp2: main.o btree-llist.o safealloc.o io.o
	$(CC) $(CFLAGS) -o yelp2 $^

main.o: main.c btree.h safealloc.h io.h
btree-std.o: btree-std.c safealloc.h btree.h btree-std.h
safealloc.o: safealloc.c safealloc.h
io.o: io.c io.h safealloc.h btree.h
btree-llist.o: btree-llist.c btree.h btree-llist.h