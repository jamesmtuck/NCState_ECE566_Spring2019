#!/bin/bash
#
# Program:  RunDiff.sh
#
# Synopsis: Compare the files passed in as arguments. Must be
#      passed as pairs, version 1 (v1) followed by version 2 (v2)
# Syntax: ./RunDiff.sh exe file1v1 file1v2 file2v1 file2v2
#

if [ $# -lt 3 ]; then
    #echo "./RunDiff.sh exe file1v1 file1v2 file2v1 file2v2 ..."
   echo "Nothing compared for $1"
   exit 0
fi

PWD=`pwd`

VERBOSE="0"
if [ "$1" = "-v" ]; then
    VERBOSE="1"; shift;
fi

# get exe name and shift it out
EXE=$1
shift 1

FAIL="no"
let tests=0
let fail=0
while test $# -gt 0 
do
    let tests=$tests+1
    # shift out two names and compare them
    tmpfile=/tmp/diff$$
    diff -w $1 $2 > $tmpfile
    if [ -s $tmpfile ]; then
	let fail=$fail+1
	difffile=output.diff.$$
	mv $tmpfile $difffile
	FAIL="yes"
	echo "[$EXE] Comparing $1 $2 .... failed. Differences in $PWD/$difffile"
    else
	if [ "$VERBOSE" = "1" ]; then
	    echo "[$EXE] Ok .. Compared $1 $2"
	fi
	rm $tmpfile
    fi
    shift
    shift
done

if [ "$FAIL" = "no" ]; then
    echo "[compared $EXE] Ok"
else
    echo "[compared $EXE] Failed: $fail of $tests."
fi

exit 0
