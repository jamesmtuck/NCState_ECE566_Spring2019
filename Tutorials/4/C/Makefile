.PHONY: t4

%.ll:%.bc
	llvm-dis $<
	cat $@

%.o:%.c
	clang -c `llvm-config --cflags` -o $@ $<

%.o:%.bc
	clang -c -o$@ $<

t4: t4.o
	clang++ -o$@ $^ `llvm-config --cflags --ldflags --libs --system-libs`


clean:
	rm -f t4.o t4 *~ main.bc main.ll

