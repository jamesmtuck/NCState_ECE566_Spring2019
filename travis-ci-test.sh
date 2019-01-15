set -e
clang --version
make -C LectureCode all
make -C simple all
pushd .
cd projects && mkdir build && cd build && cmake -DLLVM_DIR=/usr/local/lib/cmake/llvm .. && cmake --build .
popd
make -C Tutorials/1 clean all
pushd
mkdir p0-test && cd p0-test && ../wolfbench/configure && make all 
popd
