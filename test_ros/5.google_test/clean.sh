#!/bin/bash

rm -rf build_package.sh CMakeLists.txt

if [ -d "build" ]; then
    rm -r build
fi

if [ -d "devel" ]; then
    rm -r devel
fi

if [ -d "output" ]; then
    rm -r output
fi
