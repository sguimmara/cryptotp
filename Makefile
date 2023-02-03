default: build

CLEAN =	rm -f
INSTALL_PREFIX = usr/bin
BINARY = cryptotp
CC = gcc -O3 -Wall -std=c11 -D_POSIX_SOURCE

clean:
	$(CLEAN) main.o
	$(CLEAN) $(BINARY)

main.o: main.c
	$(CC) main.c -o $(BINARY)

build: main.o

install: build
	cp $(BINARY) $(INSTALL_PREFIX)
