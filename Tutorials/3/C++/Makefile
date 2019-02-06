all:
	flex -o expr.lex.cpp expr.lex
	bison -d -o expr.y.cpp expr.y
	clang++ -c -o expr.y.o expr.y.cpp `llvm-config --cxxflags`
	clang++ -c -o expr.lex.o expr.lex.cpp `llvm-config --cxxflags`
	clang++ -o expr expr.y.o expr.lex.o `llvm-config --ldflags --libs` -ly -ll `llvm-config --system-libs`


clean:
	rm -Rf expr.lex.cpp expr.y.cpp expr.y.hpp *.hh *.o expr *~ *.bc *.ll 
