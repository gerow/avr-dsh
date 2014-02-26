AVR_CC        = avr-gcc
AVR_CFLAGS    = -Os

LOCAL_CC      = $(CC)
LOCAL_CFLAGS  = -g

CFLAGS        = -Iinclude -Wall

MAKE          = make

all: dsh.o

local_test: dsh.o.local local_test.c
	$(LOCAL_CC) $(CFLAGS) $(LOCAL_CFLAGS) dsh.o.local local_test.c -o local_test

dsh.o: dsh.c include/dsh/dsh.h
	$(AVR_CC) $(CFLAGS) $(AVR_CFLAGS) -c dsh.c

dsh.o.local: dsh.c include/dsh/dsh.h
	$(LOCAL_CC) $(CFLAGS) $(LOCAL_CFLAGS) -c dsh.c -o dsh.o.local

clean:
	-rm -f *.o
	-rm -f *.o.local
	-rm -f local_test
