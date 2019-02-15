

.SUFFIXES: %.c

OBJS =  main.o \
	minic.y.o  \
	minic.lex.o \
	list.o \
	symbol.o \
	cmdline.lex.o

.PHONY: tests test all

all: p2

p2: $(OBJS)
	clang++ -g -Wno-implicit-function-declaration -o $@ $(OBJS) `llvm-config --cflags --ldflags --libs` -ly -ll `llvm-config --system-libs`

clean:
	rm -Rf p2 $(OBJS) minic.y.c minic.lex.c minic.y.h cmdline.lex.c 
	make -C ./tests clean

test: tests

tests: p2
	make -C ./tests test

%.o:%.c
	clang -g -c -o $@ $^ `llvm-config --cflags` 

cmdline.lex.c: cmdline.lex
	flex -PCmd -o$@ cmdline.lex 

%.y.c: %.y
	bison -d -o $@ $<

minic.y.h: minic.y.c 

%.lex.c: %.lex 
	flex -o$@ $<

list.c: list.h

symbol.c:symbol.h uthash.h

%:%.o
%:%.c
