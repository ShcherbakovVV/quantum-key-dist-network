#!/bin/sh

CC=clang \
CXX=/usr/bin/clang++ \
cmake . && cmake --build . -- -j $(nproc) && 
ctest && 
mv qkd/app $(pwd)
