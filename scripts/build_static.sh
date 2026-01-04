#!/bin/bash

rm -rf cmake_install.cmake CMakeCache.txt CMakeFiles lib main Makefile .cmake
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSTATICCOMPILE=ON ..
make -j$(nproc)
