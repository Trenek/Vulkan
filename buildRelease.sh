#! /bin/bash

cmake -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build --target all -j1