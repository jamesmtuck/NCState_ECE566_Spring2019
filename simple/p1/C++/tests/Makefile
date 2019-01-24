P1TOOL = ../p1
CLANG = clang
LLVM_LINK = llvm-link

programs = \
	test_00 \
	test_01 \
	test_02 \
	test_03 \
	test_04 \
        test_05 \
	test_06 \
	test_07 \
	test_08 \
	test_09

.PHONY: all install

all: $(addsuffix -build,$(programs))

test: all $(addsuffix -test,$(programs))
	./summarize.py $(addsuffix -test,$(programs))

cleanall:
	@rm -Rf $(programs) $(addsuffix -test,$(programs)) $(addsuffix .s,$(programs))

include ./Makefile.P1

