#!/usr/bin/env bash

cat << EOF > ./tool-versions.txt
===================================================
Execution Date: $(date "+%Y-%m-%d %H:%M:%S %Z")
System / Kernel: $(uname -a)
===================================================

--- GCC ---
$(gcc --version 2>&1 | head -n 1)

--- Clang ---
$(clang++ --version 2>&1 | head -n 1)

--- CMake ---
$(cmake --version 2>&1 | head -n 1)

--- Git ---
$(git --version 2>&1 | head -n 1)

--- GDB ---
$(gdb --version 2>&1 | head -n 1)

EOF

echo "成功生成工具版本记录: evidence/tool-versions.txt"
