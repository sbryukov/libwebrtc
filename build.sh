#!/bin/sh

PATH=/usr/src/ndk-standalone-9/bin:$PATH
mkdir build && cd build

cmake  -D CMAKE_TOOLCHAIN_FILE=../android_toolchain.cmake -D CMAKE_INSTALL_PREFIX=./

