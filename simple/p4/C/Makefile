.SUFFIXES: %.c %.cpp

OBJS =  main.o \
	dominance.o \
	worklist.o \
	valmap.o \
	transform.o \
	cfg.o \
	loop.o 

OBJS += CSE.o summary.o

.PHONY: all

all: p4

p4: $(OBJS)
	clang++ -g -Wno-implicit-function-declaration -o $@ $(OBJS) `llvm-config --cxxflags --ldflags --libs --system-libs`

clean:
	rm -Rf p4 $(OBJS)

%.o:%.c
	clang -g -c -o $@ $^ `llvm-config --cflags` 

%.o:%.cpp
	clang++ -g -c -o $@ $^ `llvm-config --cxxflags` 


