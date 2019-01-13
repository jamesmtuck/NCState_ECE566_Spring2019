%.ll:%.c
	clang -emit-llvm -S -o $@ $<

%.ll:%.bc
	llvm-dis $<

%:%.ll
	clang -o $@ $<


TESTS = test

clean:
	rm -f *.ll *.bc $(TESTS)
