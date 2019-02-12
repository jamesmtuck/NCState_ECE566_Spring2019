# Swap these lines if on Mac
MacOS = -lcurses
#MacOS = 

.SUFFIXES: %.c

OBJS =  main.o \
	minic.lex.o \
	minic.y.o  \
	list.o \
	symbol.o \
	cmdline.o

.PHONY: tests test all

all: p2

p2: $(OBJS)
	g++ -g -Wno-implicit-function-declaration -o $@ $(OBJS) `llvm-config --cflags --ldflags --libs` -ly -ll -lpthread -ldl $(MacOS)

clean:
	rm -Rf p2 $(OBJS) minic.y.c minic.lex.c minic.y.h cmdline.c 
	make -C ./tests clean

test: tests

tests: p2
	make -C ./tests test

%.o:%.c
	gcc -g -c -o $@ $^ `llvm-config --cflags` 

cmdline.c: cmdline.lex
	flex -PCmd -o$@ cmdline.lex 

minic.y.c: minic.y
	bison -d -o $@ $^

minic.y.h: minic.y.c 

minic.lex.c: minic.lex minic.y.h
	flex -o$@ $^

list.c: list.h

symbol.c:symbol.h uthash.h


