#!/usr/bin/env bash
set -euo pipefail

rm -rf cmake_install.cmake CMakeCache.txt CMakeFiles lib main Makefile .cmake
cmake -DCMAKE_BUILD_TYPE=Release -DSTATICCOMPILE=ON ..
make -j$(nproc)
