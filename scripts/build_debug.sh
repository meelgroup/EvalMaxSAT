#!/bin/bash

rm -rf cmake_install.cmake
rm -rf CMakeCache.txt
rm -rf CMakeFiles
rm -rf lib
rm -rf main
rm -rf Makefile
rm -rf .cmake
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
