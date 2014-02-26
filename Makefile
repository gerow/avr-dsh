CC      = cc
CFLAGS  = -g
CFLAGS += -Iinclude
MAKE    = make

all: local_test


local_test: dsh.o local_test.c
	$(CC) $(CFLAGS) dsh.o local_test.c -o local_test

dsh.o: dsh.c include/dsh/dsh.h
	$(CC) $(CFLAGS) -c dsh.c

clean:
	-rm -f *.o
	-rm -f local_test
