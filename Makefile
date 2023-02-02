default: build

CLEAN=rm -f

clean:
	$(CLEAN) main.o
	$(CLEAN) hello

build: main.o
	gcc main.c -o hello
