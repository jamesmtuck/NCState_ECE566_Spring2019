
.PHONY: tests test all

all: p1

p1: p1.y.o p1.lex.o main.o
	clang++ -g -Wno-implicit-function-declaration -o $@ $^ `llvm-config --cflags --ldflags --libs --system-libs` -ly -ll

clean:
	rm -Rf p1.y.c p1.lex.c p1.y.h *.o p1 *~ 
	make -C tests clean

test: tests

tests: p1
	make -C ./tests test


%.lex.c: %.lex
	flex -o $@ $<

%.y.c: %.y
	bison -d -o $@ $<

%.o:%.c
	clang -g -c -o $@ -Wno-deprecated-register `llvm-config --cflags` $<

%:%.o
%:%.c
