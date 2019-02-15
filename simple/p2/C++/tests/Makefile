
# Each entry specifies a test case:
#    test_00 :  test_00.c   is the test harness compiled with gcc
#               test_00.p2  is the minic code to be compiled using your p2 binary, into
#                           test_00-p2.bc
#               test_00 is the final binary created by linking test_00.bc and
#               test_00-p2.bc

# To create your own test case, make a file with these conventions:
#     <basename>.c <basename>.p2
# You can start by copying one of the existing tests. Then add the new basename to the appropriate
# list below. Edit both the .c and .p2 files to test similar functionality.

ece466 = \
	test_00 \
	test_01 \
	test_02 \
	test_03 \
	test_04 \
	test_05 \
	test_06 \
	test_07 \
	test_08 \
	test_09 # <basename>

ece566 = \
	test_10 \
	test_11 \
	test_12 \
	test_13 \
	test_14 # <basename>


# for 466
#programs = $(ece466)

# for 566
programs = $(ece466) $(ece566)


.PHONY: all install

all: $(programs)

test: all $(addsuffix -test,$(programs))
	./summarize.py $(addsuffix -test,$(programs))

cleanall: clean

include Makefile.P2


