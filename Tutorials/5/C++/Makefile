.PHONY: t4

%.ll:%.bc
	llvm-dis $<
	cat $@

%.o:%.cpp
	clang++ -c `llvm-config --cxxflags` -o $@ $<

%.o:%.bc
	clang++ -c -o$@ $<

dce: dce.o
	clang++ -o$@ $^ `llvm-config --cxxflags --ldflags --libs --system-libs`

test: dce
	clang -O0 -S -emit-llvm -o tmp.ll tmp.c
	opt -O3 -mem2reg -o tmp.bc tmp.ll
	./dce ./tmp.bc tmp-dce.bc
	llvm-dis tmp-dce.bc

clean:
	rm -f dce.o dce *~ main.bc main.ll

