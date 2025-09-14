#!/bin/bash

sudo docker run -it --rm -v $(pwd):/root/compiler maxxing/compiler-dev bash -c "
    cd compiler &&
    cmake -DCMAKE_BUILD_TYPE=Debug -B build &&
    cmake --build build &&
    cd build &&
    bash
"