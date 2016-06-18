.PHONY: clean

CC = gcc
CFLAGS = -W -fPIC -Wall -Wextra -O2 -g -std=c99
LDFLAGS = -shared -ldl

SRC = atomic_fork.c
TESTSRC = test.c
OBJ = $(SRC:.c=.o)


all: atomic_fork test

run: clean all
	./test

test:
	$(CC) -std=c99 $(TESTSRC) -L./ -lAtomicFork -o test

clean:
	rm -rf *.o *.dylib *.dSYM atomic_fork test

main.o:
	gcc $(CFLAGS) -c atomic_fork.c -o main.o

atomic_fork: main.o
	gcc $(LDFLAGS) -o libAtomicFork.dylib main.o
