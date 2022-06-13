#!/bin/sh

/usr/bin/time -f "\nTotal compilation time: %e sec" /bin/sh -c 'CC=gcc CXX=/usr/bin/g++ cmake . && cmake --build . -- -j $(nproc) && ctest'
