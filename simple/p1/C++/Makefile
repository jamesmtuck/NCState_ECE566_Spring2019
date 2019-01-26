
.PHONY: tests test all

all: p1

clean:
	rm -Rf p1.y.cpp p1.lex.cpp p1.y.hpp *.o p1 *~ 
	make -C tests clean

test: tests

tests: p1
	make -C ./tests test

p1: p1.y.o p1.lex.o main.o
	clang++ -g -Wno-implicit-function-declaration -o $@ $^ `llvm-config --cxxflags --ldflags --libs --system-libs` -ly -ll

%.lex.cpp: %.lex
	flex -o $@ $<

%.y.cpp: %.y
	bison -d -o $@ $<

%.o:%.cpp
	clang++ -g -c -o $@ -Wno-deprecated-register `llvm-config --cxxflags` $<

%:%.o
%:%.cpp
