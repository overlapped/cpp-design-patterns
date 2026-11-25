#!/bin/bash

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel $(nproc) --target all
ctest --test-dir build --output-on-failure
