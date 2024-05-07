#!/bin/bash

cmake -DCMAKE_BUILD_TYPE=$1 -S . -B build
cmake --build build