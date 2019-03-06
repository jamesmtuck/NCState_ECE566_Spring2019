.PHONY: t4

%.ll:%.bc
	llvm-dis $<
	cat $@

%.o:%.cpp
	clang++ -c `llvm-config --cxxflags` -o $@ $<

%.o:%.bc
	clang++ -c -o$@ $<

t4: t4.o
	clang++ -o$@ $^ `llvm-config --cxxflags --ldflags --libs --system-libs`


clean:
	rm -f t4.o t4 *~ main.bc main.ll

