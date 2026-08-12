{
    set -x
    cmake -S ../project -B ../project/build-gcc
    cmake --build ../project/build-gcc
    ctest --test-dir ../project/build-gcc --output-on-failure
    echo "ctest exit status $?"
} 2>&1 | tee ctest.log