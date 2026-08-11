{
    set -x
    cmake -S ../project/ -B ../project/build-gcc -DCMAKE_CXX_COMPILER=g++
    echo "Configure Exit Code: $?"
    cmake --build ../project/build-gcc
    echo "Build Exit Code: $?"
} 2>&1 | tee ./build-gcc.log

{
    set -x
    cmake -S ../project/ -B ../project/build-clang -DCMAKE_CXX_COMPILER=clang++
    echo "Configure Exit Code: $?"
    cmake --build ../project/build-clang
    echo "Build Exit Code: $?"
} 2>&1 | tee ./build-clang.log