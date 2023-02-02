default: build

build: main.o
	gcc main.c -o hello
	