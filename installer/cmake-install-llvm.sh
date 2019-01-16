#!/bin/bash

WGET=wget
CURRENT_DIR=`pwd`
INSTALL_DIR=`pwd`/install

PYTHON=python
CLASS=566
VERSION="7.0.1"
TARGET=

URL="http://releases.llvm.org"

# select file extension based on version

CLANG_SRC=""
LLVM_SRC=""
RT_SRC=""
LIBCXX_SRC=""
LIBCXXABI_SRC=""
LLDB_SRC=""
LLD_SRC=""
POLLY_SRC=""
OPENMP_SRC=""
CLANG_TOOLS_SRC=""
TEST_SUITE_SRC=""

SUFFIX=".tar.gz"

RTVERSION=$VERSION

if [ "$VERSION" == "3.4.2" ]; then
    RTVERSION="3.4"
    SUFFIX = ".tar.gz"
    CLANG_SRC="cfe-$VERSION.src"
    LLVM_SRC="llvm-$VERSION.src"
    RT_SRC="compiler-rt-3.4.src"
    LIBCXX_SRC="libcxx-$VERSION.src"
    LLDB_SRC="lldb-3.4.src"
    POLLY_SRC="polly-3.4.src"
    CLANG_TOOLS_SRC="clang-tools-extra-3.4.src"
    TEST_SUITE_SRC="test-suite-3.4.src"
else    
    SUFFIX=".tar.xz"
    CLANG_SRC="cfe-$VERSION.src"
    LLVM_SRC="llvm-$VERSION.src"
    RT_SRC="compiler-rt-$VERSION.src"
    LIBCXX_SRC="libcxx-$VERSION.src"
    LIBCXXABI_SRC="libcxxabi-$VERSION.src"
    LLDB_SRC="lldb-$VERSION.src"
    LLD_SRC="lld-$VERSION.src"
    POLLY_SRC="polly-$VERSION.src"
    OPENMP_SRC="openmp-$VERSION.src"
    CLANG_TOOLS_SRC="clang-tools-extra-$VERSION.src"
    TEST_SUITE_SRC="test-suite-$VERSION.src"
fi


if [ "$(uname)" == "Darwin" ]; then
    WGET='curl -O ' 
    TARGET='--target=x86_64'
fi

if [ -d $LLVM_SRC ]; then
    echo Found $LLVM_SRC! Not downloading source again.
else
    echo $WGET $URL/$VERSION/$LLVM_SRC$SUFFIX
    $WGET $URL/$VERSION/$LLVM_SRC$SUFFIX
    tar xf $LLVM_SRC$SUFFIX
fi

if [ -d $LLVM_SRC ]; then
    echo Everything looks sane.
else
    echo Install had problems. Quitting.
    exit
fi

if [ -d $CURRENT_DIR/$LLVM_SRC/tools ]; then
    cd $CURRENT_DIR/$LLVM_SRC/tools
else
    echo Fail! Something is wrong with your $LLVM_SRC checkout! 
    exit 1
fi

if [ -d clang ]; then
    echo Found clang! Not downloading clang again.
else
    $WGET $URL/$VERSION/$CLANG_SRC$SUFFIX
    tar xf $CLANG_SRC$SUFFIX
    mv $CLANG_SRC clang
    if [ -d clang ]; then
	echo Everything looks sane.
    else
	echo Install had problems. Quitting.
	exit
    fi
fi

cd $CURRENT_DIR

if [ -d $CURRENT_DIR/$LLVM_SRC/projects ]; then
    cd $CURRENT_DIR/$LLVM_SRC/projects
else
    echo Fail! Something is wrong wint $LLVM_SRC.
    exit 1
fi

if [ -d compiler-rt ]; then
    echo Found compiler-rt! Not downloading compiler-rt again.
else
    $WGET $URL/$RTVERSION/$RT_SRC$SUFFIX
    tar xf $RT_SRC$SUFFIX
    mv $RT_SRC compiler-rt
    if [ -d compiler-rt ]; then
	echo Everything looks sane.
    else
	echo Install had problems. Quitting.
	exit
    fi
fi

if [ -d libcxx ]; then
    echo Found libcxx! Not downloading libcxx again.
else
    $WGET $URL/$VERSION/$LIBCXX_SRC$SUFFIX
    tar xf $LIBCXX_SRC$SUFFIX
    mv $LIBCXX_SRC libcxx
    if [ -d libcxx ]; then
	echo Everything looks sane.
    else
	echo Install had problems. Quitting.
	exit
    fi
fi

if [ $LIBCXXABI_SRC != "" ]; then
    if [ -d libcxxabi ]; then
	echo Found libcxxabi! Not downloading libcxx again.
    else
	$WGET $URL/$VERSION/$LIBCXXABI_SRC$SUFFIX
	tar xf $LIBCXXABI_SRC$SUFFIX
	mv $LIBCXXABI_SRC libcxxabi
	if [ -d libcxxabi ]; then
	    echo Everything looks sane.
	else
	    echo Install had problems. Quitting.
	    exit
	fi
    fi
fi

if [ $LLD_SRC != "" ]; then
    if [ -d lld ]; then
	echo Found lld! Not downloading lld again.
    else
	$WGET $URL/$VERSION/$LLD_SRC$SUFFIX
	tar xf $LLD_SRC$SUFFIX
	mv $LLD_SRC lld
	if [ -d lld ]; then
	    echo Everything looks sane.
	else
	    echo Install had problems. Quitting.
	    exit
	fi
    fi
fi

BUILD_DIR=$CURRENT_DIR/$LLVM_SRC/build

if [ -d $BUILD_DIR ]; then
    cd $BUILD_DIR
    echo Found $BUILD_DIR.  Remove to reconfigure LLVM and Clang.
    make
else
    mkdir -p $BUILD_DIR
    mkdir -p $INSTALL_DIR
    cd $BUILD_DIR
    cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DLLVM_OPTIMIZED_TABLEGEN=On  ..
    ninja install
    #cmake --build .
    #make cxx
    #make check-libcxx
    #cmake --build . --target install
fi

if [ -x $INSTALL_DIR/bin/clang ]; then
    true
else
    echo LLVM not installed properly.
    exit 0
fi

cd $CURRENT_DIR
echo "Remember to add $INSTALL_DIR/bin to your PATH variable."
export PATH="$INSTALL_DIR/bin:$PATH"

