.PHONY: test all clean

%.o:%.c
	clang -std=c11 -c -o$@ $^ `llvm-config --cflags` 

%.o:%.cpp
	clang++ -std=c++11 -c -o$@ $^ `llvm-config --cxxflags` 

all: dce

dce: dce.o worklist.o
	clang++  -o$@ $^ `llvm-config --ldflags --libs --system-libs` 

test: dce
	clang -O0 -S -emit-llvm -o tmp.ll tmp.c
	opt -O3 -mem2reg -o tmp.bc tmp.ll
	./dce ./tmp.bc tmp-dce.bc
	llvm-dis tmp-dce.bc

clean:
	rm -f dce dce.o worklist.o *~ *.ll *.bc


