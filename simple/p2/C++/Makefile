

.SUFFIXES: %.cpp

OBJS =  main.o \
	minic.y.o  \
	minic.lex.o \
	list.o \
	symbol.o \
	cmdline.lex.o

.PHONY: tests test all

all: p2

p2: $(OBJS)
	clang++ -g -Wno-implicit-function-declaration -o $@ $(OBJS) `llvm-config --cxxflags --ldflags --libs` -ly -ll `llvm-config --system-libs`

clean:
	rm -Rf p2 $(OBJS) minic.y.cpp minic.lex.cpp minic.y.hpp cmdline.lex.cpp 
	make -C ./tests clean

test: tests

tests: p2
	make -C ./tests test

%.o:%.cpp
	clang++ -g -c -o $@ $< `llvm-config --cxxflags` 

cmdline.lex.cpp: cmdline.lex
	flex -PCmd -o$@ cmdline.lex 

minic.y.cpp: minic.y
	bison -d -o $@ $<

minic.y.hpp: minic.y.cpp

minic.lex.cpp: minic.lex
	flex -o$@ $<

list.cpp: list.h 

symbol.cpp:symbol.h uthash.h

%:%.o
%:%.cpp
