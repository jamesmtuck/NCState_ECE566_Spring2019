
.PHONY: all clean

all: p0

p0: main.c
	clang -g -c -o main.o main.c `llvm-config --cflags `
	clang++ -g -Wno-implicit-function-declaration -o p0 main.o `llvm-config --cflags --ldflags --libs --system-libs`

clean:
	rm -Rf p0 main.o *~ 
