
.PHONY: tests test all

all: p1

p1:
	flex -o p1.lex.c p1.lex
	bison -d -o p1.y.c p1.y
	gcc -g -c -o p1.lex.o p1.lex.c `llvm-config --cflags `
	gcc -g -c -o p1.y.o p1.y.c `llvm-config --cflags `
	gcc -g -c -o main.o main.c `llvm-config --cflags `
	g++ -g -Wno-implicit-function-declaration -o p1 main.o p1.y.o p1.lex.o `llvm-config --cflags --ldflags --libs --system-libs` -ly -ll -lpthread -ldl 

clean:
	rm -Rf p1.y.c p1.lex.c p1.y.h *.o p1 *~ 
	make -C tests clean

test: tests

tests: p1
	make -C ./tests test
