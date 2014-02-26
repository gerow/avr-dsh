CC = cc
CFLAGS = -g
MAKE = make

all: local_test


local_test: dsh.o local_test.c
	$(CC) $(CFLAGS) dsh.o local_test.c -o local_test

dsh.o: dsh.c include/dsh/dsh.h
	$(CC) $(CFLAGS) -c dsh.c

clean:
	-rm -rf *.o
	-rm -rf local_test
