#!/usr/bin/env bash
set -euo pipefail

rm -rf cmake_install.cmake
rm -rf CMakeCache.txt
rm -rf CMakeFiles
rm -rf lib
rm -rf main
rm -rf Makefile
rm -rf .cmake
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
make "-j$(nproc)"
