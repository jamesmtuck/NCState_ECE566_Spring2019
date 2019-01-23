all:
	flex -o expr.lex.c expr.lex
	bison -d -o expr.y.c expr.y
	clang -c -o expr.y.o expr.y.c `llvm-config --cflags`
	clang -c -o expr.lex.o expr.lex.c `llvm-config --cflags`
	clang++ -o expr expr.y.o expr.lex.o `llvm-config --ldflags --libs` -ly -ll `llvm-config --system-libs` 

clean:
	rm -Rf expr.lex.c expr.y.c expr.y.h *.o expr *~ *.ll *.bc
