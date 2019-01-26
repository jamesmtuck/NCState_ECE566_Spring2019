set -e
clang --version
make -C LectureCode all
make -C simple all
pushd .
cd projects && mkdir build && cd build && cmake -DLLVM_DIR=/usr/local/lib/cmake/llvm .. && cmake --build .
popd
pushd .
cd projects && mkdir buildcpp && cd buildcpp && cmake -DUSE_CPP=1 -DLLVM_DIR=/usr/local/lib/cmake/llvm .. && cmake --build .
popd
make -C Tutorials/1 clean all
pushd .
mkdir p0-test && cd p0-test && ../wolfbench/configure && make all 
popd
pushd .
mkdir p1-test && cd p1-test && ../wolfbench/configure --enable-p1=`cd ../projects/build/tools/p1; pwd`/p1 
popd
