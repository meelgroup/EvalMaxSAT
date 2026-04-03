#!/bin/bash

rm -rf cmake_install.cmake CMakeCache.txt CMakeFiles lib main Makefile .cmake
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=OFF ..
make -j$(nproc)
