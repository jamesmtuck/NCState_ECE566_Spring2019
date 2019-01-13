set -e
clang --version
make -C LectureCode all
make -C simple all
cd projects && mkdir build && cd build && cmake -DLLVM_DIR=/usr/local/lib/cmake/llvm .. && cmake --build .
