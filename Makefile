.PHONY: clean

CC = gcc
CFLAGS = -W -fPIC -Wall -Wextra -O2 -g -std=c99

TESTSRC = test.c


all: test

run: clean all
	./test

test:
	$(CC) -std=c99 $(TESTSRC) -o test

clean:
	rm -rf *.o *.dylib *.dSYM atomic_fork test
