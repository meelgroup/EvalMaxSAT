#!/usr/bin/env bash
set -euo pipefail

rm -rf cmake_install.cmake CMakeCache.txt CMakeFiles lib main Makefile .cmake
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF ..
make -j$(nproc)
